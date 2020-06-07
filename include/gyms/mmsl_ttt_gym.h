#pragma once

namespace CppAgents::MmslTttGym
{
    void Train(int numEpisodes);
    void Test(int numEpisodes);
    void TrainAndTest(int nIterations, int nTrain = 1000);
}