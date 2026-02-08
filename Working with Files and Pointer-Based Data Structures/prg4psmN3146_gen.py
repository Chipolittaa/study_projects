#!/usr/bin/python3

from random import choices, shuffle, randint # функции для случайной генерации
from string import ascii_letters # константа для генерации случайной области data
from argparse import ArgumentParser # парсинг аргументов

# информация об авторе
def info():
    return "Махрова Полина Степановна, группа N3146, вариант: 3-9-1-1."

# функция создания случайного ISBN-13
def random_ISBN():
    tmp = choices(range(10), k = 12) # сначала выбираются первые 12 цифр, а потом рассчитывается тринадцатая
    isbn = f"{tmp[0]}{tmp[1]}{tmp[2]}-{tmp[3]}-{tmp[4]}{tmp[5]}{tmp[6]}-{tmp[7]}{tmp[8]}{tmp[9]}{tmp[10]}{tmp[11]}-{(10 - (tmp[0] + 3 * tmp[1] + tmp[2] + 3 * tmp[3] +\
                                                                                                                            tmp[4] + 3 * tmp[5] + tmp[6] + 3 * tmp[7] +\
                                                                                                                            tmp[8] + 3 * tmp[9] + tmp[10] + 3 * tmp[11]) % 10) % 10}"
    return isbn

# функция записи в файл
def write(n, filename):
    index_list = [i for i in range(n)] # список с индексами
    shuffle(index_list) # индексы перемешиваются
    isbn_list = [random_ISBN() + '\0' for _ in range(n)] # список с ISBN-13
    data = "".join(choices(ascii_letters, k = randint(0, 30))) # случайные данные
    with open(filename, "wb") as f:
        f.write(n.to_bytes(length = 4, byteorder = "little", signed = False)) # запись количества ISBN-13 в файл
        for isbn in isbn_list:
            f.write(isbn.encode("utf-8")) # запись всех ISBN-13 в файл
        f.write(data.encode("utf-8")) # запись случайных данных в файл
        for i in index_list:
            f.write(i.to_bytes(length = 2, byteorder = "little", signed = False)) # запись индексов
    print(f"Записано ISBN-13: {n}") # информация о совершенных записях
    for i in range(n):
        print(f"[ {i} ] : {isbn_list[index_list.index(i)]}")

# основная функция
def main():
    n = randint(10, 1000) # случайное количество ISBN-13
    p = ArgumentParser() # парсер и его аргументы
    p.add_argument("filename", help = "Имя файла для записи ISBN-13", type = str)
    p.add_argument("-n", help = "Количество ISBN-13", type = int)
    p.add_argument("-v", action = "version", help = "Информация о студенте", version = info())
    args = p.parse_args() # парсинг аргументов
    if args.n is not None: # если была передана опция -n и удалось считать число, то оно проверяется на натуральность
        if args.n > 0:
            n = args.n # если число натуральное, то оно записывается в n
        else:
            print("Передано некорректное число.") # в противном случае выводится ошибка
            exit(1)
    write(n, args.filename) # запись в файл
    exit(0) # успешное завершение работы программы

# настройка для выполнения кода только из основной программы
if __name__ == "__main__":
    main()