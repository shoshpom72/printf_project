#include <stdio.h>
#include <stdarg.h>


// declare functions
void formatting(const char **format, char flags[5], int *width, int *precision, char *length, char *specifier);
void forInt(int num, int width, int precision, char flags[5], char length, int *count);
void forHex(unsigned int num, int width, int precision, const char flags[5], char length, int *count);
void forString(const char str[], int width, int precision, const char flags[5], int *count);
void forChar(char c, int width, int precision, const char flags[5], int *count);
void forUpper(const char str[], int width, int precision, const char flags[5], int *count);
void forLower(const char str[], int width, int precision, const char flags[5], int *count);
void forLenStr(const char *str, int width, int precision, const char flags[5], int *count);
void addPad(int padding, char paddingChar, int *count);


int my_printf(const char *format, ...){
    char invalid[] = "Error Not A Valid Specifier";
    // list of variable arguments
    va_list args;
    // initialize the arg list with the last non-variable argument of the function
    va_start(args, format);

    // list to hold all possible flags
    char flags[5]= {'\0', '\0', '\0', '\0', '\0'};
    // var for the width
    int width = 0 ;
    // var for the precision
    int precision = -1;
    //var for the length modifier
    char length = '\0';
    //var for the specifier
    char specifier = '\0';

    // counter for return to be the total characters printed
    int count = 0;

    // loops through each character of the format string
    while(*format){
        // if it's the %, it's the start of the format specifier
        if (*format == '%'){

            // if the next char is also a %
            if (*(format + 1) == '%'){
                // print a %
                putchar('%');
                count++;
                // skip to after both %s
                format += 2;

            }
            // move on to the next char
            format++;


            // go through the format string to break it up
            formatting(&format, flags, &width, &precision, &length, &specifier);

            // check the specifier
            switch(specifier) {
                // if it's an int
                case 'd':
                    if (length == 'h') {
                        forInt((short int)va_arg(args, int), width, precision, flags, length, &count);
                        break;
                    }
                    else if (length == 'l') {
                        forInt(va_arg(args, long int), width, precision, flags, length, &count);
                        break;
                    }
                    else{
                        forInt(va_arg(args, int), width, precision, flags, length, &count);
                        break;
                    }
                case 's':
                    forString(va_arg(args, const char *), width, precision, flags, &count);
                    break;

                case 'x':
                    forHex((unsigned int)va_arg(args,  int), width, precision, flags, length, &count);
                    break;


                case 'c':
                    // cast to const char because chars get promoted to ints in va_arg
                    forChar((const char)va_arg(args, int), width, precision, flags, &count);
                    break;
                // upper
                case 'U':
                    forUpper(va_arg(args, const char *), width, precision, flags, &count);
                    break;
                // lower
                case 'L':
                    forLower(va_arg(args, const char *), width, precision, flags, &count);
                    break;
                // len of string
                case 'S':
                    forLenStr(va_arg(args, const char *), width, precision, flags, &count);
                    break;

                default:

                    putchar('%');
                    putchar(*format);
                    format++;
                    putchar(' ');
                    count += 3;
                    for (int i = 0; invalid[i] != '\0'; i++) {
                        putchar(invalid[i]);
                        count++;
                    }

                    return count;


            }
        }


        else{
            // print out each regular character
            putchar(*format);
            //increment the character counter
            count++;
            // move to the next char
            format++;
        }



    }

    va_end(args);

    return count;
}


