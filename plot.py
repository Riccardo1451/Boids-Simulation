import pandas as pd
import matplotlib.pyplot as plt

data_sequential = pd.read_csv("sequential_update_times.csv")
data_parallel = pd.read_csv("parallel_update_times.csv")

average_sequential_time = data_sequential["Time"].mean()
average_parallel_time = data_parallel["Time"].mean()

# Aggiungi un suffisso per differenziare le due colonne
data_sequential["Sequential"] = data_sequential["Time"]
data_parallel["Parallel"] = data_parallel["Time"]

# Calcola lo speedup (Sequenziale / Parallelo)
data = pd.concat([data_sequential["Sequential"], data_parallel["Parallel"]], axis=1)
data['Speedup'] = data['Sequential'] / data['Parallel']

# Applica una media mobile per "smussare" il grafico, finestra di 5 iterazioni
window_size = 5
data['Smoothed_Speedup'] = data['Speedup'].rolling(window=window_size, min_periods=1).mean()


plt.plot(data.index, data['Smoothed_Speedup'], label="Speedup", color='b')
plt.xlabel('Iteration')
plt.ylabel('Speedup')
plt.title('Speedup of Parallel vs Sequential Execution')
plt.legend()
plt.grid(True)
plt.show()
