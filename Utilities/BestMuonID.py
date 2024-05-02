#from ROOT import RDataFrame, gROOT, EnableImplicitMT
from ROOT import TChain, gROOT, gDirectory, RooRealVar, RooExponential, RooGaussian, RooAddPdf, RooArgList, RooFit, kFALSE, RooDataHist, RooArgSet, kRed, kGreen, kDashed, TCanvas
gROOT.SetBatch(True)
import numpy as np
import pandas as pd
import math, os, draw_utilities, sys
from multiprocessing import Pool
#from progress.bar import Bar

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass

def process_2(args):
    i_it, j_it, k_it, muon_id, x, chain = args
    i = muon_id[i_it]
    j = muon_id[j_it]
    k = muon_id[k_it]
    sel = ""
    for w in i:
        sel = sel + w + "[0]+" + w + "[1]+" # example: isGlobal[0] + isGloabal[1] +
    for ww in j:
        sel = sel + ww + "[2]+" # example: isPF[2] + 
    for ind, www in enumerate(k):
        if ind < len(k)-1:
            sel = sel + www + "[3]+" # example: isPF[3] + 
        else:
            sel = sel + www + "[3]" # example: isPF[3]
    #sel = "(" + sel + " == " + str(2*len(i)+len(j)+len(k)) + ") && (isJPsiPhi==1)"
    sel = "(" + sel + " == " + str(2*len(i)+len(j)+len(k)) + ") && abs(Dimu_OS1_1-1.019)>0.045 && abs(Dimu_OS1_1-0.782)>0.045 && abs(Dimu_OS1_1-3.096)>0.105 && abs(Dimu_OS1_2-1.019)>0.045 && abs(Dimu_OS1_2-0.782)>0.045 && abs(Dimu_OS1_2-3.096)>0.105 && abs(Dimu_OS2_1-1.019)>0.045 && abs(Dimu_OS2_1-0.782)>0.045 && abs(Dimu_OS2_1-3.096)>0.105 && abs(Dimu_OS2_2-1.019)>0.045 && abs(Dimu_OS2_2-0.782)>0.045 && abs(Dimu_OS2_2-3.096)>0.105"
    print(sel)
    #sel = "(" + i + "[0]+" + i + "[1]+" + j + "[2]+" + k + "[3] == 4) && (isJPsiPhi==1)"
    id = str(i_it) +"_"+ str(j_it) +"_"+ str(k_it)
    #chain.Draw("Quadruplet_Mass_eq>>h_sigtemp"+id+"(52, 4.7, 6.5)", sel + " && isMC>0")
    #h_sigtemp = gDirectory.Get("h_sigtemp"+id) 
    #sigma = h_sigtemp.GetRMS() 
    sigma=0.025

    chain.Draw("Quadruplet_Mass_eq>>h_bkg"+id+"(52, 4.7, 6.5)", sel + " && isMC==0 && abs(Quadruplet_Mass_eq-5.3663)>"+str(3*sigma))
    chain.Draw("Quadruplet_Mass_eq>>h_sig"+id+"(52, 4.7, 6.5)", sel + " && isMC>0 && abs(Quadruplet_Mass_eq-5.3663)<"+str(3*sigma))
    h_sig = gDirectory.Get("h_sig"+id) 
    h_bkg = gDirectory.Get("h_bkg"+id)    
    

    dati = {
        'mean_nsig': 0,
        'mean_nbkg': 0,
        'sigma_nsig': 0,
        'sigma_nbkg': 0,
        'sig3sigma': 0,
        'bkg3sigma': 0,
        'ID12': 0,
        'ID3': 0,
        'ID4': 0,
    }

    dati['mean_nsig'] = h_sig.GetEntries()
    dati['mean_nbkg'] = h_bkg.GetEntries()
    dati['sig3sigma'] = dati['mean_nsig']
    dati['bkg3sigma'] = dati['mean_nbkg']*3*sigma/1.8
    dati['ID12'] = i_it
    dati['ID3'] = j_it
    dati['ID4'] = k_it
    
    del h_sig
    del sel
    del h_bkg
    return dati


