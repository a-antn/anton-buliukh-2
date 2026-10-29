#include "Matrix.h"
#include <stdexcept>
#include <utility>

namespace miit::algebra
{
    template <typename T>
    Matrix<T>::Matrix()
        : rows{0}, columns{0}, data{}
    {
    }

    template <typename T>
    Matrix<T>::Matrix(const std::size_t rows, const std::size_t columns)
        : rows{rows}, columns{columns}, data{}
    {
        if ((rows == 0) != (columns == 0))
        {
            throw std::invalid_argument("оба размера должны быть положительными либо равны нулю");
        }

        if (columns != 0 && rows > data.max_size() / columns)
        {
            throw std::length_error("слишком большой размер массива");
        }

        data.resize(rows * columns);
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T>& other)
        : rows{other.rows}, columns{other.columns}, data{other.data}
    {
    }

    template <typename T>
    Matrix<T>::Matrix(Matrix<T>&& other) noexcept
        : rows{other.rows},
          columns{other.columns},
          data{std::move(other.data)}
    {
        other.rows = 0;
        other.columns = 0;
        other.data.clear();
    }

    template <typename T>
    Matrix<T>::~Matrix() = default;

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
    {
        if (this != &other)
        {
            data = other.data;
            rows = other.rows;
            columns = other.columns;
        }

        return *this;
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept
    {
        if (this != &other)
        {
            data = std::move(other.data);
            rows = other.rows;
            columns = other.columns;
            other.rows = 0;
            other.columns = 0;
            other.data.clear();
        }

        return *this;
    }

    template <typename T>
    std::size_t Matrix<T>::getRows() const noexcept
    {
        return rows;
    }

    template <typename T>
    std::size_t Matrix<T>::getColumns() const noexcept
    {
        return columns;
    }

    template <typename T>
    bool Matrix<T>::empty() const noexcept
    {
        return data.empty();
    }

    template <typename T>
    T* Matrix<T>::operator[](const std::size_t row)
    {
        if (row >= rows)
        {
            throw std::out_of_range("индекс строки выходит за границы массива");
        }

        return data.data() + row * columns;
    }

    template <typename T>
    const T* Matrix<T>::operator[](const std::size_t row) const
    {
        if (row >= rows)
        {
            throw std::out_of_range("индекс строки выходит за границы массива");
        }

        return data.data() + row * columns;
    }

    template <typename T>
    void Matrix<T>::fill(const Generator& generator)
    {
        std::vector<T> generated(data.size());

        for (T& value : generated)
        {
            value = static_cast<T>(generator.generate());
        }

        data.swap(generated);
    }

    template <typename T>
    std::string Matrix<T>::toString() const
    {
        std::string result;

        for (std::size_t i = 0; i < rows; ++i)
        {
            for (std::size_t j = 0; j < columns; ++j)
            {
                if (j != 0)
                {
                    result += ' ';
                }

                result += std::to_string(data[i * columns + j]);
            }

            result += '\n';
        }

        return result;
    }

    template <typename T>
    void Matrix<T>::swap(Matrix<T>& other) noexcept
    {
        std::swap(rows, other.rows);
        std::swap(columns, other.columns);
        data.swap(other.data);
    }

    template class Matrix<int>;
    template class Matrix<double>;
}
