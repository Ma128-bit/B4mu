from ROOT import TChain, gROOT, gDirectory, RooRealVar, RooExponential, RooGaussian, RooAddPdf, RooArgList, RooFit, kFALSE, RooDataHist, RooArgSet, kRed, kGreen, kDashed, TCanvas
gROOT.SetBatch(True)
import numpy as np
import pandas as pd
import math, os, draw_utilities, sys
from multiprocessing import Pool

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass

var = {
    "vtx_prob": [0.0011+i*0.0002 for i in range(10)],
    "Cos2d_PV_SV": [0.7+i/31 for i in range(10)],
    "FlightDistBS_SV_Significance": [1+i/3 for i in range(10)]
}

var2 = {
    "vtx_prob": [-1],
    "Cos2d_PV_SV": [0],
    "FlightDistBS_SV_Significance": [0, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 5]
}

def ProcessControl(args):
    i_it, j_it, k_it, x, chain = args

    sel_i = var["vtx_prob"][i_it]
    sel_j = var["Cos2d_PV_SV"][j_it]
    sel_k = var["FlightDistBS_SV_Significance"][k_it]
    
    sel = f"vtx_prob>{sel_i} && Cos2d_PV_SV>{sel_j} && FlightDistBS_SV_Significance>{sel_k}" +" && (isJPsiPhi==1)"
    print(sel)
    
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
    c1.SaveAs("Best_cut/Fit_"+id+".png", "png -dpi 600")
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
    
def ProcessSignal(args):
    i_it, j_it, k_it, chain, chain1, chain2, sig1, sig2 = args

    sel_i = var2["vtx_prob"][i_it]
    sel_j = var2["Cos2d_PV_SV"][j_it]
    sel_k = var2["FlightDistBS_SV_Significance"][k_it]
    
    sel = f"vtx_prob>{sel_i} && Cos2d_PV_SV>{sel_j} && FlightDistBS_SV_Significance>{sel_k}"
    print(sel)
    sel = sel + "&& abs(Dimu_OS1_1-1.019)>0.045 && abs(Dimu_OS1_1-0.782)>0.045 && abs(Dimu_OS1_1-3.096)>0.105 && abs(Dimu_OS1_2-1.019)>0.045 && abs(Dimu_OS1_2-0.782)>0.045 && abs(Dimu_OS1_2-3.096)>0.105 && abs(Dimu_OS2_1-1.019)>0.045 && abs(Dimu_OS2_1-0.782)>0.045 && abs(Dimu_OS2_1-3.096)>0.105 && abs(Dimu_OS2_2-1.019)>0.045 && abs(Dimu_OS2_2-0.782)>0.045 && abs(Dimu_OS2_2-3.096)>0.105"
    
    id = str(i_it) +"_"+ str(j_it) +"_"+ str(k_it)
    chain.Draw("Quadruplet_Mass>>h_data"+id+"(52, 4.8, 6.1)", sel+" && abs(Quadruplet_Mass-5.279)>3*0.05 && abs(Quadruplet_Mass-5.366)>3*0.05 && isMC==0")
    hist_temp0 = gDirectory.Get("h_data"+id)    

    chain1.Draw("Quadruplet_Mass>>h_MCd"+id+"(52, 4.8, 6.1)", sel+" && abs(Quadruplet_Mass-5.279)<3*0.05 && isMC>0")
    hist_temp1 = gDirectory.Get("h_MCd"+id)    

    chain2.Draw("Quadruplet_Mass>>h_MCs"+id+"(52, 4.8, 6.1)", sel+" && abs(Quadruplet_Mass-5.366)<3*0.05 && isMC>0")
    hist_temp2 = gDirectory.Get("h_MCs"+id)    

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

    #if ((hist_temp1.GetEntries()>0.5*sig1) and (hist_temp2.GetEntries()>0.5*sig2)):
    if(True):
        dati['sig3sigma'] = hist_temp1.GetEntries()
        dati['mean_nsig'] = hist_temp2.GetEntries()
        dati['bkg3sigma'] = hist_temp0.GetEntries()
    dati['ID12'] = i_it
    dati['ID3'] = j_it
    dati['ID4'] = k_it
    
    del hist_temp0
    del hist_temp1
    del hist_temp2
    del sel
    return dati

if __name__ == "__main__":
    print("Starting!")
    chain = TChain("FinalTree")
    chain.Add("../Analysis/FinalFiles_B4mu/Analyzed_Data_B4mu_2022.root")
    chain1 = TChain("FinalTree")
    chain1.Add("../Analysis/FinalFiles_B4mu/Analyzed_MC_Bd_4mu_2022.root")
    chain2 = TChain("FinalTree")
    chain2.Add("../Analysis/FinalFiles_B4mu/Analyzed_MC_Bs_4mu_2022.root")
    out_dir="Best_cut"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    sel = "abs(Dimu_OS1_1-1.019)>0.045 && abs(Dimu_OS1_1-0.782)>0.045 && abs(Dimu_OS1_1-3.096)>0.105 && abs(Dimu_OS1_2-1.019)>0.045 && abs(Dimu_OS1_2-0.782)>0.045 && abs(Dimu_OS1_2-3.096)>0.105 && abs(Dimu_OS2_1-1.019)>0.045 && abs(Dimu_OS2_1-0.782)>0.045 && abs(Dimu_OS2_1-3.096)>0.105 && abs(Dimu_OS2_2-1.019)>0.045 && abs(Dimu_OS2_2-0.782)>0.045 && abs(Dimu_OS2_2-3.096)>0.105"
    
    chain1.Draw("Quadruplet_Mass>>h_MCd(52, 4.8, 6.1)", sel+" && abs(Quadruplet_Mass-5.279)<3*0.05 && isMC>0")
    hist_temp1 = gDirectory.Get("h_MCd")    
    sig1 = hist_temp1.GetEntries()
    chain2.Draw("Quadruplet_Mass>>h_MCs(52, 4.8, 6.1)", sel+" && abs(Quadruplet_Mass-5.366)<3*0.05 && isMC>0")
    hist_temp2 = gDirectory.Get("h_MCs")    
    sig2 = hist_temp2.GetEntries()
    del hist_temp1
    del hist_temp2
    del sel
    
    x = RooRealVar("Quadruplet_Mass_eq", "Quadruplet_Mass_eq", 5.0, 5.9)
    x.setRange("R1", 5.0, 5.25)
    x.setRange("R2", 5.55, 5.9)
    x.setRange("RT", 5.0, 5.9)
    x.setBins(52)

    num_cores = os.cpu_count()
    if num_cores>100:
        num_cores = int(num_cores/16)

    pool = Pool(processes=num_cores)
    args_list = [(i_it, j_it, k_it, x, chain) for i_it in range(len(var["vtx_prob"])) for j_it in range(len(var["Cos2d_PV_SV"])) for k_it in range(len(var["FlightDistBS_SV_Significance"]))]

    args_list2 = [(i_it, j_it, k_it, chain, chain1, chain2, sig1, sig2) for i_it in range(len(var2["vtx_prob"])) for j_it in range(len(var2["Cos2d_PV_SV"])) for k_it in range(len(var2["FlightDistBS_SV_Significance"]))]
    
    #results = pool.map(ProcessControl, args_list)
    results = pool.map(ProcessSignal, args_list2)
    
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

    df = pd.DataFrame(merged_dict)
    df.to_csv("Best_cut/best_pre_sel_2022control.csv", index=False)
