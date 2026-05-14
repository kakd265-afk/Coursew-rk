#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 64
#define FILENAME_SIZE 256

// Структура даних
typedef struct
{
    char product[STR_LEN];
    char exporter[STR_LEN];
    char importer[STR_LEN];
    int quantity;
    double price;
    int month;
} ExportRecord;

// Прототипи функцiй
int loadData(const char* filename, ExportRecord* recs, int n);
int utf8_len(const char* s);        
void printHeader(FILE* out);
void printItem(FILE* out, const ExportRecord* r);
void showImportersByPeriod(const ExportRecord* recs, int count, const char* prod, FILE* out);
void findTopImporter(const ExportRecord* recs, int count, FILE* out);

// Сортування та пошук
int compareByProduct(const void* a, const void* b);
int compareByExporter(const void* a, const void* b);
void linearSearch(const ExportRecord* recs, int count, const char* key, FILE* out);
int binarySearch(const ExportRecord* recs, int count, const char* key);

int task4_run(int argc, char* argv[])
{

    ExportRecord* recs = NULL;
    char inName[FILENAME_SIZE], outName[FILENAME_SIZE], searchKey[STR_LEN];
    int n, count, i;
    FILE* fout = NULL;

    if (argc >= 2)
        strcpy_s(inName, FILENAME_SIZE, argv[1]);
    else
    {
        printf("\n Введiть iм'я вхiдного файлу -> ");
        scanf_s("%s", inName, FILENAME_SIZE);
    }

    printf(" Введiть iм'я файлу для звiту -> \n ");
    scanf_s("%s", outName, FILENAME_SIZE);

    printf(" Введiть максимальну кiлькiсть записiв -> \n");
    scanf_s("%d", &n);

    recs = (ExportRecord*)malloc(n * sizeof(ExportRecord));
    if (recs == NULL)
    {
        printf("\n Помилка видiлення пам'ятi!");
        return 0;
    }

    count = loadData(inName, recs, n);
    if (count <= 0) { free(recs); return 0; }

    if (fopen_s(&fout, outName, "w") != 0)
    {
        printf("\n Помилка створення файлу звiту!");
        free(recs);
        return 0;
    }

    printf("--- Завантаженi данi ---\n");
    printHeader(stdout);
    for (i = 0; i < count; i++) printItem(stdout, &recs[i]);

    printf(" Введiть назву товару для фiльтрацiї (квiтень-вересень) -> \n\n");
    scanf_s(" %[^\n]", searchKey, (unsigned)STR_LEN);
    showImportersByPeriod(recs, count, searchKey, stdout);
    showImportersByPeriod(recs, count, searchKey, fout);

    findTopImporter(recs, count, stdout);
    findTopImporter(recs, count, fout);

    qsort(recs, count, sizeof(ExportRecord), compareByProduct);
    printf("--- Данi вiдсортованi за назвою товару (Quick Sort) ---\n\n");
    printHeader(stdout);
    for (i = 0; i < count; i++) printItem(stdout, &recs[i]);

    printf("\n Введiть країну-експортера для пошуку -> \n\n");
    scanf_s(" %[^\n]", searchKey, (unsigned)STR_LEN);

    printf("--- Результати лiнiйного пошуку ---\n");
    fprintf(fout, "--- Результати лiнiйного пошуку ---\n");
    linearSearch(recs, count, searchKey, stdout);
    linearSearch(recs, count, searchKey, fout);

    qsort(recs, count, sizeof(ExportRecord), compareByExporter);

    {
        int firstIdx = binarySearch(recs, count, searchKey);

        printf("--- Результати бiнарного пошуку ---\n");
        fprintf(fout, "--- Результати бiнарного пошуку ---\n");
        if (firstIdx != -1)
    {
        printHeader(stdout);
        printHeader(fout);
        for (i = firstIdx; i < count && _stricmp(recs[i].exporter, searchKey) == 0; i++)
        {
            printItem(stdout, &recs[i]);
            printItem(fout, &recs[i]);
        }
        }
        else printf("\n Записiв не знайдено.");
    }

    fclose(fout);
    free(recs);
    printf("\n\n Данi збереженi у файлi: %s. Програму завершено.\n", outName);
    system("pause");
    return 0;
}

