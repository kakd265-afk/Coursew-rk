#include <stdio.h>
#include <string.h>

#define LINE_BUF 256
#define NAME_SIZE 256

// Прототипи функцiй
unsigned char swapNibbles(unsigned char c);
void getBinary(unsigned char val, char out[]);
void printResultLine(FILE* out, unsigned char orig, unsigned char res);

int task3_run(int argc, char* argv[])
{
    FILE* fin = NULL;
    FILE* fout = NULL;
    char inName[NAME_SIZE] = {0}, outName[NAME_SIZE] = {0};
    char line[LINE_BUF];
    int i, len;
    unsigned char orig, res;
    errno_t err;

    // Перевiрка аргументiв командного рядка
    if (argc >= 3)
    {
        strcpy_s(inName, NAME_SIZE, argv[1]);
        strcpy_s(outName, NAME_SIZE, argv[2]);
    }
    else
    {
        printf("\n Введiть iм'я вхiдного файлу -> ");
        scanf_s("%s", inName, NAME_SIZE);
        printf(" Введiть iм'я вихiдного файлу -> ");
        scanf_s("%s", outName, NAME_SIZE);
    }

    fin = fopen(inName, "r");
    if (fin == NULL)
    {
        printf(" Помилка: не вдалося вiдкрити файл '%s'\n", inName);
        return 1;
    }

    fout = fopen(outName, "w");
    if (fout == NULL)
    {
        printf(" Помилка: не вдалося створити файл '%s'\n", outName);
        fclose(fin);
        return 1;
    }

    // Зчитування рядкiв з файлу
    while (fgets(line, LINE_BUF, fin) != NULL)
    {
        len = (int)strlen(line);

        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
            len--;
        }

        printf("\n Обробка рядка: \"%s\"", line);
        fprintf(fout, "\n Рядок: \"%s\"\n", line);

        for (i = 0; i < len; i++)
        {
            orig = (unsigned char)line[i];
            res = swapNibbles(orig);

            printResultLine(stdout, orig, res);
            printResultLine(fout, orig, res);
        }
    }

    fclose(fin);
    fclose(fout);

    printf("\n Роботу завершено. Результати у файлi: %s\n", outName);
    return 0;
}

// Функцiя перестановки пiвбайтiв
unsigned char swapNibbles(unsigned char c)
{
    // Використання порозрядних операцiй зсуву та маскування
    return (unsigned char)(((c & 0x0F) << 4) | ((c & 0xF0) >> 4));
}

// Формування двiйкового рядка
void getBinary(unsigned char val, char out[])
{
    int bit;
    for (bit = 7; bit >= 0; bit--)
    {
        out[7 - bit] = ((val >> bit) & 1) ? '1' : '0';
    }
    out[8] = '\0';
}

void printResultLine(FILE* out, unsigned char orig, unsigned char res)
{
    char binOrig[9], binRes[9];
    getBinary(orig, binOrig);
    getBinary(res, binRes);

    fprintf(out, "\n  Вхiд:  hex=0x%02X  bin=%s", orig, binOrig);
    fprintf(out, "\n  Вихiд: hex=0x%02X  bin=%s", res, binRes);
    fprintf(out, "\n  -------");
}