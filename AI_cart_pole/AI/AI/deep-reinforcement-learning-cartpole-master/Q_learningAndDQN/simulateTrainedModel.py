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
import keras
from functions_final import DeepQLearning
import gym
import numpy as np


# load the model
loaded_model = keras.models.load_model("trained_model_temp_1Layer.h5",custom_objects={'my_loss_fn':DeepQLearning.my_loss_fn})

sumObtainedRewards=0

# create the environment
env = gym.make("CartPole-v1",render_mode='rgb_array')
# reset the environment
(currentState,prob)=env.reset()

# the length of the video
video_length=400
# record the video
env = gym.wrappers.RecordVideo(env, 'stored_video1Layer',step_trigger = lambda x: x == 1, video_length=video_length)
#env = gym.wrappers.RecordVideo(env, 'stored_video_sami_400VL+', video_length=video_length)

#  terminal state flag to false
terminalState=False
while not terminalState:
    # get the Q-value (1 by 2 vector)
    Qvalues=loaded_model.predict(currentState.reshape(1,4))
    # select the action that gives the max Qvalue
    action=np.random.choice(np.where(Qvalues[0,:]==np.max(Qvalues[0,:]))[0])
    # random actions for comparison
    #action = env.action_space.sample()
    # apply the action
    (currentState, currentReward, terminalState,_,_) = env.step(action)
    # sum the rewards
    sumObtainedRewards+=currentReward

env.reset()
env.close()



