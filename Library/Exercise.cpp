#include "Exercise.h"

namespace miit::algebra
{
    Exercise::Exercise(Matrix<int>& matrix, Generator& generator)
        : matrix{matrix}, generator{generator}
    {
    }

    Exercise::~Exercise() = default;

    void Exercise::fill()
    {
        matrix.fill(generator);
    }
}
