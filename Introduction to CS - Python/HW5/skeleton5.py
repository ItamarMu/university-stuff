#Skeleton file for HW5 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#You may add other utility functions to this file,
#but you may NOT change the signature of the existing ones.

#Change the name of the file to your ID number (extension .py).

############
# QUESTION 1
############
def clear1 (lst):
    l=len(lst)
    for i in range (l-1,0,-1):
        if lst[i]==0:
            lst.pop()
        else:
            return None

class Polynomial():
    def __init__(self, coeffs_lst):
        self.coeffs = coeffs_lst
        
    def __repr__(self):
        terms = [" + ("+str(self.coeffs[k])+"*x^" + \
                 str(k)+")" \
                 for k in range(len(self.coeffs)) \
                 if self.coeffs[k]!=0]
        terms = "".join(terms)
        if terms == "":
            return "0"
        else:
            return terms[3:] #discard leftmost '+'

    def degree(self):
        return len(self.coeffs)-1

    def evaluate(self, x):
        S=0
        Xa=1
        for i in range(len(self.coeffs)):
            S+=self.coeffs[i]*Xa
            Xa=Xa*x
        return S

    def derivative(self):
        return Polynomial([self.coeffs[i]*i for i in range(1,len(self.coeffs))])

    def __eq__(self, other):
        assert isinstance(other, Polynomial)
        if len(self.coeffs)!=len(self.coeffs):
            return False
        for i in range(len(self.coeffs)):
            if self.coeffs[i]!=other.coeffs[i]:
                return False
        return True

    def __lt__(self, other):
        assert isinstance(other, Polynomial)  
        pass #replace this with your code
            
    def __add__(self, other):
        assert isinstance(other, Polynomial)
        ls=[]
        t=min(len(self.coeffs),len(other.coeffs))
        for i in range(t):
            ls.append(self.coeffs[i]+other.coeffs[i])
        if len(self.coeffs)==len(other.coeffs):
            clear1(ls)
            return Polynomial(ls)
        return Polynomial(ls+self.coeffs[t:]+other.coeffs[t:])

    def __neg__(self):
        return Polynomial([-num for num in self.coeffs])

    def __sub__(self, other):
        assert isinstance(other, Polynomial)
        ls=[]
        t=min(len(self.coeffs),len(other.coeffs))
        for i in range(t):
            ls.append(self.coeffs[i]-other.coeffs[i])
        if len(self.coeffs)==len(other.coeffs):
            clear1(ls)
            return Polynomial(ls)
        return Polynomial(ls+self.coeffs[t:]+[-other.coeffs[i] for i in range(t,len(other.coeffs))])
        
    def __mul__(self, other):
        assert isinstance(other, Polynomial)
        ls=[0 for i in range(len(self.coeffs)+len(other.coeffs))]
        for i in range(len(self.coeffs)):
            for j in range(len(other.coeffs)):
                ls[i+j]+=self.coeffs[i]*other.coeffs[j]
        clear1(ls)
        return Polynomial(ls)

    def find_root(self):
        return NR(lambda x: self.evaluate(x), lambda x: self.derivative().evaluate(x))


## code for Newton Raphson, needed in find_root ##
from random import *

def diff_param(f,h=0.001):
    return (lambda x: (f(x+h)-f(x))/h)


def NR(func, deriv, epsilon=10**(-8), n=100, x0=None):
    if x0 is None:
        x0 = uniform(-100.,100.)
    x=x0; y=func(x)
    for i in range(n):
        if abs(y)<epsilon:
            #print (x,y,"convergence in",i, "iterations")
            return x
        elif abs(deriv(x))<epsilon:
            #print ("zero derivative, x0=",x0," i=",i, " xi=", x)
            return None
        else:
            #print(x,y)
            x = x- func(x)/deriv(x)
            y = func(x)
    #print("no convergence, x0=",x0," i=",i, " xi=", x)
    return None




