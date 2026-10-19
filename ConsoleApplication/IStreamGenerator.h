#pragma once

#include "Generator.h"

#include <iostream>

namespace miit::algebra
{
    /**
     * @brief получает целые числа из переданного потока
     */
    class IStreamGenerator final : public Generator
    {
    private:
        std::istream& in;

    public:
        /**
         * @brief сохраняет поток из которого будут считываться значения
         * @param in поток ввода по умолчанию используется клавиатура
         */
        explicit IStreamGenerator(std::istream& in = std::cin);

        /**
         * @brief считывает целое число и проверяет результат ввода
         * @return считанное значение типа int
         * @throw std::runtime_error если ввод неверный или значения закончились
         */
        int generate() override;
    };
}
