#include "ConstantGenerator.h"

namespace miit::algebra
{
    ConstantGenerator::ConstantGenerator(const int value)
        : value{value}
    {
    }

    int ConstantGenerator::generate() const
    {
        return value;
    }
}
