import pytest
from prg5psmN3146 import *


def test_err():
    with pytest.raises(FormatError):
        a = IPv4_Deque(['278.1.1.1'])
    with pytest.raises(TypeError):
        a = IPv4_Deque([90])
    with pytest.raises(UndoError):
        IPv4_Deque(['1.1.1.1']).undo()
    with pytest.raises(RedoError):
        IPv4_Deque(['1.1.1.1']).redo()


def test_append():
    a = IPv4_Deque()
    a.append('1.1.1.1')
    assert a == IPv4_Deque(['1.1.1.1'])
    a.appendleft('0.0.0.0')
    assert a == IPv4_Deque(['0.0.0.0', '1.1.1.1'])


def test_clear():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a.clear()
    assert len(a) == 0


def test_count():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2', '2.2.2.2'])
    assert a.count('2.2.2.2') == 2


def test_extend():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    b = IPv4_Deque(['3.3.3.3', '4.4.4.4'])
    a.extend(b)
    assert a == IPv4_Deque(['1.1.1.1', '2.2.2.2', '3.3.3.3', '4.4.4.4'])
    a.undo()
    c = IPv4_Deque(['5.5.5.5'])
    a.extendleft(c)
    assert a == IPv4_Deque(['5.5.5.5', '1.1.1.1', '2.2.2.2'])


def test_index():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    assert a.index('2.2.2.2') == 1


def test_insert():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a.insert(1, '3.3.3.3')
    assert a == IPv4_Deque(['1.1.1.1', '3.3.3.3', '2.2.2.2'])


def test_pop():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2', '3.3.3.3'])
    assert a.pop() == '3.3.3.3' and len(a) == 2
    assert a.popleft() == '1.1.1.1' and len(a) == 1


def test_remove():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a.remove('1.1.1.1')
    assert len(a) == 1 and a == IPv4_Deque(['2.2.2.2'])


def test_reverse():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a.reverse()
    assert a == IPv4_Deque(['2.2.2.2', '1.1.1.1'])


def test_rotate():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2', '3.3.3.3'])
    a.rotate(2)
    assert a == IPv4_Deque(['2.2.2.2', '3.3.3.3', '1.1.1.1'])


def test_delitem():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    del a[0]
    assert a == IPv4_Deque(['2.2.2.2'])


def test_iadd():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a += '3.3.3.3'
    assert a == IPv4_Deque(['1.1.1.1', '2.2.2.2', '3.3.3.3'])


def test_imul():
    a = IPv4_Deque(['1.1.1.1', '2.2.2.2'])
    a *= 2
    assert a == IPv4_Deque(['1.1.1.1', '2.2.2.2', '1.1.1.1', '2.2.2.2'])
