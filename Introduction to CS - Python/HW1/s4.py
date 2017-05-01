import time
t0 = time.clock()
num = 2**100
cnt=0
for i in range(num):
    cnt = cnt+1
t1 = time.clock()
print("Running time: ", t1-t0, "sec")
