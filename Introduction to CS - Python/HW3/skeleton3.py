#Skeleton file for HW3 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#You may add other utility functions to this file,
#but you may NOT change the signature of the existing ones.

#Change the name of the file to your ID number (extension .py).

############
# QUESTION 2
############

def find_root(f, a, b, EPS=0.001):
    if (f(a)>0 and f(b)>0) or (f(a)<0 and f(b)<0):
        return None
    M=f((a+b)/2)
    tmp=0
    if f(a)>f(b):
        tmp=a
        a=b
        b=tmp
    while (abs(M)>EPS):
        if M<0:
            a=(a+b)/2
            M=f((a+b)/2)
        else:
            b=(a+b)/2
            M=f((b+a)/2)
    return (a+b)/2


############
# QUESTION 3
############

# b
def multi_merge_v2(lst_of_lsts):
    minS=[lst[0] for lst in lst_of_lsts]
    final=[]
    idx=0
    while minS!=[]:
        TmpMin=min(minS)
        idx=minS.index(TmpMin)
        final.append(TmpMin)
        lst_of_lsts[idx].remove(TmpMin)
        if lst_of_lsts[idx]!=[]:
            minS[idx]=lst_of_lsts[idx][0]
        else:
            minS.remove(TmpMin)
            lst_of_lsts.remove([])
    return final

def merge(lst1, lst2):
    """ merging two ordered lists using
        the two pointer algorithm """
    n1 = len(lst1)
    n2 = len(lst2)
    lst3 = [0 for i in range(n1 + n2)]  # alocates a new list
    i = j = k = 0  # simultaneous assignment
    while (i < n1 and j < n2):
        if (lst1[i] <= lst2[j]):
            lst3[k] = lst1[i]
            i = i +1
        else:
            lst3[k] = lst2[j]
            j = j + 1
        k = k + 1  # incremented at each iteration
    lst3[k:] = lst1[i:] + lst2[j:]  # append remaining elements
    return lst3

# c
def multi_merge_v3(lst_of_lsts):
    m = len(lst_of_lsts)
    merged = []
    for i in range(m):
        merged=merge(merged,lst_of_lsts[i])
    return merged



############
# QUESTION 5
############

# a
def steady_state(lst):
    pt1=0
    pt2=len(lst)-1
    mid=0
    while (pt1<=pt2):
        mid=(pt1+pt2)//2
        if lst[mid]==mid:
            return mid
        if lst[mid]>mid:
            pt2=mid-1
        else:
            pt1=mid+1
    return None
# d
def cnt_steady_states(lst):
    idx=steady_state(lst)
    if idx==None:
        return 0
    pt1,pt2,pt3,pt4=0,idx-1,idx+1,len(lst)-1
    cnt1,cnt2=0,0
    mid12,mid34=(pt1+pt2)//2,(pt3+pt4)//2
    while (pt1<=pt2):
        if idx-lst[mid12]==idx-mid12:
            pt2=mid12-1
        else:
            pt1=mid12+1
        mid12=(pt1+pt2)//2
    while (pt3<=pt4):
        if lst[mid34]-idx==mid34-idx:
            pt3=mid34+1
        else:
            pt4=mid34-1
        mid34=(pt3+pt4)//2
    return mid34-mid12


############
# QUESTION 6
############
def sort_num_list(lst):
    k=max(abs(min(lst)),abs(max(lst)))
    kLis=[i/2 for i in range(-2*k,2*k+1)]
    kCnt=[0 for i in range(-2*k,2*k+1)]
    final=[]
    tmp=0
    for num in lst:
        tmp=int(2*(num-(-abs(k))))
        kCnt[tmp]+=1
    for j in range(len(kCnt)):
        final.extend(kCnt[j]*[kLis[j]])
    return final



   
    
########
# Tester
########

