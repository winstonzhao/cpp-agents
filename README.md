See cmake template used as base: https://github.com/PhDP/cmake-gtest-gbench-starter

# CPP Agents

TF Agents already exists and seems like a really easy and cool way to do RL: https://github.com/tensorflow/agents, but for my own educational purposes I want to scaffold my understand up from the basics. Thus I want to go through and slowly create my own gyms and environments that will act as a framework for simple RL tasks.

# RoadMap

## Make Interfaces for Major Components

- [x] Environment
- [x] Agent
- [x] Policy
- [x] Trajectory

## Simple Training Example (Tic-Tac-Toe)

- [x] TTT-Environment
- [x] Greedy/Epsilon-greedy Policies
- [ ] Table Lookup MinMax Monte Carlo Agent w/ Gym
- [ ] Table Lookup MinMax Sarsa-Lambda Agent w/ Gym
- [ ] Policy Saving
- [ ] Aesthetic Graphs
- [ ] Benchmark/Optimize
