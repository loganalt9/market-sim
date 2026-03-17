import torch
import torch.nn as nn
import torch.optim as optim
import random
import numpy as np

class DQN(nn.Module):
    def __init__(self, state_dim, action_dim):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(state_dim, 64),
            nn.ReLU(),
            nn.Linear(64, 64),
            nn.ReLU(),
            nn.Linear(64, action_dim)
        )

    def forward(self, x):
        return self.net(x)

class Agent:
    def __init__(self):
        self.model = DQN(1, 3)
        self.optimizer = optim.Adam(self.model.parameters(), lr=1e-3)
        self.gamma = 0.99

    def select_action(self, state, epsilon=0.1):
        if random.random() < epsilon:
            return random.randint(0, 2)

        state = torch.tensor(state).float().unsqueeze(0)
        with torch.no_grad():
            q_vals = self.model(state)
        return torch.argmax(q_vals).item()

    def train_step(self, batch):
        states, actions, rewards, next_states, dones = batch

        states = torch.tensor(states).float()
        actions = torch.tensor(actions)
        rewards = torch.tensor(rewards).float()
        next_states = torch.tensor(next_states).float()
        dones = torch.tensor(dones).float()

        q_vals = self.model(states)
        q_vals = q_vals.gather(1, actions.unsqueeze(1)).squeeze()

        with torch.no_grad():
            next_q = self.model(next_states).max(1)[0]
            target = rewards + self.gamma * next_q * (1 - dones)

        loss = nn.MSELoss()(q_vals, target)

        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()