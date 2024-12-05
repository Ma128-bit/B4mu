from ROOT import gROOT, TH1F, RooDataHist, RooArgSet, RooExponential, RooRealVar, TChain, gDirectory, RooFit, kFALSE
gROOT.SetBatch(True)
import sys
sys.path.append('../Utilities_organized/')
import os, subprocess, argparse, draw_utilities
import pandas as pd

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass

#var = ["cLP", "tKink", "segmComp", "fv_nC", "d0sig", "fv_dphi3D", "fv_d3Dsig", "mindca_iso", "trkRel", "d0sig_max", "MVASoft1", "MVASoft2","Ptmu3", "fv_d3D", "cos(fv_dphi3D)"]
#var = ["Eta_tripl"]
#var = ["Vx1", "Vy1", "Vz1", "Vx2", "Vy2", "Vz2"]

var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_PV_SV", "Quadruplet_Eta","Quadruplet_Pt"]
invmass_SB = "(RefittedSV_Mass<5.25 || RefittedSV_Mass>5.55)"
invmass_peak = "(RefittedSV_Mass<5.55 && RefittedSV_Mass>5.25)"
binning_mass = "(65, 5.0, 6.0)"

binning_dict = {
    "vtx_prob": "(50,0.0,1.0)",
    "mu1_pfreliso03": "(50, 0, 10)",
    "mu2_pfreliso03": "(50, 0, 10)",
    "FlightDistBS_SV_Significance": "(50, 0, 400)",
    "mu1_bs_dxy_sig": "(50, -30, 30)",
    "mu2_bs_dxy_sig": "(50, -30, 30",
    "mu3_bs_dxy_sig": "(50, -30, 30)",
    "mu4_bs_dxy_sig": "(50, -30, 30)",
    "Cos2d_PV_SV": "(50, 0.95, 1)",
    "Quadruplet_Eta": "(50, -2.5, 2.5)",
    "Quadruplet_Pt": "(50, 10, 100)"
}

log_dict = {
    "vtx_prob": False,
    "mu1_pfreliso03": True,
    "mu2_pfreliso03": True,
    "FlightDistBS_SV_Significance": True,
    "mu1_bs_dxy_sig": True,
    "mu2_bs_dxy_sig": True,
    "mu3_bs_dxy_sig": True,
    "mu4_bs_dxy_sig": True,
    "Cos2d_PV_SV": True,
    "Quadruplet_Eta": False,
    "Quadruplet_Pt": False

}

def fit_bkg(data):
    data.Draw("RefittedSV_Mass>>h1(65, 5.05, 6.0)", "(isMC==0 &&" + invmass_SB+")")
    h1 = TH1F(gDirectory.Get("h1"))

    x = RooRealVar("x", "2mu+1trk inv. mass (GeV)", 5.0, 6.0)
    x.setBins(65)
    datahist = RooDataHist("datahist", h1.GetTitle(), RooArgSet(x), RooFit.Import(h1, kFALSE))

    x.setRange("R1", 5.05, 5.25)
    x.setRange("R3", 5.55, 6.0)
    x.setRange("R2", 5.25, 5.55)

    gamma = RooRealVar("#Gamma", "Gamma", -1, -2.0, -1e-2)
    exp_bkg = RooExponential("exp_bkg", "exp_bkg", x, gamma)
    exp_bkg.fitTo(datahist, RooFit.Range("R1,R3"))
    fsigregion_bkg = exp_bkg.createIntegral(x, RooFit.NormSet(x), RooFit.Range("R2"))
    fbkgregion_bkg = exp_bkg.createIntegral(x, RooFit.NormSet(x), RooFit.Range("R1,R3"))
    print(fsigregion_bkg, fbkgregion_bkg)
    print(h1.GetEntries())
    return h1.GetEntries()*fsigregion_bkg.getVal()/fbkgregion_bkg.getVal()


