import numpy as np
import matplotlib.pyplot as plt
import subprocess

numtrials = 5
start = 55
stop = 250
step = 5
for d in range(200, 800, 80):
    times = []
    for k in range(numtrials):
        temp = []
        arr1 = np.random.randint(2, size=(d, d))
        arr2 = np.random.randint(2, size=(d, d))
        output = [str(i) + "\n" for i in np.concatenate((arr1.flatten(), arr2.flatten())).tolist()]
        with open("test.txt", mode="w") as f:
            f.writelines(output)
        common_args = ["./strassen", "1", str(d), "test.txt"]
        for n in range(start, stop, step):
            temp.append(float(subprocess.run([*common_args, str(n)], capture_output=True, text=True).stdout))
        times.append(temp)
    times = np.mean(np.array(times), axis=0)
    plt.plot(np.arange(start, stop, step), times, label=f"d={d}")
    print(d)
plt.ylabel("Average Time (scaled to be between 0 and 1)")
plt.xlabel("Crossover Point")
plt.legend(loc="upper left")
plt.savefig("crossover_graph.png")
