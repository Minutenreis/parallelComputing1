import sys
from matplotlib import pyplot as plt

if len(sys.argv) != 2:
    print("Usage: python performance.py <input_file>")
    sys.exit(1)

input_file = sys.argv[1]

# Read the input file
with open(input_file, "r") as f:
    lines = f.readlines()
    header = lines[0].strip().split(",")
    data = [[float(i) for i in line.strip().split(",")] for line in lines[1:]]
    columns = list(zip(*data))

for i in range(1,len(header)):
    plt.plot([row[0] for row in data], [row[i] for row in data], label=header[i])
plt.xlabel(header[0])
plt.legend()
# plt.yscale("log")
plt.xscale("log")
plt.savefig("performance.png")