k = 3
n = 100
def ckB (num):
    if str(num).find(str(k)) >= 0 and num%k==0:
        return 'boom-boom!'
    if str(num).find(str(k)) >= 0:
        return 'boom!'
    if num%k==0:
        return 'boom!'
    return num

for i in range(1,n+1):
    print (ckB(i))
