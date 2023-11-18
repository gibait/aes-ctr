import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots(2, 1, figsize=(10, 10))

with open('single-multi-comparison.txt') as f:
    lines = f.readlines()
    x = [line.split()[1] for line in lines]
    x2 = [line.split()[2] for line in lines]
    y = [line.split()[0] for line in lines]

x = np.asarray(x, dtype=np.int64)
x2 = np.asarray(x2, dtype=np.int64)
y = np.asarray(y, dtype=np.int64)

# Convert bytes to kilobytes
y = y / 1024

ax[0].plot(x, y, label='single_thread', marker='o')
ax[0].plot(x2, y, label='multi_thread', marker='o')
ax[0].set_xlabel('time (ms)')
ax[0].set_ylabel('file size (kilo bytes)')
ax[0].set_title('single_thread vs multi_thread')
ax[0].legend()


with open('threads-comparison.txt') as f:
    lines = f.readlines()
    y = [line.split()[1] for line in lines]
    x = [line.split()[0] for line in lines]

x = np.asarray(x, dtype=np.int64)
y = np.asarray(y, dtype=np.int64)


ax[1].plot(x, y, marker='o')
ax[1].set_ylabel('time (ms)')
ax[1].set_xlabel('number of threads')
ax[1].set_title('threads comparison')
ax[1].legend()

plt.show()