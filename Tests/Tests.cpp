#include "Tests.h"

#include "ConstantGenerator.h"
#include "Exercise.h"
#include "IStreamGenerator.h"
#include "Matrix.h"
#include "MatrixIO.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

#include <cmath>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    using namespace miit::algebra;

    /**
     * @brief завершает текущий тест ошибкой если условие не выполнено
     * @param condition проверяемое условие
     * @param message описание ошибки
     */
    void require(const bool condition, const char* message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    /**
     * @brief создаёт матрицу с заранее известными значениями для теста
     * @param rows количество строк
     * @param columns количество столбцов
     * @param values значения по строкам
     * @return заполненная матрица
     */
    Matrix<int> makeMatrix(const std::size_t rows, const std::size_t columns,
        const std::initializer_list<int> values)
    {
        require(values.size() == rows * columns, "неверные данные самого теста");
        Matrix<int> matrix(rows, columns);
        auto value = values.begin();
        for (std::size_t i = 0; i < rows; ++i)
        {
            for (std::size_t j = 0; j < columns; ++j)
            {
                matrix.at(i, j) = *value++;
            }
        }
        return matrix;
    }

    /**
     * @brief сравнивает размеры и все элементы двух матриц
     * @param actual полученная матрица
     * @param expected ожидаемая матрица
     * @param message описание ошибки
     */
    void requireEqual(const Matrix<int>& actual, const Matrix<int>& expected,
        const char* message)
    {
        require(actual.getRows() == expected.getRows(), message);
        require(actual.getColumns() == expected.getColumns(), message);
        for (std::size_t i = 0; i < actual.getRows(); ++i)
        {
            for (std::size_t j = 0; j < actual.getColumns(); ++j)
            {
                require(actual.at(i, j) == expected.at(i, j), message);
            }
        }
    }

    /**
     * @brief проверяет матрицу созданную конструктором по умолчанию
     */
    void testDefaultMatrix()
    {
        const Matrix<int> matrix;
        require(matrix.empty(), "матрица по умолчанию должна быть пустой");
        require(matrix.getRows() == 0 && matrix.getColumns() == 0,
            "размер пустой матрицы должен быть 0 на 0");
        require(matrix.toString().empty(), "строка пустой матрицы должна быть пустой");
    }

    /**
     * @brief проверяет размер и начальные значения элементов
     */
    void testSizeConstructor()
    {
        const Matrix<int> zero(2, 3);
        const Matrix<int> constant(2, 3, -7);
        requireEqual(zero, makeMatrix(2, 3, {0, 0, 0, 0, 0, 0}),
            "элементы не заполнены нулями");
        requireEqual(constant, makeMatrix(2, 3, {-7, -7, -7, -7, -7, -7}),
            "элементы не заполнены начальным значением");
    }

    /**
     * @brief проверяет допустимые размеры пустой и непустой матрицы
     */
    void testInvalidDimensions()
    {
        require(Matrix<int>(0, 0).empty(), "размер 0 на 0 должен быть допустим");
        int errors = 0;
        try
        {
            Matrix<int> matrix(0, 3);
        }
        catch (const std::invalid_argument&)
        {
            ++errors;
        }
        try
        {
            Matrix<int> matrix(3, 0);
        }
        catch (const std::invalid_argument&)
        {
            ++errors;
        }
        require(errors == 2, "конструктор принял некорректный размер");
    }

    /**
     * @brief проверяет что конструктор копирования создаёт независимую матрицу
     */
    void testCopyConstructor()
    {
        const Matrix<int> original = makeMatrix(2, 2, {1, 2, 3, 4});
        Matrix<int> copy(original);
        requireEqual(copy, original, "конструктор копирования потерял значения");
        copy.at(0, 0) = 100;
        require(original.at(0, 0) == 1, "изменение копии изменило исходную матрицу");
        const Matrix<int> empty;
        require(Matrix<int>(empty).empty(), "не удалось скопировать пустую матрицу");
    }

    /**
     * @brief проверяет копирующее присваивание и возвращаемую ссылку
     */
    void testCopyAssignment()
    {
        const Matrix<int> source = makeMatrix(2, 3, {1, 2, 3, 4, 5, 6});
        Matrix<int> target(1, 1, 9);
        require(&(target = source) == &target, "присваивание должно возвращать сам объект");
        requireEqual(target, source, "присваивание не заменило размер и значения");
        target.at(0, 0) = 100;
        require(source.at(0, 0) == 1, "присваивание создало общие элементы");
        Matrix<int> another;
        another = target = source;
        requireEqual(another, source, "цепочка присваиваний работает неверно");
    }

    /**
     * @brief проверяет присваивание объекта самому себе
     */
    void testSelfAssignment()
    {
        Matrix<int> matrix = makeMatrix(2, 2, {1, 2, 3, 4});
        const Matrix<int> expected(matrix);
        Matrix<int>& alias = matrix;
        matrix = alias;
        requireEqual(matrix, expected, "копирующее самоприсваивание изменило матрицу");
        matrix = std::move(alias);
        requireEqual(matrix, expected, "перемещающее самоприсваивание изменило матрицу");
    }

    /**
     * @brief проверяет конструктор перемещения и состояние исходного объекта
     */
    void testMoveConstructor()
    {
        Matrix<int> source = makeMatrix(2, 2, {1, 2, 3, 4});
        const Matrix<int> expected(source);
        Matrix<int> target(std::move(source));
        requireEqual(target, expected, "конструктор перемещения потерял данные");
        require(source.empty() && source.getRows() == 0 && source.getColumns() == 0,
            "исходный объект после перемещения должен стать пустым");
        source = Matrix<int>(1, 1, 8);
        require(source.at(0, 0) == 8, "исходный объект нельзя использовать повторно");
    }

    /**
     * @brief проверяет перемещающее присваивание
     */
    void testMoveAssignment()
    {
        Matrix<int> source = makeMatrix(2, 3, {1, 2, 3, 4, 5, 6});
        const Matrix<int> expected(source);
        Matrix<int> target(1, 1, -1);
        require(&(target = std::move(source)) == &target,
            "перемещающее присваивание должно возвращать сам объект");
        requireEqual(target, expected, "перемещающее присваивание потеряло данные");
        require(source.empty() && source.getRows() == 0 && source.getColumns() == 0,
            "источник после перемещающего присваивания должен стать пустым");
    }

    /**
     * @brief проверяет чтение и изменение элементов через индекс и метод at
     */
    void testElementAccess()
    {
        Matrix<int> matrix(2, 3);
        matrix[1][2] = 12;
        matrix.at(0, 1) = -5;
        const Matrix<int>& constant = matrix;
        require(matrix.at(1, 2) == 12, "оператор индексации не изменил элемент");
        require(constant[0][1] == -5, "константный оператор индексации читает неверно");
        require(constant.at(1, 2) == 12, "константный метод at читает неверно");
    }

    /**
     * @brief проверяет защиту от выхода за границы в методе at и первом индексе
     */
    void testBounds()
    {
        Matrix<int> matrix(2, 3);
        const Matrix<int>& constant = matrix;
        int errors = 0;
        try
        {
            matrix.at(2, 0);
        }
        catch (const std::out_of_range&)
        {
            ++errors;
        }
        try
        {
            constant.at(0, 3);
        }
        catch (const std::out_of_range&)
        {
            ++errors;
        }
        try
        {
            matrix[2];
        }
        catch (const std::out_of_range&)
        {
            ++errors;
        }
        try
        {
            constant[2];
        }
        catch (const std::out_of_range&)
        {
            ++errors;
        }
        require(errors == 4, "выход за границы матрицы не обнаружен");
    }

    /**
     * @brief проверяет точный формат строкового представления
     */
    void testToString()
    {
        const Matrix<int> matrix = makeMatrix(2, 3, {1, -2, 30, 0, 5, -6});
        require(matrix.toString() == "1 -2 30\n0 5 -6\n",
            "нарушен формат строкового представления матрицы");
    }

    /**
     * @brief проверяет работу шаблона с вещественным типом
     */
    void testDoubleMatrix()
    {
        Matrix<double> matrix(1, 2, 1.25);
        matrix[0][1] = -2.5;
        Matrix<double> copy(matrix);
        require(copy.toString() == "1.250000 -2.500000\n",
            "шаблон матрицы неверно работает с double");
        copy.at(0, 0) = 10.0;
        require(matrix.at(0, 0) == 1.25, "копия double матрицы не независима");
        Matrix<double> moved(std::move(copy));
        require(moved.at(0, 0) == 10.0 && copy.empty(),
            "перемещение double матрицы работает неверно");
    }

    /**
     * @brief проверяет генераторы нулей и постоянного значения
     */
    void testConstantAndZeroGenerators()
    {
        ConstantGenerator constant(-17);
        ZeroGenerator zero;
        Generator& base = constant;
        for (int i = 0; i < 5; ++i)
        {
            require(base.generate() == -17, "генератор константы изменил значение");
            require(zero.generate() == 0, "генератор нулей вернул не ноль");
        }
    }

    /**
     * @brief проверяет границы случайных чисел и недопустимый диапазон
     */
    void testRandomGenerator()
    {
        RandomGenerator random(-3, 5);
        for (int i = 0; i < 200; ++i)
        {
            const int value = random.generate();
            require(value >= -3 && value <= 5, "случайное число вышло за диапазон");
        }
        RandomGenerator minimum(std::numeric_limits<int>::min(),
            std::numeric_limits<int>::min());
        RandomGenerator maximum(std::numeric_limits<int>::max(),
            std::numeric_limits<int>::max());
        require(minimum.generate() == std::numeric_limits<int>::min(),
            "диапазон из единственного минимального int работает неверно");
        require(maximum.generate() == std::numeric_limits<int>::max(),
            "диапазон из единственного максимального int работает неверно");
        bool error = false;
        try
        {
            RandomGenerator invalid(5, -3);
        }
        catch (const std::invalid_argument&)
        {
            error = true;
        }
        require(error, "генератор принял перевёрнутый диапазон");
    }

    /**
     * @brief проверяет заполнение матрицы константой и нулями
     */
    void testFill()
    {
        Matrix<int> matrix(2, 2);
        ConstantGenerator constant(7);
        ZeroGenerator zero;
        matrix.fill(constant);
        requireEqual(matrix, makeMatrix(2, 2, {7, 7, 7, 7}),
            "заполнение константой работает неверно");
        matrix.fill(zero);
        requireEqual(matrix, makeMatrix(2, 2, {0, 0, 0, 0}),
            "заполнение нулями работает неверно");
        Matrix<double> real(1, 2);
        real.fill(constant);
        require(real.at(0, 0) == 7.0 && real.at(0, 1) == 7.0,
            "генератор не заполняет double матрицу");
        Matrix<int> empty;
        empty.fill(constant);
        require(empty.empty(), "заполнение изменило пустую матрицу");
    }

    /**
     * @brief проверяет заполнение по строкам из переданного потока
     */
    void testFillFromInput()
    {
        std::istringstream input("1 -2 3 4 5 6");
        IStreamGenerator generator(input);
        Matrix<int> matrix(2, 3);
        matrix.fill(generator);
        requireEqual(matrix, makeMatrix(2, 3, {1, -2, 3, 4, 5, 6}),
            "неверный порядок заполнения из генератора ввода");
    }

    /**
     * @brief проверяет сохранение матрицы если генератор прервал заполнение
     */
    void testFillFailure()
    {
        Matrix<int> matrix(2, 2, 7);
        const Matrix<int> before(matrix);
        std::istringstream input("1 2 неверно 4");
        IStreamGenerator generator(input);
        bool error = false;
        try
        {
            matrix.fill(generator);
        }
        catch (const std::runtime_error&)
        {
            error = true;
        }
        require(error, "ошибка генератора не передана вызывающему коду");
        requireEqual(matrix, before, "ошибка генератора частично изменила матрицу");
    }

    /**
     * @brief проверяет целые числа знаки и крайние значения при ручном вводе
     */
    void testInputGeneratorValid()
    {
        const std::string values = "  +12 -7 -0 +0003 "
            + std::to_string(std::numeric_limits<int>::min()) + " "
            + std::to_string(std::numeric_limits<int>::max());
        std::istringstream input(values);
        IStreamGenerator generator(input);
        require(generator.generate() == 12, "не прочитано число с плюсом");
        require(generator.generate() == -7, "не прочитано отрицательное число");
        require(generator.generate() == 0, "не прочитан отрицательный ноль");
        require(generator.generate() == 3, "не прочитано число с ведущими нулями");
        require(generator.generate() == std::numeric_limits<int>::min(),
            "не прочитан минимальный int");
        require(generator.generate() == std::numeric_limits<int>::max(),
            "не прочитан максимальный int");
    }

    /**
     * @brief проверяет ошибочные токены нехватку данных и переполнение ввода
     */
    void testInputGeneratorInvalid()
    {
        const std::string invalid[] = {
            "--1", "+", "+-1", "слово", "", "   ",
            std::to_string(std::numeric_limits<int>::max()) + "0",
            std::to_string(std::numeric_limits<int>::min()) + "0"
        };
        for (const std::string& value : invalid)
        {
            std::istringstream input(value);
            IStreamGenerator generator(input);
            bool error = false;
            try
            {
                generator.generate();
            }
            catch (const std::runtime_error&)
            {
                error = true;
            }
            require(error, "генератор принял ошибочный ввод или нехватку данных");
        }
        std::istringstream input("1");
        IStreamGenerator generator(input);
        require(generator.generate() == 1, "не прочитано последнее число");
        bool error = false;
        try
        {
            generator.generate();
        }
        catch (const std::runtime_error&)
        {
            error = true;
        }
        require(error, "генератор не обнаружил конец потока");
    }

    /**
     * @brief проверяет вывод матрицы в произвольный поток и цепочку вывода
     */
    void testOutputOperator()
    {
        const Matrix<int> matrix = makeMatrix(2, 2, {1, -2, 3, 4});
        std::ostringstream output;
        require(&(output << matrix) == &output, "оператор вывода вернул другой поток");
        output << matrix << "конец";
        require(output.str() == "1 -2\n3 4\n1 -2\n3 4\nконец",
            "оператор вывода неверно записал матрицу");
    }

    /**
     * @brief проверяет ввод элементов в матрицу известного размера
     */
    void testInputOperator()
    {
        Matrix<int> first(1, 2);
        Matrix<int> second(2, 1);
        std::istringstream input("1 -2 3 4");
        require(&(input >> first) == &input, "оператор ввода вернул другой поток");
        input >> second;
        require(!input.fail(), "корректный ввод установил признак ошибки");
        requireEqual(first, makeMatrix(1, 2, {1, -2}), "не прочитана первая матрица");
        requireEqual(second, makeMatrix(2, 1, {3, 4}), "не прочитана вторая матрица");
    }

    /**
     * @brief проверяет отсутствие частичной записи при ошибке потокового ввода
     */
    void testInputOperatorFailure()
    {
        const std::string invalid[] = {"1 2", "1 x 3 4"};
        for (const std::string& value : invalid)
        {
            Matrix<int> matrix(2, 2, 9);
            const Matrix<int> before(matrix);
            std::istringstream input(value);
            input >> matrix;
            require(input.fail(), "ошибочный ввод не установил failbit");
            requireEqual(matrix, before, "ошибка потока частично изменила матрицу");
        }
    }

    /**
     * @brief проверяет запись и обратное чтение целой и вещественной матрицы
     */
    void testStreamRoundTrip()
    {
        const Matrix<int> source = makeMatrix(2, 2, {1, -2, 0, 4});
        Matrix<int> copy(2, 2);
        std::stringstream stream;
        stream << source;
        stream >> copy;
        require(!stream.fail(), "не удалось прочитать ранее выведенную матрицу");
        requireEqual(copy, source, "вывод и ввод изменили целую матрицу");
        Matrix<double> real(1, 2, 1.25);
        real.at(0, 1) = -2.5;
        Matrix<double> realCopy(1, 2);
        std::stringstream realStream;
        realStream << real;
        realStream >> realCopy;
        require(!realStream.fail() && realCopy.at(0, 0) == 1.25
            && realCopy.at(0, 1) == -2.5, "вывод и ввод изменили вещественную матрицу");
    }

    /**
     * @brief проверяет возведение в квадрат только первых трёх столбцов
     */
    void testTask1Result()
    {
        Matrix<int> matrix = makeMatrix(2, 5, {
            -2, 3, 0, -4, std::numeric_limits<int>::min(),
            4, -5, 6, 7, std::numeric_limits<int>::max()
        });
        ZeroGenerator generator;
        Task1 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(2, 5, {
            4, 9, 0, -4, std::numeric_limits<int>::min(),
            16, 25, 36, 7, std::numeric_limits<int>::max()
        }), "первое задание изменило неверные элементы");
    }

    /**
     * @brief проверяет отказ от первого задания при нехватке столбцов
     */
    void testTask1TooFewColumns()
    {
        Matrix<int> matrix = makeMatrix(2, 2, {1, 2, 3, 4});
        const Matrix<int> before(matrix);
        ZeroGenerator generator;
        Task1 task(matrix, generator);
        bool error = false;
        try
        {
            task.solve();
        }
        catch (const std::invalid_argument&)
        {
            error = true;
        }
        require(error, "первое задание не обнаружило нехватку столбцов");
        requireEqual(matrix, before, "нехватка столбцов частично изменила матрицу");
    }

    /**
     * @brief проверяет переполнение квадрата и сохранение всей матрицы при ошибке
     */
    void testTask1Overflow()
    {
        const int extremes[] = {
            std::numeric_limits<int>::min(), std::numeric_limits<int>::max()
        };
        for (const int extreme : extremes)
        {
            Matrix<int> matrix = makeMatrix(2, 3, {2, 3, 4, 5, 6, extreme});
            const Matrix<int> before(matrix);
            ZeroGenerator generator;
            Task1 task(matrix, generator);
            bool error = false;
            try
            {
                task.solve();
            }
            catch (const std::overflow_error&)
            {
                error = true;
            }
            require(error, "первое задание не обнаружило переполнение квадрата");
            requireEqual(matrix, before, "переполнение частично изменило матрицу");
        }
    }

    /**
     * @brief проверяет последний допустимый квадрат и следующее за ним число
     */
    void testTask1SquareBoundary()
    {
        const int boundary = static_cast<int>(
            std::sqrt(static_cast<double>(std::numeric_limits<int>::max())));
        Matrix<int> matrix = makeMatrix(1, 3, {boundary, -boundary, 0});
        ZeroGenerator generator;
        Task1 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(1, 3, {boundary * boundary, boundary * boundary, 0}),
            "первое задание отвергло или исказило допустимый квадрат");
        matrix = makeMatrix(1, 3, {1, 2, boundary + 1});
        const Matrix<int> before(matrix);
        bool error = false;
        try
        {
            task.solve();
        }
        catch (const std::overflow_error&)
        {
            error = true;
        }
        require(error, "первое задание пропустило первый недопустимый квадрат");
        requireEqual(matrix, before, "граничное переполнение частично изменило матрицу");
    }

    /**
     * @brief проверяет первое задание для пустой матрицы
     */
    void testTask1Empty()
    {
        Matrix<int> matrix;
        ZeroGenerator generator;
        Task1 task(matrix, generator);
        task.solve();
        require(matrix.empty(), "первое задание изменило пустую матрицу");
    }

    /**
     * @brief проверяет вставку первой строки в матрице из одной строки
     */
    void testTask2OneRow()
    {
        Matrix<int> matrix = makeMatrix(1, 2, {1, -2});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(2, 2, {1, -2, 1, -2}),
            "неверная вставка в матрице из одной строки");
    }

    /**
     * @brief проверяет вставку после первой строки и отсутствие вставки после второй
     */
    void testTask2TwoRows()
    {
        Matrix<int> matrix = makeMatrix(2, 2, {1, 2, 3, 4});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(3, 2, {1, 2, 1, 2, 3, 4}),
            "неверная вставка в матрице из двух строк");
    }

    /**
     * @brief проверяет что номера нечётных строк относятся к исходной матрице
     */
    void testTask2ThreeRows()
    {
        Matrix<int> matrix = makeMatrix(3, 2, {1, 2, 3, 4, 5, 6});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(5, 2, {1, 2, 1, 2, 3, 4, 5, 6, 1, 2}),
            "при трёх строках вставлены неверные строки или сместилась нумерация");
    }

    /**
     * @brief проверяет второе задание для четырёх строк
     */
    void testTask2FourRows()
    {
        Matrix<int> matrix = makeMatrix(4, 1, {10, 20, 30, 40});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(6, 1, {10, 10, 20, 30, 10, 40}),
            "неверная вставка в матрице из четырёх строк");
    }

    /**
     * @brief проверяет вставку после первой третьей и пятой исходных строк
     */
    void testTask2FiveRows()
    {
        Matrix<int> matrix = makeMatrix(5, 1, {10, 20, 30, 40, 50});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(8, 1, {10, 10, 20, 30, 10, 40, 50, 10}),
            "неверная вставка в матрице из пяти строк");
    }

    /**
     * @brief проверяет второе задание для пустой матрицы
     */
    void testTask2Empty()
    {
        Matrix<int> matrix;
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        require(matrix.empty() && matrix.getRows() == 0 && matrix.getColumns() == 0,
            "второе задание изменило пустую матрицу");
    }

    /**
     * @brief проверяет что нулевая первая строка тоже копируется и не пропадает
     */
    void testTask2ZeroFirstRow()
    {
        Matrix<int> matrix = makeMatrix(3, 2, {0, 0, -3, -4, 5, 6});
        ZeroGenerator generator;
        Task2 task(matrix, generator);
        task.solve();
        requireEqual(matrix, makeMatrix(5, 2, {0, 0, 0, 0, -3, -4, 5, 6, 0, 0}),
            "второе задание потеряло или неверно скопировало нулевую строку");
    }

    /**
     * @brief проверяет агрегацию матрицы и генератора и виртуальный вызов solve
     */
    void testExercisePolymorphism()
    {
        Matrix<int> first(1, 4);
        Matrix<int> second(2, 1);
        ConstantGenerator generator(-2);
        Task1 task1(first, generator);
        Task2 task2(second, generator);
        Exercise* exercises[] = {&task1, &task2};
        for (Exercise* exercise : exercises)
        {
            exercise->fill();
            exercise->solve();
        }
        requireEqual(first, makeMatrix(1, 4, {4, 4, 4, -2}),
            "виртуальный вызов не выполнил первое задание над переданной матрицей");
        requireEqual(second, makeMatrix(3, 1, {-2, -2, -2}),
            "виртуальный вызов не выполнил второе задание над переданной матрицей");
    }

    /**
     * @brief запускает отдельную тестовую функцию и считает результат
     * @param name название теста
     * @param test указатель на тестовую функцию
     * @param total число запущенных тестов
     * @param failed число неудачных тестов
     */
    void runTest(const char* name, void (*test)(), int& total, int& failed)
    {
        ++total;
        try
        {
            test();
            std::cout << "[OK] " << name << '\n';
        }
        catch (const std::exception& error)
        {
            ++failed;
            std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
        }
        catch (...)
        {
            ++failed;
            std::cerr << "[FAIL] " << name << ": неизвестная ошибка\n";
        }
    }
}

