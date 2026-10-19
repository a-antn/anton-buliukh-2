#pragma once

#include "Generator.h"
#include <cstddef>
#include <string>
#include <vector>

namespace miit::algebra
{
    /**
     * @brief хранит двумерный массив чисел в одном векторе
     * @tparam T тип элементов массива в проекте подготовлены int и double
     */
    template <typename T>
    class Matrix
    {
    private:
        std::size_t rows;
        std::size_t columns;
        std::vector<T> data;

    public:
        /**
         * @brief создает пустой массив размером 0 на 0
         */
        Matrix();

        /**
         * @brief создает массив заданного размера
         * @param rows количество строк
         * @param columns количество столбцов
         * @param value начальное значение элементов
         * @throws std::invalid_argument если только один из размеров равен нулю
         * @throws std::length_error если размер массива слишком большой
         */
        Matrix(const std::size_t rows, const std::size_t columns, const T& value = T{});

        /**
         * @brief создает независимую копию массива
         * @param other исходный массив
         */
        Matrix(const Matrix<T>& other);

        /**
         * @brief переносит содержимое и оставляет исходный массив пустым
         * @param other массив из которого переносится содержимое
         */
        Matrix(Matrix<T>&& other) noexcept;

        /**
         * @brief освобождает память занятую вектором
         */
        ~Matrix();

        /**
         * @brief заменяет содержимое независимой копией другого массива
         * @param other исходный массив
         * @return ссылка на текущий массив
         */
        Matrix<T>& operator=(const Matrix<T>& other);

        /**
         * @brief переносит содержимое другого массива
         * @param other массив из которого переносится содержимое
         * @return ссылка на текущий массив
         */
        Matrix<T>& operator=(Matrix<T>&& other) noexcept;

        /**
         * @brief получает количество строк
         * @return количество строк массива
         */
        std::size_t getRows() const noexcept;

        /**
         * @brief получает количество столбцов
         * @return количество столбцов массива
         */
        std::size_t getColumns() const noexcept;

        /**
         * @brief проверяет что в массиве нет элементов
         * @return true если массив пустой
         */
        bool empty() const noexcept;

        /**
         * @brief получает начало строки для обращения matrix[i][j]
         * @param row индекс строки начиная с нуля
         * @return указатель на первый элемент строки
         * @throws std::out_of_range если индекс строки выходит за границы
         * @note индекс столбца во вторых скобках проверяется вызывающим кодом
         */
        T* operator[](const std::size_t row);

        /**
         * @brief получает начало строки без возможности менять элементы
         * @param row индекс строки начиная с нуля
         * @return указатель на первый элемент строки
         * @throws std::out_of_range если индекс строки выходит за границы
         * @note индекс столбца во вторых скобках проверяется вызывающим кодом
         */
        const T* operator[](const std::size_t row) const;

        /**
         * @brief получает элемент с проверкой обоих индексов
         * @param row индекс строки начиная с нуля
         * @param column индекс столбца начиная с нуля
         * @return ссылка на элемент массива
         * @throws std::out_of_range если любой индекс выходит за границы
         */
        T& at(const std::size_t row, const std::size_t column);

        /**
         * @brief получает элемент без возможности его изменить
         * @param row индекс строки начиная с нуля
         * @param column индекс столбца начиная с нуля
         * @return константная ссылка на элемент массива
         * @throws std::out_of_range если любой индекс выходит за границы
         */
        const T& at(const std::size_t row, const std::size_t column) const;

        /**
         * @brief заполняет массив с помощью выбранного генератора
         * @param generator генератор значений
         * @note при ошибке генератора содержимое массива не меняется
         */
        void fill(Generator& generator);

        /**
         * @brief получает содержимое массива в виде строки
         * @return элементы через пробел и перевод строки после каждой строки массива
         */
        std::string toString() const;

        /**
         * @brief обменивает размеры и содержимое двух массивов
         * @param other массив для обмена
         */
        void swap(Matrix<T>& other) noexcept;
    };

    // реализации для этих типов отдельно компилируются в Matrix.cpp
    extern template class Matrix<int>;
    extern template class Matrix<double>;
}
