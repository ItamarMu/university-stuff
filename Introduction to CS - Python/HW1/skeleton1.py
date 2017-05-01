#Skeleton file for HW1 - Spring 2015 - extended intro to CS

#Add your implementation to this file

#Change the name of the file to your ID number (extension .py).


#Question 3
in_file = "our_input.txt"
out_file = "output.txt"
txt = open(in_file, 'r')
newText = open(out_file, 'w')
for line in txt:
    lineSpl=line.split()
    newText.write(str(len(lineSpl))+'\n\n')
txt.close()
newText.close()

#**************************************************************
#Question 5
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

#**************************************************************
#Question 6
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
