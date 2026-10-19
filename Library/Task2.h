#pragma once

#include "Exercise.h"

namespace miit::algebra
{
    /**
     * @brief вставляет первую строку после каждой нечетной строки исходного массива
     */
    class Task2 final : public Exercise
    {
    public:
        /**
         * @brief создает второе задание для переданного массива
         * @param matrix обрабатываемый массив
         * @param generator генератор для заполнения массива
         */
        Task2(Matrix<int>& matrix, Generator& generator);

        /**
         * @brief вставляет копию первой строки после строк с номерами 1 3 5 и далее
         * @throws std::length_error если размер результата слишком большой
         * @note номера считаются с единицы и относятся к исходному массиву
         */
        void solve() override;
    };
}
