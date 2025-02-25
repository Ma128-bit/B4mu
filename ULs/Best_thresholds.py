import json, htcondor, os, shutil
import numpy as np
import time

file = open("scan_info.txt", 'w')

input_file = "configs/config_scan.json"
with open(input_file, 'r') as f:
    data = json.load(f)

#range1 = np.linspace(0.85, 0.99, 40)
#range2 = np.linspace(0.50, 0.95, 50) 

#range1 = np.linspace(0.91, 0.99, 40)
#range2 = np.linspace(0.80, 0.99, 50)

range1 = np.linspace(0.95, 0.99, 60)
range2 = np.linspace(0.84, 0.99, 40)

bdt_cv_1 = []
bdt_cv_2 = []
for a in range1:
    for b in range2:
        if a>b:
            bdt_cv_1.append(a)
            bdt_cv_2.append(b)


for index in range(len(bdt_cv_1)):
    file.write(f"{index} {bdt_cv_1[index]} {bdt_cv_2[index]}\n")
    bdt_cv_values = [bdt_cv_1[index], bdt_cv_2[index],bdt_cv_1[index], bdt_cv_2[index],bdt_cv_1[index], bdt_cv_2[index]]

    #bdt_cv_values = [0.953, 0.864, 0.822, 0.474, 0.89, 0.621]

    new_data = {
        "categories": ["A1", "A2", "B1", "B2", "C1", "C2"],
        "A1": f"category==0 && bdt_cv > {bdt_cv_values[0]}",
        "A2": f"category==0 && bdt_cv > {bdt_cv_values[1]} && bdt_cv < {bdt_cv_values[0]}",
        "B1": f"category==1 && bdt_cv > {bdt_cv_values[2]}",
        "B2": f"category==1 && bdt_cv > {bdt_cv_values[3]} && bdt_cv < {bdt_cv_values[2]}",
        "C1": f"category==2 && bdt_cv > {bdt_cv_values[4]}",
        "C2": f"category==2 && bdt_cv > {bdt_cv_values[5]} && bdt_cv < {bdt_cv_values[4]}"
    }

    if not os.path.exists(f"Out_{index}"):
        os.makedirs(f"Out_{index}")
    else:
        shutil.rmtree(f"Out_{index}")
        os.makedirs(f"Out_{index}")

    data.update(new_data)

    output_file = f"Out_{index}/config_scan_{index}.json"
    with open(output_file, 'w') as f:
        json.dump(data, f, indent=4)

    current_directory = os.getcwd()

    job_config = {
        "executable": current_directory+"/submit.sh", 
        "arguments": f"{index}",
        "output": current_directory+f"/Out_{index}/job_output.txt", 
        "error": current_directory+f"/Out_{index}/job_error.txt",
        "log": current_directory+f"/Out_{index}/job_log.txt", 
        "request_cpus": 1, 
        "request_memory": "2GB",  
        "request_disk": "2GB",  
    }

    job = htcondor.Submit(job_config)
    schedd = htcondor.Schedd()
    submit_result = schedd.submit(job)
    time.sleep(0.1)
    
file.close()
