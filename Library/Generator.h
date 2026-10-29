#pragma once

namespace miit::algebra
{
    /**
     * @brief задает общий способ получения целого числа
     */
    class Generator
    {
    public:
        /**
         * @brief освобождает ресурсы генератора
         */
        virtual ~Generator() = 0;

        /**
         * @brief получает следующее число для заполнения массива
         * @return сгенерированное значение
         */
        virtual int generate() const = 0;
    };
}
