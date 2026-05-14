
#include <stdio.h>
#include <windows.h>
#include "Tasks.h"

int main(int argc, char *argv[]) {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int Task;

    printf("_________________________________ Курсова робота __________________________________\n");
	printf("|                                                                                 |\n");
    printf("|                                                                                 |\n");
    printf("|                                Виберіть завдання                                |\n");
    printf("|                                                                                 |\n");
    printf("|                                   Завдання 1                                    |\n");
    printf("|                                                                                 |\n");
    printf("|                               Завдання 2 Матриці                                |\n");
    printf("|                                                                                 |\n");
    printf("|                        Завдання 3  перестановка півбайтів                       |\n");
    printf("|                                                                                 |\n");
    printf("|                         Завдання 4 Транспортна компанія                         |\n");
    printf("|                                                                                 |\n");
    printf("|_________________________________________________________________________________|\n");
    scanf_s("%d", &Task);

    switch (Task) {

    case 1:
        task1_run();
        break;

    case 2:
        task2_run();
        break;

    case 3:
        task3_run(argc, argv);
        break;

    case 4:
        task4_run(argc, argv);
        break;

    default:
        printf("Невірний вибір завдання будь ласка вибери 1 або 4 \n");
        break;
    }
    return 0;
}