import os, math
import numpy as np
import seaborn as sns
from datetime import datetime
import matplotlib.pyplot as plt
import mplhep as hep
from scipy.interpolate import griddata

hep.style.use("CMS")
font = {"family": "sans-serif", "weight": "normal", "size": 20}

plt.rc("font", **font)
plt.rc("xtick", labelsize="x-small")
plt.rc("ytick", labelsize="x-small")


def read_expected_limit_old(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Limit:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[1].split(' +/-')[0].strip()
                return float(limit)  # Return the number as a float
        return -1

def read_AsymptoticLimits_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Expected 50.0%:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[-1].strip()
                return float(limit)  # Return the number as a float

def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Limit:'):  
                parts = line.split('r <')[1].split('+/-')
                limit = float(parts[0].strip())
                error = float(parts[1].split('@')[0].strip())
                return limit, error  
        return -1, -1
    
BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

current_directory = os.getcwd()
outdir = "2D_out_v10/"

BsBRs_mix = []
BdBRs_mix = []
z_mix = []

z_values=[]
index = 0
for a in BsBRs:
    z = []
    for b in BdBRs:
        z_val_asy = read_AsymptoticLimits_limit(outdir+f"Out_{index}/output_combine.txt")
        z_val_temp, z_err_temp = read_expected_limit(outdir+f"Out_{index}/result_HN_2.txt")
        if z_val_temp == -1:
            print(f"Directory Out_{index} failed")
        if z_err_temp == 0:
            print(f"Directory Out_{index} failed")
            z_val_temp = -999
        
        z.append(z_val_temp)
        #z.append(round(100*(z_val_temp-z_val_asy)/z_val_asy,2))
        #if (round(100*(z_val_temp-z_val_asy)/z_val_asy,2)>10):
        #    print("Strange index: ", index)
        BsBRs_mix.append(a)
        BdBRs_mix.append(b)
        z_mix.append(z_val_temp)
        index +=1
    z_values.insert(0, z)
if any(-999 in s for s in z_values):
    print("-999 error")
    exit()
    
z_values = np.array(z_values)

print(z_values.shape)

fig, ax = plt.subplots(figsize=(10, 8))
hep.cms.text(text="Preliminary")
hep.cms.lumitext(text=r"170.7 fb$^{-1}$ (13.6 TeV)")
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
plt.savefig("heatmap_10.pdf")

# Convert BsBRs and BdBRs to numpy arrays for interpolation
x = np.array([float(k) for k in BdBRs_mix])
y = np.array([float(k) for k in BsBRs_mix])
z= np.array([float(k) for k in z_mix])

# Create meshgrid for interpolation
xi, yi = np.meshgrid(
    np.logspace(np.log10(min(x)), np.log10(max(x)), 200),
    np.logspace(np.log10(min(y)), np.log10(max(y)), 200),
)

# Interpolate z values
zi = griddata((x, y), z, (xi, yi), method="linear")
xo = np.array([float(k) for k in xi.ravel()])
yo = np.array([float(k) for k in yi.ravel()])
zo = np.array([float(k) for k in zi.ravel()])

fig, ax = plt.subplots(figsize=(10, 8))
hep.cms.text(text="Preliminary")
hep.cms.lumitext(text=r"170.7 fb$^{-1}$ (13.6 TeV)")
heatmap = ax.pcolormesh(xi, yi, zi, shading="auto", cmap="viridis")

# Add colorbar
cbar = plt.colorbar(heatmap, ax=ax)
cbar.set_label(r"signal strength multiplier $r$")

# Plotting contour
cnt = ax.tricontour(x, y, z, levels=[1], colors="red")
ax.clabel(cnt, fmt={1: "  r = 1  "}, inline=True, fontsize=12)

#cnt2 = ax.tricontour(xo, yo, zo, levels=[1], colors="red")
# Print X and Y coordinates of points where z = 1
# Get the points where z = 1 and store them in a list
points = []
for collection in cnt.collections:
    for path in collection.get_paths():
        for point in path.vertices:
            print(f"R: {round(point[0]/point[1],4)}, X: {point[0]} Y: {point[1]}")
            points.append(point)

# Sort points by X coordinate
#points.sort(key=lambda p: p[0])

# Select 16 equispaced points along X
#selected_points = points[::max(1, len(points) // 14)]
"""
# Print the selected points
for point in selected_points:
    print(f"R: {round(point[0]/point[1],4)}, X: {point[0]} Y: {point[1]}")
point = points[len(points)-10]
print(f"R: {round(point[0]/point[1],4)}, X: {point[0]} Y: {point[1]}")

point = points[len(points)-1]
print(f"R: {round(point[0]/point[1],4)}, X: {point[0]} Y: {point[1]}")
"""
# Set x and y axis limits to match the heatmap
ax.set_xlim([min(x), max(x)])
ax.set_ylim([min(y), max(y)])
ax.set_yscale("log")
ax.set_xscale("log")

ax.set_xlabel(r"Branching Ratio of $B^0 \rightarrow 4 \mu$")
ax.set_ylabel(r"Branching Ratio of $B^0_s \rightarrow 4 \mu$")

plt.savefig("heatmap_interpolated_v10.pdf")
