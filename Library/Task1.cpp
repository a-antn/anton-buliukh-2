#include "Task1.h"
#include <limits>
#include <stdexcept>

namespace miit::algebra
{
    Task1::Task1(Matrix<int>& matrix, Generator& generator)
        : Exercise{matrix, generator}
    {
    }

    void Task1::solve()
    {
        if (matrix.empty())
        {
            return;
        }

        constexpr std::size_t columnsToSquare = 3;

        if (matrix.getColumns() < columnsToSquare)
        {
            throw std::invalid_argument("для первого задания нужно минимум три столбца");
        }

        const int maxValue = std::numeric_limits<int>::max();

        // сначала проверяем все элементы чтобы при ошибке массив не изменился
        for (std::size_t i = 0; i < matrix.getRows(); ++i)
        {
            for (std::size_t j = 0; j < columnsToSquare; ++j)
            {
                const long long value = matrix[i][j];

                if (value * value > maxValue)
                {
                    throw std::overflow_error("квадрат элемента не помещается в int");
                }
            }
        }

        for (std::size_t i = 0; i < matrix.getRows(); ++i)
        {
            for (std::size_t j = 0; j < columnsToSquare; ++j)
            {
                matrix[i][j] *= matrix[i][j];
            }
        }
    }
}
