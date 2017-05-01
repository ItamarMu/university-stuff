LINELEN = 80
PLACE = 3
def parts (st): #return array of 80 letters long bits
    zx = []
    for i in range (len(st)//LINELEN):
        zx.append(st[i*LINELEN:(i+1)*LINELEN])
    return zx


def chWord (st):
    if st[PLACE].isupper():
        return False
    for i in range(len(st)):
        if (i==PLACE):
            continue
        if st[i].islower():
            return False
    return True

def chWord2 (st):
    if (st[PLACE+1].isupper() or st[0].isupper() or st[-1].isupper()):
        return False
    for i in range(len(st)):
        if (i==PLACE+1 or i==0 or i==len(st)-1):
            continue
        if st[i].islower():
            return False
    return True
    
def check2 (zx,i,z):
    horiz = zx[i][z-4:z+5]
    if (chWord2(horiz)):
        print (horiz)
    return

def check (zx,i,z):# if yes, print the 7 letters line
    '''for t in range (i-3,i+4):
        if (t==i) and (zx[t][z].isupper()):
            print("1" + zx[t][z])
            return #if X is upper quit, else continue
        if (zx[t][z].islower()):
            print("2" + zx[t][z])
            return #vertical check
    for t in range (z-3,z+4):
        if (t!=z) and (zx[i][t].islower()):
            return
    print(zx[i][z-3:z+4])'''
    horiz = zx[i][z-3:z+4]
    ver=""
    for t in range (i-3,i+4):
        ver+=zx[t][z]
    if (chWord(ver) and chWord(horiz)):
        print("horiz: "+horiz)
        print("ver: "+ver)
    return

def x (st): #string length is 7
    """A = True #a=open("C:\\Users\\Dell\\Desktop\\asd.txt",'r').read() howToRead
    if st[3].islower(): #check if lowercase, then check three
        for i in range (7): #uppercase from down,up,right,left (2D array)
            if (i!=3) and (st[i].islower()):
                A=False
    else:
        A=False
    if A:
        print(st)"""
    zx = parts(st)
    for i in range(4,len(zx)-4):# i for line
        for z in range(4,len(zx[i])-4): # z for TOORfor i in range(3,len(zx)-3)
                check2(zx,i,z)#check2 it is..
    return
