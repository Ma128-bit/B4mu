import os, subprocess, argparse
import cmsstyle as CMS
import uproot
import numpy as np
import matplotlib.pyplot as plt
import mplhep as hep
print("Imported modules")

var = ["vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", "FlightDistBS_SV_Significance", "mu1_bs_dxy_sig", "mu2_bs_dxy_sig", "mu3_bs_dxy_sig", "mu4_bs_dxy_sig", "Cos2d_BS_SV", "Quadruplet_Eta","Quadruplet_Pt", "bdt_cv"]

binning_dict = {
    "vtx_prob": "(50,0.01,1.0)",
    "mu1_pfreliso03": "(50,0,10)",
    "mu2_pfreliso03": "(50,0,10)",
    "MVASoft1": "(50,0.2,0.8)",
    "MVASoft2": "(50,0.2,0.8)",
    "FlightDistBS_SV_Significance": "(50,0,400)",
    "RefittedSV_Mass_reso": "(50,0.01,0.08)",
    "mu1_bs_dxy_sig": "(50,-100,100)",
    "mu2_bs_dxy_sig": "(50,-100,100)",
    "mu3_bs_dxy_sig": "(50,-75,75)",
    "mu4_bs_dxy_sig": "(50,-75,75)",
    "Cos2d_BS_SV": "(50,0.95,1)",
    "Quadruplet_Eta": "(50,-2.5,2.5)",
    "RefittedSV_Mass_eq": "(50,5.25,5.5)",
    "RefittedSV_Mass": "(50,5.2,5.6)",
    "Quadruplet_Pt": "(50,10,100)",
    "bdt_cv": "(50,0,1)",
    "Mu1_Eta": "(50,-2.5,2.5)",
    "Mu1_Pt": "(50,4, 50)",
    "PVCollection_Size": "(70,0,70)"
}

log_dict = {
    "vtx_prob": True,
    "mu1_pfreliso03": True,
    "mu2_pfreliso03": True,
    "MVASoft1": False,
    "MVASoft2": False,
    "FlightDistBS_SV_Significance": True,
    "RefittedSV_Mass_reso": False,
    "mu1_bs_dxy_sig": True,
    "mu2_bs_dxy_sig": True,
    "mu3_bs_dxy_sig": True,
    "mu4_bs_dxy_sig": True,
    "Cos2d_BS_SV": True,
    "Quadruplet_Eta": False,
    "RefittedSV_Mass_eq": False,
    "RefittedSV_Mass": False,
    "Quadruplet_Pt": False,
    "bdt_cv": True,
    "100*new_ct/2.998": True,
    "Mu1_Eta": False,
    "Mu1_Pt": False,
    "PVCollection_Size": False
}

x_name = {
    "vtx_prob": "Vertex Probability",
    "mu1_pfreliso03": "#mu_{1} PF relative iso.",
    "mu2_pfreliso03": "#mu_{2} PF relative iso.",
    "MVASoft1": "",
    "MVASoft2": "",
    "FlightDistBS_SV_Significance": "L_{xy}^{sig}",
    "RefittedSV_Mass_reso": "",
    "mu1_bs_dxy_sig": "#mu_{1} dxy_{sig}",
    "mu2_bs_dxy_sig": "#mu_{2} dxy_{sig}",
    "mu3_bs_dxy_sig": "#mu_{3} dxy_{sig}",
    "mu4_bs_dxy_sig": "#mu_{4} dxy_{sig}",
    "Cos2d_BS_SV": "cos(#theta)",
    "Quadruplet_Eta": "B |#eta|",
    "RefittedSV_Mass_eq": "4#mu mass", 
    "RefittedSV_Mass": "4#mu mass", 
    "Quadruplet_Pt": "B p_{T}",
    "bdt_cv": "BDT score",
    "100*new_ct/2.998": "",
    "Mu1_Eta": "#mu_{1} |#eta|",
    "Mu1_Pt": "#mu_{1} p_{T}",
    "PVCollection_Size": "N. PV"
}

lumi={
    "2022": 34.6,
    "2023": 27.8,
    "2022+2023": 62.4,
    "2022+2023+2024": 170.7
}

