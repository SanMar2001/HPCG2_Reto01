import pandas as pd

# Archivos y sus hojas
files = {
    "needle_timesV1.doc": "needleV1",
    "needle_timesV2.doc": "needleV2",
    "needle_timesV3.doc": "needleV3",
    "dboard_timesV1.doc": "dboardV1",
    "dboard_timesV2.doc": "dboardV2",
    "dboard_timesV3.doc": "dboardV3",
}

# Valores usados en bash
n_values = [int(1e6), int(5e6), int(1e7), int(5e7), int(1e8), int(5e8), int(1e9)]
k_values = [2, 4, 8, 12, 16]
runs = 10  # repeticiones

writer = pd.ExcelWriter("times_results1.xlsx", engine="openpyxl")

for file, sheet in files.items():
    with open(file, "r") as f:
        lines = [float(line.strip()) for line in f if line.strip()]

    rows = []
    idx = 0

    if "V1" in file:
        # Esperamos runs * len(n_values) líneas
        expected = runs * len(n_values)
        assert len(lines) == expected, f"{file} tiene {len(lines)} valores, esperado {expected}"

        # Orden: primero N, luego repetición j
        for j in range(runs):
            for n in n_values:
                rows.append([n, j + 1, lines[idx]])
                idx += 1

        df = pd.DataFrame(rows, columns=["N", "Run", "Time"])

    else:
        # Esperamos runs * len(n_values) * len(k_values) líneas
        expected = runs * len(n_values) * len(k_values)
        assert len(lines) == expected, f"{file} tiene {len(lines)} valores, esperado {expected}"

        # Orden: primero K, luego N, luego repetición j
        for j in range(runs):
            for n in n_values:
                for k in k_values:
                    rows.append([n, k, j + 1, lines[idx]])
                    idx += 1

        df = pd.DataFrame(rows, columns=["N", "K", "Run", "Time"])

    # Guardar en su hoja
    df.to_excel(writer, sheet_name=sheet, index=False)

writer.close()
print("Archivo Excel generado: times_results.xlsx")
