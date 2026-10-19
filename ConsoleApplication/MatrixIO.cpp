#include "MatrixIO.h"

#include <cstddef>
#include <istream>
#include <ostream>
#include <utility>

namespace miit::algebra
{
    template <typename T>
    std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix)
    {
        return out << matrix.toString();
    }

    template <typename T>
    std::istream& operator>>(std::istream& in, Matrix<T>& matrix)
    {
        Matrix<T> result(matrix.getRows(), matrix.getColumns());
        for (std::size_t i = 0; i < matrix.getRows(); ++i)
        {
            for (std::size_t j = 0; j < matrix.getColumns(); ++j)
            {
                if (!(in >> result[i][j]))
                {
                    return in;
                }
            }
        }
        matrix = std::move(result);
        return in;
    }

    template std::ostream& operator<<(std::ostream&, const Matrix<int>&);
    template std::ostream& operator<<(std::ostream&, const Matrix<double>&);
    template std::istream& operator>>(std::istream&, Matrix<int>&);
    template std::istream& operator>>(std::istream&, Matrix<double>&);
}
