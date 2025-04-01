
import gym

import numpy as np
import time
import matplotlib.pyplot as plt

from QLearning import Q_Learning

env = gym.make('CartPole-v1')
(state, _) = env.reset()


upperBounds = env.observation_space.high
lowerBounds = env.observation_space.low
cartVelocityMin = -3
cartVelocityMax = 3
poleAngleVelocityMin = -10
poleAngleVelocityMax = 10
upperBounds[1] = cartVelocityMax
upperBounds[3] = poleAngleVelocityMax
lowerBounds[1] = cartVelocityMin
lowerBounds[3] = poleAngleVelocityMin

numberOfBinsPosition = 30
numberOfBinsVelocity = 30
numberOfBinsAngle = 30
numberOfBinsAngleVelocity = 30
numberOfBins = [numberOfBinsPosition, numberOfBinsVelocity, numberOfBinsAngle, numberOfBinsAngleVelocity]

alpha = 0.1
gamma = 1
epsilon = 0.2
numberEpisodes = 15000

Q1 = Q_Learning(env, alpha, gamma, epsilon, numberEpisodes, numberOfBins, lowerBounds, upperBounds)
Q1.simulateEpisodes()
(obtainedRewardsOptimal, env1) = Q1.simulateLearnedStrategy()

plt.figure(figsize=(12, 5))
plt.plot(Q1.sumRewardsEpisode, color='blue', linewidth=1)
plt.xlabel('Episode')
plt.ylabel('Reward')
plt.yscale('log')
plt.show()
plt.savefig('convergence.png')

env1.close()
np.sum(obtainedRewardsOptimal)

(obtainedRewardsRandom, env2) = Q1.simulateRandomStrategy()
plt.hist(obtainedRewardsRandom)
plt.xlabel('Sum of rewards')
plt.ylabel('Percentage')
plt.savefig('histogram.png')
plt.show()

(obtainedRewardsOptimal, env1) = Q1.simulateLearnedStrategy()