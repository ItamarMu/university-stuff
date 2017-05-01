import time

def elapsed(expression,number=1):
    ''' computes elapsed time for executing code
    number of times (default is 1 time). expression should
    be a string representing a Python expression. '''
    t1=time.clock()
    for i in range(number):
        eval(expression)
    t2=time.clock()
    return t2-t1