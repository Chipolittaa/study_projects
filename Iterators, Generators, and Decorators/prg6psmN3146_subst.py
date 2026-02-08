from prg6psmN3146_rnd import MyPRNG
import sys
import time
import inspect
from functools import wraps
from typing import Any, Callable, TypeVar, cast

F = TypeVar('F', bound=Callable[..., Any]) # переменная типа для декорируемых фунцкий

class shuffle_str_args: # класс-декоратор для перестановки символов в строковых аргументах функций и методов
    def __init__(self, prng: MyPRNG, filename: str | None = None):
        self.prng: MyPRNG = prng
        self.filename: str | None = filename

    def _decorate_method(self, func: F) -> F:
        @wraps(func) # обертка над декорируемой функцией
        def wrapper(*args: Any, **kwargs: Any) -> Any:
            new_args: list[Any] = [self.prng.shuffle_str(arg) if isinstance(arg, str) else arg for arg in args] # перестановка символов в строковых позиционных аргументах
            new_kwargs: dict[str, Any] = {k: self.prng.shuffle_str(v) if isinstance(v, str) else v for k, v in kwargs.items()} # перестановка символов в строковых именованных аргументах
            log_entry: str = f"{time.strftime('%d.%m.%y %H:%M:%S')} {func.__name__}(args={args}, kwargs={kwargs})" # формирование записи лога о вызове
            for i, (old, new) in enumerate(zip(args, new_args)): # добавление записей о заменах в позиционных аргументах
                if old != new:
                    log_entry += f"\n{time.strftime('%d.%m.%y %H:%M:%S')} {func.__name__}: аргумент {old} (args[{i}]) заменен на {new}"
            for key, old in kwargs.items(): # добавление записей о заменах в именованных аргументах
                if old != new_kwargs[key]:
                    log_entry += f"\n{time.strftime('%d.%m.%y %H:%M:%S')} {func.__name__}: аргумент {old} (kwargs[{key}]) заменен на {new_kwargs[key]}"
            result: Any = func(*new_args, **new_kwargs) # выполнение декорируемой функции с новыми аргументами
            log_entry += f" -> {result}\n" # добавление результата в лог
            if self.filename: # запись лога в файл или stderr
                with open(self.filename, 'a', encoding='utf-8') as f: # открытие файла для добавления записи
                    f.write(log_entry) # запись лога в файл
            else:
                sys.stderr.write(log_entry) # вывод лога в stderr, если имя файла передано не было
            return result # возврат результата функции
        return cast(F, wrapper) # возврат обернутой функции

    def __call__(self, obj: F | type) -> F | type:
        if inspect.isclass(obj): # проверка, является ли obj классом
            for name, method in inspect.getmembers(obj, inspect.isfunction): # перебор всех методов класса
                if not name.startswith('__'): # пропуск скрытых методов
                    setattr(obj, name, self._decorate_method(method)) # декорирование каждого  метода класса
            return obj # возврат декорированного класса
        return self._decorate_method(obj) # если obj - функция, декорируем её напрямую

