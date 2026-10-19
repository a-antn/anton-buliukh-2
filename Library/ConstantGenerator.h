#pragma once

#include "Generator.h"

namespace miit::algebra
{
    /**
     * @brief заполняет массив одним заданным значением
     */
    class ConstantGenerator final : public Generator
    {
    private:
        int value;

    public:
        /**
         * @brief сохраняет значение для заполнения
         * @param value значение которое будет возвращать генератор
         */
        explicit ConstantGenerator(const int value);

        /**
         * @brief получает сохраненное значение
         * @return значение заданное при создании генератора
         */
        int generate() override;
    };
}
