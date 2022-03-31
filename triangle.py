import numpy as np
from math import comb
import matplotlib.pyplot as plt
import subprocess

probs = [0.01 * i for i in range(1,6)]
output = []
for p in probs:
    output.append(float(subprocess.run(["./strassen", "2", str(p), "5"], capture_output=True, text=True).stdout))

plt.plot(np.array(probs), np.array(output), marker="o", label="Experimental Average")
plt.plot(np.array(probs), comb(1024,3) * (np.array(probs) ** 3), marker="o", label="Expectation")
plt.legend(loc="upper left")
plt.xlabel("Probability")
plt.ylabel("Number of Triangles in 1024 Vertex Graph")
plt.savefig("triangle_graph.png")
