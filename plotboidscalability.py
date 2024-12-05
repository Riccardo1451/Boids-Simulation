import pandas as pd
import matplotlib.pyplot as plt

# Carica i dati dal file CSV generato
data = pd.read_csv("ExecutionTime/boids_scalability.csv")

# Converte i tempi in millisecondi
data["Execution_Time"] = data["Execution_Time"] * 1000  # Da secondi a millisecondi

# Estrae i valori unici di boids e thread
boids_values = sorted(data["Num_Boids"].unique())
threads_values = sorted(data["Num_Threads"].unique())

# Creazione del grafico Execution Time
plt.figure(figsize=(10, 6))
for threads in threads_values:
    thread_data = data[data["Num_Threads"] == threads]
    plt.plot(thread_data["Num_Boids"], thread_data["Execution_Time"],label=f"{threads} Threads")

plt.xlabel("Number of Boids")
plt.ylabel("Execution Time (ms)")
plt.title("Execution Time vs Number of Boids")
plt.legend()
plt.grid(False)
#plt.tight_layout()
plt.savefig("execution_time_vs_boids.png")  # Salva il grafico
plt.show()

# Creazione del grafico Speedup
plt.figure(figsize=(10, 6))
for threads in threads_values:
    if threads == 1:
        continue  # Lo speedup si calcola rispetto ai thread singoli
    speedup = []
    for boids in boids_values:
        time_1_thread = data[(data["Num_Boids"] == boids) & (data["Num_Threads"] == 1)]["Execution_Time"].values[0]
        time_current_thread = data[(data["Num_Boids"] == boids) & (data["Num_Threads"] == threads)]["Execution_Time"].values[0]
        speedup.append(time_1_thread / time_current_thread)
    plt.plot(boids_values, speedup, label=f"{threads} Threads")

plt.xlabel("Number of Boids")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of Boids")
plt.legend()
plt.grid(False)
plt.tight_layout()
plt.savefig("speedup_vs_boids.png")  # Salva il grafico
plt.show()
