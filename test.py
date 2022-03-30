import numpy as np
import subprocess

for i in range(1, 6):
    arr1 = np.random.randint(5, size=(40 * i, 40 * i))
    arr2 = np.random.randint(5, size=(40 * i, 40 * i))
    output = [str(i) + "\n" for i in np.concatenate((arr1.flatten(), arr2.flatten())).tolist()]
    with open("test.txt", mode="w") as f:
        f.writelines(output)

    diagonal = subprocess.run(["./strassen", "0", f"{40 * i}", "test.txt"], capture_output=True, text=True).stdout
    diagonal = np.array([int(i) for i in diagonal.split()])
    print(np.all(diagonal == np.diagonal(np.dot(arr1, arr2))))