def test():
    
    f1 = lambda x : x - 1
    res = find_root(f1 , -10, 10)
    EPS=0.001
    if res == None or abs(f1(res)) > EPS  or \
       find_root(lambda x : x**2  , -10, 10) != None:
        print("error in find_root")
        
   
    if multi_merge_v2([[1,2,2,3],[2,3,5],[5]]) != [1, 2, 2, 2, 3, 3, 5, 5] :
        print("error in multi_merge_v2")

    if multi_merge_v3([[1,2,2,3],[2,3,5],[5]]) != [1, 2, 2, 2, 3, 3, 5, 5] :
        print("error in multi_merge_v3")

    if steady_state([-4,-1,0,3,5]) != 3 or \
       steady_state([-4,-1,2,3,5]) not in [2,3] or \
       steady_state([-4,-1,0,4,5]) != None:
        print("error in steady_state")
        
    if cnt_steady_states([-4,-1,0,3,5]) != 1 or \
       cnt_steady_states([-4,-1,2,3,5]) != 2 or \
       cnt_steady_states([-4,-1,0,4,5]) != 0:
        print("error in cnt_steady_states")

    if sort_num_list([10, -2.5, 0, 12.5, -30, 0]) \
       != [-30, -2.5, 0, 0, 10, 12.5]:
        print("error in sort_num_list")

####################
#### HW3 TESTER ####
####################

import sys

