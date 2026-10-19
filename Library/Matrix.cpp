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
    Matrix<T>::Matrix(const std::size_t rows, const std::size_t columns, const T& value)
        : rows{rows}, columns{columns}, data{}
    {
        if ((rows == 0) != (columns == 0))
        {
            throw std::invalid_argument("оба размера должны быть положительными либо равны нулю");
        }

        if (rows > data.max_size() || columns > std::vector<T>().max_size())
        {
            throw std::length_error("слишком большой размер массива");
        }

        data.assign(rows, std::vector<T>(columns, value));
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
            Matrix<T> copy(other);
            swap(copy);
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

        return data[row].data();
    }

    template <typename T>
    const T* Matrix<T>::operator[](const std::size_t row) const
    {
        if (row >= rows)
        {
            throw std::out_of_range("индекс строки выходит за границы массива");
        }

        return data[row].data();
    }

    template <typename T>
    T& Matrix<T>::at(const std::size_t row, const std::size_t column)
    {
        if (row >= rows || column >= columns)
        {
            throw std::out_of_range("индексы элемента выходят за границы массива");
        }

        return data[row][column];
    }

    template <typename T>
    const T& Matrix<T>::at(const std::size_t row, const std::size_t column) const
    {
        if (row >= rows || column >= columns)
        {
            throw std::out_of_range("индексы элемента выходят за границы массива");
        }

        return data[row][column];
    }

    template <typename T>
    void Matrix<T>::fill(Generator& generator)
    {
        std::vector<std::vector<T>> generated(rows, std::vector<T>(columns));

        for (std::size_t i = 0; i < rows; ++i)
        {
            for (std::size_t j = 0; j < columns; ++j)
            {
                generated[i][j] = static_cast<T>(generator.generate());
            }
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

                result += std::to_string(data[i][j]);
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