void formatting(const char **format, char flags[5],  int *width, int *precision, char *length, char *specifier) {
    // p is a pointer to the char that the format string is up to
    const char *p = *format;

    // initialize the flag, width precision length and specifier
    for (int i = 0; i < 5; i++) {
        flags[i] = '\0';
    }
    *width = 0;
    *precision = -1;
    *length = '\0';
    *specifier = '\0';

    // while *p is one of the flags
    while (*p == '-' || *p == '+' || *p == ' ' || *p == '#' || *p == '0' || *p == '\'') {
        // set the flags in the list to whichever flags the string contains
        if (*p == '-') {
            // left-align
            flags[0] = '-';
        }
        else if (*p == '+') {
            // + in front of pos numbers
            flags[1] = '+';
        }
        else if (*p == ' ') {
            // space in front of pos numbers
            flags[2] = ' ';
        }
        else if (*p == '0') {
            // when width is specified fill in 0s for numbers
            flags[3] = '0';
        }
        else if (*p == '#') {
            // for %x start hex number output with 0x
            flags[4] = '#';
        }

        // move the pointer to the next char
        p++;
    }

    // after the flags if *p is a digit, it's the width
    while (*p >= '0' && *p <= '9') {
        // build up the width one digit at a time
        // convert it to an int and shift it by one decimal place each time
        *width = (*width * 10) + (*p - '0');
        //move the pointer to the next char
        p++;
    }

    // if *p is a '.' then it has a precision specification
    if (*p == '.') {
        // skip the . and get the number after it
        p++;
        //initialize precision to 0
        *precision = 0;
        // while p is a number
        while (*p >= '0' && *p <= '9') {
            // same logic as the width number
            // build up the precision one digit at a time
            // convert it to an int and shift it by one decimal place each time
            *precision = (*precision * 10) + (*p - '0');
            p++;
        }
    }

    //  *p is h or l
    if (*p == 'h' || *p == 'l') {
        // set length to the length modifier
        *length = *p;
        //increment p to the next char
        p++;
    }

    // if *p is a d, x, c, or s
    if (*p == 'd' || *p == 'x' || *p == 'c' || *p == 's' || *p == 'U' || *p == 'L' || *p == 'S') {
        // set the specifier to be the specifier char
        *specifier = *p;
        // increment p to the next char
        p++;
    }

    // update the format pointer
    *format = p;

}

