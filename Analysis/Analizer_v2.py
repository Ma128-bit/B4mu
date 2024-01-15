import sys, os, time
start = time.time()
import argparse
from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    #include "Utilities_v2.h"
""")

def load_df(files, treename):
    frame = RDataFrame(treename, files)
    return frame

def list_of_root_files(directory):
    file_root = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".root"):
                rel_path = os.path.relpath(root, directory)
                file_root.append(os.path.join(rel_path, file))
    return file_root

def select_root_files(file_root, i , delta):
    selected_files = []
    for file in file_root:
        temp = ((file.split('_')[1]).split('.'))[0]
        file_ID = int(temp)
        if(file_ID>= i*delta and file_ID < (i+1)*delta):
            selected_files.append(file)
    return(selected_files)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="--Analisis inputs: directory, number of files, index")
    parser.add_argument("--index", type=int, help="index for condor submission")
    parser.add_argument("--delta", type=int, help="Number of files per submission")
    parser.add_argument("--directory_IN", type=str, help="Root files directory")
    parser.add_argument("--directory_OUT", type=str, help="Output directory")
    args = parser.parse_args()
    index = args.index
    delta = args.delta
    directory = args.directory_IN
    output_dir = args.directory_OUT
    
    file_root = list_of_root_files(directory)
    selected_files = select_root_files(file_root, index , delta)
    selected_files = sorted(selected_files)
    
    print(selected_files)
    if len(selected_files) == 0:
        print("The vector is empty. End execution.")
        exit()
    
    if directory.endswith("/"):
        selected_files = [directory + s for s in selected_files]
    else:
        selected_files = [directory + "/" + s for s in selected_files]

    branches=[]
    print("Starting!")
    
    start_2 = time.time()
    df = load_df(selected_files, "TreeMakerBkg/ntuple")
    #Find best Quadruplet
    branches.append("Quadruplet_indexs")
    df = df.Define("Quadruplet_indexs","best_quadruplets(MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz)")
    df = df.Filter("Quadruplet_indexs[0]>-1")
    branches.append("Stats")
    df = df.Define("Stats","get_stat(MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz)")
    
    #Flat muon pt eta phi
    for i in range(1,5):
        ind=str(i)
        for s in ["Pt", "Eta", "Phi"]:
            branches.append("Mu"+ind+"_"+s)
            df = df.Redefine("Mu"+ind+"_"+s,"flattening(Mu"+ind+"_"+s+", Quadruplet_indexs)")
    
    #Flat quadruplet variables
    quadruplet_related_var = ["Quadruplet_Mass", "FlightDistBS_SV_Significance", "QuadrupletVtx_Chi2", "QuadrupletVtx_NDOF","Quadruplet_Charge"]
    vertex_chi2=""
    for i in range(1, 4):
        for j in range(i+1,5):
            vertex_chi2 = vertex_chi2 + ", Vtx"+str(i)+str(j)+"_Chi2"
            quadruplet_related_var.append("Vtx"+str(i)+str(j)+"_Chi2")
            quadruplet_related_var.append("Vtx"+str(i)+str(j)+"_nDOF")
    for v in quadruplet_related_var:
        if "Vtx" not in v:
            branches.append(v)
        df = df.Redefine(v,"flattening("+v+", Quadruplet_indexs)")
            
    if not output_dir.endswith("/"):
        output_dir= output_dir + "/"
    
    df.Snapshot("FinalTree", output_dir + "Analyzed_Data_"+str(index)+".root", branches)

    print("Performed ",df.GetNRuns()," loops")
    end = time.time()
    print('Partial execution time ', end-start_2)
    print('Total execution time ', end-start)