def control_plots(file_name, year, type):
    if not os.path.exists("Control_Plots"):
        subprocess.run(["mkdir", "Control_Plots"])
    
    # Data ALL
    data = TChain("FinalTree")
    data.Add(file_name)
    if(type=="diff"):
        scale = fit_bkg(data)
        print(scale)
    
    for k in range(len(var)):
        varname = var[k]
        logy = log_dict[varname]
        s = str(k)
        binning = binning_dict[varname]
        legend_label = ""
        if(type=="diff"):
            legend_label = "SB subtracted"
            data.Draw(varname + ">>hdata_bkg" + s+ binning, "(isMC==0 &&" + invmass_SB+")")
            data.Draw(varname + ">>hdata_sig" + s + binning, "(isMC==0 &&" + invmass_peak+")")
            hdata_bkg = TH1F(gDirectory.Get("hdata_bkg" + s))
            hdata_sig = TH1F(gDirectory.Get("hdata_sig" + s))
        
            data.Draw(varname + ">>hMC_sig" + s + binning, "(isMC!=0 &&" +invmass_peak+")")
            hMC_sig = TH1F(gDirectory.Get("hMC_sig" + s))
        
            # Scaling the SB distribution to the number of background events in 1.93,2.01
            normSB = hdata_bkg.GetEntries()
            hdata_bkg.Scale(scale / normSB)
            #print("Entries in hdata_sig before SB subtraction:", hdata_sig.GetEntries())
            hdata_sig.Add(hdata_bkg, -1) 
        
        if(type=="sPlot"):
            legend_label = "sWeighted"
            data.Draw(varname + ">>hdata_sig" + s+ binning, "nsigBs_sw*(isMC==0)")
            hdata_sig = TH1F(gDirectory.Get("hdata_sig" + s))
            data.Draw(varname + ">>hMC_sig" + s + binning, "weight*nsigBs_sw*(isMC>0)")
            hMC_sig = TH1F(gDirectory.Get("hMC_sig" + s))
            
        # Rescaling
        hMC_sig.Scale(1 / hMC_sig.Integral())
        hdata_sig.Scale(1 / hdata_sig.Integral())

        canvas = ROOTDrawer(SetGridx = True, SetLogY=logy)
        canvas.HaddTH1(hMC_sig, Color=4, SetXName=varname, SetYName="a.u.", Fill=True, label="MC BsJPsiPhi", FillStyle = 3004)
        
        canvas.HaddTH1(hdata_sig, Color=1, SetXName=varname, SetYName="a.u.", Fill=False, label="data ("+legend_label+")", DrawOpt="PE1")
        
        h_x_ratio = hdata_sig.Clone()
        h_x_ratio.Sumw2()
        h_x_ratio.Divide(hMC_sig)
        if logy ==True:
            maxim=5
        else:
            maxim=1.2
        canvas.HaddTH1(h_x_ratio, Color=1, SetXName=varname, SetYName="ratio data/MC", pull=True, DrawOpt="pe", MarkerStyle=68, YRange = [0.001,maxim])
        canvas.DefTLine(Color=2, Orientation=1, Y=1., pull=True)
        canvas.HaddPull(SetGridx = True, YRange = [0, 2])
        canvas.MakeLegend()
        canvas.Save("Control_Plots/" + varname + "_"+year+"_"+type+".png", era=int(year), extra="Preliminary")

        h_x_ratio.Delete();
        if(type=="diff"):
            hdata_bkg.Delete();
        hdata_sig.Delete();
        hMC_sig.Delete(); 

if __name__ == "__main__": 
    parser = argparse.ArgumentParser(description="--plots for control plots")
    parser.add_argument("--file", type=str, help="file name")
    parser.add_argument("--year", type=str, help="year (2022 or 2023)")
    parser.add_argument("--type", type=str, help="sPlot or diff")
    args = parser.parse_args()
    file = args.file
    year = args.year
    type = args.type
    control_plots(file, year, type)