############
# QUESTION 2
############

### Tree node class - code from lecture, You need to add a field ###

class Tree_node():
    def __init__(self,key,val):
        self.key=key
        self.val=val
        self.left=None
        self.right=None
        self.route=0

    def __repr__(self):
        return "[" + str(self.left) + " " + str(self.key) + " " + \
                    str(self.val) + " " + str(self.right) + "]"

### Binary search tree - code from lecture - DO NOT CHANGE ! ###

def insert(root,key,val):
    if root==None:
        root = Tree_node(key,val)
    elif key==root.key:
        root.val = val     # update the val for this key
    elif key<root.key:
        root.left = insert(root.left,key,val)
    elif key>root.key:
        root.right = insert(root.right,key,val)
    return root

def lookup(root,key):
    if root==None:
        return None
    elif key==root.key:
        return root.val
    elif key < root.key:
        return lookup(root.left,key)
    else:
        return lookup(root.right,key)


### End code from lecture ###

# a
def weight(node):
    if node==None:
        return 0
    if weight(node.left)>weight(node.right):
        if node.left!=None:
            node.left.route=1
        return weight(node.left)+node.val
    else:
        if node.right!=None:
            node.right.route=1
        return weight(node.right)+node.val
    
# b
def HP1(node,lst):
    if node==None:
        return None
    if node.route==1:
        lst.append(node.key)
        return HP1(node.right,lst),HP1(node.left,lst)
    else:
        return None
        
    
def heavy_path(node):
    node.route=1
    weight(node)
    lst=[]
    HP1(node,lst)
    return lst
    

# c
def FC(node,k,last):
    if node==None:
        return last
    if abs(k-last)<abs(k-node.key):
        return last
    if node.right==None and node.left==None:
        return node.key
    if node.right==None and node.left!=None:
        return FC(node.left,k,node.key)
    if node.right!=None and node.left==None:
        return FC(node.right,k,node.key)
    if abs(node.right.key-k)<abs(node.left.key-k):
        return FC(node.right,k,node.key)
    else:
        return FC(node.left,k,node.key)
        
        

def find_closest_key(node, k):
    return FC(node,k,node.key)
   



############
# QUESTION 3
############
def count_words_naive(words):
    count_list=[]
    words_set = set(words) #set of different words (no repetition)
    for word in words_set:
        count_list += [ [word, words.count(word)] ]
    return count_list


#########################################
### SimpleDict CODE ###
#########################################

class SimpleDict:
    def __init__(self, m, hash_func=hash):
        """ initial hash table, m empty entries """      
        self.table = [ [] for i in range(m)]
        self.hash_mod = lambda x: hash_func(x) % m

    def __repr__(self):
        L = [self.table[i] for i in range(len(self.table))]
        return "".join([str(i) + " " + str(L[i]) + "\n" for i in range(len(self.table))])

    def __eq__(self, other):#for testing
        return self.table == other.table

    def items(self):
        return [item for chain in self.table for item in chain]

    def values(self):
        lst=[]
        for i in range(len(self.table)):
            for pair in self.table[i]:
                lst.append(pair[1])
        return lst
    
    def find(self, key):
        i=self.hash_mod(key)
        for pair in self.table[i]:
            if key==pair[0]:
                return pair[1]
        return None

            
    def insert(self, key, value):
        """ insert an item into table
            if key already exists - update value
            key must be hashable """
        i=self.hash_mod(key)
        for pair in self.table[i]:
            if key==pair[0]:
                pair[1]=value
                return None #no reason to continue
        self.table[i].append([key,value])
        return None
            
#########################################
### SimpleDict CODE - end ###
#########################################
from urllib.request import urlopen

def download(url):
    ''' url should be a string containing the full path, incl. http://  '''
    f=urlopen(url)
    btext=f.read()
    text = btext.decode('utf-8')
    #read from the object, storing the page's contents in text.
    f.close()
    return text