int loadData(const char* filename, ExportRecord* recs, int n)
{
    FILE* f;
    int count = 0;
    if (fopen_s(&f, filename, "r") != 0) return -1;

    while (count < n)
    {
        ExportRecord* r = &recs[count];
        if (fscanf_s(f, " %[^;];", r->product, (unsigned)STR_LEN) != 1) break;
        if (fscanf_s(f, " %[^;];", r->exporter, (unsigned)STR_LEN) != 1) break;
        if (fscanf_s(f, " %[^;];", r->importer, (unsigned)STR_LEN) != 1) break;
        if (fscanf_s(f, "%d;%lf;%d", &r->quantity, &r->price, &r->month) != 3) break;

        count++;
    }
    fclose(f);
    return count;
}

int utf8_len(const char* s)
{
    if (s == NULL) return 0;
    int len = 0;
    for (int i = 0; s[i]; i++)
        if ((s[i] & 0xC0) != 0x80) len++;
    return len;
}

void print_padded(FILE* out, const char* s, int width)
{
    int pad = width - utf8_len(s);
    fprintf(out, "%s", s);
    if (pad > 0)
        fprintf(out, "%*s", pad, "");
}

void printHeader(FILE* out)
{
    print_padded(out, "Товар", 32); fprintf(out, " ");
    print_padded(out, "Експортер", 22); fprintf(out, " ");
    print_padded(out, "Iмпортер", 22);
    fprintf(out, " %8s %10s %12s\n", "К-сть", "Цiна", "Мiс.");
    fprintf(out, "%-32s %-22s %-22s %8s %4s %6s\n",
        "--------------------------------",
        "----------------------",
        "----------------------",
        "--------", "----------", "------");
}

void printItem(FILE* out, const ExportRecord* r)
{
    print_padded(out, r->product, 32); fprintf(out, " ");
    print_padded(out, r->exporter, 22); fprintf(out, " ");
    print_padded(out, r->importer, 22);
    fprintf(out, " %8d %10.2f %6d\n",
        r->quantity, r->price, r->month);
}

int compareByProduct(const void* a, const void* b)
{
    return _stricmp(((ExportRecord*)a)->product, ((ExportRecord*)b)->product);
}

int compareByExporter(const void* a, const void* b)
{
    return _stricmp(((ExportRecord*)a)->exporter, ((ExportRecord*)b)->exporter);
}

void linearSearch(const ExportRecord* recs, int count, const char* key, FILE* out)
{
    int i, found = 0;
    for (i = 0; i < count; i++)
    {
        if (_stricmp(recs[i].exporter, key) == 0)
        {
            if (found == 0) printHeader(out);
            printItem(out, &recs[i]);
            found++;
        }
    }
    if (!found) fprintf(out, "\n Записiв не знайдено.");
}
// Бінарний пошук
int binarySearch(const ExportRecord* recs, int count, const char* key)
{
    int low = 0, high = count - 1, mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        int cmp = _stricmp(recs[mid].exporter, key);
        if (cmp == 0)
        {
            while (mid > 0 && _stricmp(recs[mid - 1].exporter, key) == 0) mid--;
            return mid;
        }
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void showImportersByPeriod(const ExportRecord* recs, int count, const char* prod, FILE* out)
{
    fprintf(out, "Iмпортери товару \"%s\" (Квiтень-Вересень):\n\n", prod);
    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (_stricmp(recs[i].product, prod) == 0 && recs[i].month >= 4 && recs[i].month <= 9)
        {
            fprintf(out, "-> %s (мiсяць: %d)\n", recs[i].importer, recs[i].month);
            found = 1;
        }
    }
    if (!found) fprintf(out, "\n Записiв не знайдено.");
}
// Пошук найбільшого імпортера
void findTopImporter(const ExportRecord* recs, int count, FILE* out)
{
    struct { char name[STR_LEN]; long total; } list[100] = { 0 };
    int n = 0, i, j, pos, best = 0;

    for (i = 0; i < count; i++)
    {
        pos = -1;
        for (j = 0; j < n; j++)
            if (strcmp(list[j].name, recs[i].importer) == 0) { pos = j; break; }

        if (pos != -1) list[pos].total += recs[i].quantity;
        else {
            strcpy_s(list[n].name, STR_LEN, recs[i].importer);
            list[n].total = recs[i].quantity;
            n++;
        }
    }
    for (i = 1; i < n; i++) if (list[i].total > list[best].total) best = i;
    if (n > 0) fprintf(out, "Найбiльший iмпортер: %s (%ld од.)\n\n", list[best].name, list[best].total);
}