int Tests::runAll()
{
    int total = 0;
    int failed = 0;
    runTest("конструктор по умолчанию", testDefaultMatrix, total, failed);
    runTest("размер и начальные значения", testSizeConstructor, total, failed);
    runTest("проверка размеров", testInvalidDimensions, total, failed);
    runTest("конструктор копирования", testCopyConstructor, total, failed);
    runTest("копирующее присваивание", testCopyAssignment, total, failed);
    runTest("самоприсваивание", testSelfAssignment, total, failed);
    runTest("конструктор перемещения", testMoveConstructor, total, failed);
    runTest("перемещающее присваивание", testMoveAssignment, total, failed);
    runTest("доступ к элементам", testElementAccess, total, failed);
    runTest("выход за границы", testBounds, total, failed);
    runTest("строковое представление", testToString, total, failed);
    runTest("матрица double", testDoubleMatrix, total, failed);
    runTest("генераторы константы и нулей", testConstantAndZeroGenerators, total, failed);
    runTest("случайный генератор", testRandomGenerator, total, failed);
    runTest("заполнение матрицы", testFill, total, failed);
    runTest("заполнение из потока", testFillFromInput, total, failed);
    runTest("ошибка при заполнении", testFillFailure, total, failed);
    runTest("корректный ручной ввод", testInputGeneratorValid, total, failed);
    runTest("ошибки ручного ввода", testInputGeneratorInvalid, total, failed);
    runTest("оператор вывода", testOutputOperator, total, failed);
    runTest("оператор ввода", testInputOperator, total, failed);
    runTest("ошибка потокового ввода", testInputOperatorFailure, total, failed);
    runTest("вывод и обратный ввод", testStreamRoundTrip, total, failed);
    runTest("task1 первые три столбца", testTask1Result, total, failed);
    runTest("task1 нехватка столбцов", testTask1TooFewColumns, total, failed);
    runTest("task1 переполнение", testTask1Overflow, total, failed);
    runTest("task1 граница квадрата", testTask1SquareBoundary, total, failed);
    runTest("task1 пустая матрица", testTask1Empty, total, failed);
    runTest("task2 одна строка", testTask2OneRow, total, failed);
    runTest("task2 две строки", testTask2TwoRows, total, failed);
    runTest("task2 три строки", testTask2ThreeRows, total, failed);
    runTest("task2 четыре строки", testTask2FourRows, total, failed);
    runTest("task2 пять строк", testTask2FiveRows, total, failed);
    runTest("task2 пустая матрица", testTask2Empty, total, failed);
    runTest("task2 нулевая первая строка", testTask2ZeroFirstRow, total, failed);
    runTest("агрегация и полиморфизм", testExercisePolymorphism, total, failed);

    std::cout << "\nвсего тестов: " << total << ", ошибок: " << failed << '\n';
    return failed;
}