void forInt(int num, int width, int precision, char flags[5], char length, int *count) {

    // determine if the num is negative
    int isNeg = 0;
    // keep track of how much padding needed
    int padding = 0;
    // if precision > length of the num, 0s need to be filled in
    int precisionZeros = 0;


    // if num is negative
    if (num < 0) {
        // set isNeg to 1
        isNeg = 1;
        // negate num to be pos
        num = -num;

    }



    // place to store the numbers before they can be printed
    char numString[50];

    // index value
    int i = 0;

    // store the length of num so if there's a width that's greater than the len
    // the padding could know how much space it needs
    int lenNum = 0;

    // if num is 0
    if (num == 0) {
        // put a 0 in numString
        numString[i++] = '0';
        // set lenNum to 1
        lenNum = 1;
    } else {
        // while num still has values to be printed
        while (num > 0) {
            // fill in numString index i with the last digit of num and change it to a char
            // increment i to the next index
            numString[i++] = (num % 10) + '0';
            // take the last digit off of num
            num /= 10;
        }

        // the len of num is the index + 1 because the index starts at 0
        lenNum = i;
    }
    // figure out how much padding you need

    // if precision is not specified treat as no precision
    if (precision == -1) {
        precision = lenNum;
    }
    else if (precision > 0 && flags[3] == '0'){
        flags[3] = '\0';
    }

    // if the precision is greater than the len of num, then it needs to be filled in with 0s
    if (precision > lenNum) {
        // set the number of leading 0s to be the difference of precision and lenNum
        precisionZeros = precision - lenNum;
    }
    // set the total width to be the extra 0s and the length of the number together
    int totWidth = lenNum + precisionZeros;


    // if the num needs a +/- sign
    if (isNeg || flags[1] == '+' || flags[2] == ' ') {
        // add to the total width to account for signs (+/-)
        totWidth++;
    }
    // if the width is larger than the total width then it needs to be filled with extra padding
    if (width > totWidth) {
        // account for that padding
        padding = width - totWidth;
    }
    // if padding is necessary and it's not left-aligned
    if (padding > 0 && flags[0] != '-') {

        if (flags[2] == ' ') {
            // print a space
            putchar(' ');
            // increment the char counter
            *count += 1;
            // decrement padding
            padding--;
        }
        if (flags[3] == '0' && flags[1] != '+') {
            // if num is neg
            if (isNeg) {
                // print a - first
                putchar('-');
                // increment the char counter
                *count += 1;
                // reset isNeg so it doesn't print another -
                isNeg = 0;
            }


            // print 0 padding
            addPad(padding, '0', count);
        }
        // if there's space padding,
        // and it's not left-aligned, handle it before the signs
        else {
            //print space padding
            addPad(padding, ' ', count);
        }
    }


// first check if there's a + flag and it's positive
    if (flags[1] == '+' && !isNeg) {
        // print a +
        putchar('+');
        // increment the char counter
        *count += 1;
    }
    // then check if there's a ' ' flag and it's positive
    else if (flags[2] == ' ' && !isNeg && flags[3] != '0') {
        // print a space
        putchar(' ');
        // increment the char counter
        *count += 1;

    }

    // if num is negative
    else if (isNeg) {
        // print a - before the 0s
        putchar('-');
        // increment the char counter
        *count += 1;

    }
    // pad with zeros
    addPad(precisionZeros, '0',  count);

    if (flags[2] == ' ' && flags[3] == '0'){
        putchar('0');
        *count += 1;
    }

    // while there are numbers to print
    while (i--){
        // print the value at the end of the string because it was appended in reverse order
        putchar(numString[i]);
        // increment the counter
        *count += 1;
    }

    // if output is left aligned
    if (flags[0] == '-'){
        // print padding spaces
        addPad(padding, ' ', count);
    }

}
void forHex(unsigned int num, int width, int precision, const char flags[5], char length, int *count) {

    // similar strategy to int but to got from int to hex
    // one at a time take the whole num and %16 each time
    // instead of %10

    char hexVal[] = "0123456789abcdef";

    // if precision > length of the num, 0s need to be filled in
    int precisionZeros = 0;
    // if the width > precision zeros + length of the num it needs to be filled in
    int padding = 0;
    // to hold hex values before printing
    char hexString[50];

    int lenHex = 0;


    int i = 0;

    if (num == 0){
        hexString[i++] = '0';
        lenHex = 1;

    }
    else{
        unsigned int temp = num;

        while (temp > 0){
            // find the hex val from the right side on using %16
            unsigned int hex = temp % 16;
            // fill in the hex string with the hex value of that number
            hexString[i++] = hexVal[hex];
            // remove that part of the number
            temp /= 16;

        }
    }

    lenHex = i;

    // if precision is > lenHex set the amt of padding zeros to be the difference
    if (precision > lenHex){
        precisionZeros = precision - lenHex;
    }
    // find the total width
    int totalWidth = precisionZeros + lenHex;

    // if the width is greater than the total width of the numbers
    if (width > totalWidth){
        // figure out how much padding it needs
        padding = width - totalWidth;
    }
    // if there's a # flag take away 2 from padding to account for the 0x
    if (flags[4] == '#' && num != 0){
        padding -= 2;

    }


    // if it's not left aligned
    if (padding > 0 && flags[0] != '-'){
        // if there's a 0 flag
        if (flags[3] == '0'){
            // if the flag is set to #
            // print a 0x before the 0s
            if (flags[4] == '#' && num != 0){
                putchar('0');
                putchar('x');
                *count += 2;
            }
            // pad with 0s
            addPad(padding, '0', count);
        }
        else {
            // pad with space
            addPad(padding, ' ', count);
        }

    }

    // if the flag is set to # print a 0x at the beginning
    if (flags[4] == '#' && flags[3] != '0' && num != 0){
        putchar('0');
        putchar('x');
        *count += 2;
    }

    // print the precision zeros
    addPad(precisionZeros, '0', count);

    while (i > 0){
        // print the hex number stored in hexString
        putchar(hexString[--i]);
        *count += 1;
    }

    if (flags[0] == '-' && padding > 0){
        addPad(padding, ' ', count);
    }

}

void forString(const char *str, int width, int precision, const char flags[5], int *count) {
    if (str == NULL) {
        str = "(null)";
    }

    // keep track of padding
    int padding = 0;

    // hold the len of the string
    int lenStr = 0;

    // find the length of the string


    // the precision needs to be greater than the str or to be -1 (default)
    // to print the whole string
    // if precision is < lenStr then lenStr = precision
    // if precision is -1 that's the default and should also find the full length of str
    while (str[lenStr] != '\0' && (precision == -1 || lenStr < precision)) {
        // increment the lenStr counter
        lenStr++;
    }


    padding = width - lenStr;

    // if it's not left-aligned
    if (padding > 0 && flags[0] != '-') {
        if (flags[3] == '0'){
            // print padding zeros
            addPad(padding, '0', count);

        }
        else{
            // print padding spaces
            addPad(padding, ' ', count);
        }

    }
    // print the string
    for (int i = 0; i < lenStr; i++) {
        // print each individual letter from the str string
        putchar(str[i]);
        // increment counter
        *count += 1;
    }

    // if left aligned print the padding spaces now
    if (padding > 0 && flags[0] == '-') {
        // print padding spaces
        addPad(padding, ' ', count);
        }

}