def clean(text):
    ''' converts text to lower case, then replaces all characters except
       letters, spaces, newline and carriage return by spaces '''
    letter_set = "abcdefghijklmnopqrstuvwxyz \n\r"
    text = str.lower(text)
    cleaned = ""
    for letter in text:
        if letter in letter_set:
            cleaned += letter
        else:
            cleaned += " "
    return cleaned

def count_words(words):
    D=SimpleDict(200)
    for word in words:
        tmp=D.find(word)
        if tmp==None:
            D.insert(word,1)
        else:
            D.insert(word,tmp+1)
    return D

def sort_by_cnt(count_dict):
    return sorted(count_dict.items(), key=lambda pair: pair[1], reverse=True)


############
# QUESTION 4
############

# a
def next_row(lst):
    nxt=[1]
    for i in range (1,len(lst)):
        nxt.append(lst[i-1]+lst[i])
    nxt.append(1)
    return nxt

# b   
def generate_pascal():
    a=[1]
    while True:
        yield a
        a=next_row(a)

# c
def generate_bernoulli():
    a=[1]
    while True:
        yield a
        a=next_row(a)
        a[-1]+=a[-2]

############
# QUESTION 5
############

##In order to test Q5 uncomment the following line
#from matrix import * #matrix.py needs to be at the same directory
from matrix import *
# a
def upside_down(im):
    n,m = im.dim()
    im2 = Matrix(n,m)
    for i in range(n):
        for j in range(m):
            im2[n-i-1,j]=im[i,j]
    return im2
# b
def makeRow(cor,rightL,leftL,m):
    idxs=[cor] #282->281
    while len(idxs)<m:
        for i in range(len(rightL)):
            if rightL[idxs[-1]]==leftL[i]:
                idxs.append(i)
                break
    return idxs
    
def find_D(idx,upL,downL,m):
    for i in range(len(downL)):
        if downL[idx]==upL[i]:
            return i
    return None

def flat(mat):
    n,m=mat.dim()
    lst=[mat[i,j] for i in range(n) for j in range(m)]
    return lst

def find_ULcorner(upL,downL,rightL,leftL,m):
    r,c=Matrix.load("./puzzle/im1.bitmap").dim()
    Bl=True
    for i in range(len(upL)):
        Bl=True
        for j in range(len(upL)):
            if upL[i]==downL[j] or leftL[i]==rightL[j]:
                Bl=False
                break
        if Bl:
            return i
    return None

def joinM1(lst):
    r,c=lst[0].dim()
    m=len(lst)
    mat=Matrix(r,c*m-m+1)
    for i in range(m-1):
        for j in range(r):
            for k in range(c-1):
                mat[j,k+(c-1)*i]=lst[i][j,k]
    for j in range(r):
        for k in range(c):
            mat[j,k+(c-1)*(m-1)]=lst[-1][j,k]
    return mat

def joinM2(lst):
    r,c=lst[0].dim()
    m=len(lst)
    mat=Matrix(r*m-m+1,c)
    for i in range(m-1):
        for j in range(r-1):
            for k in range(c):
                mat[j+(r-1)*i,k]=lst[i][j,k]
    for j in range(r):
        for k in range(c):
            mat[j+(r-1)*(m-1),k]=lst[-1][j,k]
    return mat


