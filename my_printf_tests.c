#include <stdio.h>
#include <string.h>
#include "my_printf.h"
#define MAX 2147483647
#define MIN -2147483648

// test function declarations
void test_forInt();
void test_forHex();
void test_forString();
void test_forChar();
void test_forUpper();
void test_forLower();
void test_forLenStr();
void test_largeNumbers();
void test_negativeNumbers();
void test_edgeValues();
void test_dynamicWidthAndPrecision();
void test_combinedFlags();
void test_stringEdgeCases();
void test_specialCharacters();
void test_invalidCases();


// %d tests
void test_forInt() {
    printf("Testing Integers:\n");
    printf("Expected: %10.5d\n", 12345);
    my_printf("Result  : %10.5d\n", 12345);

    printf("Expected: %08d\n", -35);
    my_printf("Result  : %08d\n", -35);

    printf("Expected: %-10d\n", 9876);
    my_printf("Result  : %-10d\n", 9876);

    printf("Expected: %+d\n", 62);
    my_printf("Result  : %+d\n", 62);

    printf("Expected: %+d\n", -562);
    my_printf("Result  : %+d\n", -562);

    printf("Expected: %d\n", 0);
    my_printf("Result  : %d\n", 0);
}
// %x tests
void test_forHex() {
    printf("Testing Hexadecimal:\n");
    printf("Expected: %#8.4x\n", 0x1A3);
    my_printf("Result  : %#8.4x\n", 0x1A3);

    printf("Expected: %010x\n", 0xBEEF);
    my_printf("Result  : %010x\n", 0xBEEF);

    printf("Expected: %#4.2x\n", 0);
    my_printf("Result  : %#4.2x\n", 0);

    printf("Expected: %8x\n", 0x123);
    my_printf("Result  : %8x\n", 0x123);

    printf("Expected: %x\n", 0xFFFFFFFF);
    my_printf("Result  : %x\n", 0xFFFFFFFF);
}
// %s tests
void test_forString() {
    printf("Testing Strings:\n");
    printf("Expected: %-10s\n", "hello");
    my_printf("Result  : %-10s\n", "hello");

    printf("Expected: %.3s\n", "world");
    my_printf("Result  : %.3s\n", "world");

    printf("Expected: %10.5s\n", "abcdefgh");
    my_printf("Result  : %10.5s\n", "abcdefgh");
}
// %c tests
void test_forChar() {
    printf("Testing Characters:\n");
    printf("Expected: %5c\n", 'A');
    my_printf("Result  : %5c\n", 'A');

    printf("Expected: %-4c\n", 'B');
    my_printf("Result  : %-4c\n", 'B');

    printf("Expected: %03c\n", 'C');
    my_printf("Result  : %03c\n", 'C');

    printf("Expected: %8c\n", 'D');
    my_printf("Result  : %8c\n", 'D');
}

void test_forUpper() {
    printf("Testing Upper:\n");

    // make a normal string and an uppercase version to test the output of %U
    char str1[] = "Hello World!";
    char expected1[] = "HELLO WORLD!";
    printf("Expected: %s\n", expected1);
    my_printf("Result  : %U\n", str1);

    // test with an uppercase string
    char str2[] = "THIS IS A TEST!";
    printf("Expected: %s\n", str2);
    my_printf("Result  : %U\n", str2);

    // test with a string that has chars other than letters
    char str3[] = "Test123!@#";
    char expected3[] = "TEST123!@#";
    printf("Expected: %s\n", expected3);
    my_printf("Result  : %U\n", str3);
}

void test_forLower() {
    printf("Testing Lower:\n");

    // make a normal string and a lowercase version to test the output of %L
    char str1[] = "Hello World!";
    char expected1[] = "hello world!";
    printf("Expected: %s\n", expected1);
    my_printf("Result  : %L\n", str1);

    // test with a lowercase string
    char str2[] = "this is a test!";
    printf("Expected: %s\n", str2);
    my_printf("Result  : %L\n", str2);

    // test with a string that has chars other than letters
    char str3[] = "Test123!@#";
    char expected3[] = "test123!@#";
    printf("Expected: %s\n", expected3);
    my_printf("Result  : %L\n", str3);

}
void test_forLenStr() {
    printf("Testing LenStr:\n");

    // test with a basic string
    char str1[] = "Hello";
    printf("Expected: %d\n", 5);
    my_printf("Result  : %S\n", str1);  // Assuming %l uses the lenstr function

    // test with an empty string
    char str2[] = "";
    printf("Expected: %d\n", 0);
    my_printf("Result  : %S\n", str2);

    // test a string with dif types of chars
    char str3[] = "Test with spaces 123!@#";
    printf("Expected: %d\n", 23);
    my_printf("Result  : %S\n", str3);

    // test with a very long string
    char str4[] = "This is a string to test the %S to make sure it calculates the right len";
    printf("Expected: %d\n", 72);
    my_printf("Result  : %S\n", str4);
}


