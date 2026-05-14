
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 10
#define MIN_N 6

// Прототипи функцiй
void inputMatrix(double a[][MAX_N], int n);
void fillRandom(double a[][MAX_N], int n);
void printMatrix(double a[][MAX_N], int n);
void avgEvenColumns(double a[][MAX_N], int n, double avgs[], int* count);

void task2_run()
{
    srand((unsigned)time(NULL));

    double matrix[MAX_N][MAX_N];
    double avgs[MAX_N];
    int n, count, i, choice;

    do
    {
        printf("\n Введiть розмiр квадратної матрицi N (%d <= N <= %d) -> ", MIN_N, MAX_N);
        scanf_s("%d", &n);
        if (n < MIN_N || n > MAX_N)
        {
            printf(" Помилка! Некоректний розмiр.");
        }
    } while (n < MIN_N || n > MAX_N);

    printf("\n Оберiть спосiб заповнення матрицi:");
    printf("\n 1 - Введення з клавiатури");
    printf("\n 2 - Генерацiя випадкових чисел");
    printf("\n Ваш вибiр -> ");
    scanf_s("%d", &choice);

    if (choice == 1)
    {
        inputMatrix(matrix, n);
    }
    else
    {
        fillRandom(matrix, n);
        printf("\n Матрицю заповнено автоматично.");
    }

    printMatrix(matrix, n);
    avgEvenColumns(matrix, n, avgs, &count);

    printf("\n Середнє арифметичне парних стовпцiв:");
    for (i = 0; i < count; i++)
    {
        // Виведення за номером стовпця (2, 4, 6...)
        printf("\n Стовпець %d: середнє = %.4f", (i + 1) * 2, avgs[i]);
    }
    printf("\n");
}

// Реалізація функцій

void inputMatrix(double a[][MAX_N], int n)
{
    int i, j;
    printf("\n Введiть елементи матрицi:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf(" a[%d][%d] = ", i + 1, j + 1);
            scanf_s("%lf", &a[i][j]);
        }
    }
}

void fillRandom(double a[][MAX_N], int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            a[i][j] = (double)(rand() % 201 - 100);
        }
    }
}

void printMatrix(double a[][MAX_N], int n)
{
    int i, j;
    printf("\n Поточна матриця %dx%d:\n", n, n);
    for (i = 0; i < n; i++)
    {
        printf("\n");
        for (j = 0; j < n; j++)
        {
            printf("%8.2f", a[i][j]);
        }
    }
    printf("\n");
}
// Функція для обчислення середнього арифметичного парних стовпців
void avgEvenColumns(double a[][MAX_N], int n, double avgs[], int* count)
{
    int i, j;
    double sum;
    *count = 0;

    for (j = 1; j < n; j += 2)
    {
        sum = 0.0;
        for (i = 0; i < n; i++)
        {
            sum += a[i][j];
        }
        avgs[*count] = sum / n;
        (*count)++;
    }
}