def reconstruct_image(m):
    upL,downL,rightL,leftL=[],[],[],[]
    r,c=Matrix.load("./puzzle/im1.bitmap").dim()
    for i in range(1,m**2+1):
        im1=Matrix.load("./puzzle/im"+str(i)+".bitmap")
        upL.append(flat(im1[0:1,0:c]))
        downL.append(flat(im1[r-1:r,0:c]))
        rightL.append(flat(im1[0:r,c-1:c]))
        leftL.append(flat(im1[0:r,0:1]))
    corner=find_ULcorner(upL,downL,rightL,leftL,m)
    row=makeRow(corner,rightL,leftL,m)
    Ncorner=find_D(row[0],upL,downL,m)
    f1=[row]
    while len(f1)<m:
        row=makeRow(Ncorner,rightL,leftL,m)
        f1.append(row)
        Ncorner=find_D(row[0],upL,downL,m)
    f2=[[] for i in range(m)]
    for i in range(m):
        for j in range(m):
            f2[i].append(Matrix.load("./puzzle/im"+str(f1[i][j]+1)+".bitmap"))
    f3=['']*m
    for i in range(len(f2)):
        f3[i]=joinM1(f2[i])
    f4=joinM2(f3)
    return f4


########
# Tester
########

def test():

    #Question 1
    q = Polynomial([0, 0, 0, 6])
    if str(q) != "(6*x^3)":
        print("error in Polynomial.__init__ or Polynomial.in __repr__")
    if q.degree() != 3:
        print("error in Polynomial.degree")
    p = Polynomial([3, -4, 1])
    if p.evaluate(10) != 63:
        print("error in Polynomial.evaluate")
    dp = p.derivative()
    ddp = p.derivative().derivative()
    if ddp.evaluate(100) != 2:
        print("error in Polynomial.derivative")
    if not p == Polynomial([3, -4, 1]) or p==q:
        print("error in Polynomial.__eq__")
    r = p+q
    if r.evaluate(1) != 6:
        print("error in Polynomial.__add__")
    if not (q == Polynomial([0, 0, 0, 5]) + Polynomial([0, 0, 0, 1])):
        print("error in Polynomial.__add__ or Polynomial.__eq__")
    if (-p).evaluate(-10) != -143:
        print("error in Polynomial.__neg__")
    if (p-q).evaluate(-1) != 14:
        print("error in Polynomial.__sub__")
    if (p*q).evaluate(2) != -48:
        print("error in Polynomial.__mult__")
    if (Polynomial([0])*p).evaluate(200) != 0:
        print("error in Polynomial class")
    root = p.find_root()
    if root-3 > 10**-7 and root-1 > 10**-7:
        print("error in Polynomial.find_root")


    #Question 2
    t = None
    t = insert(t, 1, 85) #the first time we change t from None to a "real" Node
    insert(t, 2.3, -30)
    insert(t, -10, 7.5)
    insert(t, 2, 10.3)
    if weight(t) != 92.5:
        print("error in weight()")
    if heavy_path(t) != [1, -10]:
        print("error in heavy path()")

    if find_closest_key(t, -5) != -10:
        print("error in find_closest_key()")
    if find_closest_key(t, 2.2) != 2.3:
        print("error in find_closest_key()")



    #Question 3  
    h = SimpleDict(200)
    h.insert("ab", 2)
    h.insert("ef", 1)
    h.insert("cd", 3)
    if len(h.items()) != 3:
        print("error in insert()")
    if h.find("ab") != 2:
        print("error in find()")
    if h.find("ef") != 1:
        print("error in find()")
    if h.find("cd") != 3:
        print("error in find()")
    
    d = count_words(["ab", "cd", "cd", "ef", "cd", "ab"]) 
    if d is None:
        print("error in count_words()")
    if len(d.items()) != 3:
        print("error in count_words()")
    if d.find("ab") != 2:
        print("error in count_words()")
    if d.find("ef") != 1:
        print("error in count_words()")
    if d.find("cd") != 3:
        print("error in count_words()")
    
    if sort_by_cnt(d) != [['cd', 3], ['ab', 2], ['ef', 1]] and sort_by_cnt(d) != [('cd', 3), ('ab', 2), ('ef', 1)]:
        print("error in sort_by_cnt()")
    
    # Question 4
    gp = generate_pascal()
    if gp == None:
        print("error in generate_pascal()")
    elif next(gp)!=[1] or next(gp)!=[1,1] or next(gp)!=[1,2,1]:
        print("error in generate_pascal()")
