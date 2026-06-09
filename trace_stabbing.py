import matplotlib.pyplot as plt
import numpy as  np
from math import log2

f = open('output_stabbing_query.txt', 'r')
lines = f.readlines()
f.close()

lines=lines[1:]
n=[]
op_ITc=[]
op_STc=[]
op_ITmiss=[]
op_STmiss=[]
op_IT=[]
op_ST=[]

for line in lines:
    x=line.split(',')
    N=float(x[0])
    n.append(N)
    op_ITc.append(float(x[1]))
    op_STc.append(float(x[2]))
    op_ITmiss.append(float(x[3]))
    op_STmiss.append(float(x[4]))
    op_IT.append(float(x[5]))
    op_ST.append(float(x[6]))

### --- CENTER INTERVALS ---
plt.figure()
# --- INTERVAL TREE (Points + Regression) ---
coeffIT=np.polyfit(n, op_ITc, 1)
plt.plot(n, op_ITc,'+',label="Interval Tree (Experimental)")
plt.plot(n, [coeffIT[0]*k + coeffIT[1] for k in n],'-',label=f"Interval Tree Fit: {coeffIT[0]:.3f}*n + {coeffIT[1]:.3f}")

# --- SEGMENT TREE (Points + Regression) ---
coeffST=np.polyfit(n, op_STc, 1)
plt.plot(n, op_STc,'+',label="Segment Tree (Experimental)")
plt.plot(n, [coeffST[0]*k + coeffST[1] for k in n],'-',label=f"Segment Tree Fit: {coeffST[0]:.3f}*n + {coeffST[1]:.3f}")
plt.xlabel("Number of intervals (n)")
plt.ylabel("Average number of operations")
plt.title("Query Time Complexity: Interval Tree vs. Segment Tree (Center Intervals)")
plt.legend()



### --- Miss queries ---
plt.figure()
# --- INTERVAL TREE ---
plt.plot(n, op_ITmiss,'+',label="Interval Tree (Experimental)")

# --- SEGMENT TREE ---
coeffST=np.polyfit(n, op_STc, 1)
plt.plot(n, op_STmiss,'+',label="Segment Tree (Experimental)")

# --- Theory ---
plt.plot(n, [log2(k) for k in n],'-',label=f"Theory O(log(n))")
plt.xlabel("Number of intervals (n)")
plt.ylabel("Average number of operations")
plt.title("Query Time Complexity: Interval Tree vs. Segment Tree (Outside Intervals)")
plt.legend()


### --- Normal random queries ---
plt.figure()
# --- INTERVAL TREE ---
plt.plot(n, op_IT,'+',label="Interval Tree (Experimental)")

# --- SEGMENT TREE ---
coeffST=np.polyfit(n, op_STc, 1)
plt.plot(n, op_ST,'+',label="Segment Tree (Experimental)")

# --- Theory ---
plt.plot(n, [log2(k) for k in n],'-',label=f"Theory O(log(n))")
plt.xlabel("Number of intervals (n)")
plt.ylabel("Average number of operations")
plt.title("Query Time Complexity: Interval Tree vs. Segment Tree (Normal random queries)")
plt.legend()

plt.show()
