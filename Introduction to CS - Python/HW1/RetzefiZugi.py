input_str = input("Please enter a positive integer: ")
a=str(eval(input_str))
def streaks (num):
    highSt=''
    save=''
    for digit in num:
        if int(digit)%2 != 0:
           save+=digit
           if len(save)>len(highSt):
            highSt=save
        else:
            save=''
    highIX=num.find(highSt)
    if highSt=='':
        return 0,-1,None
    return len(highSt),highIX,highSt

length,start,seq=streaks(a)
print("The maximal length is", length)
print("Sequence starts at", start)
print("Sequence is", seq)
