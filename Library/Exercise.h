#pragma once

#include "Generator.h"
#include "Matrix.h"

namespace miit::algebra
{
    /**
     * @brief объединяет массив и генератор для решения задания
     * @note массив и генератор существуют отдельно и должны жить дольше задания
     */
    class Exercise
    {
    protected:
        Matrix<int>& matrix;
        Generator& generator;

    public:
        /**
         * @brief сохраняет ссылки на массив и способ его заполнения
         * @param matrix массив который будет изменять задание
         * @param generator генератор значений для заполнения массива
         */
        Exercise(Matrix<int>& matrix, Generator& generator);

        /**
         * @brief завершает работу с заданием не удаляя массив и генератор
         */
        virtual ~Exercise();

        /**
         * @brief заполняет связанный массив выбранным генератором
         */
        void fill();

        /**
         * @brief выполняет конкретное задание над массивом
         */
        virtual void solve() = 0;
    };
}