ALL_TESTS = {
    21: dict(
        seif_string='Q21',
        function=lambda f, a, b, EPS, check_func: check_func(find_root(f, a, b, EPS)),
        func_name="find_root",
        total_grade=7,
        tests=[
            dict(args=(lambda x: x - 1, -10, 10, 0.001, 
                       lambda result: result is not None and abs(result-1) <= 0.001),
                 expected=True, symbol='T21_1', grade=3),
            dict(args=(lambda x: x**2 - 4, 1, 4, 0.001, 
                       lambda result: result is not None and abs(result-2) <= 0.001),
                 expected=True, symbol='T21_2', grade=2),
            dict(args=(lambda x: x**2 - 4, 3, 4, 0.001, lambda result: result is None),
                 expected=True, symbol='T21_3', grade=2),
            ],
        ),

    32: dict(
        seif_string='Q32',
        function=lambda lst_of_lsts, i: (multi_merge_v2(lst_of_lsts), lst_of_lsts)[i],
        func_name="multi_merge_v2",
        total_grade=20,
        tests=[
            dict(args=([[1,2,2,3],[2,3,5],[5]], 0), 
                 expected=[1, 2, 2, 2, 3, 3, 5, 5],
                 symbol='T32_1', grade=5),
            dict(args=([[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 0), 
                 expected=[0, 1, 1, 2, 2, 3, 4, 4, 7, 7, 8, 11, 14, 15, 16, 17, 18, 19, 20], 
                 symbol='T32_2', grade=4),
            # check the given lists didn't change after calling the function
            dict(args=([[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 1), 
                 expected=[[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 
                 symbol='T32_3', grade=2, help='function multi_merge_v2 changed the lists.'),
            ],
        ),

    33: dict(
        seif_string='Q33',
        function=lambda lst_of_lsts, i: (multi_merge_v3(lst_of_lsts), lst_of_lsts)[i],
        func_name="multi_merge_v3",
        total_grade=4,
        tests=[
            dict(args=([[1,2,2,3],[2,3,5],[5]], 0), 
                 expected=[1, 2, 2, 2, 3, 3, 5, 5],
                 symbol='T33_1', grade=2),
            dict(args=([[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 0), 
                 expected=[0, 1, 1, 2, 2, 3, 4, 4, 7, 7, 8, 11, 14, 15, 16, 17, 18, 19, 20], 
                 symbol='T33_2', grade=1),
            # check the given lists didn't change after calling the function
            dict(args=([[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 1), 
                 expected=[[0,1,2,4,7,8,11,15], [1,2,3,4,7,14,16,17], [18,19,20]], 
                 symbol='T33_3', grade=1, help='function multi_merge_v3 changed the lists.'),
            ],
        ),

    51: dict(
        seif_string='Q51',
        function=steady_state,
        func_name="steady_state",
        total_grade=7,
        tests=[
            dict(args=([0,2,3,4], ), expected=0, symbol='T51_1', grade=2),
            dict(args=([-4,-1,0,3,5], ), expected=3, symbol='T51_2', grade=2),
            dict(args=([-3,-2,-1,0], ), expected=None, symbol='T51_3', grade=2, 
                 help="should return None when there's no steady index"),
            ],
        ),
    54: dict(
        seif_string='Q54',
        function=cnt_steady_states,
        func_name="cnt_steady_states",
        total_grade=4,
        tests=[
            dict(args=([0,2,3,4], ), expected=1, symbol='T54_1', grade=1),
            dict(args=([-1,0,2,3,4], ), expected=3, symbol='T54_2', grade=1),
            dict(args=([-3,-2,-1,0], ), expected=0, symbol='T54_3', grade=1),
            ],
        ),

    61: dict(
        seif_string='Q61',
        function=lambda l, i: (sort_num_list(l), l)[i],
        func_name="sort_num_list",
        total_grade=11,
        tests=[
            dict(args=([10., -2.5, 0., 12.5, -30, 0.], 0), expected=[-30., -2.5, 0., 0., 10., 12.5],
                 symbol='T61_1', grade=5),
            dict(args=([5., 4.5, 4., 3.5, 3., 2.5, 2.], 0), expected=[2., 2.5, 3., 3.5, 4., 4.5, 5.],
                 symbol='T61_2', grade=3),
            dict(args=([5., 4.5, 4., 3.5, 3., 2.5, 2.], 1), expected=[5., 4.5, 4., 3.5, 3., 2.5, 2.],
                 symbol='T61_3', grade=2, help="function sort_num_list changed the list."),
            ],
        ),

}


def run_with_limited_time(func, args=(), kwargs={}, timeout_duration=10):
    """
    This function will spawn a thread and run the given function using the args, kwargs and
    return the given default value if the timeout_duration is exceeded
    """
    import threading

    class InterruptableThread(threading.Thread):
        def __init__(self):
            threading.Thread.__init__(self)
            self.result = None

        def run(self):
            try:
                self.result = func(*args, **kwargs)
            except Exception:
                self.result = (sys.exc_info()[0], sys.exc_info()[1])

    it = InterruptableThread()
    it.daemon = True
    it.start()
    it.join(timeout_duration)
    if it.isAlive():
        return [True, it.result]
    else:
        return [False, it.result]


def t(n=0):
    err_l = []
    err_s = []
    grade = 0

    for seif in ALL_TESTS:
        if n == 0 or seif == n or n == -1:
            function = ALL_TESTS[seif]['function']
            tests = ALL_TESTS[seif]['tests']
            seif_string = ALL_TESTS[seif]['seif_string']
            total_grade = ALL_TESTS[seif]['total_grade']
            func_name = ALL_TESTS[seif]['func_name']
            timeout_symbol = "T" + seif_string[1:] + "_t"
            time_to_run = ALL_TESTS[seif].get('time_to_run', 40)
            symbol = ''

            if n != -1:
                print("Test %s: %s: (%d)" % (func_name, seif_string, total_grade))

            for test in tests:
                exception_symbol = test['symbol'] + "_X"
                reduce = False

                timeout = run_with_limited_time(function, test['args'], {}, time_to_run)
                if timeout[0]:
                    err_l.append("%s: Timeout in %s (running time was longer than %d seconds) - [%s] - (%d)\n" % (seif_string, func_name, time_to_run, timeout_symbol, test['grade']))
                    reduce = True
                    symbol = timeout_symbol
                else:
                    res = timeout[1]
                    if isinstance(res, tuple):
                        e = timeout[1][1]
                        err_l.append("%s: Exception in %s (%s) - [%s] - (%d)\n" % (seif_string, func_name, e, exception_symbol, test['grade']))
                        reduce = True
                        symbol = exception_symbol
                    else:
                        try:
                            if res != test['expected']:
                                err_l.append("%s: Error in %s - [%s] - (%d)" % (seif_string, func_name, test['symbol'], test['grade']))
                                if 'help' in test:
                                    err_l.append(test['help'])
                                err_l.append("Expected: " + str(test['expected']))
                                err_l.append("Got:      " + str(res) + "\n")
                                reduce = True
                                symbol = test['symbol']
                        except Exception:
                            e = sys.exc_info()[1]
                            err_l.append("%s: Exception in %s (%s) - [%s] - (%d)\n" % (seif_string, func_name, e, exception_symbol, test['grade']))
                            reduce = True
                            symbol = exception_symbol

                if reduce:
                    err_s.append(symbol)
                    grade -= test['grade']

    if n != -1:
        print()
        print("\n".join(str(err) for err in err_l))
        print(grade)

    return err_s

t()

        
