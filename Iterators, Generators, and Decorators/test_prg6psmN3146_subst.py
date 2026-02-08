from prg6psmN3146_rnd import midsquare, MyPRNG
from prg6psmN3146_subst import shuffle_str_args
from collections.abc import Iterator
from typing import Any, Callable


def test_function() -> None: # тест декорирования функции
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    @shuffle_str_args(prng) # определение декорируемой функции
    def foo(a: str, b: int) -> str:
        return a + str(b) # пусть функция возвращает строку, состояющую из строки и числа
    result: str = foo("hello", 5) # вызов функции
    assert isinstance(result, str) and set(result) == set("hello5") and len(result) == len("hello5") # проверка результата на принадлежность классу строк и его соответствия перемешанной строке


def test_class() -> None: # тест декорирования класса
    gen: Iterator[int] = midsquare(1234) # создание генератора
    prng: MyPRNG = MyPRNG(gen) # создание экземпляра MyPRNG
    @shuffle_str_args(prng) # определение декорируемого класса
    class Boo:
        def method(self, s: str) -> str:
            return s # пусть метод возвращает переданную ему строку
    obj: Any = Boo() # создание экземпляра класса
    result: str = obj.method("test") # вызов метода
    assert isinstance(result, str) and set(result) == set("test") and len(result) == len("test") # проверка результата