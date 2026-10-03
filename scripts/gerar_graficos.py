import csv
import matplotlib.pyplot as plt
import os

arquivo = "resultados/resultados.csv"
pasta = "resultados/graficos"

os.makedirs(pasta, exist_ok=True)

dados = []

with open(arquivo, newline="") as f:
    leitor = csv.DictReader(f)

    for linha in leitor:
        dados.append({
            "V": int(linha["V"]),
            "memoria": int(linha["memoria_bytes"]),
            "leitura": float(linha["ms_leitura"]),
            "tarjan": float(linha["ms_tarjan"]),
            "armadilha": float(linha["ms_armadilha"])
        })

dados.sort(key=lambda x: x["V"])
vertices = [d["V"] for d in dados]

# Tempo de Tarjan
plt.figure()
plt.plot(vertices, [d["tarjan"] for d in dados], marker="o")
plt.xlabel("Número de vértices (V)")
plt.ylabel("Tempo (ms)")
plt.title("Tempo de execução de Tarjan")
plt.grid(True)
plt.savefig(f"{pasta}/tempo_tarjan.png", dpi=300, bbox_inches="tight")
plt.close()

# Tempo da análise de armadilhas
plt.figure()
plt.plot(vertices, [d["armadilha"] for d in dados], marker="o")
plt.xlabel("Número de vértices (V)")
plt.ylabel("Tempo (ms)")
plt.title("Tempo da análise de armadilhas de trânsito")
plt.grid(True)
plt.savefig(f"{pasta}/tempo_armadilhas.png", dpi=300, bbox_inches="tight")
plt.close()

# Tempo de leitura
plt.figure()
plt.plot(vertices, [d["leitura"] for d in dados], marker="o")
plt.xlabel("Número de vértices (V)")
plt.ylabel("Tempo (ms)")
plt.title("Tempo de leitura e validação")
plt.grid(True)
plt.savefig(f"{pasta}/tempo_leitura.png", dpi=300, bbox_inches="tight")
plt.close()

# Memória
plt.figure()
plt.plot(vertices, [d["memoria"] for d in dados], marker="o")
plt.xlabel("Número de vértices (V)")
plt.ylabel("Memória (bytes)")
plt.title("Consumo de memória da lista de adjacência")
plt.grid(True)
plt.savefig(f"{pasta}/memoria_lista.png", dpi=300, bbox_inches="tight")
plt.close()

print("Gráficos gerados com sucesso.")
