import pandas as pd
import matplotlib.pyplot as plt

# File CSV contenenti i tempi medi per ogni configurazione
thread_configs = [1, 2, 4, 8, 16, 32, 64]
mean_times = []
mean_times_ms = []

for threads in thread_configs:
    data = pd.read_csv(f"ExecutionTime/parallel_update_{threads}_threads.csv")
    mean_times.append(data["Time"].mean())
    mean_time_ms = data["Time"].mean() * 1000  # Converti in millisecondi
    mean_times_ms.append(mean_time_ms)

# Calcola lo speedup rispetto al tempo con 1 thread
speedup = [mean_times[0] / t for t in mean_times]

# Crea il grafico
plt.figure(figsize=(10, 6))
plt.plot(thread_configs, speedup, label="Measured speedup")
plt.plot(thread_configs, thread_configs, linestyle='--', color='r', label="Ideal speedup (linear)")
#plt.plot(thread_configs, mean_times_ms, label="Execution Time", color='Blue')

plt.xlabel("Number of Threads")
#plt.ylabel("Execution Time (ms)")
#plt.title("Execution time")
plt.ylabel("SpeedUp")
plt.title("Speedups")
plt.ylim(0.5,4)
plt.xticks(thread_configs)
plt.legend()
plt.grid(False)
plt.show()
