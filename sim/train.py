from env import LOBEnv
from agent import Agent
import random

env = LOBEnv()
agent = Agent()

replay_buffer = []

for episode in range(200):
    state = env.reset()
    total_reward = 0

    for t in range(50):
        action = agent.select_action(state)

        next_state, reward, done, _ = env.step(action)

        replay_buffer.append((state, action, reward, next_state, done))

        if len(replay_buffer) > 1000:
            replay_buffer.pop(0)

        if len(replay_buffer) > 32:
            batch = random.sample(replay_buffer, 32)
            agent.train_step(zip(*batch))

        state = next_state
        total_reward += reward

        if done:
            break

    print(f"Episode {episode}, Reward: {total_reward}")