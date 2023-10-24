import matplotlib.pyplot as plt
import matplotlib
import csv
import os
matplotlib.use('agg')

arraySize = []
dataAccessSpeed = []
script_dir = os.path.dirname(__file__)
rel_path = "dataAccessSpeed.csv"
abs_file_path = os.path.join(script_dir, rel_path)

with open(abs_file_path, 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    header = next(plots)
    for row in plots:
        arraySize.append(int(row[0]))
        dataAccessSpeed.append(float(row[1]))

plt.plot(arraySize, dataAccessSpeed, label='Data Access Speed')
plt.xlabel('Array Size')
plt.ylabel('Data Access Speed [GB/s]')
plt.title('Data Access Speed for different Array Sizes')
plt.legend()
plt.savefig(os.path.join(script_dir, 'dataAccessSpeed.png'))
