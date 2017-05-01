def max1(L):
    if len(L)==1:
        return L[0]
    return max(L[0], max1(L[1:]))

def max11(L,left,right):
    if left==right:
        return L[left]
    return max(L[left], max11(L,left+1,right))

def max_list11(L):
    return max11(L,0,len(L)-1)
