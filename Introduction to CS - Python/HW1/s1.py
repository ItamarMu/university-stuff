import time
num = eval(input("Please enter a positive integer: "))
m = num
cnt = 0
t0 = time.clock()
while m>0:
    if m%10 == 0:
        cnt+=1
    m = m//10
t1 = time.clock()
print(num, "has", cnt, "zeros")
print("Running time: ", t1-t0, "sec")
