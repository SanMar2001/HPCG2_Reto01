import pandas as pd

# Archivos y nombres de hoja
files = {
    "needle_timesV1.doc": "needleV1",
    "needle_timesV2.doc": "needleV2",
    "needle_timesV3.doc": "needleV3",
    "dboard_timesV1.doc": "dboardV1",
    "dboard_timesV2.doc": "dboardV2",
    "dboard_timesV3.doc": "dboardV3",
}

# Parámetros usados en el script de bash
n_values = [1e6, 5e6, 1e7, 5e7, 1e8, 5e8, 1e9]
k_values = [2, 4, 8, 12, 16]
runs = 10  # cantidad de repeticiones

writer = pd.ExcelWriter("times_results.xlsx", engine="openpyxl")

for file, sheet in files.items():
    with open(file, "r") as f:
        lines = [float(x.strip()) for x in f.readlines() if x.strip()]

    if "V1" in file:  
        # Solo n y repeticiones
        rows = []
        idx = 0
        for n in n_values:
            for j in range(runs):
                rows.append([int(n), lines[idx]])
                idx += 1
        df = pd.DataFrame(rows, columns=["N", "Time"])

    else:  
        # n, k y repeticiones
        rows = []
        idx = 0
        for n in n_values:
            for k in k_values:
                for j in range(runs):
                    rows.append([int(n), k, lines[idx]])
                    idx += 1
        df = pd.DataFrame(rows, columns=["N", "K", "Time"])
        df = df.sort_values(by=["N", "K"])  # ordenar por N y K

    df.to_excel(writer, sheet_name=sheet, index=False)

writer.close()
print("Archivo Excel generado: times_results.xlsx")