def plots(file_name, year):
    from ROOT import gROOT, TH1F, TChain, gDirectory, kRed, gPad, TLegend
    gROOT.SetBatch(True)
    if not os.path.exists("BDT_Plots"):
        subprocess.run(["mkdir", "BDT_Plots"])
    
    # Data ALL
    data = TChain("FinalTree")
    data.Add(file_name)
    
    for k in range(len(var)):
        varname = var[k]
        logy = log_dict[varname]
        s = str(k)
        binning = binning_dict[varname]
        numbers = binning.strip("()").split(",")
        numbers = [float(x) if x.isdigit() else float(x) for x in numbers]

        legend_label = "sWeighted"
        data.Draw(varname + ">>hdata_sig" + s+ binning, "isMC==0 & (RefittedSV_Mass<5.090 | RefittedSV_Mass>5.529)")
        hdata_sig = TH1F(gDirectory.Get("hdata_sig" + s))
        data.Draw(varname + ">>hMC_sig" + s + binning, "bdt_weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC==1)")
        hMC_sig = TH1F(gDirectory.Get("hMC_sig" + s))
        data.Draw(varname + ">>hMC2_sig" + s + binning, "bdt_weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC==2)")
        hMC2_sig = TH1F(gDirectory.Get("hMC2_sig" + s))

        # Rescaling
        hMC_sig.Scale(1 / hMC_sig.Integral(1,int(numbers[0])))
        hMC2_sig.Scale(1 / hMC2_sig.Integral(1,int(numbers[0])))
        hdata_sig.Scale(1 / hdata_sig.Integral(1,int(numbers[0])))

        CMS.SetExtraText("Preliminary")
        CMS.SetLumi(f"{year}, {lumi[year]}")
        CMS.SetEnergy(13.6)
        if logy:
            canvas = CMS.cmsCanvas("", numbers[1], numbers[2], 0.0001, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*5, x_name[varname], f"a.u.",  square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
        else:
            if varname!="Quadruplet_Eta":
                canvas = CMS.cmsCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.5, x_name[varname], f"a.u.", square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)
            else:
                canvas = CMS.cmsCanvas("", numbers[1], numbers[2], 0, max(hdata_sig.GetMaximum(),hMC_sig.GetMaximum())*1.5, x_name[varname], f"a.u.",  square=CMS.kSquare, iPos=11, extraSpace=0, scaleLumi=None)

        canvas.SetCanvasSize(1200,700)
        canvas.cd(1)
        if logy:
            gPad.SetLogy()
        hMC_sig.SetLineColor(kRed)
        hMC_sig.SetFillColor(kRed)
        hMC_sig.SetLineWidth(2)
        hMC_sig.SetFillStyle(3004)
        hMC2_sig.SetLineColor(4)
        hMC2_sig.SetFillColor(4)
        hMC2_sig.SetLineWidth(2)
        hMC2_sig.SetFillStyle(3005)
        hMC_sig.Draw("Hsame")
        hMC2_sig.Draw("Hsame")
        hdata_sig.SetLineColor(1)
        hdata_sig.SetLineWidth(2)
        hdata_sig.SetMarkerStyle(20)
        hdata_sig.SetMarkerSize(1.2)
        hdata_sig.Draw("samePE1")

        legend = TLegend(0.61, 0.7, 0.9, 0.9)
        legend.AddEntry(hdata_sig, "Data Sidebands", "lep") 
        legend.AddEntry(hMC_sig, "MC B^{0}_{s} #rightarrow 4#mu", "f")  
        legend.AddEntry(hMC2_sig, "MC B^{0} #rightarrow 4#mu", "f")  
        legend.SetBorderSize(0)       
        legend.SetFillStyle(0)    
        legend.Draw("same")


        canvas.Update()
        varname = varname.replace("*", "_")
        varname = varname.replace("/", "_")
        canvas.SaveAs("BDT_Plots/" + varname + "_" + year + ".pdf")
        canvas.Clear()

        hdata_sig.Delete();
        hMC_sig.Delete(); 

