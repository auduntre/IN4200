import matplotlib.pyplot as plt
import numpy as np

time_pe_iter = [0.522235, 0.325801, 0.237650, 0.203265, 
                0.181847, 0.173853, 0.166139, 0.161014,
                0.171461, 0.169670, 0.167551, 0.166142,
                0.164439, 0.166116, 0.177190, 0.214110]

time_top_n = [0.040201, 0.021957, 0.015883, 0.012900,
              0.011126, 0.010167, 0.008694, 0.008646,
              0.008775, 0.013923, 0.009423, 0.013236,
              0.013750, 0.013750, 0.013378, 0.013131]

threads = [i for i in range(1, 17)]


plt.figure()
plt.plot(threads, time_pe_iter, 'r-o')

plt.xlabel("Threads")
plt.ylabel("Time [sec]")

plt.figure()
plt.plot(threads, time_top_n, 'b-o')

plt.xlabel("Threads")
plt.ylabel("Time [sec]")

plt.show()

