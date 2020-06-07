#pragma once

namespace CppAgents::MmmcTttGym
{
    void Train(int numEpisodes);
    void Test(int numEpisodes);
    void TrainAndTest(int nIterations, int nTrain = 100000);
}