from ROOT import gROOT, TCanvas, TChain, EnableImplicitMT, RooRealVar, RooArgSet, RooDataSet, RooJohnson, RooFit, RDataFrame, gInterpreter, RooExtendPdf, TFile, TH1, gStyle
from ROOT import RooWorkspace, RooArgList, RooAddPdf, RooExponential, RooGaussian, RooCBShape, RooNumber, RooVoigtian, RooStats, RooAddPdf, TText, kBlack, TLatex

import ctypes
import argparse, json, os
from uncertainties import ufloat
from scipy.stats import beta
import numpy as np 
import cmsstyle as CMS
CMS.SetEnergy(13.6, unit='TeV')
CMS.SetLumi(170.7, unit='fb', run="", round_lumi=1)
CMS.SetExtraText("Preliminary")

import logging

gROOT.SetBatch(True)
EnableImplicitMT(16)
gStyle.SetOptStat(True)
TH1.SetDefaultSumw2()

gInterpreter.Declare("""
    double new_weight(int isMC, double weight, double w_Bs, double w_Bd, double BDT_eff_Bs, double BDT_eff_Bd){
        if(isMC==1) return BDT_eff_Bs*w_Bs*weight;
        if(isMC==2) return BDT_eff_Bd*w_Bd*weight;
        else return 1;
    }
    double dividebyNevents(ROOT::VecOps::RVec<Char_t> ID, int isMC, double weight, double N_evt_Bs_2022, double N_evt_Bs_2023, double N_evt_Bs_2024, double N_evt_Bd_2022, double N_evt_Bd_2023, double N_evt_Bd_2024){
        if(isMC==0) return weight;
        TString id_str(ID.data(), ID.size()-1);
        if(id_str=="Bs2022") return weight/N_evt_Bs_2022;
        if(id_str=="Bs2023") return weight/N_evt_Bs_2023;
        if(id_str=="Bs2024") return weight/N_evt_Bs_2024;
        if(id_str=="Bd2022") return weight/N_evt_Bd_2022;
        if(id_str=="Bd2023") return weight/N_evt_Bd_2023;
        if(id_str=="Bd2024") return weight/N_evt_Bd_2024;
        else return weight;
    }

""")

from statsmodels.stats.proportion import proportion_confint
def cp_intervals(Nobs, Ntot, cl=0.99, verbose = False):
    eff = 1.*Nobs/Ntot
    lo, hi = proportion_confint(Nobs, Ntot, 1.-cl, method='beta')

    lor = lo/eff if eff else -99
    hir = hi/eff if eff else -99
    if verbose :
        print('-- Clopper Pearson --')
        print('\n'.join([
        'Ntot:  {T}','Nobs:  {O}','eff:  {E}','low:  {L}','high:  {H}'
        ]).format(T=Ntot, O=Nobs, E=eff, L=lo, H=hi))

    return lor, hir

