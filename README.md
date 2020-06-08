See cmake template used as base: https://github.com/PhDP/cmake-gtest-gbench-starter

# CPP Agents

TF Agents already exists and seems like a really easy and cool way to do RL: https://github.com/tensorflow/agents, but for my own educational purposes I want to scaffold my understand up from the basics. Thus I want to go through and slowly create my own gyms and environments that will act as a framework for simple RL tasks.

# RoadMap

## Simple Training Example (Tic-Tac-Toe)

- [x] TTT-Environment
- [x] Greedy/Epsilon-greedy Policies
- [x] Table Lookup MinMax Monte Carlo Agent w/ Gym
- [x] Table Lookup MinMax Sarsa-Lambda Agent w/ Gym
- [ ] Write Up w/ Aesthetic Graphs
- [x] Benchmark/Optimize

## Make Interfaces for Major Components

- [x] Environment
- [x] Agent
- [x] Policy
- [x] Trajectory

## Backlog

- [x] Abstract MinMax Agents
- [x] Abstract Tic-Tac-Toe Gyms
