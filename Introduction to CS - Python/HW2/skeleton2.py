#Skeleton file for HW2 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#You may add other utility functions to this file,
#but you may NOT change the signature of the existing ones.

#Change the name of the file to your ID number (extension .py).

############
# QUESTION 1
############

# 1c
def reverse_sublist(lst, start, end):
    tmp=0
    TL=end-start
    for i in range (start,start+TL//2):
        tmp=lst[i]
        lst[i]=lst[end-1-(i-start)]
        lst[end-1-(i-start)]=tmp
    return None

# 1d
def rotate1(lst):
    lastVar=lst[-1]
    for i in range(len(lst)-1,0,-1):
        lst[i]=lst[i-1]
    lst[0]=lastVar
    return None

# 1e
def rotatek_v1(lst, k):
    k=k%len(lst)
    for i in range (k):
        rotate1(lst)
    return None

# 1f
def rotatek_v2(lst, k):
    k=k%len(lst)
    reverse_sublist(lst,0,len(lst))
    reverse_sublist(lst,0,k)
    reverse_sublist(lst,k,len(lst))
    return None

############
# QUESTION 2b
############

def power_new(a,b):
    """ computes a**b using iterated squaring """
    result = 1
    b_bin = bin(b)[2:]
    reverse_b_bin = b_bin[: :-1]
    for bit in reverse_b_bin: 
        if bit=='1':
            result=result*a
        a=a*a
    return result


############
# QUESTION 3b
############

def add_hex(A,B):
    Ar=A[::-1]
    Br=B[::-1]
    hexa_map='0123456789abcdef'
    hexa_sum=''
    Carry=0
    lenBig=max(len(Ar),len(Br))
    for i in range (lenBig):
        hexa_sum+=hexa_map[(hexa_map.index(Ar[i:i+1])+hexa_map.index(Br[i:i+1])+Carry)%16]
        if hexa_map.index(Ar[i:i+1])+hexa_map.index(Br[i:i+1])+Carry>15:
            Carry=1 
        else:
            Carry=0
    if Carry==1:
        hexa_sum+='1'
    return hexa_sum[::-1]



############
# QUESTION 4b
############

def sum_divisors(n):
    if n==0 or n==1:
        return 0
    rootN=int(n**0.5)
    divisors=[1]
    if rootN**2==n:
        divisors.append(rootN)
        rootN-=1
    for i in range(2,rootN+1):
        if n%i==0:
            divisors.append(i)
            divisors.append(n//i)
    return sum(divisors)

def is_finite(n):
    allSums=[]
    curSum=n
    while allSums.count(curSum)==0:
        allSums.append(curSum)
        curSum=sum_divisors(curSum)
    if curSum==0:
        return True
    else:
        return False

def cnt_finite(limit):
    count=0
    for i in range (1,limit+1):
        if is_finite(i):
            count+=1
    return count





############
# QUESTION 5
############

def altsum_digits(n, d):
    sN=str(n)
    firstOne=sN[:d]
    maxAlSum=0
    cnt=0
    for dig in firstOne:
        maxAlSum+=int(dig)*((-1)**cnt)
        cnt+=1
    lastAlSum=maxAlSum
    cnt=0
    newAlSum=0
    for dig in sN[d:]:
        newAlSum=int(dig)*((-1)**(d+1))-lastAlSum+int(sN[cnt])
        if newAlSum>maxAlSum:
            maxAlSum=newAlSum
        lastAlSum=newAlSum
        cnt+=1
    return maxAlSum
    
    
########
# Tester
########

def test():

    lst = [1,2,3,4,5]
    reverse_sublist (lst,0,4)
    if lst != [4, 3, 2, 1, 5]:
        print("error in reverse_sublist()")        
    lst = ["a","b"]
    reverse_sublist (lst,0,1)
    if lst != ["a","b"]:
        print("error in reverse_sublist()")        

    lst = [1,2,3,4,5]
    rotate1(lst)
    if lst != [5,1,2,3,4]:
        print("error in rotate1()")        

    lst = [1,2,3,4,5]
    rotatek_v1(lst,2)
    if lst != [4,5,1,2,3]:
        print("error in rotatek_v1()")        
    
    lst = [1,2,3,4,5]
    rotatek_v2(lst,2)
    if lst != [4,5,1,2,3]:
        print("error in rotatek_v2()")        

    if power_new(2,3) != 8:
        print("error in power_new()")

    if add_hex("a5","17")!="bc":
        print("error in add_hex()")
    
    if sum_divisors(6)!=6 or \
       sum_divisors(4)!=3:        
        print("error in sum_divisors()")

    if is_finite(6) or \
       not is_finite(4):
        print("error in is_finite()")

    if cnt_finite(6) != 5:
        print("error in cnt_finite()")
        
    if altsum_digits(5**36,12)!=18:
        print("error in altsum_digits()")        

