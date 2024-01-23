import matplotlib.pyplot as plt

def TComp(k,N,p):
    return k * N / p

def TComm(c,N,p):
    return c * N / p

def Tidle(m,p):
    return m / p

def T(k,c, m,NComp,NComm,p):
    return TComp(k,NComp,p) + TComm(c,NComm,p) + Tidle(m,p)

processors = range(1,1025)
TArr = [T(0.8,5,0.5, 5000**3,6*5000**2, p) for p in processors]
TCompArr = [TComp(0.8, 5000**3, p) for p in processors]
TCommArr = [TComm(5, 6*5000**2, p) for p in processors]
TidleArr = [Tidle(0.5, p) for p in processors]

plt.plot(processors, TArr, label="$T^p$")
plt.plot(processors, TCompArr, label="$T^p_{comp}$")
plt.plot(processors, TCommArr, label="$T^p_{comm}$")
plt.plot(processors, TidleArr, label="$T^p_{idle}$")
plt.legend()
plt.xlabel("Number of processes")
plt.ylabel("Time")
plt.xscale("log", base=2)
plt.yscale("log")
plt.savefig("perf.png")