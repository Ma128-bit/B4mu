import json, os, shutil
import numpy as np
import time
from datetime import datetime

import re
def group_files_by_point(folder_path, prefix):
    files = [f for f in os.listdir(folder_path) if f.startswith(prefix) and f.endswith('.root')]
    groups = {}
    pattern = re.compile(r'POINT\.(-?\d+(?:\.\d+)?)')  # trova POINT.<numero>
    for file in files:
        match = pattern.search(file)
        if match:
            point_value = match.group(1)  # es. '0.85' o '-328788899'
            group_key = f"POINT.{point_value}"
            groups.setdefault(group_key, []).append(file)
        else:
            print(f"Unexpected file format: {file}")
    return groups

def keep_most_recent_in_group(folder_path, grouped_files):
    for point, files in grouped_files.items():
        if len(files) <= 1:
            continue  # niente da cancellare
        files_with_time = [(f, os.path.getmtime(os.path.join(folder_path, f))) for f in files]
        most_recent = max(files_with_time, key=lambda x: x[1])[0]
        for f, _ in files_with_time:
            if f != most_recent:
                os.remove(os.path.join(folder_path, f))
                #print(f"Deleted: {f}")
        print(f"Kept most recent for {point}: {most_recent}")

## OLD -->
def list_files_with_prefix(folder_path, prefix):
    files = [f for f in os.listdir(folder_path) if f.startswith(prefix) and os.path.isfile(os.path.join(folder_path, f))]
    trimmed_files = set()
    for file in files:
        try:
            trimmed_name = file.split("HybridNew")[0] + "H"
            trimmed_files.add(trimmed_name)
        except IndexError:
            print(f"Unexpected file format: {file}")
    
    return sorted(trimmed_files)


def delete_old_files(folder_path, prefix):
    files = [f for f in os.listdir(folder_path) if f.startswith(prefix) and os.path.isfile(os.path.join(folder_path, f))]
    
    if not files:
        print("No files found with the specified prefix.")
        return
    
    files_with_dates = [(f, os.path.getmtime(os.path.join(folder_path, f))) for f in files]
    most_recent_file = max(files_with_dates, key=lambda x: x[1])[0]
    
    #print(f"Most recent file: {most_recent_file}")
    
    for file, _ in files_with_dates:
        if file != most_recent_file:
            file_path = os.path.join(folder_path, file)
            os.remove(file_path)
            #print(f"Deleted: {file_path}")
    
    #print("Cleaning completed!")
## <-- OLD 

def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Expected 50.0%:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[-1].strip()
                return float(limit)  # Return the number as a float


#BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
#BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11", "1e-10", "3e-10", "7e-10", "1e-9"]

current_directory = os.getcwd()

BsBRs_mix = []
BdBRs_mix = []
for a in BsBRs:
    for b in BdBRs:
        BsBRs_mix.append(a)
        BdBRs_mix.append(b)

outdir = "2D_out_ANv10/"
for index in range(len(BsBRs_mix)):
    #files = list_files_with_prefix(outdir+f"Out_{index}/Datacards/toys", "higgsCombine.Test.POINT")
    #for f in files:
    #    delete_old_files(outdir+f"Out_{index}/Datacards/toys", f)
    grouped = group_files_by_point(outdir+f"Out_{index}/Datacards/toys", "higgsCombine.Test.POINT")
    keep_most_recent_in_group(outdir+f"Out_{index}/Datacards/toys", grouped)  

    command = "cd "+outdir+f"Out_{index}/Datacards/toys; hadd merged.root *.root"

    os.system(command)

    id_ = -2
    for q in ["0.025", "0.16", "0.5", "0.84", "0.975"]:
    #for q in ["0.5"]:
        command2 = "cd "+outdir+f"Out_{index}/Datacards; combine -M HybridNew datacard_combined.txt --generateNuisances=1 --generateExternalMeasurements=0 --fitNuisances=1 --testStat LHC --readHybridResults --grid=toys/merged.root --rule CLs --expectedFromGrid "+q+f" --cminDefaultMinimizerStrategy 0 --plot=limit_scan_{id_}.png > ../result_HN_{id_}.txt; cd ../.."
        command2 += "/.." if outdir!="" else ""
        os.system(command2)
        id_ = id_ +1
        command2=""

