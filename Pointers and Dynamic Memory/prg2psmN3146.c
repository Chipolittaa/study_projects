#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// Функция для обработки ошибок
void print_error(const char *message) {
    fprintf(stderr, "Ошибка: %s\n", message);
    exit(EXIT_FAILURE);
}

// Функция для выделения памяти под матрицу (массив указателей на столбцы)
uint32_t **allocate_matrix(int rows, int cols) {
    uint32_t **matrix = malloc(cols * sizeof(uint32_t *));
    if (!matrix) {
        print_error("Не удалось выделить память под матрицу.");
    }
    for (int i = 0; i < cols; i++) {
        matrix[i] = malloc(rows * sizeof(uint32_t));
        if (!matrix[i]) {
            print_error("Не удалось выделить память под столбец матрицы.");
        }
    }
    return matrix;
}

// Функция для освобождения памяти матрицы
void free_matrix(uint32_t **matrix, int cols) {
    for (int i = 0; i < cols; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Функция для вывода матрицы
void print_matrix(uint32_t **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%u ", matrix[j][i]);
        }
        printf("\n");
    }
}

// Функция для заполнения матрицы случайными числами
void fill_matrix_random(uint32_t **matrix, int rows, int cols) {
    srand(time(NULL));
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = rand();
        }
    }
}

// Функция для заполнения матрицы вручную
void fill_matrix_manual(uint32_t **matrix, int rows, int cols) {
    printf("Введите элементы матрицы (%d строк и %d столбцов):\n", rows, cols);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (scanf("%u", &matrix[i][j]) != 1) {
                print_error("Введено некорректное число.");
            }
        }
    }
}

// Функция для вычисления характеристики столбца
uint32_t compute_column_characteristic(uint32_t *column, int rows) {
    uint32_t result = ~0; // Инициализация для "стрелки Пирса"
    for (int i = 0; i < rows; i++) {
        result = ~(result | column[i]);
    }
    return result;
}

// Функция для циклического сдвига столбца
void cyclic_shift_column(uint32_t *column, int rows, int shift) {
    shift = shift % rows; // Убедимся, что сдвиг не превышает количество строк
    if (shift == 0) return;

    uint32_t *temp = malloc(rows * sizeof(uint32_t));
    if (!temp) {
        print_error("Не удалось выделить память для сдвига столбца.");
    }

    for (int i = 0; i < rows; i++) {
        temp[(i + shift) % rows] = column[i];
    }

    for (int i = 0; i < rows; i++) {
        column[i] = temp[i];
    }

    free(temp);
}

// Основная функция для преобразования матрицы
void transform_matrix(uint32_t **matrix, int rows, int cols) {
    const char *debug = getenv("LAB2DEBUG");
    int debug_mode = debug && strcmp(debug, "1") == 0;

    for (int i = 0; i < cols; i++) {
        uint32_t characteristic = compute_column_characteristic(matrix[i], rows);
        int shift = (characteristic >> 24) & 0xFF; // Число единиц в старшем байте

        if (debug_mode) {
            printf("Характеристика столбца %d: %u\n", i + 1, characteristic);
            printf("Сдвиг для столбца %d: %d\n", i + 1, shift);
        }

        cyclic_shift_column(matrix[i], rows, shift);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_error("Отсутствует обязательный аргумент.");
    }

    int rows = 0, cols = 0;

    if (sscanf(argv[1], "%d", &rows) != 1 || sscanf(argv[2], "%d", &cols) != 1 || rows <= 0 || cols <= 0) {
        print_error("Некорректные размеры матрицы.");
    }

    int manual_input = 0;
    if (argc > 3 && strcmp(argv[3], "-m") == 0) {
        manual_input = 1;
    } else if (argc > 3) {
        print_error("Опция не поддерживается.");
    }

    uint32_t **matrix = allocate_matrix(rows, cols);

    if (manual_input) {
        fill_matrix_manual(matrix, rows, cols);
    } else {
        fill_matrix_random(matrix, rows, cols);
    }

    printf("Исходная матрица:\n");
    print_matrix(matrix, rows, cols);

    transform_matrix(matrix, rows, cols);

    printf("Преобразованная матрица:\n");
    print_matrix(matrix, rows, cols);

    free_matrix(matrix, cols);
    return 0;
}
