import os
import numpy as np
import matplotlib.pyplot as plt
import mplhep as hep
from scipy.interpolate import interp1d

# --- Stile ---
hep.style.use("CMS")
plt.rc("font", family="sans-serif", weight="normal", size=20)
plt.rc("xtick", labelsize="x-small")
plt.rc("ytick", labelsize="x-small")

# --- Funzione per leggere limiti ---
def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Limit:'):
                parts = line.split('r <')[1].split('+/-')
                limit = float(parts[0].strip())
                error = float(parts[1].split('@')[0].strip())
                return limit, error
    return -1, -1

# --- Setup BRs e directory ---
BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]
outdir = "2D_out_ANv10/"

# --- Lettura valori ---
z_mix = {q: [] for q in ["-2", "-1", "0", "1", "2"]}
BRs_mix = {"Bs": [], "Bd": []}

index = 0
for a in BsBRs:
    for b in BdBRs:
        BRs_mix["Bs"].append(float(a))
        BRs_mix["Bd"].append(float(b))
        for q in ["-2", "-1", "0", "1", "2"]:
            z_val, z_err = read_expected_limit(outdir + f"Out_{index}/result_HN_{q}.txt")
            if z_val == -1 or z_err == 0:
                print(f"Directory Out_{index} failed")
                z_val = -999
            z_mix[q].append(z_val)
        index += 1

# --- Setup BRs e directory ---
BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["1e-10", "3e-10", "7e-10", "1e-9"]
outdir = "2D_out_ANv10/"

index = 0
for a in BsBRs:
    for b in BdBRs:
        BRs_mix["Bs"].append(float(a))
        BRs_mix["Bd"].append(float(b))
        for q in ["-2", "-1", "0", "1", "2"]:
            z_val, z_err = read_expected_limit(outdir + f"Out_{index}/result_HN_{q}.txt")
            if z_val == -1 or z_err == 0:
                print(f"Directory Out_{index} failed")
                z_val = -999
            z_mix[q].append(z_val)
        index += 1

if any(-999 in z_list for z_list in z_mix.values()):
    print("-999 error")
    exit()

x = np.array(BRs_mix["Bd"])
y = np.array(BRs_mix["Bs"])
z0 = np.array(z_mix["0"])
z_plus1 = np.array(z_mix["1"])
z_minus1 = np.array(z_mix["-1"])

# --- Funzione per estrarre contorni ---
def get_contour_vertices(x, y, z, level=1):
    fig_tmp, ax_tmp = plt.subplots()
    cnt = ax_tmp.tricontour(x, y, z, levels=[level])
    plt.close(fig_tmp)
    if not cnt.collections:
        return np.empty((0,2))
    paths = [path.vertices for collection in cnt.collections for path in collection.get_paths()]
    main_path = max(paths, key=lambda p: p.shape[0])
    main_path = main_path[np.argsort(main_path[:,0])]
    return main_path

c_plus = get_contour_vertices(x, y, z_plus1)
c_minus = get_contour_vertices(x, y, z_minus1)

# Interpolazione contorno inferiore
interp_minus = interp1d(c_minus[:,0], c_minus[:,1], bounds_error=False, fill_value='extrapolate')
y_minus_interp = interp_minus(c_plus[:,0])

# --- Plot principale ---
fig, ax = plt.subplots(figsize=(10, 8))
hep.cms.text(text="Preliminary", ax=ax)
hep.cms.lumitext(text=r"170.7 fb$^{-1}$ (13.6 TeV)", ax=ax)

# Banda ±1 sigma
ax.fill_between(
    c_plus[:,0],
    y_minus_interp,
    c_plus[:,1],
    color="gold",
    alpha=0.5,
    label=r"$\pm1\sigma$ band"
)

# Contorni
c0 = ax.tricontour(x, y, z0, levels=[1], colors='red', linewidths=2)
ax.clabel(c0, fmt={1: '  r = 1  '}, inline=True, fontsize=14)

# Assi log
ax.set_xscale('log')
ax.set_yscale('log')
ax.set_xlabel(r"Branching Ratio of $B^0 \rightarrow 4 \mu$")
ax.set_ylabel(r"Branching Ratio of $B^0_s \rightarrow 4 \mu$")
ax.set_xlim(x.min(), x.max())
ax.set_ylim(y.min(), y.max())

ax.legend(frameon=False, fontsize=14, loc="upper left")
ax.grid(which='both', linestyle='--', linewidth=0.5, alpha=0.5)

plt.tight_layout()
plt.savefig("heatmap_brazilian_v10.pdf")
plt.show()