def load_info_from_json(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    return json_file["RootFile"], json_file["TreeName"], json_file["Mass_var"], json_file["MC_id"], json_file["Branches4sel"], json_file["Unblind"], json_file["N_control"], json_file["N_control_err"], json_file["BDT_cut"], json_file["BDT_eff_Bs"], json_file["BDT_eff_Bd"], json_file["BDT_eff_BsJpsiphi"], json_file["N_evt_Bs_2022"],json_file["N_evt_Bs_2023"], json_file["N_evt_Bs_2024"], json_file["N_evt_Bd_2022"], json_file["N_evt_Bd_2023"], json_file["N_evt_Bd_2024"]

def load_initial_param(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    return json_file["Bs_lambda"], json_file["Bs_gamma"], json_file["Bs_delta"], json_file["B0_lambda"], json_file["B0_gamma"], json_file["B0_delta"]

def category_sel(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    categories_list = json_file["categories"]
    category = {cat: "" for cat in categories_list}
    for c in categories_list:
        category[c] = json_file[c]
    print(category)
    return category

def plotData(model, dataset, name="test.png"):
    can = TCanvas()
    can.SetCanvasSize(1000,800)
    plot = mass.frame()
    plot.SetTitle("Plot of "+name)
    dataset.plotOn(plot, binning, RooFit.MarkerColor(0), RooFit.LineColor(0) )
    model.plotOn( plot, RooFit.NormRange(fit_range), RooFit.Range("full"), RooFit.LineColor(4))
    dataset.plotOn( plot, RooFit.CutRange(fit_range), binning )
    plot.Draw()
    can.Update()
    can.SaveAs(name)
    del can
    del plot

def plotMC(model, dataset, range="sig", name="test.png"):
    can = TCanvas()
    can.SetCanvasSize(1000,800)
    plot = mass.frame(5.090 , 5.529, 50)
    plot.SetTitle("Plot of "+name)
    dataset.plotOn(plot)
    model.paramOn(plot, RooFit.Layout(0.5, 0.9, 0.9))
    model.plotOn(plot, RooFit.NormRange(range), RooFit.LineColor(2) )
    plot.Draw()
    can.Update()
    can.SaveAs(name)
    del can
    del plot

"""
def plot(modelB, modelS, datasetB, datasetS, name="test.png", te=""):
    can = TCanvas()
    #can.SetCanvasSize(1000,800)
    plot = mass.frame()
    plot.SetTitle("Plot of "+name)
    datasetB.plotOn(plot, binning, RooFit.MarkerColor(0), RooFit.LineColor(0) )
    modelB.plotOn( plot, RooFit.NormRange(fit_range), RooFit.Range(fit_range), RooFit.LineColor(4))
    datasetS.plotOn(plot, binning, RooFit.Range("sig"), RooFit.MarkerColor(0), RooFit.LineColor(0) )
    modelS.plotOn(plot, RooFit.Range("sig"), RooFit.LineColor(2) )
    datasetB.plotOn( plot, RooFit.CutRange(fit_range), binning )

    legend = plot.BuildLegend()
    legend.SetBorderSize(0)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.04)

    data_hist = plot.getHist()

    if data_hist:
        maxony = 0.0
        for i in range(data_hist.GetN()):
            x = ctypes.c_double(0)
            y = ctypes.c_double(0)
            data_hist.GetPoint(i, x, y)
            
            y_err_high = data_hist.GetErrorYhigh(i)

            temp_max = y.value+y_err_high

            if temp_max>maxony:
                maxony = temp_max

    else:
        print("Errore: nessun RooHist trovato!")

    text = TText(0.5, 0.5, te)
    text.SetNDC(True)
    
    can = CMS.cmsCanvas("", plot.GetXaxis().GetXmin(), plot.GetXaxis().GetXmax(), 0, 0.5+maxony, "4#mu mass", "Entries", square=False, iPos=0, extraSpace=0)
    can.SetCanvasSize(1200,900)
    plot.Draw("same")
    legend.Draw("same")
    text.Draw("same")
    can.Update()
    CMS.UpdatePad()
    can.SaveAs(name)
    del can
    del plot
"""

def plot(modelB, modelS, datasetB, datasetS, name="test.png", te=""):

    from ROOT import (
        TCanvas, TLegend, TText, RooFit
    )
    import ctypes

    # -----------------------------
    # Frame RooFit
    # -----------------------------
    plot = mass.frame()
    plot.SetTitle("Plot of " + name)

    # -----------------------------
    # DATA + MODELS (con Name!)
    # -----------------------------

    datasetS.plotOn(plot, binning, RooFit.Range("sig"), RooFit.MarkerColor(0), RooFit.LineColor(0) )
    modelS.plotOn(plot, RooFit.Range("sig"), RooFit.Name("modelS"),RooFit.LineColor(2) )

    datasetB.plotOn(plot, binning, RooFit.Name("h_datasetB"),  RooFit.MarkerColor(0), RooFit.LineColor(0) )
    data_hist = plot.getHist("h_datasetB")


    if isUnblind:
        modelB.plotOn( plot, RooFit.NormRange(fit_range), RooFit.Range("full"), RooFit.Name("modelB"),RooFit.LineColor(4))
        datasetB.plotOn( plot, RooFit.CutRange("full"), binning, RooFit.Name("datasetB"))
    else:
        modelB.plotOn( plot, RooFit.NormRange(fit_range), RooFit.Range(fit_range), RooFit.Name("modelB"),RooFit.LineColor(4))
        datasetB.plotOn( plot, RooFit.CutRange(fit_range), binning, RooFit.Name("datasetB"))


    # -----------------------------
    # Calcolo massimo Y (con errori)
    # -----------------------------

    maxony = 0.0
    if data_hist:
        for i in range(data_hist.GetN()):
            x = ctypes.c_double(0)
            y = ctypes.c_double(0)
            data_hist.GetPoint(i, x, y)

            y_err_high = data_hist.GetErrorYhigh(i)
            temp_max = y.value + y_err_high

            if temp_max > maxony:
                maxony = temp_max
    else:
        print("Errore: nessun RooHist trovato!")

    # -----------------------------
    # Canvas CMS
    # -----------------------------
    can = CMS.cmsCanvas("", plot.GetXaxis().GetXmin(), plot.GetXaxis().GetXmax(), 0, 1.5 * maxony, "m(4#mu) (GeV)", "Events / 40 MeV", square=False, iPos=0, extraSpace=0.05, yTitOffset=1.0)
    can.SetCanvasSize(1200, 900)

    plot.Draw("same")

    # -----------------------------
    # Legenda MANUALE (corretta)
    # -----------------------------
    legend = CMS.cmsLeg(0.6, 0.66, 0.87, 0.82, textSize=0.035, textFont=42, textColor=kBlack, columns=None)

    CMS.addToLegend(
        legend,
        (plot.findObject("datasetB"), f"Data{' (blind)' if not isUnblind else ''}", "pe"),
        (plot.findObject("modelB"),  "Background-only fit", "l"),
        (plot.findObject("modelS"),  "Signal",         "l"),
    )

    legend.Draw("same")

    # -----------------------------
    # Testo extra
    # -----------------------------
    latex = TLatex()
    latex.SetNDC(True)
    latex.SetTextSize(0.035)
    latex.SetTextFont(42)
    latex.SetTextAlign(13)   # 1 = left, 3 = top
    latex.DrawLatex(0.6, 0.64, te)

    latex_cat_r = TLatex()
    latex_cat_r.SetNDC(True)
    latex_cat_r.SetTextSize(0.04)
    latex_cat_r.SetTextFont(62)
    #latex_cat_r.SetTextAlign(31)  # right-top
    latex_cat_r.DrawLatex(0.6, 0.85, f"Category {cat}")

    # -----------------------------
    # Update & Save
    # -----------------------------
    can.Update()
    CMS.UpdatePad()
    can.SaveAs(name)

    del can
    del plot

def addweight(rdf, N_control, BDT_eff_Bs, BDT_eff_Bd, submit, N_evt_Bs_2022, N_evt_Bs_2023,  N_evt_Bs_2024, N_evt_Bd_2022, N_evt_Bd_2023, N_evt_Bd_2024, inputfile_loc):
    rdf = rdf.Redefine("weight", f"dividebyNevents(ID, isMC, weight, {N_evt_Bs_2022}, {N_evt_Bs_2023}, {N_evt_Bs_2024}, {N_evt_Bd_2022}, {N_evt_Bd_2023}, {N_evt_Bd_2024})")
    rdf = rdf.Define("w_central", f"weight * weight_pileUp * ctau_weight_central * bdt_reweight_0 * bdt_reweight_1 * bdt_reweight_2 * {N_control}") 
    rdf = rdf.Define("signal_weight", "new_weight("+mcid_name+",w_central,"+BRBs+","+BRBd+","+str(BDT_eff_Bs)+","+str(BDT_eff_Bd)+")")
    rdf = rdf.Define("signal_weight_hbr", "new_weight("+mcid_name+",w_central, 1.0e-9 ,4.0e-10,"+str(BDT_eff_Bs)+","+str(BDT_eff_Bd)+")")

    #Bd
    fd_fs = 4.5139  # from paper (computed in /work/regartne/CMSSW_11_3_4/src/B4mu/b4mu-analysis/systematics/fs_fd/fs_fd.py )
    fd_fs_err = (
        4.5139 / 100
    ) * 4  # 4 percent uncertainty based on biggest uncertaitny #0.0096  # stat + sys

    
    #light_w = 1.0061924528364175
    #heavy_w = 0.977140710129535
    #rdf = rdf.Define("weight_up_Bd", f"signal_weight * {(fd_fs+fd_fs_err)/fd_fs} * {light_w}") 
    #rdf = rdf.Define("weight_down_Bd", f"signal_weight * {(fd_fs-fd_fs_err)/fd_fs} * {heavy_w}") 
    rdf = rdf.Define("weight_up_Bd", f"signal_weight * {(fd_fs+fd_fs_err)/fd_fs}") 
    rdf = rdf.Define("weight_down_Bd", f"signal_weight * {(fd_fs-fd_fs_err)/fd_fs}") 

    #Bs
    rdf = rdf.Define("w_up", f"weight * weight_pileUp * ctau_weight_heavy * bdt_reweight_0 * bdt_reweight_1 * bdt_reweight_2 *{N_control}") 
    rdf = rdf.Define("weight_up", "new_weight("+mcid_name+",w_up,"+BRBs+","+BRBd+","+str(BDT_eff_Bs)+","+str(BDT_eff_Bd)+")")
    
    rdf = rdf.Define("w_down", f"weight * weight_pileUp * ctau_weight_light * bdt_reweight_0 * bdt_reweight_1 * bdt_reweight_2 *{N_control}") 
    rdf = rdf.Define("weight_down", "new_weight("+mcid_name+",w_down,"+BRBs+","+BRBd+","+str(BDT_eff_Bs)+","+str(BDT_eff_Bd)+")")
    if submit==False:
        os.makedirs("RootFiles", exist_ok=True)
        rdf.Snapshot(tree_name, "RootFiles/Dataset.root")
    else:
        rdf.Snapshot(tree_name, inputfile_loc)

#UN USED
def fit_temp_old(DataSet, mass):
    mu = RooRealVar("mu", "mu", 5.366, 4.1, 6.80);
    lam = RooRealVar("lam", "lam", 0.04, 0.001, 1.)
    gam = RooRealVar("gam", "gam", 0.3, -6.0, 6.0)
    delt = RooRealVar("delt", "delt", 1.0, 0.01, 5.0)
    JS = RooJohnson("JS", "JS", mass, mu, lam, gam, delt)
    events = RooRealVar("events", "events", 1.0, 0.0, 10.0)
    #model = RooExtendPdf("model", "model", JS, events)
    model = RooAddPdf("model", "model", RooArgList(JS), RooArgList(events))


    model.fitTo(DataSet, RooFit.SumW2Error(True), RooFit.Extended(True), RooFit.PrintLevel(-1))
    return events.getVal()

def fit_temp(DataSet, mass, DataSet_hbr, lam_in, gamm_in, delt_in):
    mu = RooRealVar("mu", "mu", 4.50, 6.0)
    lam = RooRealVar("lam", "lam", lam_in, 0.001, 1.5)
    gam = RooRealVar("gam", "gam", gamm_in, -2.5, 2.5)
    delt = RooRealVar("delt", "delt", delt_in, 0.1, 10)
    frac = RooRealVar("frac", "frac", 0.20, 0.01, 0.99)
    
    johnson = RooJohnson("johnson", "johnson", mass, mu, lam, gam, delt)
    gauss = RooGaussian("gauss", "gauss", mass, mu, lam)
    JS = RooAddPdf("JS", "JS", RooArgList(gauss, johnson), RooArgList(frac))

    JS.fitTo(DataSet_hbr, RooFit.SumW2Error(True), RooFit.Optimize(True), RooFit.PrintLevel(-1))
    mu.setConstant(True)
    lam.setConstant(True)
    gam.setConstant(True)
    delt.setConstant(True)
    frac.setConstant(True)

    events = RooRealVar("events", "events", 1.0, 0.0, 15.0)
    #model = RooExtendPdf("model", "model", JS, events)
    model = RooAddPdf("model", "model", RooArgList(JS), RooArgList(events))
    #model.fitTo(DataSet, RooFit.SumW2Error(True), RooFit.Extended(True), RooFit.PrintLevel(-1))
    model.fitTo(DataSet, RooFit.SumW2Error(True), RooFit.Extended(True),  RooFit.Optimize(True), RooFit.PrintLevel(-1))    
    return events.getVal()

# Uncertainty for BDT reweighting
BDT_unc_ = {
    "A1": 0.9052820868984488,
    "A2": 0.9991309722784815,
    "B1": 0.9333830749464549,
    "B2": 1.0071341545238461,
    "C1": 0.9969644754262247,
    "C2": 0.9943751910580685
}

if __name__ == "__main__":
    # Import info from user
    parser = argparse.ArgumentParser(description="config and root file")
    parser.add_argument("--config", type=str, help="config name")
    parser.add_argument("--submit", action='store_true', help="submit")
    parser.add_argument("--best_cut_scan", action='store_true', help="submit")
    parser.add_argument("--index", type=int, default=0, help="index for submit")
    parser.add_argument("--inputfile_loc", type=str, default="", help="inputfile_loc")
    parser.add_argument("--Bs", type=str, default="0", help="Bs BR")
    parser.add_argument("--Bd", type=str, default="0", help="Bd BR")
    parser.add_argument("--multipdf", action='store_true', help="is multipdf")
    args = parser.parse_args()
    submit = args.submit
    index = args.index
    inputfile_loc = args.inputfile_loc
    configfile = args.config
    isMultipdf = args.multipdf
    best_cut_scan = args.best_cut_scan
    BRBs = args.Bs
    BRBd = args.Bd

    if (BRBs=="0") and (BRBd=="0"):
        print("Wrong BRs ... exiting")
        exit()

    categories = category_sel(configfile)

    # add anticorrelation for combine
    BDT_unc = {cat: 1.0 for cat in categories}

    if not best_cut_scan:
        for key, value in BDT_unc_.items():
            result = abs(1 - value)
            result = np.sqrt( result**2 + 0.1**2)  # 10% unc for difference between 2m2k and signal data/MC
            if key in ["A2", "B2", "C2"]:
                result *= -1
            result += 1
            BDT_unc[key] = result

    print("isMultipdf:", isMultipdf)

    root_file, tree_name, mass_name, mcid_name, branches, isUnblind, N_control, N_control_err, BDT_cut, BDT_eff_Bs, BDT_eff_Bd, BDT_eff_BsJpsiphi, N_evt_Bs_2022, N_evt_Bs_2023, N_evt_Bs_2024, N_evt_Bd_2022, N_evt_Bd_2023, N_evt_Bd_2024 = load_info_from_json(configfile)
    Bs_lambda, Bs_gamma, Bs_delta, B0_lambda, B0_gamma, B0_delta = load_initial_param(configfile)
    
    # Add weight in the Tree
    rdf = RDataFrame(tree_name, root_file)
    rdf = rdf.Filter(f"bdt_cv>{BDT_cut}")
    addweight(rdf, N_control, BDT_eff_Bs/BDT_eff_BsJpsiphi, BDT_eff_Bd/BDT_eff_BsJpsiphi, submit, N_evt_Bs_2022, N_evt_Bs_2023, N_evt_Bs_2024, N_evt_Bd_2022, N_evt_Bd_2023, N_evt_Bd_2024, inputfile_loc)
    del rdf
    
    tree = TChain(tree_name)
    if submit==False:
        tree.AddFile("RootFiles/Dataset.root")  
    else:
        tree.AddFile(inputfile_loc)  

    mass = RooRealVar(mass_name, mass_name, 4.5, 6.5)
    roomc = RooRealVar(mcid_name, mcid_name, 0, 2)
    weight_b = RooRealVar("signal_weight", "signal_weight", 0. ,5.0e+6)
    weight_b_hbr = RooRealVar("signal_weight_hbr", "signal_weight_hbr", 0. ,5.0e+8)
    weight_data = RooRealVar("weight", "weight", 0. ,5.0e+6)

    weight_b_up = RooRealVar("weight_up", "weight_up", 0. ,5.0e+6)
    weight_b_down = RooRealVar("weight_down", "weight_down", 0. ,5.0e+6)
    weight_b_up_Bd = RooRealVar("weight_up_Bd", "weight_up_Bd", 0. ,5.0e+6)
    weight_b_down_Bd = RooRealVar("weight_down_Bd", "weight_down_Bd", 0. ,5.0e+6)

    real_vars = []
    for branch in branches.keys():
        real_vars.append(RooRealVar(branch, branch, branches[branch][0], branches[branch][1]))

    variables = RooArgSet(mass)
    variables.add(roomc)
    variables.add(weight_b)
    variables.add(weight_data)
    variables.add(weight_b_hbr)
    variables.add(weight_b_up)
    variables.add(weight_b_down)
    variables.add(weight_b_up_Bd)
    variables.add(weight_b_down_Bd)

    for var in real_vars:
        variables.add(var)

    binning = RooFit.Binning(50, 4.5, 6.5)
    mass.setRange("loSB", 4.5, 5.090 )
    mass.setRange("hiSB", 5.529, 6.5 )
    mass.setRange("sig", 5.090 , 5.529 )
    mass.setRange("sigBd", 5.090 , 5.438 )
    mass.setRange("sigBs", 5.180 , 5.529 )

    #Range per cat:
    mass.setRange("sigBdA", 5.150 , 5.357 )
    mass.setRange("sigBsA", 5.260 , 5.450 )
    mass.setRange("sigBdB", 5.120 , 5.396 )
    mass.setRange("sigBsB", 5.220 , 5.479 )
    mass.setRange("sigBdC", 5.090 , 5.438 )
    mass.setRange("sigBsC", 5.180 , 5.529 )

    mass.setRange("loSBA", 4.5, 5.150) 
    mass.setRange("hiSBA", 5.450, 6.5 )

    mass.setRange("loSBB", 4.5, 5.120) 
    mass.setRange("hiSBB", 5.479, 6.5 )

    mass.setRange("loSBC", 4.5, 5.090) 
    mass.setRange("hiSBC", 5.529, 6.5 )

    mass.setRange("full", 4.5, 6.5)

    datacard_dir="Datacards"
    plots_dir="Plots"
    if submit==True:
        datacard_dir=f"Out_{index}/Datacards"
        plots_dir=f"Out_{index}/Plots"
        logging.basicConfig(
            filename=f'Out_{index}/combine.log',      
            level=logging.INFO,       
            format='%(asctime)s - %(levelname)s - %(message)s'
        )
    else:
        logging.basicConfig(
            filename='combine.log',      
            level=logging.INFO,       
            format='%(asctime)s - %(levelname)s - %(message)s'
        )
        os.makedirs(datacard_dir, exist_ok=True)
        os.makedirs(plots_dir, exist_ok=True)
    command = "cd "+datacard_dir+"; combineCards.py "

    for cat in categories.keys():
        # Load DataSets
        cat_val = 0 if "A" in cat else 1 if "B" in cat else 2 if "C" in cat else None
        #fit_range = "full" if isUnblind else "loSBA,hiSBA" if cat_val==0 else "loSBB,hiSBB" if cat_val==1 else "loSBC,hiSBC" if cat_val==2 else "loSB,hiSB"
        fit_range = "loSBA,hiSBA" if cat_val==0 else "loSBB,hiSBB" if cat_val==1 else "loSBC,hiSBC" if cat_val==2 else "loSB,hiSB"
        cat_id = "A" if cat_val==0 else "B" if cat_val==1 else "C" if cat_val==2 else ""
        oscat = 0 if "1" in cat else -0.8
        if best_cut_scan:
            #data = RooDataSet('data', 'data', tree, variables, "isMC==0 && "+categories[cat], "wfinal")
            #data_nobdt = RooDataSet('data_nobdt', 'data_nobdt', tree, variables, f"isMC==0 && category=={cat_val}", "wfinal")
            data = RooDataSet('data', 'data', tree, variables, "isMC==0 && "+categories[cat], "weight")
            data_nobdt = RooDataSet('data_nobdt', 'data_nobdt', tree, variables, f"isMC==0 && category=={cat_val}", "weight")
        else:
            data = RooDataSet('data', 'data', tree, variables, "isMC==0 && "+categories[cat])
            data_nobdt = RooDataSet('data_nobdt', 'data_nobdt', tree, variables, f"isMC==0 && category=={cat_val}")
        MCBs = RooDataSet('MCBs', 'MCBs', tree, variables, "isMC==1 &&"+categories[cat], "signal_weight")
        MCBd = RooDataSet('MCBd', 'MCBd', tree, variables, "isMC==2 && "+categories[cat], "signal_weight")
        MCBs_hbr = RooDataSet('MCBs_hbr', 'MCBs_hbr', tree, variables, "isMC==1 &&"+categories[cat], "signal_weight_hbr")
        MCBd_hbr = RooDataSet('MCBd_hbr', 'MCBd_hbr', tree, variables, "isMC==2 && "+categories[cat], "signal_weight_hbr")
        MC_all = RooDataSet('MC_all', 'MC_all', tree, variables, "(isMC==2 || isMC==1) && "+categories[cat], "signal_weight")

        # Signals Models and Fits
        # RooJohnson model 
        muBs = RooRealVar("muBs", "muBs", 5.36, 5.3, 5.4)
        lambdaBs = RooRealVar("lambdaBs", "lambdaBs", Bs_lambda[cat_val], 0.001, 1.5)
        gammaBs = RooRealVar("gammaBs", "gammaBs", Bs_gamma[cat_val], -2.5, 2.5)
        deltaBs = RooRealVar("deltaBs", "deltaBs", Bs_delta[cat_val], 0.1, 15)
        fracBs = RooRealVar("fracBs", "fracBs", 0.20, 0.01, 0.99)
        johnson_Bs = RooJohnson("johnson_Bs", "johnson_Bs", mass, muBs, lambdaBs, gammaBs, deltaBs)
        gauss_Bs = RooGaussian("gauss_Bs", "gauss_Bs", mass, muBs, lambdaBs)

        JS_Bs = RooAddPdf("JS_Bs", "JS_Bs", RooArgList(gauss_Bs, johnson_Bs), RooArgList(fracBs))

        if(BRBs!="0"):
            JS_Bs.fitTo(MCBs_hbr, RooFit.Range("sigBs"+cat_id), RooFit.SumW2Error(True), RooFit.Optimize(True), RooFit.PrintLevel(-1))
            muBs.setConstant(True)
            lambdaBs.setConstant(True)
            gammaBs.setConstant(True)
            deltaBs.setConstant(True)
            fracBs.setConstant(True)

        # CB model  
        #muBs = RooRealVar("muBs", "muBs", 5.366, 4.1, 6.80);
        #lambdaBs = RooRealVar("lambdaBs", "lambdaBs", 0.03, 0.06)
        #gammaBs = RooRealVar("gammaBs", "gammaBs", 2.2, 0.01, 5.0) #RooNumber.infinity()
        #deltaBs = RooRealVar("deltaBs", "deltaBs", 1.7, 0.01, 5.0)
        #JS_Bs = RooCBShape("CBShape", "CBShape", mass, muBs, lambdaBs, gammaBs, deltaBs)

        # Voigtian model  
        #muBs = RooRealVar("muBs", "muBs", 5.366, 4.1, 6.80);
        #lambdaBs = RooRealVar("lambdaBs", "lambdaBs", 0.04, 0.001, 1.0)
        #gammaBs = RooRealVar("gammaBs", "gammaBs", 0.04, 0.001, 1.0)
        #deltaBs = RooRealVar("deltaBs", "deltaBs", 2.0 , 0., 5.0)
        #JS_Bs = RooVoigtian("RooVoigtian", "RooVoigtian", mass, muBs, lambdaBs, gammaBs)

        eventsBs = RooRealVar("eventsBs", "eventsBs", 1.0, 0.0, 15.0)
        #model_Bs = RooExtendPdf("model_Bs", "model_Bs", JS_Bs, eventsBs)
        model_Bs = RooAddPdf("model_Bs", "model_Bs", RooArgList(JS_Bs), RooArgList(eventsBs))
        if(BRBs!="0"):
            #model_Bs.fitTo(MCBs, RooFit.Range("sigBs"+cat_id), RooFit.SumW2Error(True), RooFit.Extended(True), RooFit.PrintLevel(-1))
            model_Bs.fitTo(MCBs, RooFit.Range("sigBs"+cat_id), RooFit.SumW2Error(True), RooFit.Extended(True),  RooFit.Optimize(True), RooFit.PrintLevel(-1))
            muBs.setConstant(False)
            lambdaBs.setConstant(False)
            gammaBs.setConstant(False)
            deltaBs.setConstant(False)
            fracBs.setConstant(False)
            plotMC(model_Bs, MCBs, range="sigBs"+cat_id, name=plots_dir+"/Fit_"+cat+"_MC_Bs.png")
        else:
            eventsBs.setVal(0)
            eventsBs.setConstant(True)
        
        muBd = RooRealVar("muBd", "muBd", 5.27, 5.2, 5.35)
        lambdaBd = RooRealVar("lambdaBd", "lambdaBd", B0_lambda[cat_val], 0.001, 1.5)
        gammaBd = RooRealVar("gammaBd", "gammaBd", B0_gamma[cat_val], -2.5, 2.5)
        deltaBd = RooRealVar("deltaBd", "deltaBd", B0_delta[cat_val], 0.1, 10)
        fracBd = RooRealVar("fracBd", "fracBd", 0.20, 0.01, 0.99)
        johnson_Bd = RooJohnson("johnson_Bd", "johnson_Bd", mass, muBd, lambdaBd, gammaBd, deltaBd)
        gauss_Bd = RooGaussian("gauss_Bd", "gauss_Bd", mass, muBd, lambdaBd)

        JS_Bd = RooAddPdf("JS_Bd", "JS_Bd", RooArgList(gauss_Bd, johnson_Bd), RooArgList(fracBd))

        if(BRBd!="0"):
            JS_Bd.fitTo(MCBd_hbr, RooFit.Range("sigBd"+cat_id), RooFit.SumW2Error(True), RooFit.Optimize(True), RooFit.PrintLevel(-1))
            muBd.setConstant(True)
            lambdaBd.setConstant(True)
            gammaBd.setConstant(True)
            deltaBd.setConstant(True)
            fracBd.setConstant(True)

        eventsBd = RooRealVar("eventsBd", "eventsBd", 1.0, 0.0, 15.0)
        #model_Bd = RooExtendPdf("model_Bd", "model_Bd", JS_Bd, eventsBd)
        model_Bd = RooAddPdf("model_Bd", "model_Bd", RooArgList(JS_Bd), RooArgList(eventsBd))
        if(BRBd!="0"):
            #model_Bd.fitTo(MCBd, RooFit.Range("sigBd"+cat_id), RooFit.SumW2Error(True), RooFit.Extended(True), RooFit.PrintLevel(-1))
            model_Bd.fitTo(MCBd, RooFit.Range("sigBd"+cat_id), RooFit.SumW2Error(True), RooFit.Extended(True),  RooFit.Optimize(True), RooFit.PrintLevel(-1))
            muBd.setConstant(False)
            lambdaBd.setConstant(False)
            gammaBd.setConstant(False)
            deltaBd.setConstant(False)
            fracBd.setConstant(False)
            plotMC(model_Bd, MCBd, range="sigBd"+cat_id, name=plots_dir+"/Fit_"+cat+"_MC_Bd.png")
        else:
            eventsBd.setVal(0)
            eventsBd.setConstant(True)
        
        # Save Fit results/plots
        bs_nevt = eventsBs.getVal()
        bd_nevt = eventsBd.getVal()
        print("Bs events: ", bs_nevt)
        print("Bd events: ", bd_nevt)
        logging.info(f"Category {cat} - Central Bs events: {bs_nevt}")
        logging.info(f"Category {cat} - Central Bd events: {bd_nevt}")

        if(BRBd!="0" and BRBs!="0"):
            MCBs_up = RooDataSet('MCBs_up', 'MCBs_up', tree, variables, "isMC==1 &&"+categories[cat], "weight_up")
            MCBs_down = RooDataSet('MCBs_down', 'MCBs_down', tree, variables, "isMC==1 &&"+categories[cat], "weight_down")
            N_sig_HES = fit_temp(MCBs_up, mass, MCBs_hbr, Bs_lambda[cat_val], Bs_gamma[cat_val], Bs_delta[cat_val])
            N_sig_LES = fit_temp(MCBs_down, mass, MCBs_hbr, Bs_lambda[cat_val], Bs_gamma[cat_val], Bs_delta[cat_val])
            #N_sig_HES = 0
            #N_sig_LES = 0
            MCBd_up = RooDataSet('MCBd_up', 'MCBd_up', tree, variables, "isMC==2 &&"+categories[cat], "weight_up_Bd")
            MCBd_down = RooDataSet('MCBd_down', 'MCBd_down', tree, variables, "isMC==2 &&"+categories[cat], "weight_down_Bd")
            N_sig_up = fit_temp(MCBd_up, mass, MCBd_hbr, B0_lambda[cat_val], B0_gamma[cat_val], B0_delta[cat_val])
            N_sig_down = fit_temp(MCBd_down, mass, MCBd_hbr, B0_lambda[cat_val], B0_gamma[cat_val], B0_delta[cat_val])
            #print("Bs events: ", N_sig_HES, " - ", N_sig_LES)
            print(f"Category {cat} - Bd events: up ", N_sig_up, " - down ", N_sig_down)
            logging.info(f"Category {cat} - Bd events: up {N_sig_up} - down {N_sig_down}")
        else:
            N_sig_HES = 0
            N_sig_LES = 0
            N_sig_up = 0
            N_sig_down = 0
        bs_nevt_ = ufloat(bs_nevt, max(abs(N_sig_HES - bs_nevt), abs(N_sig_LES - bs_nevt)))
        bd_nevt_ = ufloat(bd_nevt, max(abs(bd_nevt - N_sig_up), abs(bd_nevt - N_sig_down)))
        ratio_ = bs_nevt_/(bs_nevt_ + bd_nevt_)
        ratio = RooRealVar("ratio", "ratio", ratio_.nominal_value, ratio_.nominal_value - ratio_.std_dev, ratio_.nominal_value + ratio_.std_dev)
            
        sig_model = RooAddPdf("sig_model", "sig_model", RooArgList(JS_Bs,  JS_Bd), ratio)            

        """
            bkg_pdf = bkg_multi_pdf.getCurrentPdf()
            variables = bkg_pdf.getVariables()
            alpha=variables[1]
            #iterator = variables.createIterator()
            #var = iterator.Next()
            #while var:
            #    if isinstance(var, RooRealVar):
            #        print(f"Variable name: {var.GetName()}, value: {var.getVal()}, range: [{var.getMin()}, {var.getMax()}]")
            #    var = iterator.Next()
            #exit()
        """

        # Bkg Model and Fit:
        if isMultipdf == True:
            print("****************** MultiPDF ******************")
            multipdfFile = TFile.Open("MultiPdfWorkspaces/workspace_"+cat+".root","read")
            ws = multipdfFile.Get('w')
            bkg_multi_pdf = ws.pdf("multipdf_bkg_"+cat)
            bkg_pdf = bkg_multi_pdf.getCurrentPdf()
        else:
            alpha = RooRealVar("alpha_"+cat, "alpha_"+cat, -0.9, -10, 10)
            bkg_pdf = RooExponential("bkg_"+cat, "bkg_"+cat, mass, alpha)
        pdf_norm = RooRealVar("nbkg_"+cat, "nbkg_"+cat, 10.0, 0.0, 5000.0)
        #pdf_norm = RooRealVar("bkg_"+cat+"_norm", "bkg_"+cat+"_norm", 10.0, 0.0, 5000.0)
        #bkg_model = RooExtendPdf("bkg_model", "bkg_model", bkg_pdf, pdf_norm)
        bkg_model = RooAddPdf("bkg_model", "bkg_model", RooArgList(bkg_pdf), RooArgList(pdf_norm))
        bkg_model.fitTo(data, RooFit.Range(fit_range), RooFit.Extended(True), RooFit.PrintLevel(-1))
        
        """
        # Fix to have right bkg events in the whale range
        norm_full = bkg_pdf.createIntegral(RooArgSet(mass), RooFit.Range("full")).getVal()
        norm_sb   = bkg_pdf.createIntegral(RooArgSet(mass), RooFit.Range(fit_range)).getVal()
        pdf_norm.setVal(pdf_norm.getVal() * (norm_full / norm_sb))
        """

        print("Bkg events: ", pdf_norm.getVal())

        # Save Plot:
        latex_str = ""
        lines = []

        if BRBs != "0":
            base, exp = BRBs.split("e")
            exp = int(exp)
            lines.append(f"BR(B_{{s}}^{{0}} #rightarrow 4#mu) = {base} #times 10^{{{exp}}}")

        if BRBd != "0":
            base, exp = BRBd.split("e")
            exp = int(exp)
            lines.append(f"BR(B^{{0}} #rightarrow 4#mu) = {base} #times 10^{{{exp}}}")

        # Combina le righe usando #splitline
        if len(lines) == 1:
            latex_str = lines[0]
        elif len(lines) >= 2:
            latex_str = f"#splitline{{{lines[0]}}}{{{lines[1]}}}"
    
        plot(bkg_model, sig_model, data, MC_all, name=plots_dir+"/Fit_"+cat+"_dataMC.pdf", te=latex_str)

        # Create workspace
        if submit==True:
            output = TFile.Open(f"Out_{index}/Workspaces/workspace_"+cat+".root","recreate")
        else:
            os.makedirs("Workspaces", exist_ok=True)
            output = TFile.Open("Workspaces/workspace_"+cat+".root","recreate")
        print("Creating workspace")
        w = RooWorkspace('w')

        getattr(w, 'import')(mass)

        if isMultipdf == True:
            bkg_multi_pdf.SetName("multipdf_bkg_"+cat)
            getattr(w, 'import')(bkg_multi_pdf)
            #pdf_norm.SetName("multipdf_bkg_"+cat+"_norm")
            #getattr(w, 'import')(pdf_norm)
        else:
            bkg_pdf.SetName("bkg_"+cat)
            getattr(w, 'import')(bkg_pdf)
            #getattr(w, 'import')(pdf_norm)

        w.factory("muBs"+cat+"[%f,%f]" % (muBs.getMin(), muBs.getMax()))
        w.factory("lambdaBs"+cat+"[%f,%f,%f]" % (lambdaBs.getVal(), lambdaBs.getVal()-0.001, lambdaBs.getVal()+0.001))
        w.factory("gammaBs"+cat+"[%f,%f,%f]" % (gammaBs.getVal(), gammaBs.getVal()-0.001, gammaBs.getVal()+0.001))
        w.factory("deltaBs"+cat+"[%f,%f,%f]" % (deltaBs.getVal(), 0.8*deltaBs.getVal(), 1.2*deltaBs.getVal()))
        w.factory("fracBs"+cat+"[%f,%f,%f]" % (fracBs.getVal(), fracBs.getVal()-0.001, fracBs.getVal()+0.001))

        w.factory("muBd"+cat+"[%f,%f]" % (muBd.getMin(), muBd.getMax()))
        w.factory("lambdaBd"+cat+"[%f,%f,%f]" % (lambdaBd.getVal(), lambdaBd.getVal()-0.001, lambdaBd.getVal()+0.001))
        w.factory("gammaBd"+cat+"[%f,%f,%f]" % (gammaBd.getVal(), gammaBd.getVal()-0.001, gammaBd.getVal()+0.001))
        w.factory("deltaBd"+cat+"[%f,%f,%f]" % (deltaBd.getVal(), 0.8*deltaBd.getVal(), 1.2*deltaBd.getVal()))
        w.factory("fracBd"+cat+"[%f,%f,%f]" % (fracBd.getVal(), fracBd.getVal()-0.001, fracBd.getVal()+0.001))

        if(BRBd=="0"):
            #w.factory("RooVoigtian::sig_"+cat+"("+mass_name+", muBs, lambdaBs, gammaBs)")
            w.factory("RooJohnson::johnson_"+cat+"("+mass_name+", muBs"+cat+", lambdaBs"+cat+", gammaBs"+cat+", deltaBs"+cat+")")
            w.factory("RooGaussian::gaussian_"+cat+"("+mass_name+", muBs"+cat+", lambdaBs"+cat+")")
            w.factory("RooAddPdf::sig_"+cat+"(gaussian_"+cat+", johnson_"+cat+", fracBs"+cat+")")
            #w.factory("RooCBShape::sig_"+cat+"("+mass_name+", muBs, lambdaBs, gammaBs, deltaBs)")
        elif(BRBs=="0"):
            w.factory("RooJohnson::johnson_"+cat+"("+mass_name+", muBd"+cat+", lambdaBd"+cat+", gammaBd"+cat+", deltaBd"+cat+")")
            w.factory("RooGaussian::gaussian_"+cat+"("+mass_name+", muBd"+cat+", lambdaBd"+cat+")")
            w.factory("RooAddPdf::sig_"+cat+"(gaussian_"+cat+", johnson_"+cat+", fracBd"+cat+")")

        else:
            #w.factory("ratio[%f]" % ratio.getVal())
            w.factory("ratio"+cat+"[%f,%f]" % (ratio.getMin(), ratio.getMax()))
            w.factory("RooJohnson::johnson_Bs"+cat+"("+mass_name+", muBs"+cat+", lambdaBs"+cat+", gammaBs"+cat+", deltaBs"+cat+")")
            w.factory("RooJohnson::johnson_Bd"+cat+"("+mass_name+", muBd"+cat+", lambdaBd"+cat+", gammaBd"+cat+", deltaBd"+cat+")")
            w.factory("RooGaussian::gauss_Bs"+cat+"("+mass_name+", muBs"+cat+", lambdaBs"+cat+")")
            w.factory("RooGaussian::gauss_Bd"+cat+"("+mass_name+", muBd"+cat+", lambdaBd"+cat+")")
            w.factory("RooAddPdf::JS_Bs"+cat+"(gauss_Bs"+cat+", johnson_Bs"+cat+", fracBs"+cat+")")
            w.factory("RooAddPdf::JS_Bd"+cat+"(gauss_Bd"+cat+", johnson_Bd"+cat+", fracBd"+cat+")")

            w.factory("RooAddPdf::sig_"+cat+"(JS_Bs"+cat+", JS_Bd"+cat+", ratio"+cat+")")
        
        it = w.allVars().createIterator()
        all_vars = [it.Next() for _ in range(w.allVars().getSize())]
        for var in all_vars:
            #if var.GetName() in ["lambdaBs"+cat, "gammaBs"+cat, "deltaBs"+cat, "lambdaBd"+cat, "gammaBd"+cat, "deltaBd"+cat]:
            if var.GetName() in ["lambdaBs"+cat, "gammaBs"+cat, "lambdaBd"+cat, "gammaBd"+cat, "fracBs"+cat, "fracBd"+cat]:
                var.setConstant(True)

        if isUnblind==False:
            reducedData = RooStats.AsymptoticCalculator.GenerateAsimovData(bkg_model, RooArgSet(mass))
            fulldata = data.reduce(RooArgSet(mass),mass_name+">5.566 || "+mass_name+"<5.079")
            fulldata_nobdt = data_nobdt.reduce(RooArgSet(mass),mass_name+">5.566 || "+mass_name+"<5.079")
            n = fulldata_nobdt.sumEntries()
            k = fulldata.sumEntries()
        else:
            reducedData = data.reduce(RooArgSet(mass))
            fulldata = data.reduce(RooArgSet(mass))
            fulldata_nobdt = data_nobdt.reduce(RooArgSet(mass))
            n = fulldata_nobdt.sumEntries()
            k = fulldata.sumEntries()

        reducedData.SetName("data_obs_"+cat)
        getattr(w,'import')(reducedData)
        w.Print()
        w.Write()
        output.Close()

        # dump the text datacard
        #alpha_ = 0.01
        #p_u, p_o = beta.ppf([alpha_ / 2, 1 - alpha_ / 2], [k, k + 1], [n - k + 1, n - k])
        #eff_kn = k / n
        bkg_norm_lo, bkg_norm_hi = cp_intervals(Nobs =k, Ntot= n)


        command += "datacard"+cat+".txt "
        with open(datacard_dir+"/datacard"+cat+".txt", 'w') as card:
            card.write(
            '''
imax 1 number of bins
jmax 1 number of processes minus 1
kmax * number of nuisance parameters
--------------------------------------------------------------------------------
shapes background    {inclusive}       ../Workspaces/workspace_{inclusive}.root w:{multipdf_}bkg_{inclusive}
shapes signal        {inclusive}       ../Workspaces/workspace_{inclusive}.root w:sig_{inclusive}
shapes data_obs      {inclusive}       ../Workspaces/workspace_{inclusive}.root w:data_obs_{inclusive}
--------------------------------------------------------------------------------
bin               {inclusive}
observation       {obs:f}
--------------------------------------------------------------------------------
bin                                     {inclusive}           {inclusive}
process                                 signal              background
process                                 0                   1
rate                                    {signal:.4f}        {bkg:.4f}
--------------------------------------------------------------------------------
lumi          lnN                       1.025               -   
MC_q2         lnN                       1.1                 -   
norm_lifetime           lnN                       1.001       -
BDT           lnN                       {BDT_uncV:.4f}       -
BR_norm       lnN                       {BRnorm_unc}                -
{not_single_sig}fsfd_or_lifet     lnN                       {fsfd_or_lifet_unc}               -
norm_fit      gmN                       {N_control}         {alpha_:.8f}         -
muBs{inclusive}          param                     {muBs:.4f}          {muBs_permille:.8f}
muBd{inclusive}          param                     {muBd:.4f}          {muBd_permille:.8f}
deltaBs{inclusive}          param                     {deltaBs:.4f}          {deltaBs_permille:.8f}
deltaBd{inclusive}          param                     {deltaBd:.4f}          {deltaBd_permille:.8f}
{ismpdf}alpha_{inclusive}        param                     {slopeval:.4f}          {slopeerr:.4f}
{single_sig}ratio{inclusive}         param                     {ratio:.4f}          {ratio_unc:.4f}
--------------------------------------------------------------------------------
{multipdf_}bkg_rate_{inclusive} rateParam {inclusive} background 1. [{p_u:.4f},{p_o:.4f}]
#{multipdf_}bkg_rate_{inclusive} flatParam
'''.format(
                    multipdf_ = "multipdf_" if isMultipdf else "",
                    ismpdf = "#" if isMultipdf else "",
                    fsfd_or_lifet_unc = 1.00 if (BRBd!="0" and BRBs!="0") else 1.04+oscat if BRBs!="0" else 1.04,
                    single_sig = "" if (BRBd!="0" and BRBs!="0") else "#", 
                    not_single_sig = "#" if (BRBs!="0") else "", 
                    inclusive = cat,
                    obs      = data.sumEntries() if isUnblind==True else -1, 
                    signal   = bs_nevt + bd_nevt, # number of EXPECTED signal events, INCLUDES the a priori normalisation.
                    bkg      = pdf_norm.getVal() if pdf_norm.getVal()>=1 else 0.01,
                    BDT_uncV  = BDT_unc[cat],
                    alpha_=(bs_nevt + bd_nevt) / N_control,
                    BRnorm_unc=1.086,
                    N_control=int(N_control),
                    muBs=muBs.getVal(),  # mean from fit to MC!
                    muBd=muBd.getVal(),
                    muBs_permille=(muBs.getVal() / 1000) * 2,
                    muBd_permille=(muBd.getVal() / 1000) * 2,

                    deltaBs=deltaBs.getVal(),  # mean from fit to MC!
                    deltaBd=deltaBd.getVal(),
                    deltaBs_permille=(deltaBs.getVal() / 100) * 7,
                    deltaBd_permille=(deltaBd.getVal() / 100) * 7,

                    ratio=ratio_.nominal_value,
                    ratio_unc=ratio_.std_dev,
                    slopeval = alpha.getVal()if isMultipdf==False else 1.0, 
                    slopeerr = 1.0 if isMultipdf==True else alpha.getError() if (alpha.getError()>0 and isMultipdf==False) else 1.0, 
                    p_u=bkg_norm_lo if pdf_norm.getVal()>=1 else 0.0, #p_u / eff_kn,
                    p_o=bkg_norm_hi if pdf_norm.getVal()>=1 else 500.0 #p_o / eff_kn
                    )
            )
            if isMultipdf:
                card.write(f"multipdf_bkg_cat_{cat} discrete")
        del w
        del output
        del bkg_model
        del sig_model
        del model_Bd
        del model_Bs
        del data
        del data_nobdt
        del MCBs
        del MCBd
        if isMultipdf == True:
            del ws
            multipdfFile.Close()
            del multipdfFile
    
    command += " > datacard_combined.txt;"
    os.system(command)

    # Change number of nuisance with *
    with open(datacard_dir+"/datacard_combined.txt", "r") as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        if line.startswith("kmax") and "number of nuisance parameters" in line:
            lines[i] = "kmax * number of nuisance parameters\n"
            break

    with open(datacard_dir+"/datacard_combined.txt", "w") as file:
        file.writelines(lines)

    os.system(f"cd {datacard_dir}; text2workspace.py datacard_combined.txt -o combined.root --X-assign-flatParam-prior")


import gc
gc.collect() 

print("CreateDatacards ALL DONE!!!!")

"""
{multipdf_}bkg_{inclusive}_norm rateParam {inclusive} background 1. [{p_u:.4f},{p_o:.4f}]
{multipdf_}bkg_{inclusive}_norm flatParam

alpha_{inclusive}      param   {slopeval:.4f} {slopeerr:.4f}
bkg_{inclusive}        flatParam
bkg_{inclusive}        rateParam                 {inclusive}        background      1.
alpha_{inclusive}      param   {slopeval:.4f} {slopeerr:.4f}
bkg_{inclusive}_norm rateParam {inclusive} background 1. [0.99,1.01]
bkg_{inclusive}_norm flatParam

"""
