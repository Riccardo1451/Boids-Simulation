import pandas as pd
import matplotlib.pyplot as plt

# Leggi i dati dai file CSV
data_sequential = pd.read_csv("sequential_update_times.csv")
data_parallel = pd.read_csv("parallel_update_times.csv")

# Aggiungi un suffisso per differenziare le colonne
data_sequential["Sequential"] = data_sequential["Time"]
data_parallel["Parallel"] = data_parallel["Time"]

avg_sequential_time = data_sequential["Sequential"].mean()
avg_parallel_time = data_parallel["Parallel"].mean()

global_speedup = avg_sequential_time / avg_parallel_time

# Combina i dati in un unico DataFrame
data = pd.concat([data_sequential["Sequential"], data_parallel["Parallel"]], axis=1)

# Calcolo degli outlier usando l'IQR
def remove_outliers(series):
    Q1 = series.quantile(0.25)  # Primo quartile
    Q3 = series.quantile(0.75)  # Terzo quartile
    IQR = Q3 - Q1  # Interquartile Range
    lower_bound = Q1 - 1.5 * IQR  # Limite inferiore
    upper_bound = Q3 + 1.5 * IQR  # Limite superiore
    return series[(series >= lower_bound) & (series <= upper_bound)]

# Rimuovi outlier per entrambe le colonne
data["Sequential"] = remove_outliers(data["Sequential"])
data["Parallel"] = remove_outliers(data["Parallel"])

# Calcola lo speedup (Sequenziale / Parallelo)
data['Speedup'] = data['Sequential'] / data['Parallel']

# Applica una media mobile per smussare il grafico
window_size = 20
data['Smoothed_Speedup'] = data['Speedup'].rolling(window=window_size, min_periods=1).mean()

# Grafico dello speedup smussato
plt.plot(data.index, data['Smoothed_Speedup'], label="Smoothed Speedup", color='b')
plt.xlabel('Iteration')
plt.axhline(global_speedup, color='r', linestyle='--', label=f"Global Speedup ({global_speedup:.2f})")
plt.ylabel('Speedup')
plt.title('Speedup of Parallel vs Sequential Execution (Outliers Removed)')
plt.legend()
plt.grid(True)
plt.show()
