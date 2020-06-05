#pragma once
#include <stdlib.h>

namespace CppAgents
{

    int GetRandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

    float GetRandomFloat(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
} // namespace CppAgents