import matplotlib.pyplot as plt

def S(alpha, p):
    return 1 / ((1 - alpha) + alpha / p)

alphaA = 0.6
alphaB = 0.8
alphaC = 0.5

print("alphaA", alphaA)
print("alphaB", alphaB)
print("alphaC", alphaC)

for p in [2,4,8,16,64]:
    print(f"S(alphaA, {p})", S(alphaA, p))
    print(f"S(alphaB, {p})", S(alphaB, p))
    print(f"S(alphaC, {p})", S(alphaC, p))

processes = range(1,1025)
a0 = [S(0.25, p) for p in processes]
a1 = [S(0.5, p) for p in processes]
a2 = [S(0.75, p) for p in processes]
a3 = [S(0.9, p) for p in processes]

plt.plot(processes, a0, label="alpha=0.25")
plt.plot(processes, a1, label="alpha=0.5")
plt.plot(processes, a2, label="alpha=0.75")
plt.plot(processes, a3, label="alpha=0.9")
plt.legend()
plt.xlabel("Number of processes")
plt.ylabel("Speedup")
plt.xscale("log", base=2)
plt.savefig("amdahl.png")