def plot_roc_curve(root_file, year, fold=6):
    from sklearn.metrics import roc_curve, auc
    with uproot.open(root_file) as f:
        tree = f["FinalTree"].arrays(library="pd")
        tree = tree[(tree["isMC"] > 0) | (tree["isMC"] == 0 & ((tree["RefittedSV_Mass"] < 5.090) | (tree["RefittedSV_Mass"] > 5.529)))]
        y_true_test = tree[tree["evt"]%10 == fold]["isMC"].values
        y_true_test[y_true_test > 0] = 1
        scores_test = tree[tree["evt"]%10 == fold]["bdt_fold1"].values
        weight_test = tree[tree["evt"]%10 == fold]["weight_pileUp"].values
        weight_test *= tree[tree["evt"]%10 == fold]["ctau_weight_central"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_0"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_1"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_2"].values

        y_true_train = tree[tree["evt"]%10 != fold]["isMC"].values
        y_true_train[y_true_train > 0] = 1
        scores_train = tree[tree["evt"]%10 != fold]["bdt_fold1"].values
        weight_train = tree[tree["evt"]%10 != fold]["weight_pileUp"].values
        weight_train *= tree[tree["evt"]%10 != fold]["ctau_weight_central"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_0"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_1"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_2"].values

        
    # Calcola la curva ROC
    fpr, tpr, _ = roc_curve(y_true_test, scores_test, sample_weight=weight_test)
    fpr2, tpr2, _ = roc_curve(y_true_train, scores_train, sample_weight=weight_train)
    roc_auc = auc(fpr, tpr)
    roc_auc2 = auc(fpr2, tpr2)
    
    # Plot della ROC Curve
    plt.figure(figsize=(12, 8))
    hep.style.use("CMS")
    plt.plot(fpr, tpr, color='blue', lw=2, label=f'ROC curve test (AUC = {roc_auc:.4f})')
    plt.plot(fpr2, tpr2, color='red', lw=2, label=f'ROC curve train (AUC = {roc_auc2:.4f})')
    plt.plot([0, 1], [0, 1], color='gray', linestyle='--')
    plt.xlim([0.0001, 1.0])
    plt.ylim([0, 1.05])
    plt.xscale('log')  
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.legend(loc='lower right')
    hep.cms.label("Preliminary", data=True, lumi=lumi[year], year=year)
    plt.grid()
    plt.savefig(f"BDT_Plots/roc{fold}.pdf")

def plot_bdt_score_fold(root_file, year, fold=6):
    from scipy.stats import ks_2samp
    with uproot.open(root_file) as f:
        tree = f["FinalTree"].arrays(library="pd")
        tree = tree[(tree["isMC"] > 0) | (tree["isMC"] == 0 & ((tree["RefittedSV_Mass"] < 5.090) | (tree["RefittedSV_Mass"] > 5.529)))]
        y_true_test = tree[tree["evt"]%10 == fold]["isMC"].values
        y_true_test[y_true_test > 0] = 1
        scores_test = tree[tree["evt"]%10 == fold]["bdt_fold1"].values
        y_true_train = tree[tree["evt"]%10 != fold]["isMC"].values
        y_true_train[y_true_train > 0] = 1
        scores_train = tree[tree["evt"]%10 != fold]["bdt_fold1"].values
        
        weight_test = tree[tree["evt"]%10 == fold]["weight_pileUp"].values
        weight_test *= tree[tree["evt"]%10 == fold]["ctau_weight_central"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_0"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_1"].values
        weight_test *= tree[tree["evt"]%10 == fold]["bdt_reweight_2"].values

        weight_train = tree[tree["evt"]%10 != fold]["weight_pileUp"].values
        weight_train *= tree[tree["evt"]%10 != fold]["ctau_weight_central"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_0"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_1"].values
        weight_train *= tree[tree["evt"]%10 != fold]["bdt_reweight_2"].values

        sig_bdt_score_test = scores_test[y_true_test == 1]
        sig_bdt_w_test = weight_test[y_true_test == 1]
        bkg_bdt_score_test = scores_test[y_true_test == 0]
        sig_bdt_score_train = scores_train[y_true_train == 1]
        sig_bdt_w_train = weight_train[y_true_train == 1]
        bkg_bdt_score_train = scores_train[y_true_train == 0]

    ks_signal = ks_2samp(sig_bdt_score_train ,sig_bdt_score_test)
    ks_bkg = ks_2samp(bkg_bdt_score_train,bkg_bdt_score_test)
            
    print("KS test (signal):", ks_signal)
    print("KS test (bkg):", ks_bkg)

    # Plot della distribuzione della probabilità della classe positiva
    counts_sig_test, bin_edges_sig_test = np.histogram(sig_bdt_score_test, bins=25, weights=sig_bdt_w_test, density=True)
    bin_widths_sig_test = np.diff(bin_edges_sig_test)
    yerr_sig_test = np.sum(counts_sig_test)*np.sqrt(counts_sig_test)/np.sum(sig_bdt_score_test)  
    #yerr_sig_test = np.sqrt(counts_sig_test)  # Fluttuazione di Poisson
    xerr_sig_test = np.diff(bin_edges_sig_test) / 2  # Mezzo della larghezza del bin
    bin_centers_sig_test = (bin_edges_sig_test[:-1] + bin_edges_sig_test[1:]) / 2

    counts_bkg_test, bin_edges_bkg_test = np.histogram(bkg_bdt_score_test, bins=25, density=True)
    bin_widths_bkg_test = np.diff(bin_edges_bkg_test)
    yerr_bkg_test = np.sum(counts_bkg_test)*np.sqrt(counts_bkg_test)/np.sum(bkg_bdt_score_test)   # Fluttuazione di Poisson
    xerr_bkg_test = np.diff(bin_edges_bkg_test) / 2  # Mezzo della larghezza del bin
    bin_centers_bkg_test = (bin_edges_bkg_test[:-1] + bin_edges_bkg_test[1:]) / 2

    plt.figure(figsize=(12, 8))
    hep.style.use("CMS")
    plt.hist(sig_bdt_score_train, weights=sig_bdt_w_train, bins=25, histtype='stepfilled', color = 'blue', alpha=0.5, edgecolor='blue', label='Signal Train', density=True)
    plt.hist(bkg_bdt_score_train, bins=25, histtype='step', color = 'red', edgecolor='red', hatch='/', label='Bkg Train', density=True)
    plt.errorbar(bin_centers_sig_test, counts_sig_test, xerr=xerr_sig_test, yerr=yerr_sig_test, fmt='o', color='blue', label='Signal Test')
    plt.errorbar(bin_centers_bkg_test, counts_bkg_test, xerr=xerr_bkg_test, yerr=yerr_bkg_test, fmt='o', color='red', label='Bkg Test')
    plt.xlabel('BDT score')
    plt.ylabel('a.u.')
    plt.legend(loc='upper left', bbox_to_anchor=(0.6, 1))
    ks_text = f"Kolmogorov-Smirnov p-value \nSignal: {(100*ks_signal.pvalue):.2f}% \nBkg: {(100*ks_bkg.pvalue):.2f}%"
    plt.text(0.15, 0.9, ks_text, transform=plt.gca().transAxes, ha='left', va='center', fontsize=15, 
        bbox=dict(facecolor='white'))
    plt.grid(True)
    plt.yscale('log')
    hep.cms.label("Preliminary", data=True, lumi=lumi[year], year=year)

    plt.savefig(f'BDT_Plots/kolmogorov_test_fold{fold}.pdf', dpi = 500)


if __name__ == "__main__": 
    print("Start!")
    parser = argparse.ArgumentParser(description="--plots for control plots")
    parser.add_argument("--file", type=str, help="file name")
    parser.add_argument("--year", type=str, help="year (2022 or 2023 or 2024)")
    args = parser.parse_args()
    file = args.file
    year = args.year
    #for i in range(9):
    #    plot_roc_curve(file, year, fold=i)
    plot_roc_curve(file, year, fold=0)
    plot_bdt_score_fold(file, year, fold=6)
    plots(file, year)
