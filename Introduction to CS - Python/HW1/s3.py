import time
num = eval(input("Please enter a positive integer: "))
t0 = time.clock()
cnt = str.count(str(num), "0")
t1 = time.clock()
print(num, "has", cnt, "zeros")
print("Running time: ", t1-t0, "sec")
