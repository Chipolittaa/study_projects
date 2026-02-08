#include <stdio.h> // библиотека для ввода/вывода
#include <stdlib.h> // библиотека для работы с динамической памятью и рандомом
#include <stdbool.h> // библиотека для работы с булевым типом данных
#include <stdint.h> // библиотека для работы с uint32_t и uint16_t
#include <string.h> // библиотека для работы со строками
#include <time.h> // библиотека для задания сида в srand

// структура узла списка
typedef struct Node {
    char isbn[18];
    struct Node *both;
} Node;

// список
typedef struct List {
    Node *begin;
    Node *end;
    size_t size;
} List;

// структура одной записи ISBN13 (вспомогательная)
typedef struct ISBN13 {
    char isbn[18];
    uint16_t index;
} ISBN13;

// ксор двух узлов
Node *XOR(Node *a, Node *b) {
    return (Node *)((uintptr_t) a ^ (uintptr_t) b);
}

// инициализация списка
void init(List *l) {
    l->begin = l->end = NULL;
    l->size = 0;
}

// добавление в начало
int push_front(List *l, const char str[18]) {
    Node *newNode = (Node *) malloc(sizeof(Node)); // выделение памяти для нового узла
    if (!newNode) return 3; // если не получилось, то возврат ошибки
    strcpy(newNode->isbn, str); // копирование строки
    newNode->both = l->begin;
    if (l->begin) l->begin->both = XOR(XOR(l->begin->both, NULL), newNode); // изменение результата ксора у первого элемента, если список непустой
    else l->end = newNode; // если список пустой, то новый узел становится концом
    l->begin = newNode; // новым список становится началом
    l->size++; // изменение размера
    return 0; // если все успешно, то возвращается 0
}

// добавление в конец
int push_back(List *l, const char str[18]) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode) return 3;
    strcpy(newNode->isbn, str);
    newNode->both = l->end;
    if (l->begin) l->end->both = XOR(XOR(l->end->both, NULL), newNode);
    else l->begin = newNode;
    l->end = newNode;
    l->size++;
    return 0;
}

// удаление из начала
void pop_front(List *l) {
    if (!l->begin) return; // если список пустой, то операция не имеет смысла
    Node *next = l->begin->both; // указатель на следующий элемент (второй)
    if (next) next->both = XOR(next->both, l->begin); // если второй элемент есть, то результат ксора изменяется
    else l->end = NULL; // в противном случае последний элемент сбрасывается
    free(l->begin); // очистка памяти
    l->begin = next; // изменение начала списка
    l->size--; // уменьшение размера
}

// удаление с конца
void pop_back(List *l) {
    if (!l->begin) return;
    Node *prev = l->end->both;
    if (prev) prev->both = XOR(prev->both, l->end);
    else l->begin = NULL;
    free(l->end);
    l->end = prev;
    l->size--;
}

// очистка списка
void clean(List *l) {
    Node *cur = l->begin, *prev = NULL; // текущий и предыдущий элементы для прохода по списку
    while (cur) {
        Node *next = XOR(prev, cur->both); // следующий элемент списка
        if (prev) free(prev); // очистка памяти от предыдущего элемента
        prev = cur; // переход к следующему элементу
        cur = next;
    }
    if (prev) free(prev); // очистка от последнего элемента
    l->begin = l->end = NULL; // сброс начала, конца и размера списка
    l->size = 0;
}

// вывод списка
void dump(List *l, const char *filename) {
    FILE *f = (strcmp(filename, "")) ? fopen(filename, "w") : NULL; // если передано название файла, то он открывается для записи
    Node *cur = l->begin, *prev = NULL;
    while (cur) {
        if (f) fprintf(f, "%lu %s\n", (uintptr_t) cur->both, cur->isbn); // запись в файл
        else printf("%lu %s\n", (uintptr_t) cur->both, cur->isbn); // вывод в поток
        Node *next = XOR(prev, cur->both); // переход к следующему элементу
        prev = cur;
        cur = next;
    }
}

// перемешивание списка 
void shuffle(List *l) {
    if (!l->begin) return; // если список пустой, то смысла в операции нет
    List tmp; // временный список
    init(&tmp); // его инициализация
    bool false_flag = true; // флаг для отслеживания еще не тронутых элементов
    bool *flags_arr = (bool *) malloc(sizeof(bool) * l->size); // массив с флагами для каждого элемента (был он переставлен или нет)
    for (size_t i = 0; i < l->size; ++i) {
        flags_arr[i] = false; // инициализация элементов массива
    }
    while (false_flag) { // цикл выполняется, пока не будут перемещены все элементы
        size_t index = (size_t) rand() % l->size; // случайный индекс, который будет перемещаться
        while (flags_arr[index]) index = (size_t) rand() % l->size; // его повторная генерация, если индекс уже был перемещен
        flags_arr[index] = true; // флаг перемещения поднимается
        false_flag = false; // флаг отслеживания сбрасывается
        for (size_t i = 0; i < l->size; ++i) {
            if (!flags_arr[i]) {
                false_flag = true; // если в массиве есть false, то флаг поднимается
                break;
            }
        }
        Node *cur = l->begin, *prev = NULL;
        size_t count = 0;
        while (count != index) { // проход по списку до элемента с индексом index
            Node *next = XOR(prev, cur->both);
            prev = cur;
            cur = next;
            count++;
        }
        push_back(&tmp, cur->isbn); // добавление этого элемента в конец tmp
    }
    free(flags_arr); // очистка массива
    clean(l); // очистка изначального списка
    *l = tmp; // переназначение указателя на новый список
}

