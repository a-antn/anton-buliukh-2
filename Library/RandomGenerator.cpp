#include "RandomGenerator.h"
#include <stdexcept>

namespace miit::algebra
{
    RandomGenerator::RandomGenerator(const int min, const int max)
    {
        if (min > max)
        {
            throw std::invalid_argument("минимум не должен быть больше максимума");
        }

        distribution = std::uniform_int_distribution<int>(min, max);
        generator = std::mt19937(std::random_device{}());
    }

    int RandomGenerator::generate()
    {
        return distribution(generator);
    }
}
