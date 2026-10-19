#include "IStreamGenerator.h"

#include <stdexcept>

miit::algebra::IStreamGenerator::IStreamGenerator(std::istream& in)
    : in{in}
{
}

int miit::algebra::IStreamGenerator::generate()
{
    int value = 0;
    if (!(this->in >> value))
    {
        throw std::runtime_error("не удалось считать целое число");
    }
    return value;
}