// функция сравнения для qsort()
int cmp(const void *a, const void *b) {
    return (int)(((ISBN13 *)a)->index) - (int)(((ISBN13 *)b)->index);
}

// проверка на корректность
bool valid(const char *str) {
    if (strlen(str) != 17) return false;
    for (size_t i = 0; i < 17; ++i) {
        if (i == 3 || i == 5 || i == 9 || i == 15) {
            if (str[i] != '-') return false;
        }
        else if (str[i] < '0' || str[i] > '9') return false;
    }
    if (((str[0] - '0') + 3 * (str[1] - '0') +
        (str[2] - '0') + 3 * (str[4] - '0') +
        (str[6] - '0') + 3 * (str[7] - '0') +
        (str[8] - '0') + 3 * (str[10] - '0') +
        (str[11] - '0') + 3 * (str[12] - '0') +
        (str[13] - '0') + 3 * (str[14] - '0') +
        (str[16] - '0')) % 10 != 0) return false;

    return true;
}

// информация об авторе
void info() {
    printf("Махрова Полина Степановна, группа N3146, вариант: 3-9-1-1.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Ошибка: нет обязательного аргумента.\n"); // если передано мало аргументов, то выводится ошибка
        exit(1);
    }
    if (!strcmp(argv[1], "-v")) {
        info(); // если передана опция -v
        exit(0);
    }
    if (argv[1][0] == '-') {
        fprintf(stderr, "Ошибка: опция '%s' не поддерживается.", argv[1]); // если передается неподдерживаемая опция, то выводится ошибка
        exit(2);
    }
    srand(time(NULL)); // задание начального значения для генератора
    int exit_code = 0; // код возврата
    FILE *file = fopen(argv[1], "rb"); // открытие файла для чтения
    List lst; // создание списка
    init(&lst); // инициализация
    uint32_t cnt = 0, s_ofst = 0, i_ofst = 0; // cnt - количество ISBN13, s_ofst - положение последней непрочитанной строки, i_ofst - положение последнего непрочитанного индекса
    uint16_t i = 0; // индекс
    if (file) {
        fread(&cnt, sizeof(uint32_t), 1, file); // чтение количества
        ISBN13 *isbn_arr = (ISBN13 *) malloc(sizeof(ISBN13) * cnt); // вспомогательный массив с ISBN13
        if (!isbn_arr) {
            fprintf(stderr, "Ошибка: не удалось выделить память.\n");
            clean(&lst);
            fclose(file);
            exit(3);
        }
        for (uint32_t j = 0; j < cnt; ++j) {
            char c;
            size_t size = 0;
            memset(isbn_arr[j].isbn, 0, sizeof(isbn_arr[j].isbn)); // обнуление буфера
            while (fread(&c, sizeof(char), 1, file) == 1 && c != '\0') {
                isbn_arr[j].isbn[size++] = c; // запись строки посимвольно
            }
            s_ofst = ftell(file); // сохранение положения курсора
            fseek(file, 0, SEEK_END); // переход к концу файла
            fseek(file, ftell(file) - cnt * 2 + i_ofst, SEEK_SET); // переход к последнему непрочитанному индексу
            fread(&i, sizeof(uint16_t), 1, file); // чтение индекса
            isbn_arr[j].index = i; // запись индекса
            i_ofst += 2; // изменение положения индекса (2 байта)
            fseek(file, s_ofst, SEEK_SET); // возврат к строке
        }
        fclose(file); // закрытие файла
        qsort(isbn_arr, cnt, sizeof(ISBN13), cmp); // сортировка
        for (uint32_t j = 0; j < cnt; ++j) {
            if (valid(isbn_arr[j].isbn)) {
                if (push_back(&lst, isbn_arr[j].isbn)) {
                    fprintf(stderr, "Ошибка: не удалось выделить память.\n"); // если не удалось добавить строку, то выводится ошибка
                    free(isbn_arr);
                    clean(&lst);
                    exit(3);
                }
            }
            else fprintf(stderr, "Строка '%s' была пропущена.\n", isbn_arr[j].isbn); // если не пройдена проверка на корректность, то выводится предупреждение
        }
        free(isbn_arr); // очистка массива
    }
    char *str = NULL, **args = NULL; // str - вводимая строка, args - указатель на аргументы команды
    int index = 0, args_size = 0, ch; // index - текущая длина строки, args_size - текущая длина массива с аргументами, ch - символ для чтения строки
    bool PushFrontFlag = false, PusBackFlag = false, DumpFlag = false; // флаги для команд push_front, push_back и dump
    while ((ch = getchar()) != EOF) { // чтение каждого символа
        char c = (char) ch;
        if ((!PushFrontFlag && !PusBackFlag && c != '\n' && c != ' ') || ((PushFrontFlag || PusBackFlag) && c != '\n' && c != ' ')) { // чтение команды или аргументов
            str = (char *) realloc(str, sizeof(char) * ++index); // увеличение размера str
            if (!str) {
                fprintf(stderr, "Ошибка: не удалось выделить память.\n");
                exit_code = 3;
                break;
            }
            str[index - 1] = c; // запись символа
        }
        else if (!(PushFrontFlag || PusBackFlag || DumpFlag) && (c == ' ' || c == '\n')) { // если пользователь ввел команду, то ее нужно проверить на существование
            str[index] = '\0';
            index = 0;
            if (!strcmp(str, "push_front")) PushFrontFlag = 1; // если введена команда push_front, то флаг PushFrontFlag поднимается
            else if (!strcmp(str, "push_back")) PusBackFlag = 1; // если введена команда push_back, то флаг PusBackFlag поднимается
            else if (!strcmp(str, "pop_front")) pop_front(&lst); // если введена команда pop_front, то первый элемент списка удаляется
            else if (!strcmp(str, "pop_back")) pop_back(&lst); // если введена команда pop_back, то последний элемент списка удаляется
            else if (!strcmp(str, "dump")) { // если введена команда dump:
                if (c != ' ') dump(&lst, ""); // вывод в поток вывода, если файл не был передан
                else DumpFlag = 1; // если был передан файл, то флаг DumpFlag поднимается
            }
            else if (!strcmp(str, "shuffle")) shuffle(&lst); // если введена команда shuffle, то производится перемешивание списка
            else {
                fprintf(stderr, "Ошибка: команда '%s' не существует.\n", str);
                exit_code =  4;
                break;
            }
            if (str) { // очистка строки
                free(str);
                str = NULL;
            }
        }
        else if ((PushFrontFlag || PusBackFlag || DumpFlag) && (c == '\n' || c == ' ')){ // если происходит обработка аргументов команды и встречен символ перевода строки или пробела, то:
            str[index] = '\0';
            index = 0;
            args = (char **) realloc(args, sizeof(char *) * ++args_size); // увеличение размера массива
            if (!args) {
                fprintf(stderr, "Ошибка: не удалось выделить память.\n");
                exit_code = 3;
                break;
            }
            args[args_size - 1] = str; // сохранение строки в массив
            if (c == '\n') { // если был введен символ перевода строки, то произошла обработка последнего аргумента
                if (PushFrontFlag) { // push_front
                    for (int i = args_size - 1; i >= 0; --i) { // цикл по аргументам
                        if (valid(args[i])) { // проверка на корректность
                            if (push_front(&lst, args[i]) != 0) { // добавление в список
                                fprintf(stderr, "Ошибка: не удалось выделить память.\n");
                                exit_code = 3;
                                break;
                            }
                        }
                        else {
                            fprintf(stderr, "Строка '%s' была пропущена.\n", args[i]); // если проверка не пройдена, то выводится предупреждение
                        }
                        free(args[i]); // освобождение памяти
                    }
                    if (exit_code != 0) break; // цикл прерывается, если была ошибка
                    PushFrontFlag = 0; // флаг опускается
                }
                else if (PusBackFlag) { // push_back
                    for (int i = 0; i < args_size; ++i) { // цикл по аргументам
                        if (valid(args[i])) { // проверка на корректность
                            if (push_back(&lst, args[i]) != 0) { // добавление в список
                                fprintf(stderr, "Ошибка: не удалось выделить память.\n");
                                exit_code = 3;
                                break;
                            }
                        }
                        else {
                            fprintf(stderr, "Строка '%s' была пропущена.\n", args[i]); // если проверка не пройдена, то выводится предупреждение
                        }
                        free(args[i]); // освобождение памяти
                    }
                    if (exit_code != 0) break; // цикл прерывается, если была ошибка
                    PusBackFlag = 0; // флаг опускается
                }
                else if (DumpFlag) { // dump
                    for (int i = 0; i < args_size; ++i) { // цикл по аргументам
                        dump(&lst, args[i]); // запись в файл
                    }
                   DumpFlag = 0; // флаг опускается
                }
                free(args); // освобождение памяти от массива
                args = NULL; // сброс указателя
                args_size = 0; // сброс размера
            }
            str = NULL; // сброс указателя строки
        }
    }
    file = fopen(argv[1], "wb"); // открытие файла для записи
    if (lst.size) {
        fwrite(&lst.size, sizeof(uint32_t), 1, file); // если список непустой, то в файл записывается размер
    }
    Node *cur = lst.begin, *prev = NULL;
    while (cur) {
        fwrite(cur->isbn, sizeof(char), 18, file); // запись всех ISBN-13
        Node *next = XOR(prev, cur->both);
        prev = cur;
        cur = next;
    }
    for (i = 0; i < lst.size; ++i) fwrite(&i, sizeof(uint16_t), 1, file); // запись индексов
    fclose(file); // закрытие файла
    clean(&lst); // очистка списка
    exit(exit_code); // завершение работы программы с переданным кодом
}