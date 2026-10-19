#pragma once

#include "Generator.h"

namespace miit::algebra
{
    /**
     * @brief заполняет массив нулями
     */
    class ZeroGenerator final : public Generator
    {
    public:
        /**
         * @brief создает генератор нулей
         */
        ZeroGenerator();

        /**
         * @brief получает нулевое значение
         * @return ноль
         */
        int generate() override;
    };
}
