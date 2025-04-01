"""
Tested on:

tensorboard==2.11.2
tensorboard-data-server==0.6.1
tensorboard-plugin-wit==1.8.1
tensorflow==2.11.0
tensorflow-estimator==2.11.0
tensorflow-intel==2.11.0
tensorflow-io-gcs-filesystem==0.30.0

keras==2.11.0

gym==0.26.2
"""

import numpy as np
import random
import tensorflow as tf
from tensorflow.keras.layers import Dense
from tensorflow.keras import Sequential
from tensorflow.keras.optimizers import RMSprop
from collections import deque
from tensorflow import gather_nd
from tensorflow.keras.losses import MeanSquaredError


class DeepQLearning:


    def __init__(self, env, gamma, epsilon, numberEpisodes):

        self.env = env
        self.gamma = gamma
        self.epsilon = epsilon
        self.numberEpisodes = numberEpisodes

        # state dimension
        self.stateDimension = 4
        # action dimension
        self.actionDimension = 2
        # this is the maximum size of the replay buffer
        self.replayBufferSize = 300
        # this is the size of the training batch that is randomly sampled from the replay buffer
        self.batchReplayBufferSize = 100


        #  every updateTargetNetworkPeriod we update the target network parameters
        self.updateTargetNetworkPeriod = 100

        # this is the counter for updating the target network
        # if this counter exceeds (updateTargetNetworkPeriod-1) we update the network
        # parameters and reset the counter to zero, this process is repeated until the end of the training process
        self.counterUpdateTargetNetwork = 0

        #   the sum of rewards during each training episode
        self.sumRewardsEpisode = []

        # replay buffer
        self.replayBuffer = deque(maxlen=self.replayBufferSize)


        # create network
        self.mainNetwork = self.createNetwork()


        # create network
        self.targetNetwork = self.createNetwork()

        # copy the initial weights to targetNetwork
        self.targetNetwork.set_weights(self.mainNetwork.get_weights())

        # this list is used in the cost function
        self.actionsAppend = []


    def my_loss_fn(self, y_true, y_pred):
        # Indices pour sélectionner les valeurs
        indices = tf.range(tf.shape(y_true)[0])
        indices = tf.stack([indices, tf.cast(self.actionsAppend, tf.int32)], axis=1)

        # Sélection des valeurs de y_true et y_pred
        y_true_selected = tf.gather_nd(y_true, indices)
        y_pred_selected = tf.gather_nd(y_pred, indices)

        # Calcul de l'erreur quadratique moyenne
        mse = tf.reduce_mean(tf.square(y_true_selected - y_pred_selected))
        return mse



    def createNetwork(self):
        model = Sequential()
        model.add(Dense(128, input_dim=self.stateDimension, activation='relu'))
        model.add(Dense(56, activation='relu'))
        model.add(Dense(self.actionDimension, activation='linear'))
        # Compiler le modèle avec la nouvelle fonction de perte
        model.compile(optimizer=RMSprop(), loss=self.my_loss_fn, metrics=['accuracy'])
        return model



    def trainingEpisodes(self):

        # loop through the episodes
        for indexEpisode in range(self.numberEpisodes):

            # list that stores rewards per episode
            rewardsEpisode = []

            print("Simulating episode {}".format(indexEpisode))

            # reset the environment at the beginning of every episode
            (currentState, _) = self.env.reset()


            # this will loop until a terminal state is reached
            terminalState = False
            while not terminalState:
                # select an action on the basis of the current state
                action = self.selectAction(currentState, indexEpisode)

                #  return the state, reward, and boolean if the state is a terminal state
                (nextState, reward, terminalState, _, _) = self.env.step(action)
                rewardsEpisode.append(reward)

                # add current state, action, reward, next state, and terminal flag to the replay buffer
                self.replayBuffer.append((currentState, action, reward, nextState, terminalState))

                # train network
                self.trainNetwork()

                # set the current state for the next step
                currentState = nextState

            print("Sum of rewards {}".format(np.sum(rewardsEpisode)))
            self.sumRewardsEpisode.append(np.sum(rewardsEpisode))



    def selectAction(self, state, index):
        import numpy as np

        # first index episodes we select completely random actions to have enough exploration

        if index < 1:
            return np.random.choice(self.actionDimension)


        # used for the epsilon greedy
        randomNumber = np.random.random()

        # after index episodes, we slowly start to decrease the epsilon parameter
        if index > 200:
            self.epsilon = 0.999 * self.epsilon

        # if this condition is satisfied, we are exploring,  we select random actions
        if randomNumber < self.epsilon:
            # returns a random action selected
            return np.random.choice(self.actionDimension)

            # otherwise, we are selecting greedy actions
        else:
            # we return the index where Qvalues[state,:] has the max value


            Qvalues = self.mainNetwork.predict(state.reshape(1, 4))

            return np.random.choice(np.where(Qvalues[0, :] == np.max(Qvalues[0, :]))[0])



    def trainNetwork(self):

        # if the replay buffer has at least batchReplayBufferSize elements,
        # then train the model

        if (len(self.replayBuffer) > self.batchReplayBufferSize):

            # sample a batch from the replay buffer
            randomSampleBatch = random.sample(self.replayBuffer, self.batchReplayBufferSize)

            #  current state batch
            # next state batch
            #  used as inputs for prediction
            currentStateBatch = np.zeros(shape=(self.batchReplayBufferSize, 4))
            nextStateBatch = np.zeros(shape=(self.batchReplayBufferSize, 4))
            #  enumerate the tuple entries of the randomSampleBatch
            # index will loop through the number of tuples
            for index, tupleS in enumerate(randomSampleBatch):
                # first entry of the tuple is the current state
                currentStateBatch[index, :] = tupleS[0]
                # fourth entry of the tuple is the next state
                nextStateBatch[index, :] = tupleS[3]

            #  the target network to predict Q-values
            QnextStateTargetNetwork = self.targetNetwork.predict(nextStateBatch)
            # the main network to predict Q-values
            QcurrentStateMainNetwork = self.mainNetwork.predict(currentStateBatch)


            # input for training
            inputNetwork = currentStateBatch
            # output for training
            outputNetwork = np.zeros(shape=(self.batchReplayBufferSize, 2))

            #  list will contain the actions that are selected from the batch
            #  list is used in my_loss_fn to define the loss-function
            self.actionsAppend = []
            for index, (currentState, action, reward, nextState, terminated) in enumerate(randomSampleBatch):

                # if the next state is the terminal state
                if terminated:
                    y = reward
                    # if the next state if not the terminal state
                else:
                    y = reward + self.gamma * np.max(QnextStateTargetNetwork[index])

                # append the action to the list
                self.actionsAppend.append(action)

                # copy the Q-values from the main network
                outputNetwork[index] = QcurrentStateMainNetwork[index]
                # this is what matters
                outputNetwork[index, action] = y

            # train the network
            self.mainNetwork.fit(inputNetwork, outputNetwork, batch_size=self.batchReplayBufferSize, verbose=0,
                                 epochs=100)

            # update the target network
            self.counterUpdateTargetNetwork += 1
            if (self.counterUpdateTargetNetwork > (self.updateTargetNetworkPeriod - 1)):
                # copy the weights to targetNetwork
                self.targetNetwork.set_weights(self.mainNetwork.get_weights())
                print("Target network updated!")
                print("Counter value {}".format(self.counterUpdateTargetNetwork))
                # reset the counter
                self.counterUpdateTargetNetwork = 0
