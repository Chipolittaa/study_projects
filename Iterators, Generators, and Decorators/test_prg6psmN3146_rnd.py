from prg6psmN3146_rnd import midsquare, MyPRNG
from collections.abc import Iterator
import pytest
from typing import List


def test_midsquare_create() -> None: # тест генератора на существование
    gen: Iterator[int] = midsquare(1234) # создание генератора с сидом 1234
    assert isinstance(gen, Iterator) # проверка, что результат является итератором


def test_midsquare_sequence() -> None: # тест генерации последовательности
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    values: List[int] = [prng.next_int() for _ in range(3)] # генерация 3 чисел
    assert len(values) == 3 # проверка длины последовательности


def test_next_float() -> None: # тест метода next_float
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    f: float = prng.next_float() # получение вещественного числа
    assert 0.0 <= f < 1.0 # проверка диапазона вещественного числа


def test_next_str() -> None: # тест метода next_str
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    s: str = prng.next_str() # получение случайной строки
    assert isinstance(s, str) and 1 <= len(s) <= 50 # порверка строки на длину (от 1 до 50)

    
def test_shuffle_str() -> None: # тест метода shuffle_str
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    s: str = "hello" # исходная строка
    shuffled: str = prng.shuffle_str(s) # перестановка символов
    assert set(shuffled) == set(s) and len(shuffled) == len(s) # проверка совпадения набора символов и длины исходной строки c перемешанной