void test_largeNumbers() {
    printf("Testing large Numbers:\n");
    printf("Expected: %20d\n", 1234567890);
    my_printf("Result  : %20d\n", 1234567890);

    printf("Expected: %020d\n", 1234567890);
    my_printf("Result  : %020d\n", 1234567890);

    printf("Expected: %20.10d\n", 1234567890);
    my_printf("Result  : %20.10d\n", 1234567890);
}

void test_negativeNumbers() {
    printf("Testing Negative Numbers\n");
    printf("Expected: %-20d\n", -12445);
    my_printf("Result  : %-20d\n", -12445);

    printf("Expected: %020d\n", -18945);
    my_printf("Result  : %020d\n", -18945);

    printf("Expected: %+d\n", -99999);
    my_printf("Result  : %+d\n", -99999);
}

void test_edgeValues() {
    printf("Testing boundary values...\n");
    printf("Expected: %d\n", MAX);
    my_printf("Result  : %d\n", MAX);

    printf("Expected: %d\n", MIN);
    my_printf("Result  : %d\n", MIN);

    printf("Expected: %x\n", MAX);
    my_printf("Result  : %x\n", MAX);
}

void test_combinedFlags() {
    printf("Testing combined flags...\n");
    printf("Expected: %+010d\n", 760);
    my_printf("Result  : %+010d\n", 760);

    printf("Expected: %-#10x\n", 0xFEED);
    my_printf("Result  : %-#10x\n", 0xFEED);

}
void test_dynamicWidthAndPrecision() {
    printf("Testing dynamic width and precision...\n");

    // Dynamic width for integers
    printf("Expected: %*d\n", 6, 167);
    my_printf("Result  : %*d\n", 6, 167);

    printf("Expected: %*d\n", -8, -385);
    my_printf("Result  : %*d\n", -8, -385);

    // Dynamic precision for integers
    printf("Expected: %.*d\n", 4, 89);
    my_printf("Result  : %.*d\n", 4, 89);

    // Dynamic width and precision combined for integers
    printf("Expected: %*.*d\n", 7, 3, 22);
    my_printf("Result  : %*.*d\n", 7, 3, 22);

    // Dynamic width for strings
    printf("Expected: %*s\n", 10, "test");
    my_printf("Result  : %*s\n", 10, "test");

    // Dynamic precision for strings
    printf("Expected: %.*s\n", 4, "dynamic");
    my_printf("Result  : %.*s\n", 4, "dynamic");

    // Dynamic width and precision combined for strings
    printf("Expected: %*.*s\n", 12, 5, "precision");
    my_printf("Result  : %*.*s\n", 12, 5, "precision");
}


void test_stringEdgeCases() {
    printf("Testing Edge Cases For Strings:\n");
    printf("Expected: %.0s\n", "abcdef");
    my_printf("Result  : %.0s\n", "abcdef");

    printf("Expected: %10s\n", "");
    my_printf("Result  : %10s\n", "");
}

void test_specialCharacters() {
    printf("Testing Special Characters:\n");
    printf("Expected: %s\n", "Hello\nWorld");
    my_printf("Result  : %s\n", "Hello\nWorld");

    printf("Expected: %s\n", "\tTabbed");
    my_printf("Result  : %s\n", "\tTabbed");
}


void test_invalidCases() {
    printf("Testing Invalid Cases:\n");
    printf("Expected: Invalid specifier: %y\n");
    my_printf("Result  : Invalid specifier: %y\n");

    printf("Expected: Invalid specifier: % \n");
    my_printf("Result  : Invalid specifier: % \n");


    //  mix of valid and invalid specifiers
    printf("Expected: Mix: %d %s % %c\n", 47, "hello", 52);
    my_printf("Result  : Mix: %d %s % %c\n", 47, "hello", 52);

    // %%
    printf("Expected: Just %%\n");
    my_printf("Result  : Just %%\n");


    printf("Expected: Invalid: %y but ends normal\n");
    my_printf("Result  : Invalid: %y but ends normal\n");
}

int main() {
    printf("Running all tests for my_printf...\n");
    test_forInt();
    test_forHex();
    test_forString();
    test_forChar();
    test_forUpper();
    test_forLower();
    test_forLenStr();
    test_largeNumbers();
    test_negativeNumbers();
    test_edgeValues();
    test_dynamicWidthAndPrecision();
    test_combinedFlags();
    test_stringEdgeCases();
    test_specialCharacters();
    test_invalidCases();
    printf("All tests completed.\n");



    return 0;
}
