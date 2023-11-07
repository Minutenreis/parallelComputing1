import matplotlib.pyplot as plt
import matplotlib
import csv
import os

matplotlib.use("agg")

arraySize = []
dataAccessSpeed = []
script_dir = os.path.dirname(__file__)
rel_path = "memory_bandwidth.csv"
abs_file_path = os.path.join(script_dir, rel_path)

with open(abs_file_path, "r") as csvfile:
    plots = csv.reader(csvfile, delimiter=",")
    header = next(plots)
    for row in plots:
        arraySize.append(int(row[0]))
        dataAccessSpeed.append(float(row[1]))

plt.plot(arraySize, dataAccessSpeed, label="Data Access Speed")
plt.xlabel("Array Size [bytes]")
plt.xscale("log", base=2)
plt.ylabel("Data Access Speed [GB/s]")
plt.axvline(
    192 * 1024, color="r", linestyle="--", label="L1d Cache Size"
)  # todo: adjust to real cachesizes
plt.axvline(
    2 * 1024 * 1024, color="r", linestyle="--", label="L2 Cache Size"
)  # todo: adjust to real cachesizes
plt.axvline(
    6 * 1024 * 1024, color="r", linestyle="--", label="L3 Cache Size"
)  # todo: adjust to real cachesizes
plt.title("Data Access Speed for different Array Sizes")
plt.legend()
plt.savefig(os.path.join(script_dir, "memory_bandwidth_plot.png"))
