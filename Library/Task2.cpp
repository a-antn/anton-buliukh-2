#include "Task2.h"
#include <limits>
#include <stdexcept>

namespace miit::algebra
{
    Task2::Task2(Matrix<int>& matrix, const Generator& generator)
        : Exercise{matrix, generator}
    {
    }

    void Task2::solve()
    {
        if (matrix.empty())
        {
            return;
        }

        const std::size_t rows = matrix.getRows();
        const std::size_t columns = matrix.getColumns();
        const std::size_t addedRows = rows / 2 + rows % 2;

        if (addedRows > std::numeric_limits<std::size_t>::max() - rows)
        {
            throw std::length_error("слишком много строк в результате");
        }

        Matrix<int> result(rows + addedRows, columns);
        std::size_t targetRow = 0;

        for (std::size_t i = 0; i < rows; ++i)
        {
            for (std::size_t j = 0; j < columns; ++j)
            {
                result[targetRow][j] = matrix[i][j];
            }

            ++targetRow;

            // индекс 0 соответствует первой строке индекс 2 соответствует третьей
            if (i % 2 == 0)
            {
                for (std::size_t j = 0; j < columns; ++j)
                {
                    result[targetRow][j] = matrix[0][j];
                }

                ++targetRow;
            }
        }

        matrix.swap(result);
    }
}
