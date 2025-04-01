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

from functions_final import DeepQLearning

import gymnasium as gym

# create environment
env = gym.make('CartPole-v1')

# select the parameters
gamma = 1
#  epsilon-greedy approach
epsilon = 0.1
# number of training episodes
numberEpisodes = 1000
# create an object
LearningQDeep = DeepQLearning(env, gamma, epsilon, numberEpisodes)
# run the learning process
LearningQDeep.trainingEpisodes()
# get the obtained rewards in every episode
LearningQDeep.sumRewardsEpisode
#  summarize the model
LearningQDeep.mainNetwork.summary()
# save the model
LearningQDeep.mainNetwork.save("trained_model_temp_1Layer.h5")


