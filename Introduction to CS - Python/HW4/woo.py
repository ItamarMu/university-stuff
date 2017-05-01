
__author__ = 'shaked'
 
# -------------------------- #
# This is a test suite for HW4 of CS101
# If you want to run it, just change the import to call your answers file:
# Where: 'mod = import_module("hw4_shaked")'
# Change to: 'mod = import_module("your_file_name")' (No .py extension needed)
# Example: mod = import_module("123454321")
# Make sure they are in the same folder.
#
# You can see that you have a test case for almost every method in the hw4 file.
# Most of the test cases, uses random data. So every run you get different inputs to your methods.
# You can run this file multiple times to make sure you catch most of the possible cases.
# If you want, you can specifically define the amount of loops each test case is making
# just by changing the 'loops' variable most of the test cases get as parameter.
# Default 'loops' is set to 16.
#
# Please be noted that this tests don't check if you stands the complexity requirements,
# they only check if you return the correct value for a given input.
#
# If you have any questions or you find some bugs in this test file, please contact me =)
#
# Enjoy! Shaked.
# -------------------------- #
 
# External Modules
from random import randint
import sys
from itertools import combinations
from timeit import timeit
 
# Internal Modules
from importlib import import_module
mod = import_module("skeleton4")
for func in mod.__dict__:
    if not func.startswith("_"):
        globals()[func] = mod.__dict__[func]
 
# Make sure we can test every case
sys.setrecursionlimit(5000)
 
 
class HW4Tester:
    def __init__(self, loops):
        self.loops = loops
        self.ERRORS = False
 
    def test_max22(self, loops=None):
        if not loops:
            loops = self.loops
 
        for i in range(loops):
            test_list = [randint(-100, 100) for i in range(randint(1, 64))]
 
            expected = max(test_list) if test_list else None
            actual = max_list22(test_list)
 
            if not actual == expected:
                self.ERRORS = True
                print(">> Error in test %s: Expected: %s but got: %s. Run on input: %s"
                      % (str("max22"), str(expected), str(actual), str(test_list)))
 
    def change(self, amount, coins):
        if amount == 0:
            return 1
        if amount < 0 or len(coins) == 0:
            return 0
 
        return self.change(amount, coins[:-1]) + self.change(amount - coins[-1], coins)
 
 
    def test_change_fast(self, l_loops=None, verbose=False):
        if not l_loops:
            l_loops = self.loops
 
        for i in range(l_loops):
            amount = randint(0, 32)
            coins = sorted(list(set([randint(1, 10) for i in range(0, 8)])))
 
            expected = self.change(amount, coins)
            actual = change_fast(amount, coins)
 
            if not actual == expected:
                self.ERRORS = True
                print(">> Error in test_%s: Expected: %s but got: %s. Run on input: %s"
                      % (str("change_fast"), str(expected), str(actual), (amount, coins)))
            else:
                if verbose:
                    print(">> Done in test_%s: Expected: %s and got: %s. Run on input: %s"
                          % (str("change_fast"), str(expected), str(actual), str(amount, str(coins))))
 
    '''def win(self, n, m, hlst, show=False):
        assert n > 0 and m > 0 and min(hlst) >= 0 and max(hlst) <= n and len(hlst) == m
        if sum(hlst) == 0:
            return True
        for i in range(m):  # for every column, i
            for j in range(hlst[i]):  # for every possible move, (i,j)
                move_hlst = [n] * i + [j] * (m - i)  # full height up to i, height j onwards
                new_hlst = [min(hlst[i], move_hlst[i]) for i in range(m)]  # munching
                if not self.win(n, m, new_hlst):
                    if show:
                        print(new_hlst)
                    return True
        return False
 
    def test_win_fast(self, l_loops=None, verbose=False):
        if not l_loops:
            l_loops = self.loops
 
        for i in range(l_loops):
            n = randint(1, 6)
            m = randint(1, 6)
            test_hlst = [randint(0, n) for i in range(m)]
 
            actual = win_fast(n, m, test_hlst, verbose)
            expected = self.win(n, m, test_hlst, verbose)
 
            if not actual == expected:
                self.ERRORS = True
                print(">> Error in test_%s: Expected: %s but got: %s. Run on input: %s"
                      % (str("win_fast"), str(expected), str(actual), str((n, m, test_hlst))))
 '''
    def __compare_lists__(self, lst1, lst2):
        if not len(lst1) == len(lst2):
            return False
 
        my_lst1 = lst1[:]
        my_lst2 = lst2[:]
        for i in range(len(my_lst1)):
            my_lst1[i] = sorted(my_lst1[i])
            my_lst2[i] = sorted(my_lst2[i])
 
        if not sorted(my_lst1) == sorted(my_lst2):
            return False
 
        return True
 
    def combos(self, lst, k):
        return [list(combo) for combo in combinations((e for e in lst), k)]
 
    def test_choose_sets(self, l_loops=None, verbose=False):
        if not l_loops:
            l_loops = self.loops
 
        for i in range(l_loops):
            test_list = list(range(0, randint(0, 16)))
            k = randint(0, len(test_list))
 
            if verbose:
                print(">> Run test_%s on: %s"
                      % (str("choose_sets"), str((test_list, k))))
 
            expected = self.combos(test_list, k)
            actual = choose_sets(test_list, k)
 
            if not self.__compare_lists__(actual, expected):
                self.ERRORS = True
                print(">> Error in test %s: Expected: %s but got: %s. "
                      "Run on input: %s"
                      % (str("choose_sets"), str(expected),
                         str(actual), str((test_list, k))))
 
 
    def test_density_primes(self, verbose=False):
        for n in [100, 200, 300, 400, 500]:
            res = density_primes(n)
 
            if verbose:
                print("Check number of %d bits, Got: %f probability." % (n, res))
 
 
def run_tests(loops=1024):
    t = HW4Tester(loops)
    print(">> Running given tests...")
    test()
    print(">> Done.")
 
    print()
 
    print(">> Running awesome random tests...")
    t.test_max22()
    print(">> ...")
    t.test_change_fast()
    print(">> It can take a while... But it's totally worth it ^_^")
    #t.test_win_fast()
    print(">> ...")
    t.test_choose_sets()
    # t.test_density_primes()
    print(">> Done.")
    print()
 
    if t.ERRORS:
        print(">> Sorry, but you have a few errors :(")
    else:
        print(">> If no errors showed up, all tests passed =)")
 
 
run_tests()
