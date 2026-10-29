#pragma once

#include "Matrix.h"

#include <iosfwd>

namespace miit::algebra
{
    /**
     * @brief выводит содержимое матрицы в переданный поток
     * @param out поток вывода
     * @param matrix матрица для вывода
     * @return тот же поток чтобы можно было продолжить вывод
     */
    template <typename T>
    std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix);

    /**
     * @brief считывает элементы матрицы сохраняя ее размеры
     * @param in поток ввода
     * @param matrix матрица для заполнения
     * @return тот же поток с признаком ошибки при неверном вводе
     */
    template <typename T>
    std::istream& operator>>(std::istream& in, Matrix<T>& matrix);

    extern template std::ostream& operator<<(std::ostream&, const Matrix<int>&);
    extern template std::ostream& operator<<(std::ostream&, const Matrix<double>&);
    extern template std::istream& operator>>(std::istream&, Matrix<int>&);
    extern template std::istream& operator>>(std::istream&, Matrix<double>&);
}
