#include "Tests.h"

/**
 * @brief запускает проект модульных тестов
 * @return 0 если все тесты пройдены иначе 1
 */
int main()
{
    return Tests::runAll() == 0 ? 0 : 1;
}
