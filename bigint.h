#ifndef BIGINT_H
#define BIGINT_H

#include <stdio.h>
#include <stdarg.h>

#define _BIG_INT_MAX_LENGTH 500

typedef struct bigint_t {
        int data[_BIG_INT_MAX_LENGTH];
        int sign;
        int length;
} bigint_t;

/* INITIALIZATION FUNCTIONS */
bigint_t bigint_create(char *num);
/****************************/

/* CONVERSION FUNCTIONS */
// TODO: uh rename
bigint_t bigint_itbi(int num);
/************************/

/* PRINTING FUNCTIONS */
void bigint_print(bigint_t num);
void bigint_exponent_print(bigint_t num);

// bigint_printf() works like printf()
// NOTE: the arguments must be bigint_t
// %E => exponent form                  ex. 5.3E4
// %N => standard notation              ex. 5300
void bigint_printf(char *format, ...);
/**********************/

/* OPERATION FUNCTIONS */
bigint_t bigint_multiply(bigint_t num1, bigint_t num2);
bigint_t bigint_factorial(int num);
/***********************/

bigint_t bigint_create(char *num)
{
        bigint_t n = { {0}, 1, 0 };

        if (*num == '-') {
                n.sign = -1;
                num++;
        }

        int index = 0;
        while (*num != '\0') {
                n.data[index] = *num - '0';
                num++;
                index++;
        }
        n.length = index;

        return n;
}

// converts an int to a bigint_t, (itbi = int to bigint)
bigint_t bigint_itbi(int num)
{
        bigint_t result = { {0}, (num > 0) ? 1 : -1, 0 };
        if (num < 0) {
                num *= -1;
        }
        
        int num_len = 0;
        int n = num;
        while (n != 0) {
                n /= 10;
                num_len++;
        }

        result.length = num_len;

        for (int i = num_len - 1; i >= 0; i--) {
                int digit = num % 10;
                result.data[i] = digit;
                num /= 10;
        }

        return result;
}

void bigint_print(bigint_t num)
{
        if (num.sign == -1) {
                printf("-");
        }
        for (int i = 0; i < num.length; i++) {
                printf("%d", num.data[i]);
        }
        printf("\n");
}

void bigint_exponent_print(bigint_t num)
{

        if (num.sign == -1) {
                printf("-");
        }

        printf("%d.", num.data[0]);
        int exponent = num.length - 1;
        for (int i = 1; i < num.length; i++) {
                printf("%d", num.data[i]);
        }
        printf("E%d", exponent);

}

void bigint_printf(char *format, ...)
{
        va_list args;
        char *f_ptr = format;
        va_start(args, format);

        while (*f_ptr != '\0') {

                if (*f_ptr == '%' && *(f_ptr + 1) == 'E') {
                        f_ptr += 2;
                        bigint_t n = va_arg(args, bigint_t);
                        bigint_exponent_print(n);
                        continue;
                }
                else if (*f_ptr == '%' && *(f_ptr + 1) == 'N') {
                        f_ptr += 2;
                        bigint_t n = va_arg(args, bigint_t);
                        bigint_print(n);
                        continue;
                }

                printf("%c", *f_ptr);

                f_ptr++;
        }

        va_end(args);
}

// multiply two bigint_t numbers using long multiplication
bigint_t bigint_multiply(bigint_t num1, bigint_t num2)
{
        bigint_t result = { {0}, 1, 0 };
        result.length = num1.length + num2.length;
        result.sign = num1.sign * num2.sign;

        if (result.length > _BIG_INT_MAX_LENGTH) {
                return result;
        }

        int start = result.length - 1;
        int num1_iterations = 0;
        int offset = 0;

        for (int i = num2.length - 1; i >= 0; i--) {

                int n2 = num2.data[i];
                int carry = 0;

                for (int j = num1.length - 1; j >= 0; j--) {

                        int n1 = num1.data[j];
                        int prod = n1 * n2;
                        int sum = result.data[start - offset] + prod + carry;

                        result.data[start - offset] = sum % 10;

                        carry = sum / 10;

                        offset++;

                }
                num1_iterations++;

                offset = num1_iterations;

                if (carry > 0) {
                        result.data[i] += carry;
                }

        }

        if (result.data[0] == 0) {
                bigint_t adjusted_result = { {0}, result.sign, result.length - 1 };
                for (int i = 1; i < result.length; i++) {
                        adjusted_result.data[i - 1] = result.data[i];
                }
                return adjusted_result;
        }

        return result;
}

bigint_t bigint_factorial(int num)
{
        bigint_t result = bigint_itbi(num);

        for (int i = num - 1; i >= 2; i--) {
                bigint_t n = bigint_itbi(i);
                result = bigint_multiply(result, n);
        }

        return result;
}

#endif /* BIGINT_H */ 
