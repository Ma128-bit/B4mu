import sys, os, time
start = time.time()
import argparse
from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    #include "Utilities_v2.h"
""")

from ROOT import flat1D, flat2D, flat_index, add_index

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
    parser.add_argument("--isMC", type=int, help="0 for data 1 for MC")
    args = parser.parse_args()
    index = args.index
    delta = args.delta
    directory = args.directory_IN
    output_dir = args.directory_OUT
    isMC = args.isMC
    
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

    print("Starting!")
    df = load_df(selected_files, "TreeMakerBkg/ntuple")
    #Find best Quadruplet
    df = df.Define("Quadruplet_indexs","best_quadruplets(MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz)")
    df = df.Filter("Quadruplet_indexs[0]>-1")

    for chi in range(5):
        start_2 = time.time()
        branches=[]
        branches.append("isMC")
        df = df.Define("isMC", add_index(isMC))
        branches.append("Quadruplet_index")
        rdf = df.Define("Quadruplet_index", flat_index(chi), ["Quadruplet_indexs"])
        branches.append("chi2_label")
        rdf = rdf.Define("chi2_label", add_index(chi))
        
        rdf = rdf.Filter("Quadruplet_index>-1")
        rdf = rdf.Define("Stats","get_stat(Quadruplet_index, MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, Muon_isTrackerMuon, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz)")
        branches = branches + ["isGlobal", "isPF", "isLoose", "isMedium","isTight", "isSoft", "isTracker"]
        rdf = rdf.Define("isGlobal", flat1D(0), ["Stats"])
        rdf = rdf.Define("isPF", flat1D(1), ["Stats"])
        rdf = rdf.Define("isLoose", flat1D(2), ["Stats"])
        rdf = rdf.Define("isMedium", flat1D(3), ["Stats"])
        rdf = rdf.Define("isTight", flat1D(4), ["Stats"])
        rdf = rdf.Define("isSoft", flat1D(5), ["Stats"])
        rdf = rdf.Define("isTracker", flat1D(6), ["Stats"])
                        
        #Flat muon pt eta phi
        for i in range(1,5):
            ind=str(i)
            for s in ["Pt", "Eta", "Phi"]:
                branches.append("Mu"+ind+"_"+s)
                rdf = rdf.Redefine("Mu"+ind+"_"+s,"flattening(Mu"+ind+"_"+s+", Quadruplet_index)")
        
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
            rdf = rdf.Redefine(v,"flattening("+v+", Quadruplet_index)")


        #Mass filter
        #if isMC==0:
            #rdf = rdf.Filter("(Quadruplet_Mass-5.366)>0.15 || (5.366-Quadruplet_Mass)>0.15")
        
        #Dimuon masses
        rdf = rdf.Define("Dimuon_index","Dimuon(Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, MuonPt, MuonEta, MuonPhi, MuonCharge)")
        #branches.append("Dimuon_mass")
        rdf = rdf.Define("Dimuon_mass","DimuonMass(Dimuon_index, MuonPt, MuonEta, MuonPhi, MuonEnergy)")
        
        #Dimuon vertex chi2:
        #branches.append("Dimuon_chi2")
        rdf = rdf.Define("Dimuon_chi2","DimuonChi2(Dimuon_index, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, MuonPt"+ vertex_chi2+")")
        
        #Flat mass and chi2
        for i in range(2):
            for j in range(2):
                name_mass = "Dimu_OS"+str(i+1)+"_"+str(j+1)
                name_chi2 = "Dimu_OS"+str(i+1)+"_"+str(j+1)+"_chi2"
                branches.append(name_mass)
                branches.append(name_chi2)
                rdf = rdf.Define(name_mass, flat2D(i, j), ["Dimuon_mass"])
                rdf = rdf.Define(name_chi2, flat2D(i, j), ["Dimuon_chi2"])
    
        #BsJPsiPhi selections
        branches.append("BsJPsiPhi_sel_OS1")
        branches.append("BsJPsiPhi_sel_OS2")
        
        rdf = rdf.Define("BsJPsiPhi_sel_OS1","BsJPsiPhi(Dimu_OS1_1, Dimu_OS1_2, Dimu_OS1_1_chi2, Dimu_OS1_2_chi2)")
        rdf = rdf.Define("BsJPsiPhi_sel_OS2","BsJPsiPhi(Dimu_OS2_1, Dimu_OS2_2, Dimu_OS2_1_chi2, Dimu_OS2_2_chi2)")

        if not output_dir.endswith("/"):
            output_dir= output_dir + "/"
        
        rdf.Snapshot("FinalTree", output_dir + "Analyzed_Data_chi_"+str(chi)+"_index_"+str(index)+".root", branches)
        
        print("Performed ",rdf.GetNRuns()," loops")
        del rdf
        del branches
        end = time.time()
        print('Partial execution time ', end-start_2)
    
    print('Total execution time ', end-start)
