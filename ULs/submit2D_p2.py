import json, os, shutil
import numpy as np
import time
import subprocess
import re


def read_expected_limit(filename, q="50.0"):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith('Expected '+q+'%:'):  # Search for the target line
                # Split the line and extract the number after 'r <'
                limit = line.split('r <')[-1].strip()
                return float(limit)  # Return the number as a float
    return -1


def aggiorna_periodic_release(path_file):
    with open(path_file, 'r') as f:
        lines = f.readlines()

    nuova_riga = 'periodic_release =  (NumJobStarts < 3000) && ((CurrentTime - EnteredCurrentStatus) > 30)\n'
    with open(path_file, 'w') as f:
        for line in lines:
            if line.strip().startswith('periodic_release ='):
                f.write(nuova_riga)
            else:
                f.write(line)

def get_condor_jobs(user="mbuonsante"):
    try:
        # esegue condor_q per l'utente
        result = subprocess.run(
            ["condor_q"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        
        # cerca la riga con "Total for <user>: ..."
        for line in result.stdout.splitlines():
            if line.startswith(f"Total for {user}:"):
                # parsing con regex
                pattern = (r"Total for .*: (\d+) jobs; (\d+) completed, (\d+) removed, "
                           r"(\d+) idle, (\d+) running, (\d+) held, (\d+) suspended")
                match = re.search(pattern, line)
                if match:
                    return {
                        "jobs": int(match.group(1)),
                        "completed": int(match.group(2)),
                        "removed": int(match.group(3)),
                        "idle": int(match.group(4)),
                        "running": int(match.group(5)),
                        "held": int(match.group(6)),
                        "suspended": int(match.group(7)),
                    }
        return None
    except subprocess.CalledProcessError as e:
        print("Errore nell'esecuzione di condor_q:", e.stderr)
        return None

#BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
#BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["1e-10", "3e-10", "7e-10", "1e-9"]

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


for index in range(19,len(BsBRs_mix)):
    if not os.path.exists(f"Out_{index}/Datacards/toys"):
        os.makedirs(f"Out_{index}/Datacards/toys")
    else:
        shutil.rmtree(f"Out_{index}/Datacards/toys")
        os.makedirs(f"Out_{index}/Datacards/toys")

    for q in [" 2.5", "16.0", "50.0", "84.0", "97.5"]:
        ULAs = read_expected_limit(f"Out_{index}/output_combine.txt", q)
        if ULAs == -1:
            print(f"No asintotic limits in Out_{index} ... skipping submission")
            continue
        Nround=4
        if (BdBRs_mix[index] in {"4e-13", "9e-13"}):
            N_max = 60
            n_toys = 1000
        else:
            N_max = 25
            n_toys = 3000
        ULAs = round(ULAs, Nround)
        min_value = round(ULAs*0.6, Nround)
        max_value = round(ULAs*2.2, Nround)
        step = round((max_value-min_value)/N_max, Nround*4)
        if step == 0:
            step = 1/10**Nround
        
        command = f"combineTool.py -M HybridNew -d ../combined.root --generateNuisances=1 --generateExternalMeasurements=0 --fitNuisances=1 --testStat LHC --clsAcc=0 --singlePoint {min_value}:{max_value}:{step} -T {n_toys} -s -1 --saveToys --saveHybridResult --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerStrategy 0 --job-mode condor --task-name toys_{index}_{q.replace(' ', '_')}"

        while ((get_condor_jobs() is None) or (get_condor_jobs()["jobs"] > 5000 - (Nround + 1))):
            if get_condor_jobs() is None:
                print("Problem condor")
            time.sleep(5)
            print("Aspettando condor jobs")
        os.system(f"cd Out_{index}/Datacards/toys; "+command+f"; cd {current_directory}")
            

        min_value = round(ULAs*0.8, Nround)
        max_value = round(ULAs*1.5, Nround)
        step = round((max_value-min_value)/N_max, Nround*4)
        if step == 0:
            continue
        
        command = f"combineTool.py -M HybridNew -d ../combined.root --generateNuisances=1 --generateExternalMeasurements=0 --fitNuisances=1 --testStat LHC --clsAcc=0 --singlePoint {min_value}:{max_value}:{step} -T {n_toys} -s -1 --saveToys --saveHybridResult --X-rtd MINIMIZER_freezeDisassociatedParams --cminDefaultMinimizerStrategy 0 --job-mode condor --task-name toys_{index}_{q.replace(' ', '_')}_p2"

        while ((get_condor_jobs() is None) or (get_condor_jobs()["jobs"] > 5000 - (Nround + 1))):
            if get_condor_jobs() is None:
                print("Problem condor")
            time.sleep(5)
            print("Aspettando condor jobs")
            
        os.system(f"cd Out_{index}/Datacards/toys; "+command+f"; cd {current_directory}")
        
        #aggiorna_periodic_release(f"Out_{index}/Datacards/toys/condor_toys_{index}.sub")
        
        #os.system(f"cd Out_{index}/Datacards/toys; condor_submit -name tmp condor_toys_{index}.sub -batch-name toys_{index}; cd {current_directory}")



