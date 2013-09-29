#!/usr/bin/env python

class Class():
    """A simple example class"""
    i = 12345

    def __init__(self):
        self.data = []

    def f(self):
        return 'hello'
        
    def n(self, name):
        return 'hello ' + name


def main():
    x = Class()
    print Class.i
    print x.i
#print Class.f() # cannot call
    print Class.f(x)
    print x.f()  # do the same thing as Class.f(x)
    print x.n('Berm')
    print Class.__doc__
    print x.__doc__
#print type(Class.data) # cannot call
    print type(x.data)
    x.counter = 1
    while x.counter < 10:
        x.counter = x.counter * 2
    print x.counter
    del x.counter
    xn = x.n
    del x
    print xn('BB')


if __name__ == '__main__':
    main()

