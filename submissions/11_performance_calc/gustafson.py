import matplotlib.pyplot as plt

def S(alpha, p):
    return 1-alpha + alpha * p

def E(alpha, p):
    return S(alpha, p) / p

print("64 ressources: S(0.8,64)" , S(0.8,64))
print("64 ressources: E(0.8,64)" , E(0.8,64))

processors = range(1,1025)
s= [S(0.8, p) for p in processors]
e= [E(0.8, p) for p in processors]

plt.plot(processors, s, label="S(alpha=0.8)")
plt.legend()
plt.xlabel("Number of processes")
plt.ylabel("Speedup")
plt.xscale("log", base=2)
plt.yscale("log", base=2)
plt.savefig("gustafson_S.png")
plt.clf()
plt.plot(processors, e, label="E(alpha=0.8)")
plt.legend()
plt.xlabel("Number of processes")
plt.ylabel("Efficiency")
plt.xscale("log", base=2)
plt.savefig("gustafson_E.png")