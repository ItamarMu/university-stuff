import time
num = eval(input("Please enter a positive integer: "))
cnt = 0
snum = str(num)
t0 = time.clock()
for digit in snum:
    if digit == "0":
        cnt+=1
t1 = time.clock()
print(num, "has", cnt, "zeros")
print("Running time: ", t1-t0, "sec")
