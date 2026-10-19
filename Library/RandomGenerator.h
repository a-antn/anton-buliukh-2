#pragma once

#include "Generator.h"
#include <random>

namespace miit::algebra
{
    /**
     * @brief получает случайные целые числа из заданного диапазона
     */
    class RandomGenerator final : public Generator
    {
    private:
        std::uniform_int_distribution<int> distribution;
        std::mt19937 generator;

    public:
        /**
         * @brief создает генератор случайных чисел
         * @param min нижняя граница диапазона включительно
         * @param max верхняя граница диапазона включительно
         * @throws std::invalid_argument если минимум больше максимума
         */
        RandomGenerator(const int min, const int max);

        /**
         * @brief получает случайное число
         * @return число от min до max
         */
        int generate() override;
    };
}
