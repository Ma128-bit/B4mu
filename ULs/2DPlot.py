import json, htcondor, os, shutil
import numpy as np
import time

#BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
#BdBRs = ["4e-13", "9e-13", "4e-12", "1e-11", "1.5e-11", "2e-11", "5e-11", "8e-11"]

BsBRs = ["9e-11", "1e-10", "3e-10", "4e-10", "5e-10", "6e-10", "7e-10", "1e-9"]
BdBRs = ["1e-10", "3e-10", "7e-10", "1e-9"]

BsBRs_mix = []
BdBRs_mix = []
for a in BsBRs:
    for b in BdBRs:
        BsBRs_mix.append(a)
        BdBRs_mix.append(b)


for index in range(len(BsBRs_mix)):    
    if not os.path.exists(f"Out_{index}/Datacards/toys"):
           os.makedirs(f"Out_{index}/Datacards/toys")
    else:
        shutil.rmtree(f"Out_{index}/Datacards/toys")
        os.makedirs(f"Out_{index}/Datacards/toys")

inputs = [(f"{i}", f"{BsBRs_mix[i]}", f"{BdBRs_mix[i]}") for i in range(len(BsBRs_mix))]

current_directory = os.getcwd()

itemdata = [{"argument": " ".join(input)} for input in inputs]

job_config = {
    "executable": current_directory+"/submit2D_p1.sh", 
    "arguments": "$(argument)",
    "output": current_directory+f"/Out_$(ProcId)/job_output.txt", 
    "error": current_directory+f"/Out_$(ProcId)/job_error.txt",
    "log": current_directory+f"/Out_$(ProcId)/job_log.txt", 
    "request_cpus": 1, 
    "request_memory": "2GB",  
    "request_disk": "4GB",  
}

job = htcondor.Submit(job_config)
schedd = htcondor.Schedd()
submit_result = schedd.submit(job, itemdata = iter(itemdata))

    
