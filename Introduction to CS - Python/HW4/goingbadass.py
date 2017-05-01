import random

def is_prime(m, show_witness=False, sieve=False):

    """ probabilistic test for m's compositeness 
    adds a trivial sieve to quickly eliminate divisibility
    by small primes """

    if sieve:
        for prime in [2,3,5,7,11,13,17,19,23,29]:
            if m % prime == 0:
                return False

    for i in range(0,100):
        a = random.randint(1,m-1) # a is a random integer in [1..m-1]
        if pow(a,m-1,m) != 1:
            if show_witness:  # caller wishes to see a witness
                print(m,"is composite","\n",a,"is a witness, i=",i+1)
            return False

    return True

def find_prime(n):

    """ find random n-bit long prime"""

    while True:
        tmp = random.randrange(2**(n-1),2**n)
        if is_prime(tmp):
            return tmp


def DH_exchange(p):

    """ generates a shared DH key """

    g=random.randint(1,p-1)

    a=random.randint(1,p-1)# Alice's  secret
    x=pow(g,a,p)
    
    b=random.randint(1,p-1)# Bob's  secret
    y=pow(g,b,p)

    # generating joint, secret key
    key_A=pow(y,a,p)
    key_B=pow(x,b,p)

    return g, a, b, x, y, key_A # key_B = key_A


def crack_DH(p, g, x):

    ''' find secret "a" that satisfies g**a%p == x
        Not feasible for large p '''

    for a in range(1,p-1):
        if a%100000==0:
            print(a) #just to estimate running time
        if pow(g,a,p) == x:
            print (a)

    return None
