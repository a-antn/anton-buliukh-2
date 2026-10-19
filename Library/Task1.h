#pragma once

#include "Exercise.h"

namespace miit::algebra
{
    /**
     * @brief возводит в квадрат элементы первых трех столбцов
     */
    class Task1 final : public Exercise
    {
    public:
        /**
         * @brief создает первое задание для переданного массива
         * @param matrix обрабатываемый массив
         * @param generator генератор для заполнения массива
         */
        Task1(Matrix<int>& matrix, Generator& generator);

        /**
         * @brief заменяет элементы первых трех столбцов их квадратами
         * @throws std::invalid_argument если в непустом массиве меньше трех столбцов
         * @throws std::overflow_error если квадрат не помещается в int
         * @note при ошибке содержимое массива не меняется
         */
        void solve() override;
    };
}
