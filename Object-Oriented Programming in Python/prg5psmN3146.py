from collections import deque
from re import match


class FormatError(Exception):
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return f"Строка '{self.name}' не является IPv4-адресом."


class UndoError(Exception):
    def __str__(self):
        return "Нет состояний для undo()."


class RedoError(Exception):
    def __str__(self):
        return "Нет состояний для redo()."


def is_ipv4(*args):
    pattern = "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
    for value in args:
        if not isinstance(value, str) and not isinstance(value, IPv4_Deque):
            raise TypeError
        if isinstance(value, str) and not match(pattern, value) or isinstance(value, IPv4_Deque) and not all(match(pattern, i) for i in value):
            raise FormatError(value)
    return True


class IPv4_Deque(deque):
    def __init__(self, args=()):
        is_ipv4(*args)
        super().__init__(value for value in args)
        self.__undo_list = []
        self.__redo_list = []

    def append(self, ipv4):
        is_ipv4(ipv4)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().append(ipv4)

    def appendleft(self, ipv4):
        is_ipv4(ipv4)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().appendleft(ipv4)

    def clear(self, redo_flag=False):
        if not redo_flag:
            self.__undo_list.append(self.copy())
            self.__redo_list.clear()
        return super().clear()

    def count(self, ipv4):
        is_ipv4(ipv4)
        return super().count(ipv4)

    def extend(self, *args, flag=False):
        is_ipv4(*args)
        if not flag:
            self.__undo_list.append(self.copy())
            self.__redo_list.clear()
        return super().extend(*args)

    def extendleft(self, *args):
        is_ipv4(*args)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().extendleft(*args)

    def index(self, ipv4, start=None, stop=None):
        is_ipv4(ipv4)
        return super().index(ipv4, start if start else 0, stop if stop else len(self))

    def insert(self, index, ipv4):
        is_ipv4(ipv4)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().insert(index, ipv4)
    
    def pop(self):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().pop()
    
    def popleft(self):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().popleft()

    def redo(self):
        if len(self.__redo_list) == 0:
            raise RedoError
        self.__undo_list.append(self.copy())
        self.clear(True)
        self.extend(self.__redo_list.pop(-1), flag=True)

    def remove(self, ipv4):
        is_ipv4(ipv4)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().remove(ipv4)
    
    def reverse(self):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().reverse()
    
    def rotate(self, n=1):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().rotate(n)

    def undo(self):
        if len(self.__undo_list) == 0:
            raise UndoError
        self.__redo_list.append(self.copy())
        self.clear(True)
        self.extend(self.__undo_list.pop(-1), flag=True)

    def __delitem__(self, key):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().__delitem__(key)

    def __iadd__(self, other):
        is_ipv4(other)
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().__iadd__(other if not isinstance(other, str) else [other])

    def __imul__(self, value):
        self.__undo_list.append(self.copy())
        self.__redo_list.clear()
        return super().__imul__(value)
