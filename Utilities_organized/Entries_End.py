import sys, os, subprocess, json
num_cores = os.cpu_count()
print("N. CPU cores: ", num_cores)
import warnings
import time
from datetime import datetime
warnings.filterwarnings("ignore", category=UserWarning, module="numpy")
import numpy as np
warnings.filterwarnings("default", category=UserWarning, module="numpy")
import pandas as pd
import uproot
import argparse
from multiprocessing import Pool

def make_sum(print_lable, file_name, csv = False):
    f = uproot.open(file_name)
    obj = f["FinalTree"]
    data = obj["lumi"].array()
    if csv == True:
        data.to_csv(print_lable + ".csv", index=False)
    print(print_lable+"   Events: ", len(data))
    return len(data)


if __name__ == "__main__":
    dir = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_B4mu_03_08_24/"
    
    Bd_MC = dir+"Analyzed_MC_Bd_4mu_2022.root"
    Bs_MC = dir+"Analyzed_MC_Bs_4mu_2022.root"
    BsJPsiPhi_MC = dir+"Analyzed_MC_BsJPsiPhi_2022.root"

    Bd_MC23 = dir+"Analyzed_MC_Bd_4mu_2023.root"
    Bs_MC23 = dir+"Analyzed_MC_Bs_4mu_2023.root"
    BsJPsiPhi_MC23 = dir+"Analyzed_MC_BsJPsiPhi_2023.root"


    if not os.path.exists("Entries"):
        subprocess.run(["mkdir", "Entries"])
        
    start_time = time.time()
    with Pool() as p:
        list = p.starmap(make_sum, [('Bd2022',Bd_MC, False),('Bs2022',Bs_MC, False),('BsJPsiPhi2022',BsJPsiPhi_MC, False), ('Bd2023',Bd_MC23, False),('Bs2023',Bs_MC23, False),('BsJPsiPhi2023',BsJPsiPhi_MC23, False)])
    
    df_out = pd.DataFrame(list, columns=["Entries_postAna"])
    df_out['Index'] = ["Bd2022", "Bs2022", "BsJPsiPhi2022", "Bd2023", "Bs2023", "BsJPsiPhi2023"]
    column_order = ['Index'] + [col for col in df_out if col != 'Index']
    df_out = df_out[column_order]
    df_out.to_csv('Entries/Entries_post.csv', index=False)
    print("Entries/Entries_post.csv Saved!")

    end_time = time.time()
    execution_time = (end_time - start_time)/60
    formatted_time = "{:.{}f}".format(execution_time, 2)
    print(f"Execution time: {formatted_time} min")







	