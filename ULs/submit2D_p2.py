import json, os, shutil
import numpy as np
import time

def read_expected_limit(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Expected 50.0%:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[-1].strip()
                return float(limit)  # Return the number as a float


BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

min_value = 0.9
max_value = 1.8
step = 0.1
current_directory = os.getcwd()

BsBRs_mix = []
BdBRs_mix = []
for a in BsBRs:
    for b in BdBRs:
        BsBRs_mix.append(a)
        BdBRs_mix.append(b)


for index in range(len(BsBRs_mix)):
    ULAs = read_expected_limit(f"Out_{index}/output_combine.txt")
    Nround=2
    if ULAs<1:
        Nround=2
    ULAs = round(ULAs, Nround)
    ULAs_up = round(ULAs*12/10, Nround)
    step = round((ULAs_up-ULAs)/3, Nround)
    min_value = round(ULAs-3*step, Nround)
    max_value = round(ULAs_up+6*step, Nround)
    
    if not os.path.exists(f"Out_{index}/Datacards/toys"):
        os.makedirs(f"Out_{index}/Datacards/toys")
    else:
        shutil.rmtree(f"Out_{index}/Datacards/toys")
        os.makedirs(f"Out_{index}/Datacards/toys")

    command = f"combineTool.py -M HybridNew -d ../combined.root --generateNuisances=1 --generateExternalMeasurements=0 --fitNuisances=1 --testStat LHC --clsAcc=0 --singlePoint {min_value}:{max_value}:{step} -T 5000 -s -1 --saveToys --saveHybridResult --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerStrategy 0 --job-mode condor --task-name toys_{index}"

    os.system(f"cd Out_{index}/Datacards/toys; "+command+f"; cd {current_directory}")
