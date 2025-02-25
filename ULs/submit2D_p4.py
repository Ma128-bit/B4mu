import os
import numpy as np
import seaborn as sns
from datetime import datetime
import matplotlib.pyplot as plt
import mplhep as hep

hep.style.use("CMS")
font = {"family": "serif", "weight": "normal", "size": 20}

plt.rc("font", **font)
plt.rc("xtick", labelsize="x-small")
plt.rc("ytick", labelsize="x-small")


def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Limit:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[1].split(' +/-')[0].strip()
                return float(limit)  # Return the number as a float


BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

current_directory = os.getcwd()

BsBRs_mix = []
BdBRs_mix = []

z_values=[]
index = 0
for a in BsBRs:
    z = []
    for b in BdBRs:
        z.append(read_expected_limit(f"Out_{index}/result_HN.txt"))
        index +=1
    z_values.insert(0, z)

z_values = np.array(z_values)

print(z_values.shape)

fig, ax = plt.subplots(figsize=(10, 8))
hep.cms.text(text="Preliminary")
hep.cms.lumitext(text=r"171.5 fb$^{-1}$ (13.6 TeV)")
BsBRs.reverse()
sns.heatmap(
    z_values,
    annot=True,
    fmt=".2f",
    xticklabels=BdBRs,
    yticklabels=BsBRs,
    cmap="viridis",
    cbar_kws={"label": r"signal strength multiplier $r$"},
    ax=ax,
)

ax.set_xlabel(r"Branching Ratio of $B^0 \rightarrow 4 \mu$")
ax.set_ylabel(r"Branching Ratio of $B^0_s \rightarrow 4 \mu$")

# ax.set_title("Heatmap of BR Bd vs BR Bs with exp. limit (HybridNew)")
plt.savefig("heatmap_cms_style.png")
