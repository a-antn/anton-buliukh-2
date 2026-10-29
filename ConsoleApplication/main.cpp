#include "ConstantGenerator.h"
#include "Generator.h"
#include "IStreamGenerator.h"
#include "Matrix.h"
#include "MatrixIO.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace
{
    /**
     * @brief способы заполнения массива
     */
    enum FillMode { RANDOM = 1, MANUAL, ZERO, CONSTANT };

    /**
     * @brief считывает положительный размер массива
     * @param message текст приглашения к вводу
     * @return размер массива в виде size_t
     */
    std::size_t getSize(const char* message)
    {
        std::cout << message;
        miit::algebra::IStreamGenerator input;
        const int value = input.generate();
        if (value <= 0)
        {
            throw std::invalid_argument("размер должен быть больше нуля");
        }
        return static_cast<std::size_t>(value);
    }

    /**
     * @brief создает генератор выбранный пользователем
     * @return указатель который управляет временем жизни генератора
     */
    std::unique_ptr<miit::algebra::Generator> getGenerator()
    {
        using namespace miit::algebra;

        std::cout << "выберите способ заполнения массива:\n"
                  << RANDOM << " - случайными числами\n"
                  << MANUAL << " - вручную\n"
                  << ZERO << " - нулями\n"
                  << CONSTANT << " - постоянным значением\n";

        IStreamGenerator input;
        switch (input.generate())
        {
        case RANDOM:
        {
            std::cout << "введите минимум:\n";
            const int min = input.generate();
            std::cout << "введите максимум:\n";
            const int max = input.generate();
            return std::make_unique<RandomGenerator>(min, max);
        }
        case MANUAL:
            std::cout << "введите элементы по строкам через пробел или перенос строки:\n";
            return std::make_unique<IStreamGenerator>();
        case ZERO:
            return std::make_unique<ZeroGenerator>();
        case CONSTANT:
            std::cout << "введите постоянное значение:\n";
            return std::make_unique<ConstantGenerator>(input.generate());
        default:
            throw std::invalid_argument("неверный способ заполнения");
        }
    }
}

/**
 * @brief показывает заполнение матрицы и выполнение двух заданий
 * @return 0 при успешном завершении иначе 1
 */
int main()
{
    using namespace miit::algebra;

    try
    {
        const std::size_t rows = getSize("введите количество строк:\n");
        const std::size_t columns = getSize("введите количество столбцов не меньше трех:\n");
        if (columns < 3)
        {
            throw std::invalid_argument("для первого задания нужно не меньше трех столбцов");
        }

        const auto generator = getGenerator();

        Matrix<int> first(rows, columns);
        Task1 task1(first, *generator);
        first.fill(*generator);

        // оба задания получают одинаковые исходные значения
        const Matrix<int> original(first);
        Matrix<int> second;
        second = original;
        Task2 task2(second, *generator);

        std::cout << "\nисходная матрица:\n" << original;

        task1.solve();
        std::cout << "\nквадраты элементов первых трех столбцов:\n" << first;

        task2.solve();
        std::cout << "\nпервая строка после каждой нечетной исходной строки:\n" << second;

        // показываем чтение через оператор >> без повторного ввода с клавиатуры
        std::istringstream input(original.toString());
        Matrix<int> restored(rows, columns);
        if (!(input >> restored))
        {
            throw std::runtime_error("не удалось восстановить матрицу из строки");
        }
        std::cout << "\nматрица после чтения из строки через >>:\n" << restored;
        std::cout << "элемент с индексами [0][0]: " << restored[0][0] << '\n';

        Matrix<int> moved(std::move(restored));
        Matrix<int> assigned;
        assigned = std::move(moved);
        std::cout << "\nматрица после перемещения:\n" << assigned;
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "ошибка: " << error.what() << '\n';
        return 1;
    }
}
