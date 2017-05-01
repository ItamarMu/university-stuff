#Skeleton file for HW4 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#You may add other utility functions to this file,
#but you may NOT change the signature of the existing ones.

#Change the name of the file to your ID number (extension .py).

############
# QUESTION 1
############
def max1(L):
    if len(L)==1:
        return L[0]
    return max(L[0], max1(L[1:]))

def max2(L):
    if len(L)==1:
        return L[0]
    l = max2(L[:len(L)//2])
    r = max2(L[len(L)//2:])
    return max(l,r)

def max11(L,left,right):
    if left==right:
        return L[left]
    return max(L[left], max11(L,left+1,right))

def max_list11(L):
    return max11(L,0,len(L)-1)

def max22(L, left, right):
    if left==right:
        return L[left]
    center=(left+right)//2
    return max(max22(L,left,center),max22(L,center+1,right))


def max_list22(L):
    return max22(L,0,len(L)-1)


############
# QUESTION 2
############
def chng(amount,coins,dc):
    if (amount == 0):
        return 1
    elif (amount < 0 or coins == ()):
        return 0
    if (amount,coins) in dc:
        return dc[amount,coins]
    else:
        dc[amount,coins]=chng(amount, coins[:-1],dc) + \
                          chng(amount - coins[-1], coins,dc)
        return dc[amount,coins]
        

def change_fast(amount, coins):
    memo=dict()
    return chng(amount,tuple(coins),memo)



############
# QUESTION 3
############
def win(n,m,hlst,dc,show=False):
    assert n>0 and m>0 and min(hlst)>=0 and max(hlst)<=n and len(hlst)==m
    if sum(hlst)==0:
        return True
    if hlst in dc:
        return dc[hlst]
    else:
        for i in range(m):  # for every column, i
            for j in range(hlst[i]): # for every possible move, (i,j)
                move_hlst = tuple([n]*i+[j]*(m-i)) # full height up to i, height j onwards
                new_hlst = tuple([min(hlst[i],move_hlst[i]) for i in range(m)]) # munching
                dc[new_hlst]=win(n,m,new_hlst,dc)
                if not dc[new_hlst]:
                    if show:
                        print(new_hlst)
                    return True
        return False


def win_fast(n, m, hlst, show=False):
    memo=dict()
    return win(n, m, tuple(hlst), memo, show)


############
# QUESTION 4
############

def choose_sets(lst, k):
    if k==len(lst):
        return [lst]
    if k==0:
        return [[]]
    tmp=choose_sets(lst[:-1],k-1)
    return [[lst[-1]]+tmp[i] for i in range(len(tmp))]+choose_sets(lst[:-1],k)

############
# QUESTION 5
############

import random

def is_prime(m,show_witness=False,sieve=False):
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
        

def density_primes(n, times=10000):
    cnt=0
    for i in range(times):
        num=random.randrange(2**(n-1),2**n)
        if is_prime(num):
            cnt+=1
    return cnt/times







   

########
# Tester
########

def test():

    # Q1 basic tests

    if max_list22([1,20,3]) != 20:
        print("error in max22()")
    if max_list22([1,20,300,400]) != 400:
        print("error in max22()")
        
    # Q2 basic tests
    if change_fast(10, [1,2,3]) != 14:
        print("error in change_fast()")

    # Q3 basic tests
    if win_fast(3, 4, [3,3,3,3]) != True:
        print("error in win_fast()")
    if win_fast(1, 1, [1]) != False:
        print("error in win_fast()")

    # Q4 basic tests
    if choose_sets([1,2,3,4], 0) != [[]]:
        print("error in choose_sets()")
    tmp = choose_sets(['a','b','c','d','e'], 4)
    if tmp == None:
        print("error in choose_sets()")
    else:
        tmp = sorted([sorted(e) for e in tmp])
        if tmp != [['a', 'b', 'c', 'd'], ['a', 'b', 'c', 'e'], ['a', 'b', 'd', 'e'], ['a', 'c', 'd', 'e'], ['b', 'c', 'd', 'e']]:
            print("error in choose_sets()")




    