void forChar(const char c, int width, int precision, const char flags[5], int *count) {

    // keep track of padding
    int padding = 0;

    // if the width is greater than the len of a single char
    if (width > 1){
        padding = width - 1;
    }
    // pad if not left aligned
    if (padding > 0 && flags[0] != '-'){
        if (flags[3] == '0'){
            addPad(padding, '0', count);
        }
        else {
            addPad(padding, ' ', count);
        }

    }
    // print c
    putchar(c);
    *count += 1;

    // if left aligned pad with spaces after c
    if (padding > 0 && flags[0] == '-'){
        addPad(padding, ' ', count);
    }

}

void forUpper(const char *str, int width, int precision, const char *flags, int *count) {
    int i = 0;

    // find the length of the input string
    while (str[i] != '\0'){
        i++;
    }
    // make a new string of the same length
    char newStr[i];

    i = 0;

    // loop through the string
    while (str[i] != '\0'){
        // if its a lower case letter
        if (str[i] >= 'a' && str[i] <= 'z') {
            // convert it to upper case and put it in the new string
            newStr[i] = str[i] - ('a' - 'A');
        }
        else {
            // if its already upper just put it in the new string
            newStr[i] = str[i];
        }
        i++;
    }
    // end off the string
    newStr[i] = '\0';

    // call forString with the new upper case string
    forString(newStr, width, precision, flags, count);

}

void forLower(const char *str, int width, int precision, const char *flags, int *count) {
    int i = 0;

    // find the length of the input string
    while (str[i] != '\0'){
        i++;
    }
    // make a new string of the same length
    char newStr[i];

    i = 0;

    // loop through the string
    while (str[i] != '\0'){
        // if its a lower case letter
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // convert it to upper case and put it in the new string
            newStr[i] = str[i] + ('a' - 'A');
        }
        else {
            // if its already upper just put it in the new string
            newStr[i] = str[i];
        }
        i++;
    }
    // end off the string
    newStr[i] = '\0';

    // call forString with the new upper case string
    forString(newStr, width, precision, flags, count);

}

void forLenStr(const char *str, int width, int precision, const char flags[5], int *count){

    int len = 0;
    // place to store the numbers before they can be printed
    char numString[50];

    int i = 0;


    // get the length of the str
    while (str[len] != '\0'){
        len++;
    }

    // if len is 0 just print a 0
    if (len == 0){
        putchar('0');
        *count += 1;
    }
    else{
        // while there are more numbers in len
        while (len > 0){
            // add the last digit to the ith position
            numString[i++] = (len % 10) + '0';
            // remove it from the end
            len /= 10;
        }
        i--;
        // while there are still numbers to print
        while (i >= 0){
            // print them
            putchar(numString[i--]);
            *count += 1;
        }

    }




}

// function to deal with the padding
void addPad(int padding, char paddingChar, int *count){
    while (padding--){
        // print the padding char
        putchar(paddingChar);
        // increment counter
        *count += 1;
    }

}




int main() {


    printf("%+d\n", 123);
    my_printf("%+d\n", 123);

    printf("%+d\n", -123);
    my_printf("%+d\n", -123);

    printf("%09d\n", -223);
    my_printf("%09d\n", -223);

    printf("%9d\n", -938327953);
    my_printf("%9d\n", -938327953);


    printf("%-9x\n", 733);
    my_printf("%-9x\n", 733);


    printf("%#9.5x\n", 123);
    my_printf("%#9.5x\n", 123);

    printf("%0#10x\n", 0);
    my_printf("%0#10x\n", 0);


    printf("%.3s\n", "hello");
    my_printf("%.3s\n", "hello");

    printf("%9.3s\n", "hello");
    my_printf("%9.3s\n", "hello");

    printf("%-9.3s\n", "hello");
    my_printf("%-9.3s\n", "hello");


    printf("%c\n", 68);
    my_printf("%c\n", 68);

    printf("%-9c\n", 'A');
    my_printf("%-9c\n", 'A');

    printf("%09c\n", 123);
    my_printf("%09c\n", 123);


    my_printf("%-6U%U\n", "shosh", "Pom");
    my_printf("%-6.3U%.3U\n", "Shosh", "pom");

    my_printf("%-6L%L\n", "SHOSH", "Pom");
    my_printf("%-6.3L%.3L\n", "Shosh", "POM");


    my_printf("%S", "This is a test");


    return 0;


}
