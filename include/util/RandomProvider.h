#pragma once
#include <stdlib.h>

namespace CppAgents
{
    // TODO: call srand()
    using get_random_int_t = std::function<int(int, int)>;
    using get_random_float_t = std::function<float(float, float)>;

    inline int GetRandomInt(int min, int max)
    {
        return rand() % (max - min + 1) + min;
    }

    inline float GetRandomFloat(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }
} // namespace CppAgents