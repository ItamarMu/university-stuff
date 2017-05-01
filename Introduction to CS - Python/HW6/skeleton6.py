#Skeleton file for HW6 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#You may add other utility functions to this file,
#but you may NOT change the signature of the existing ones.

#Change the name of the file to your ID number (extension .py).

from matrix import *


############
# QUESTION 1
############

def fingerprint(mat):
    assert isinstance(mat,Matrix)
    k,makesure = mat.dim()
    assert k==makesure

    return sum(mat[i,j] for i in range(k) for j in range(k))

def move_right(mat, i, j, k, fp):
    return fp-sum([mat[i+z1,j] for z1 in range(k)]) + \
                sum([mat[i+z2,j+k] for z2 in range(k)])

def move_down(mat, i, j, k, fp):
    return fp-sum([mat[i,j+z1] for z1 in range(k)]) + \
                sum([mat[i+k,j+z2] for z2 in range(k)])

    
def has_repeating_subfigure(mat, k):
    n,m=mat.dim() #n for rows, m for columns
    fp=fingerprint(mat[:k,:k])
    fp1=fp
    stor={fp:True}
    for i in range(n-k):
        fp=move_down(mat,i,0,k,fp)
        if stor.get(fp)==None:
            stor[fp]=True
        else:
            return True
    for j in range(m-k):
        fp1=move_right(mat,0,j,k,fp1)
        if stor.get(fp1)==None:
            stor[fp1]=True
        else:
            return True
        fp=fp1
        for i in range(n-k):
            fp=move_down(mat,i,j+1,k,fp)
            if stor.get(fp)==None:
                stor[fp]=True
            else:
                return True
    return False


########
# Tester
########

def test():
    #Question 1
    im = Matrix.load("./sample.bitmap")
    k = 2
    if fingerprint(im[:k,:k]) != 384 or \
       fingerprint(im[1:k+1,1:k+1]) != 256 or \
       fingerprint(im[0:k,1:k+1]) != 511:
        print("error in fingerprint()")
    if move_right(im, 0, 0, k, 384) != 511:
        print("error in move_right()")
    if move_down(im, 0, 1, k, 511) != 256:
        print("error in move_down()")
    if has_repeating_subfigure(im, k) != True or\
       has_repeating_subfigure(im, k=3) != False:
        print("error in repeating_subfigure()")




