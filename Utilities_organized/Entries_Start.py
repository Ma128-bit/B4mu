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

histonames= ["InitialPlots/hEvtCount", "PlotsAfterTrigger/hEvtCount", "PlotsAfter4Muons/hEvtCount", "PlotsAfterBCand/hEvtCount"]

C_names = [s.replace("/hEvtCount", "") for s in histonames]
def load_histo(file_name):
	"""Load ROOT data and turn tree into a pd dataframe"""
	#print("Loading data from", file_name)
	f = uproot.open(file_name)
	sum_out = []
	list = [sum_out]
	for k in range(len(histonames)):
		obj = f[histonames[k]]
		num_entries = obj.values()
		num_entries = sum(num_entries)
		sum_out.append(num_entries)
	df = pd.DataFrame(list, columns=C_names)
	return df

        
def load_data(print_lable, input_list):
	datasets = []
	j = 1
	print(" ", print_lable, "   ", 0, "/",len(input_list), end='\r')
	for entry in input_list:
		files = subprocess.check_output("find %s -type f -name '*root'" % entry, shell=True)
		for f in files.splitlines():
			datasets.append(load_histo(f.decode()))
		print(" ", print_lable, "   ", j, "/",len(input_list), "    ", end='\r')
		j=j+1
	df_all = pd.concat(datasets, ignore_index=True)
	return df_all

def make_sum(print_lable, files, csv = False):
	Run = load_data(print_lable, files)
	if csv == True:
		Run.to_csv(print_lable + ".csv", index=False)
	Run_sum = []
	for k in C_names:
		Run_sum.append(Run[k].sum())
	print("\n  Events: ", Run_sum)
	return Run_sum

if __name__ == "__main__":

    Bd_MC = ["BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/240523_144034/0000", "BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/240523_144153/0000"]
    Bs_MC = ["Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/240523_144046/0000", "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/240523_144206/0000"]
    BsJPsiPhi_MC = ["BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/240523_144059/0000", "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/240523_144218/0000"]

    Bd_MC23 = ["BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/240523_144111/0000", "BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/240523_144231/0000"]
    Bs_MC23 = ["Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/240523_144126/0000", "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/240523_144246/0000"]
    BsJPsiPhi_MC23 = ["BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/240523_144140/0000", "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/240523_144258/0000"]


    Bd_MC_        = ["/lustre/cms/store/user/mbuonsan/"+i for i in Bd_MC]
    Bs_MC_        = ["/lustre/cms/store/user/mbuonsan/"+i for i in Bs_MC]
    BsJPsiPhi_MC_ = ["/lustre/cms/store/user/mbuonsan/"+i for i in BsJPsiPhi_MC]

    Bd_MC_23        = ["/lustre/cms/store/user/mbuonsan/"+i for i in Bd_MC23]
    Bs_MC_23        = ["/lustre/cms/store/user/mbuonsan/"+i for i in Bs_MC23]
    BsJPsiPhi_MC_23 = ["/lustre/cms/store/user/mbuonsan/"+i for i in BsJPsiPhi_MC23]

    if not os.path.exists("Entries"):
        subprocess.run(["mkdir", "Entries"])
        
    start_time = time.time()
    with Pool() as p:
        list = p.starmap(make_sum, [('Bd2022',Bd_MC_, False),('Bs2022',Bs_MC_, False),('BsJPsiPhi2022',BsJPsiPhi_MC_, False), ('Bd2023',Bd_MC_23, False),('Bs2023',Bs_MC_23, False),('BsJPsiPhi2023',BsJPsiPhi_MC_23, False)])
    
    df_out = pd.DataFrame(list, columns=C_names)
    df_out['Index'] = ["Bd2022", "Bs2022", "BsJPsiPhi2022", "Bd2023", "Bs2023", "BsJPsiPhi2023"]
    column_order = ['Index'] + [col for col in df_out if col != 'Index']
    df_out = df_out[column_order]
    df_out.to_csv('Entries/Entries_pre.csv', index=False)
    print("Entries/Entries_pre.csv Saved!")

    end_time = time.time()
    execution_time = (end_time - start_time)/60
    formatted_time = "{:.{}f}".format(execution_time, 2)
    print(f"Execution time: {formatted_time} min")







	