def process(args):
    i_it, j_it, k_it, muon_id, x, chain = args
    i = muon_id[i_it]
    j = muon_id[j_it]
    k = muon_id[k_it]
    sel = ""
    for w in i:
        sel = sel + w + "[0]+" + w + "[1]+" # example: isGlobal[0] + isGloabal[1] +
    for ww in j:
        sel = sel + ww + "[2]+" # example: isPF[2] + 
    for ind, www in enumerate(k):
        if ind < len(k)-1:
            sel = sel + www + "[3]+" # example: isPF[3] + 
        else:
            sel = sel + www + "[3]" # example: isPF[3]
    sel = "(" + sel + " == " + str(2*len(i)+len(j)+len(k)) + ") && (isJPsiPhi==1) && vtx_prob>0.0001"
    print(sel)
    #sel = "(" + i + "[0]+" + i + "[1]+" + j + "[2]+" + k + "[3] == 4) && (isJPsiPhi==1)"
    id = str(i_it) +"_"+ str(j_it) +"_"+ str(k_it)
    chain.Draw("Quadruplet_Mass_eq>>h_temp"+id+"(52, 5.0, 5.9)", sel)
    hist_temp = gDirectory.Get("h_temp"+id)    
    
    data = RooDataHist("data", hist_temp.GetTitle(), RooArgSet(x), RooFit.Import(hist_temp, kFALSE))
    
    gamma = RooRealVar("#Gamma", "Gamma", -0.2, -10, 10);
    exp_bkg = RooExponential("exp_bkg", "exp_bkg", x, gamma);

    mean =RooRealVar("mean", "Media gaussiana", 5.367, 5.33, 5.40);
    sigma = RooRealVar("sigma", "Deviazione standard gaussiana", 0.02, 0.01, 0.06);
    gauss_pdf = RooGaussian("gauss_pdf", "Signal Gaussian PDF", x, mean, sigma);
        
    nsig = RooRealVar("nsig", "Numero di segnali", 60, 10, 1000);
    nbkg = RooRealVar("nbkg", "Numero di background", hist_temp.GetEntries(), 40, 2*hist_temp.GetEntries());

    model = RooAddPdf("model", "model", RooArgList(gauss_pdf,  exp_bkg), RooArgList(nsig, nbkg))

    exp_bkg.fitTo(data, RooFit.Range("R1,R2"), RooFit.Verbose(0))
    model.fitTo(data, RooFit.Range("RT"), RooFit.Verbose(0))

    range_up = mean.getVal() + 3*sigma.getVal()
    range_down = mean.getVal() - 3*sigma.getVal()
    x.setRange("signal"+id, range_down, range_up)
    #num_sig = nsig.getVal() * gauss_pdf.createIntegral(x, RooFit.NormSet(x), RooFit.Range("signal"+id))
    #num_bkg = nbkg.getVal() * exp_bkg.createIntegral(x, RooFit.NormSet(x), RooFit.Range("signal"+id))

    xframe = x.frame()
    xframe.SetTitle("Plot of "+ id)
    model.paramOn(xframe, RooFit.Parameters(RooArgSet(nsig, nbkg, mean, sigma, gamma)), RooFit.Layout(0.6, 0.9, 0.9))
    data.plotOn(xframe)
    model.plotOn(xframe, RooFit.Components(RooArgSet(gauss_pdf)), RooFit.LineColor(kRed), RooFit.LineStyle(kDashed))
    model.plotOn(xframe, RooFit.Components(RooArgSet(exp_bkg)), RooFit.LineColor(kGreen), RooFit.LineStyle(kDashed))
    model.plotOn(xframe)

    c1 = TCanvas("c1", "c1", 900, 900)
    xframe.Draw()
    c1.SaveAs("MuonID_plots/Fit_"+id+".png", "png -dpi 600")
    c1.Clear()

    dati = {
        'mean_nsig': 0,
        'mean_nbkg': 0,
        'sigma_nsig': 0,
        'sigma_nbkg': 0,
        'sig3sigma': 0,
        'bkg3sigma': 0,
        'ID12': 0,
        'ID3': 0,
        'ID4': 0,
    }

    dati['mean_nsig'] = nsig.getVal()
    dati['mean_nbkg'] = nbkg.getVal()
    dati['sigma_nsig'] = nsig.getError()
    dati['sigma_nbkg'] = nbkg.getError()
    #dati['sig3sigma'] = num_sig
    #dati['bkg3sigma'] = num_bkg
    dati['ID12'] = i_it
    dati['ID3'] = j_it
    dati['ID4'] = k_it
    
    del data
    del hist_temp
    del sel
    del c1
    del xframe
    return dati
    

if __name__ == "__main__":
    if len(sys.argv) > 1:
        type = sys.argv[1]

    chain = TChain("FinalTree")
    #chain.Add("../Analysis/FinalFiles/Analyzed_Data_2022and23.root")
    chain.Add("../Analysis/FinalFiles_B4mu/Analyzed_Data_B4mu_2022.root")
    out_dir="MuonID_plots"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    #muon_id = [["isGlobal"], ["isTracker"], ["isLoose"], ["isSoft"], ["isMedium"], ["isTight"], ["isGlobal", "isPF"], ["isSoft", "isPF"], ["isTracker", "isPF"], ["isGlobal", "isMedium"], ["isGlobal", "isTight"], ["isTracker", "isMedium"]]
    #muon_id = [["isGlobal"], ["isTracker"], ["isMedium"], ["isTight"], ["isGlobal", "isPF"], ["isGlobal", "isMedium"], ["isGlobal", "isTight"], ["isTracker", "isMedium"]]
    muon_id = [["isGlobal"], ["isTracker"], ["isLoose"], ["isSoft"], ["isMedium"], ["isTight"], ["isGlobal", "isMedium"], ["isGlobal", "isTight"], ["isTracker", "isMedium"]]
    x = RooRealVar("Quadruplet_Mass_eq", "Quadruplet_Mass_eq", 5.0, 5.9)
    x.setRange("R1", 5.0, 5.25)
    x.setRange("R2", 5.55, 5.9)
    x.setRange("RT", 5.0, 5.9)
    x.setBins(52)

    num_cores = os.cpu_count()
    if num_cores>100:
        num_cores = int(num_cores/16)
    print(num_cores)

    pool = Pool(processes=num_cores)
    num_iterations = len(muon_id)
    args_list = [(i_it, j_it, k_it, muon_id, x, chain) for i_it in range(num_iterations) for j_it in range(num_iterations) for k_it in range(num_iterations)]
    if(type == "MC_vs_sidebands"):
        results = pool.map(process_2, args_list)
    else:
        type = "Fit"
        results = pool.map(process, args_list)
    pool.close()
    pool.join()
    merged_dict = {
        'mean_nsig': [],
        'mean_nbkg': [],
        'sigma_nsig': [],
        'sigma_nbkg': [],
        'sig3sigma': [],
        'bkg3sigma': [],
        'ID12': [],
        'ID3': [],
        'ID4': [],
    }
    for dictionary in results:
        for key, val in dictionary.items():
            merged_dict[key].append(val)

    np.savez("MuonID_plots/bestID_2022signal"+type+".npz", results)
    print(merged_dict)
    df = pd.DataFrame(merged_dict)
    df.to_csv("MuonID_plots/DFbestID_2022signal"+type+".csv", index=False)
