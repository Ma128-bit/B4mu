import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from scipy.interpolate import LinearNDInterpolator, RBFInterpolator
import math, os
import mplhep as hep
hep.style.use("CMS")
font = {"family": "sans-serif", "weight": "normal", "size": 23}

plt.rc("font", **font)
plt.rc("xtick", labelsize="x-small")
plt.rc("ytick", labelsize="x-small")

def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Expected 50.0%:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[-1].strip()
                return float(limit)  # Return the number as a float

def read_as_dataframe(filename):
    # Read the file as a DataFrame, assuming space as the delimiter
    df = pd.read_csv(filename, sep='\s+', header=None)
    return df

def log_significance(S, B):
    significance = 0
    if ((S + B) * math.log(1 + S / B) - S)>0:
        significance = math.sqrt(2 * ((S + B) * math.log(1 + S / B) - S))
    return significance

dir = "submission_out"
filename = dir+'/scan_info.txt'  
df = read_as_dataframe(filename)
df['expected_limitA'] = df[0].apply(lambda x: read_expected_limit(dir+f'/Out_{x}/output_combineA.txt'))
df['expected_limitB'] = df[0].apply(lambda x: read_expected_limit(dir+f'/Out_{x}/output_combineB.txt'))
df['expected_limitC'] = df[0].apply(lambda x: read_expected_limit(dir+f'/Out_{x}/output_combineC.txt'))

#print(df)
#print(df.isna().sum())

after_10={
    'A': [0.9765, 0.9562],
    'B': [0.9863, 0.9133],
    'C': [0.9847, 0.8465]
}
for c in ["A", "B", "C"]:
    max_row = df.loc[df['expected_limit'+c].idxmin()] 
    print(max_row[1], max_row[2], np.nanmin(df['expected_limit'+c]))
    
    
    x = df[1]
    y = df[2]
    z = df['expected_limit'+c]
    x_bins = 60  
    y_bins = 40 
    
    #hist, x_edges, y_edges = np.histogram2d(x, y, bins=[x_bins, y_bins], weights=z, range=[(0.85, 0.99), (0.50, 0.95)])
    hist, x_edges, y_edges = np.histogram2d(x, y, bins=[x_bins, y_bins], weights=z, range=[(0.95, 0.99), (0.84, 0.99)])

    # Calcola i centri delle celle per i grafici
    x_centers = (x_edges[:-1] + x_edges[1:]) / 2
    y_centers = (y_edges[:-1] + y_edges[1:]) / 2

    # Crea una griglia per il grafico
    X, Y = np.meshgrid(x_centers, y_centers)

    # Imposta la dimensione della figura
    plt.figure(figsize=(10, 8))  # Modifica le dimensioni come desiderato

    # Crea il grafico con pcolormesh
    plt.pcolormesh(X, Y, hist.T, shading='auto', cmap='viridis', norm=LogNorm())
    hep.cms.text(text="Preliminary")
    hep.cms.lumitext(text=r"170.7 fb$^{-1}$ (13.6 TeV)")
    plt.colorbar(label='r')
    plt.scatter(max_row[1], max_row[2], color='red', label=f'Best wp', zorder=3, s=30)
    plt.scatter(after_10[c][0], after_10[c][1], color='orange', label=f'Selected wp', zorder=3, s=25)

    #if c=="B":
        #plt.xlim(0.95, 0.982)
    plt.legend(loc='upper left', frameon=True)

    # Etichette degli assi
    plt.xlabel('Cut n. 1')
    plt.ylabel('Cut n. 2')

    # Aggiungi una legenda
    plt.savefig('WP_cat'+c+'.png', dpi=300)
    plt.clf()
    """
    x = x.to_numpy()
    y = y.to_numpy()
    z = z.to_numpy()
    interpolator = LinearNDInterpolator(list(zip(x, y)), z, fill_value=np.nan)
    x_new = np.linspace(0.95, 0.99, 1000)  # Maggiore risoluzione su x
    y_new = np.linspace(0.84, 0.99, 1000)  # Maggiore risoluzione su y
    x_new_grid, y_new_grid = np.meshgrid(x_new, y_new)

    z_interpolated = interpolator(x_new_grid, y_new_grid)
    z_min = np.nanmin(z_interpolated)

    df_interpolated = pd.DataFrame({
        'x': x_new_grid.ravel(),
        'y': y_new_grid.ravel(),
        'z': z_interpolated.ravel()
    })
    # Filtra il DataFrame per mantenere solo le righe con valori di z minori di z_min - 10%
    z_threshold = z_min * 1.1
    df_filtered = df_interpolated[df_interpolated['z'] < z_threshold]
    random_row = df_filtered.sample(n=1)
    print(random_row)
    """ 
    """
    max_index = np.unravel_index(np.nanargmin(z_interpolated), z_interpolated.shape)

    x_max = x_new[max_index[1]]  # max_index[1] corrisponde all'indice lungo x
    y_max = y_new[max_index[0]]  # max_index[0] corrisponde all'indice lungo y
    print("New: ", x_max, y_max)
    z_max = z_interpolated[max_index]

    z_threshold = 1.1 * z_max
    region_mask = z_interpolated <= z_threshold

    # Trova i punti che soddisfano la condizione
    x_region, y_region = x_new_grid[region_mask], y_new_grid[region_mask]

    # Grafico dell'interpolazione
    plt.figure(figsize=(8, 6))
    plt.pcolormesh(x_new_grid, y_new_grid, z_interpolated, shading='auto', cmap='viridis')
    plt.colorbar(label='Interpolated Value')  # Barra dei colori
    plt.xlabel('Cut n. 1')
    plt.ylabel('Cut n. 2')

    # Evidenzia il massimo sul grafico
    plt.scatter(x_max, y_max, color='red', label=f'Min: ({x_max:.2f}, {y_max:.2f})', zorder=5)

    # Aggiungi la regione dove z è almeno il 90% del massimo
    #plt.fill(x_region, y_region, color='red', alpha=1., label='10% > min')

    # Aggiungi la legenda
    plt.legend()

    #plt.figure(figsize=(8, 6))
    #plt.pcolormesh(x_new_grid, y_new_grid, z_interpolated, shading='auto', cmap='viridis')
    #plt.colorbar(label='Interpolated Value')  # Barra dei colori
    #plt.xlabel('Cut n. 1')
    #plt.ylabel('Cut n. 2')
    #plt.title('Interpolazione di dati sparsi')
    plt.savefig('grafico'+c+'.png', dpi=300)
    plt.clf()
    """