import matplotlib.pyplot as plt
import numpy as  np
from math import log2

f = open('output_storage.txt', 'r')
lines = f.readlines()
f.close()

lines=lines[1:]
n=[]
space_IT=[]
space_ST=[]
t_space=[]
for line in lines:
    x=line.split(',')
    N=float(x[0])
    n.append(N)
    space_IT.append(float(x[1]))
    #space_ST.append(float(x[2])/(N*log2(N)))

    space_ST.append(float(x[2]))
    t_space.append(N*log2(N))


plt.figure()
coeff=np.polyfit(n, space_IT, 1)
plt.plot(n, space_IT,'+',label="Experimental points")
plt.plot(n, [coeff[0]*k + coeff[1] for k in n],'-',label=f"Linear regression: {coeff[0]:.3f}*n + {coeff[1]:.3f}")
plt.xlabel("Number of intervals")
plt.xlabel("Number of intervals (n)")
plt.ylabel("Average space used (Number of elements stored)")
plt.title("Interval Tree: Average Space Complexity vs. Number of Intervals")
plt.legend()

plt.figure()
#coeffST=np.polyfit(n, space_ST, 0)
plt.plot(n, space_ST, '+', color='green', label="Experimental points")
plt.plot(n, t_space, '-', color='red', label="Theorical result")
#plt.axhline(y=coeffST[0],linestyle='-',label=f"Linear regression: {coeffST[0]:.3f}")

plt.xlabel("Number of intervals (n)")
plt.ylabel("Average space used (Number of elements stored)")
plt.title("Segment Tree: Average Space Complexity vs. Number of Intervals")
plt.legend()

plt.show()
