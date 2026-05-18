#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 80

// Прототипи функцiй
int binToOctArray(char s[], char result[]);
int binToOctPtr(char* s, char* result);

int main()
{
    char binary[N], resMas[N], resPtr[N];
    int choice;

    printf("\n Введiть двiйкове число: ");
    scanf_s("%s", binary, N);

    printf("\n Результат роботи через масиви:");
    if (binToOctArray(binary, resMas))
        printf("\n %s (2) -> %s (8)", binary, resMas);
    else
        printf("\n Помилка введення!");

    printf("\n\n Результат роботи через вказiвники:");
    if (binToOctPtr(binary, resPtr))
        printf("\n %s (2) -> %s (8)", binary, resPtr);
    else
        printf("\n Помилка введення!");

    printf("\n");
    return 0;
}

// Реалiзацiя через iндексацiю масивiв
int binToOctArray(char s[], char result[])
{
    int len = strlen(s);
    int i, j, digit, octLen;
    int bits[N] = {0};

    if (len == 0) return 0;

    // Перевiрка коректностi та заповнення тимчасового масиву бітів
    for (i = 0; i < len; i++)
    {
        if (s[i] < '0' || s[i] > '1') return 0;
        bits[i] = s[i] - '0';
    }

    int paddedLen = len;
    while (paddedLen % 3 != 0) paddedLen++;

    for (i = paddedLen - 1; i >= 0; i--)
    {
        int src = i - (paddedLen - len);
        bits[i] = (src >= 0) ? bits[src] : 0;
    }

    octLen = paddedLen / 3;
    for (i = 0; i < octLen; i++)
    {
        digit = bits[i * 3] * 4 + bits[i * 3 + 1] * 2 + bits[i * 3 + 2];
        result[i] = digit + '0';
    }
    result[octLen] = '\0';

    //Видалення ведучих нулiв, якщо число не "0"
    if (result[0] == '0' && strlen(result) > 1)
    {
        for (i = 0; result[i]; i++) result[i] = result[i + 1];
    }

    return 1;
}

// Реалiзацiя через адресну арифметику
int binToOctPtr(char* s, char* result)
{
    int len = 0;
    int i;
    char* p = s;
    while (*p) { len++; p++; }

    if (len == 0) return 0;

    // Доповнення для трiад
    int pad = (3 - len % 3) % 3;
    int totalBits = len + pad;
    int* bits = (int*)malloc(totalBits * sizeof(int));
    int* bPtr;
    char* resPtr;
    int firstNonZero;
    int digit;

    if (!bits) return 0;

    for (i = 0; i < pad; i++) *(bits + i) = 0;
    for (i = 0; i < len; i++)
    {
        if (*(s + i) < '0' || *(s + i) > '1') { free(bits); return 0; }
        *(bits + pad + i) = *(s + i) - '0';
    }

    bPtr = bits;
    resPtr = result;
    firstNonZero = 0;

    for (i = 0; i < totalBits; i += 3)
    {
        digit = (*bPtr) * 4 + (*(bPtr + 1)) * 2 + (*(bPtr + 2));
        if (digit != 0 || firstNonZero || i == totalBits - 3)
        {
            *resPtr = digit + '0';
            resPtr++;
            firstNonZero = 1;
        }
        bPtr += 3;
    }
    *resPtr = '\0';

    free(bits);
    return 1;
}
