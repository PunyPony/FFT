#!/usr/bin/python3
import subprocess as sp
import matplotlib.pyplot as plt
import numpy as np
import random as rd
import matplotlib
import matplotlib.pyplot as plt
import time

def alea(sup,inf):
    a = rd.randint(sup, inf)
    b = rd.randint(sup, inf)
    return a,b

def checkfft(n):
    tps1 = time.clock()
    error = 0
    for k in range(0,100) :
        rand = alea(10**n, 10**(n+1)-1)
        respy = str(rand[0]*rand[1])
        resfft = str(sp.check_output(("./fft", str(rand[0]), str(rand[1]))))
        resfft = resfft[2:len(resfft) - 1]
        if resfft != respy :
            error +=1
            print(respy)
            print(resfft)
    tps2 = time.clock()
    print(tps2 - tps1, "s")
    return error

def pymult(n):
    tps1 = time.clock()
    for k in range(1,2) :
        rand = alea(10**n, 10**(n+1)-1)
        respy = rand[0]*rand[1]
    tps2 = time.clock()
    #print(tps2 - tps1, "s")
    return tps2 - tps1

def fft(n):
    tps1 = time.clock()
    for k in range(1,2) :
        rand = alea(10**n, 10**(n+1)-1)
        resfft = sp.check_output(("./fft", str(rand[0]), str(rand[1])))
        #resfft = resfft[2:len(resfft) - 1]
    tps2 = time.clock()
    #print(tps2 - tps1, "s")
    return tps2 - tps1

t = []
resfft=[]
respymult=[]
for k in range(1,100000,1000) :
    t.append(k)
    resfft.append(0.1*fft(k))
    respymult.append(pymult(k))
print("end of calc")
plt.plot(t, resfft, color="red", linewidth=2, linestyle="-", label='resfft')
plt.plot(t, respymult, color="blue", linewidth=2, linestyle="-", label='respymult')
plt.ylabel('Computation time (s)')
plt.xlabel("Number of digits")
plt.show()

print("error :", checkfft(10))
print(pymult(100000))
print(fft(100000))


