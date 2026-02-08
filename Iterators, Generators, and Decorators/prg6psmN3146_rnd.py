from abc import ABC, abstractmethod
from collections.abc import Iterator
import string


def midsquare(seed: int, N: int = 32) -> Iterator[int]: # определение генератора срединных квадратов
    if seed < 0: # проверка сида
        raise ValueError("Начальное значение должно быть неотрицательным.")
    if N < 8 or N % 4 != 0: # проверка корректности N
        raise ValueError("Размерность чисел должна быть кратна четырем и больше восьми.")
    current: int = seed # назначение текущего значения последовательности
    while True: # бесконечный цикл для генерации чисел
        sqr: int = current * current # вычисление квадрата текущего значения
        shifted: int = sqr >> (N // 4) # сдвиг вправо на N // 4 для извлечения средних битов
        mask: int = (1 << N) - 1 # маска для получения N битов
        current = shifted & mask # извлечение нового значения с помощью побитового И. таким образом, полученное число будет состоять из середины квадрата предыдущего
        yield current # возврат текущего значения


class PRNGBase(ABC): # абстрактный базовый класс для ГПСЧ
    @abstractmethod
    def next_int(self) -> int:
        # абстрактный метод для получения целого числа
        pass

    @abstractmethod
    def next_float(self) -> float:
        # абстрактный метод для получения числа от 0.0 до 1.0
        pass

    @abstractmethod
    def next_str(self) -> str:
        # абстрактный метод для получения случайной строки
        pass

    @abstractmethod
    def shuffle_str(self, src: str) -> str:
        # абстрактный метод для перестановки символов в строке
        pass


class MyPRNG(PRNGBase): # реализация класса MyPRNG
    def __init__(self, gen: Iterator[int]):
        self.gen: Iterator[int] = iter(gen)
        self.max_int: int = 2**32 - 1 # максимальное значение для нормализации (32 бита)

    def next_int(self) -> int:
        return next(self.gen) # получение следующего целого числа из генератора

    def next_float(self) -> float:
        return self.next_int() / (self.max_int + 1) # нормализация next_int к диапазону [0.0, 1.0)

    def next_str(self) -> str:
        length: int = (self.next_int() % 50) + 1 # определение случайной длины строки от 1 до 50
        chars: list[str] = [string.ascii_letters[self.next_int() % len(string.ascii_letters)] for _ in range(length)] # генерация списка символов из алфавита
        return ''.join(chars) # объединение всех полученных символов в строку

    def shuffle_str(self, src: str) -> str:
        chars: list[str] = list(src) # преобразование строки в список символов для более удобной работы
        for i in range(len(chars) - 1, 0, -1): # применение метода Фишера-Йетса для перестановки символов в строке
            j: int = self.next_int() % (i + 1) # генерация случайного индекса
            chars[i], chars[j] = chars[j], chars[i] # обмен элементов
        return ''.join(chars) # объединение символов обратно в строку