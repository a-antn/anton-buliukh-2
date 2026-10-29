#include "Exercise.h"

namespace miit::algebra
{
    Exercise::Exercise(Matrix<int>& matrix, const Generator& generator)
        : matrix{matrix}, generator{generator}
    {
    }

    Exercise::~Exercise() = default;
}
