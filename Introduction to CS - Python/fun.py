def H1(ls,k):
    if k==len(ls):
        return [ls]
    if k==0:
        return [[]]
    a=H1(ls[1:],k-1)
    b=H1(ls[1:],k)
    return [[ls[0]]+i for i in a]+b
