#include <cstdio>
#include <limits>
#include <climits>
#include <source_location>
#include <stdint.h>
#include <initializer_list>
#include <type_traits_>

#define LIMITS_TEST_PRINT(TYPE, FORMAT)             \
printf(#TYPE " " FORMAT " " FORMAT " " FORMAT "\n", \
    std::numeric_limits<TYPE>::lowest(),            \
    std::numeric_limits<TYPE>::min(),               \
    std::numeric_limits<TYPE>::max());              \


void testLimits() {
    printf("type rtti-type lowest() min() max()\n");
    LIMITS_TEST_PRINT(char, "%d");
    LIMITS_TEST_PRINT(signed char, "%d");
    LIMITS_TEST_PRINT(unsigned char, "%d");
    LIMITS_TEST_PRINT(wchar_t, "%d");
    LIMITS_TEST_PRINT(char8_t, "%d");
    LIMITS_TEST_PRINT(char16_t, "%d");
    LIMITS_TEST_PRINT(char32_t, "%d");
    LIMITS_TEST_PRINT(short, "%d");
    LIMITS_TEST_PRINT(unsigned short, "%d");
    LIMITS_TEST_PRINT(int, "%d");
    LIMITS_TEST_PRINT(unsigned int, "%u");
    LIMITS_TEST_PRINT(long, "%ld");
    LIMITS_TEST_PRINT(unsigned long, "%lu");
    LIMITS_TEST_PRINT(long long, "%lld");
    LIMITS_TEST_PRINT(unsigned long long, "%llu");
}

#define CLIMITS_TEST_PRINT(OPTION, FORMAT)  \
printf(#OPTION " " FORMAT "\n", OPTION);    \


void testCLimits() {
    CLIMITS_TEST_PRINT(CHAR_BIT, "%d");
    CLIMITS_TEST_PRINT(MB_LEN_MAX, "%d");
    CLIMITS_TEST_PRINT(CHAR_MIN, "%d");
    CLIMITS_TEST_PRINT(CHAR_MAX, "%d");
    CLIMITS_TEST_PRINT(SCHAR_MIN, "%d");
    CLIMITS_TEST_PRINT(SHRT_MIN, "%d");
    CLIMITS_TEST_PRINT(INT_MIN, "%d");
    CLIMITS_TEST_PRINT(LONG_MIN, "%ld");
    CLIMITS_TEST_PRINT(LLONG_MIN, "%lld");
    CLIMITS_TEST_PRINT(SCHAR_MAX, "%d");
    CLIMITS_TEST_PRINT(SHRT_MAX, "%d");
    CLIMITS_TEST_PRINT(INT_MAX, "%d");
    CLIMITS_TEST_PRINT(LONG_MAX, "%ld");
    CLIMITS_TEST_PRINT(LLONG_MAX, "%lld");
    CLIMITS_TEST_PRINT(UCHAR_MAX, "%u");
    CLIMITS_TEST_PRINT(USHRT_MAX, "%u");
    CLIMITS_TEST_PRINT(UINT_MAX, "%u");
    CLIMITS_TEST_PRINT(ULONG_MAX, "%lu");
    CLIMITS_TEST_PRINT(ULLONG_MAX, "%llu");
}

void log(const char *message, const std::source_location& location = std::source_location::current())
{
    printf("Debug: %s:%d %s\n", location.file_name(), location.line(), message);
}

void testSourceLocation() {
    log("hello");
    log("world");
    log("test", std::source_location::current());
    log("test", std::source_location::current());
}

void testStdint() {
    uint8_t a;
    uint16_t b;
    uint32_t c;
    uint64_t d;
    (void) a;
    (void) b;
    (void) c;
    (void) d;

    printf("INT_LEAST32_MIN %d\n", INT_LEAST32_MIN);
}

void testInitializerList(std::initializer_list<int> l) {
    for (const int *iter = std::begin(l); iter != std::end(l); iter++)
        printf("%d ", *iter);
    printf("\n");

    for (int x : l)
        printf("%d ", x);
    printf("\n");

    // Implicit bounding
    for (auto x : {-1, -2, -3})
        printf("%d ", x);
    printf("\n");

    auto al = {10, 11, 12};

    printf("The list bound to auto has size() = %ld\n", al.size());
}

int main(int argc, char **argv) {
    testLimits();
    testCLimits();
    testSourceLocation();
    testStdint();
    testInitializerList({1, 2, 3, 4, 10});
    return 0;
}
