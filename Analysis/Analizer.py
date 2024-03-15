import sys, os, time
start = time.time()
import argparse
from ROOT import RDataFrame, gROOT, EnableImplicitMT, gInterpreter

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    #include "Utilities.h"
""")

from ROOT import flat2D, flat1D_int, flat1D_double, flat0D_int, flat0D_double, add_int, add_double, TwoObjMassFit, FourObjMassFit

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

def MuonIDs(rdf, branches):
    rdf = rdf.Define("Stats","get_stat(Quadruplet_index, MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, Muon_isTrackerMuon, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz)")
    branches = branches + ["isGlobal", "isPF", "isLoose", "isMedium","isTight", "isSoft", "isTracker"]
    rdf = rdf.Define("isGlobal", flat1D_int(0), ["Stats"])
    rdf = rdf.Define("isPF", flat1D_int(1), ["Stats"])
    rdf = rdf.Define("isLoose", flat1D_int(2), ["Stats"])
    rdf = rdf.Define("isMedium", flat1D_int(3), ["Stats"])
    rdf = rdf.Define("isTight", flat1D_int(4), ["Stats"])
    rdf = rdf.Define("isSoft", flat1D_int(5), ["Stats"])
    rdf = rdf.Define("isTracker", flat1D_int(6), ["Stats"])
    return rdf

def Flat_MuVar(rdf, branches):
    for i in range(1,5):
        ind=str(i)
        for s in ["Pt", "Eta", "Phi"]:
            branches.append("Mu"+ind+"_"+s)
            branches.append("RefTrack"+ind+"_"+s)
            rdf = rdf.Redefine("Mu"+ind+"_"+s,"flattening(Mu"+ind+"_"+s+", Quadruplet_index)")
            rdf = rdf.Redefine("RefTrack"+ind+"_"+s,"flattening(RefTrack"+ind+"_"+s+", Quadruplet_index)")
    return rdf

def QuadMuVar(rdf, branches, analysis_type):
    quadruplet_related_var = ["Quadruplet_Mass", "FlightDistBS_SV_Significance", "QuadrupletVtx_Chi2", "QuadrupletVtx_NDOF","Quadruplet_Charge", "QuadrupletVtx_x", "QuadrupletVtx_y", "QuadrupletVtx_z", 
                              "RefittedPV_x", "RefittedPV_y", "RefittedPV_z", "Quadruplet_Pt", "Quadruplet_Eta", "Quadruplet_Phi", "FlightDistPVSV", "mu1_pfreliso03", "mu2_pfreliso03", "mu3_pfreliso03", 
                              "mu4_pfreliso03", "vtx_prob"] #FlightDistBS_SV_Significance = lxy_sig
    for var in quadruplet_related_var:
        branches.append(var)
        
    vertex_chi2=""
    for i in range(1, 4):
        for j in range(i+1,5):
            vertex_chi2 = vertex_chi2 + ", Vtx"+str(i)+str(j)+"_Chi2"
            quadruplet_related_var.append("Vtx"+str(i)+str(j)+"_Chi2")
            quadruplet_related_var.append("Vtx"+str(i)+str(j)+"_nDOF")
    for v in quadruplet_related_var:
        rdf = rdf.Redefine(v,"flattening("+v+", Quadruplet_index)")

    branches.append("Quadruplet_Mass_no_refit") #Not refitted 4mu mass
    rdf = rdf.Define("Quadruplet_Mass_no_refit", "NoRefitMass"+analysis_type+"(MuonPt, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, Mu3_Eta, Mu4_Eta, Mu3_Phi, Mu4_Phi, MuonEta, MuonPhi, MuonEnergy)")
    return rdf, vertex_chi2

def MVA_inputs(rdf, branches):
    #bs_dxy_sig
        
    #cos(θ) angle between B flight direction and 4-muon momentum
    branches.append("Cos2d_PV_SV") #cos2d
    rdf = rdf.Define("Cos2d_PV_SV","TreeFin_CosAngle(QuadrupletVtx_x, QuadrupletVtx_y, QuadrupletVtx_z, RefittedPV_x, RefittedPV_y, RefittedPV_z, Quadruplet_Pt, Quadruplet_Eta, Quadruplet_Phi, FlightDistPVSV)")
        
    #∆R max (maximum R distance between any of the 4 muons and the direction of the sum of the 4 muons momenta)
    branches.append("dR_max") #dr
    rdf = rdf.Define("dR_max", "dR_Max(Quadruplet_Eta, Quadruplet_Phi, Mu1_Eta, Mu1_Phi, Mu2_Eta, Mu2_Phi, Mu3_Eta, Mu3_Phi, Mu4_Eta, Mu4_Phi)")
    return rdf

def DiMuVar(rdf, branches, vertex_chi2):
    #Dimuon masses
    rdf = rdf.Define("Dimuon_index","Dimuon(Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, MuonPt, MuonEta, MuonPhi, MuonCharge)")
    rdf = rdf.Define("Dimuon_mass","DimuonMass(Dimuon_index, MuonPt, MuonEta, MuonPhi, MuonEnergy)")
    #Dimuon dR
    rdf = rdf.Define("Dimuon_dR","DimuondR(Dimuon_index, MuonEta, MuonPhi)")  
    #Dimuon vertex chi2:
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

    branches_add ["Dimu_OS1_dR", "Dimu_OS2_dR", "Quadruplet_Mass_eq", "Dimu_OS_max", "Dimu_OS_min", "isJPsiPhi"]
    for b in branches_add:
        branches.append(b)
    
    # Flat Dimuon_dR
    rdf = rdf.Define("Dimu_OS1_dR", flat0D_double(0), ["Dimuon_dR"])
    rdf = rdf.Define("Dimu_OS2_dR", flat0D_double(1), ["Dimuon_dR"])

    #Di muon final Mass
    rdf = rdf.Define("DimuonMassfinal","DimuonMassfinal(Dimu_OS1_1, Dimu_OS1_2, Dimu_OS2_1, Dimu_OS2_2)")
    rdf = rdf.Define("Dimu_OS_max", flat0D_double(0), ["DimuonMassfinal"])
    rdf = rdf.Define("Dimu_OS_min", flat0D_double(1), ["DimuonMassfinal"])
    rdf = rdf.Define("Quadruplet_Mass_eq","BsJPsiPhiMass(Dimu_OS_max, Dimu_OS_min, Quadruplet_Mass)")
    rdf = rdf.Define("isJPsiPhi","BsJPsiPhi(Dimu_OS_max, Dimu_OS_min)")
    return rdf

def DiMassVar_control(rdf, branches, analysis_type):
    branches.append("Dimu_mass")
    branches.append("Ditrk_mass")
    rdf = rdf.Define("Di_mass", "DiMass"+analysis_type+"(Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, Mu3_Eta, Mu4_Eta, Mu3_Phi, Mu4_Phi, MuonPt, MuonEta, MuonPhi, MuonEnergy)")
    rdf = rdf.Define("Dimu_mass", flat0D_double(0), ["Di_mass"])
    rdf = rdf.Define("Ditrk_mass", flat0D_double(1), ["Di_mass"])
    return rdf

    
          
def GenVar(rdf, branches, isMC):
    if isMC != 0:
        rdf = rdf.Define("gen_info", "GenMatching_v2(MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, GenParticle_Pt, GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2,  GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId)")
        rdf = rdf.Define("GenMu_Pt", flat1D_double(0), ["gen_info"])
        rdf = rdf.Define("GenMu_Eta", flat1D_double(1), ["gen_info"])
        rdf = rdf.Define("GenMu_Phi", flat1D_double(2), ["gen_info"])
    for mu in range(1,5):
        branches.append("GenMu"+str(mu)+"_Pt")
        branches.append("GenMu"+str(mu)+"_Eta")
        branches.append("GenMu"+str(mu)+"_Phi")
        if isMC != 0:
            rdf = rdf.Define("GenMu"+str(mu)+"_Pt", flat0D_double(mu-1), ["GenMu_Pt"])
            rdf = rdf.Define("GenMu"+str(mu)+"_Eta", flat0D_double(mu-1), ["GenMu_Eta"])
            rdf = rdf.Define("GenMu"+str(mu)+"_Phi", flat0D_double(mu-1), ["GenMu_Phi"])
        else:
            rdf = rdf.Define("GenMu"+str(mu)+"_Pt", add_double(0.))
            rdf = rdf.Define("GenMu"+str(mu)+"_Eta", add_double(0.))
            rdf = rdf.Define("GenMu"+str(mu)+"_Phi", add_double(0.))
    return rdf


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="--Analisis inputs: directory, number of files, index")
    parser.add_argument("--index", type=int, help="index for condor submission")
    parser.add_argument("--delta", type=int, help="Number of files per submission")
    parser.add_argument("--directory_IN", type=str, help="Root files directory")
    parser.add_argument("--directory_OUT", type=str, help="Output directory")
    parser.add_argument("--isMC", type=int, help="0 for data 1 for MC")
    parser.add_argument("--analysis_type", type=str, help="Analysis type")
    args = parser.parse_args()
    index = args.index
    delta = args.delta
    directory = args.directory_IN
    output_dir = args.directory_OUT
    isMC = args.isMC
    analysis_type = args.analysis_type
    
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

    if(analysis_type=="B2mu2K"):
        tree_dir_name = "TreeB2mu2K"
    elif(analysis_type=="B2muKpi"):
        tree_dir_name = "TreeB2muKpi"
    elif(analysis_type=="B4mu"):
        tree_dir_name = "TreeMakerBkg"
    else:
        print("Wrong analysis type. End execution.")
        exit()
        
    df = load_df(selected_files, tree_dir_name+"/ntuple")
    
    #Find best Quadruplet
    df = df.Define("isMC", add_int(isMC))
    if(analysis_type=="B4mu"):
        df = df.Define("Quadruplet_indexs","B4mu_QuadSel(isMC, evt, MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz, GenParticle_Pt, GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2, GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId)")
    else:
        df = df.Define("Quadruplet_indexs","B2muX_QuadSel(isMC, evt, MuonPt, MuonEta, MuonPhi, Mu1_Pt, Mu2_Pt, Mu3_Pt, Mu4_Pt, Mu3_Eta, Mu4_Eta, NGoodQuadruplets, QuadrupletVtx_Chi2, Quadruplet_Mass, Muon_isGlobal, Muon_isPF, Muon_isLoose, Muon_isMedium, Muon_isTight, Muon_isSoft, MuonPt_HLT, MuonEta_HLT, MuonPhi_HLT, FlightDistBS_SV_Significance, Muon_vz, GenParticle_Pt, GenParticle_Pt_v2, GenParticle_Eta_v2, GenParticle_Phi_v2, GenParticle_PdgId, GenParticle_MotherPdgId, GenParticle_GrandMotherPdgId)")
    df = df.Filter("Quadruplet_indexs[0]>-1")

    for chi in range(1):
        start_2 = time.time()
        branches=["evt", "isMC", "run", "lumi"]
        rdf = df.Define("Quadruplet_index", flat0D_int(chi), ["Quadruplet_indexs"])
        rdf = rdf.Filter("Quadruplet_index>-1")
        branches.append("chi2_label")
        rdf = rdf.Define("chi2_label", add_int(chi))
        
        if(analysis_type=="B4mu"):
            rdf = MuonIDs(rdf, branches) #Add muonIDs
        rdf = Flat_MuVar(rdf, branches) #Flat muon pt eta phi
        rdf, vertex_chi2 = QuadMuVar(rdf, branches, analysis_type) #Quadruplet variables
        rdf = MVA_inputs(rdf, branches) #Define MVA input variables
        if(analysis_type=="B4mu"):
            rdf = DiMuVar(rdf, branches, vertex_chi2) #Define Di-Muon variables
            rdf = GenVar(rdf, branches, isMC) #Gen-Level variables

        if(analysis_type!="B4mu"):
            rdf = DiMassVar_control(rdf, branches, analysis_type)
            """
            branches.append("PhiMassTest2K")
            branches.append("PhiMassTestKpi")
            rdf = rdf.Define("PhiMassTest2K", TwoObjMassFit(0.493677, 0.493677), ["RefTrack3_Pt", "RefTrack4_Pt", "RefTrack3_Eta", "RefTrack4_Eta","RefTrack3_Phi", "RefTrack4_Phi"])
            rdf = rdf.Define("PhiMassTestKpi", TwoObjMassFit(0.493677, 0.139570), ["RefTrack3_Pt", "RefTrack4_Pt", "RefTrack3_Eta", "RefTrack4_Eta","RefTrack3_Phi", "RefTrack4_Phi"])
            """
            
        if not output_dir.endswith("/"):
            output_dir= output_dir + "/"
        
        rdf.Snapshot("FinalTree", output_dir + "Analyzed_Data_chi_"+str(chi)+"_index_"+str(index)+".root", branches)
        
        print("Performed ",rdf.GetNRuns()," loops")
        del rdf
        del branches
        end = time.time()
        print('Partial execution time ', end-start_2)
    
    print('Total execution time ', end-start)
