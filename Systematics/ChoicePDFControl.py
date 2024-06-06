from ROOT import TChain, gROOT, gDirectory, kFALSE, kRed, kGreen, kDashed, TCanvas, TFile
from ROOT import RooFit, RooRealVar, RooArgList, RooAddPdf, RooWorkspace, RooDataHist, RooArgSet, RooDataSet, RooNumber
from ROOT import RooExponential, RooGaussian, RooChebychev, RooPolynomial, RooBernstein, RooGenericPdf, RooJohnson, RooVoigtian, RooCBShape

gROOT.SetBatch(True)
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import math, os, sys


def fit(id, mass, data, out_dir, bkg_pdf, sig_pdf, sig_pdf2=None):    
    print(id)
    nsig = RooRealVar("nsig", "Numero di segnali", 60, 10, 1000);
    nsig2 = RooRealVar("nsig2", "Numero di segnali 2", 60, 10, 1000);
    nbkg = RooRealVar("nbkg", "Numero di background", 10, 1, 1000);

    if(sig_pdf2 is None):
        model = RooAddPdf("model", "model", RooArgList(sig_pdf,  bkg_pdf), RooArgList(nsig, nbkg))
    else:
        model = RooAddPdf("model", "model", RooArgList(sig_pdf, sig_pdf2, bkg_pdf), RooArgList(nsig, nsig2, nbkg))

    #bkg_pdf.fitTo(data, RooFit.Range("left,right"),  RooFit.PrintLevel(-1))
    model.fitTo(data, RooFit.Range("all"), RooFit.PrintLevel(-1))

    xframe = mass.frame()
    xframe.SetTitle("Plot of "+ id)
    data.plotOn(xframe)
    if(sig_pdf2 is None):
        model.paramOn(xframe, RooFit.Parameters(RooArgSet(nsig, nbkg)), RooFit.Layout(0.6, 0.9, 0.9))
        model.plotOn(xframe, RooFit.Range("all"), RooFit.Components(RooArgSet(sig_pdf)), RooFit.LineColor(kRed), RooFit.LineStyle(kDashed))
    else:
        model.paramOn(xframe, RooFit.Parameters(RooArgSet(nsig, nsig2, nbkg)), RooFit.Layout(0.6, 0.9, 0.9))
        model.plotOn(xframe, RooFit.Range("all"), RooFit.Components(RooArgSet(sig_pdf, sig_pdf2)), RooFit.LineColor(kRed), RooFit.LineStyle(kDashed))
    model.plotOn(xframe, RooFit.Components(RooArgSet(bkg_pdf)), RooFit.LineColor(kGreen), RooFit.LineStyle(kDashed))
    model.plotOn(xframe)

    c1 = TCanvas("c1", "c1", 900, 900)
    xframe.Draw()
    c1.SaveAs(out_dir+"/Fit_"+id+".png", "png -dpi 600")
    c1.Clear()

    if(sig_pdf2 is None):
        out = nsig.getVal(), nsig.getError()
        del nsig
        del nbkg
        return out
    else:
        out = nsig.getVal()+nsig2.getVal(), math.sqrt((nsig.getError())**2 + (nsig2.getError())**2)
        del nsig
        del nbkg
        return out

if __name__ == "__main__":
    if len(sys.argv) > 1:
        year = sys.argv[1]
    else:
        year=""

    out_dir="ChoicePDFControl_zur"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
        
    filename = "../Utilities_organized/ROOTFiles_zur/AllControl"+year+".root"
    tree_name = "FinalTree"
    weight_name = "weight_pileUp"
    mass_name = "Quadruplet_Mass_eq"
    max_order = 4
    
    #open input file
    rootfile = TFile(filename, 'READ')
    tree = rootfile.Get(tree_name)
    
    mass = RooRealVar(mass_name, mass_name, 100, 5.0, 6.0, 'GeV')
    roomc = RooRealVar("isMC", "isMC", 0, 1)
    rooweight = RooRealVar(weight_name, weight_name, 0, 100)
        
    #set of variables
    variables = RooArgSet(mass)
    variables.add(roomc)
    variables.add(rooweight)
    
    mass.setRange('all', 5.0, 6.0)
    mass.setRange('left', 5.0, 5.25)
    mass.setRange('right', 5.45, 6.0)
    
    #take rooDataSet from tree
    data = RooDataSet('data', '', tree, variables, "isMC==0", weight_name)

    #define the set of bkg pdfs
    bkg_pdfs = RooWorkspace('bkg_pdfs')
    getattr(bkg_pdfs, 'import')(mass)

    c_powerlaw = RooRealVar("c_PowerLaw", "", 1, -100, 100)
    powerlaw = RooGenericPdf("PowerLaw", "TMath::Power(@0, @1)", RooArgList(mass, c_powerlaw))
    getattr(bkg_pdfs, 'import')(powerlaw)
    
    bkg_pdfs.factory("Exponential::Exponential({M}, slope[0, -1000, 100])".format(M=mass_name))

    # Bernstein: oder n has n+1 coefficients (starts from constant)
    for i in range(1, max_order+1):
        c_bernstein = '{'+','.join(['c_Bernstein{}{}[.1, 0.0, 1.0]'   .format(i, j) for j in range(i+1)])+'}'
        print(c_bernstein)
        bkg_pdfs.factory('Bernstein::Bernstein{}({}, {})'.format(i, mass_name, c_bernstein))
    
    # Chebychev: order n has n coefficients (starts from linear)
    for i in range(max_order):
        c_chebychev = '{'+','.join(['c_Chebychev{}{}[.1, 0.0, 10.0]'.format(i+1, j) for j in range(i+1)])+'}'
        bkg_pdfs.factory('Chebychev::Chebychev{}({}, {})'.format(i+1, mass_name, c_chebychev)) 
    
    # Polynomial: order n has n coefficients (starts from constant)
    for i in range(1, max_order):
        c_polynomial = '{'+','.join(['c_Polynomial{}{}[.1, -100, 100]'.format(i+1, j) for j in range(i+1)])+'}'
        bkg_pdfs.factory('Polynomial::Polynomial{}({}, {})'.format(i, mass_name, c_polynomial)) 

    bkgpdfs_list = RooArgList(bkg_pdfs.allPdfs())
    bkgpdfs_list = [bkgpdfs_list.at(j) for j in range(bkgpdfs_list.getSize())]
    bkgpdfs_names = [p.GetName() for p in bkgpdfs_list]

    #define the set of sig pdfs
    sig_pdfs = RooWorkspace('sig_pdfs')
    getattr(sig_pdfs, 'import')(mass)

    mean =RooRealVar("mean", "mean", 5.367, 5.1, 5.5);
    sigma = RooRealVar("sigma", "sigma", 0.02, 0.01, 0.06);
    Gaussian = RooGaussian("Gaussian", "Gaussian", mass, mean, sigma);
    #getattr(sig_pdfs, 'import')(Gaussian)

    mu = RooRealVar("mu", "mu", 5.46, 5.1, 5.80);
    lambd = RooRealVar("lambd", "lambd",0.1, 0.01, 1.0)
    gamm = RooRealVar("gamm", "gamm",  10., 3.0, 20.0)
    delta = RooRealVar("delta", "delta", 8.0, 3.0, 20.0)
    Johnson_pdf = RooJohnson("Johnson_pdf", "Johnson_pdf", mass, mu, lambd, gamm, delta)
    #getattr(sig_pdfs, 'import')(Johnson_pdf)

    meanCB = RooRealVar("meanCB", "meanCB", 5.367, 5.1, 5.5)
    sigmaCB1 = RooRealVar("sigmaCB1", "sigmaCB1", 0.001, 1.0)
    alpha1 = RooRealVar("alpha1", "alpha1", 1.5 , 0.8, 4.0) #RooNumber.infinity()
    nSigma1 = RooRealVar("n1", "n1", 150, 100, 200)
    CBShape = RooCBShape("CBShape", "CBShape", mass, meanCB, sigmaCB1, alpha1, nSigma1)

    getattr(sig_pdfs, 'import')(Johnson_pdf)
    getattr(sig_pdfs, 'import')(CBShape)
    getattr(sig_pdfs, 'import')(Gaussian)
    

    sigpdfs_list = RooArgList(sig_pdfs.allPdfs())
    sigpdfs_list = [sigpdfs_list.at(j) for j in range(sigpdfs_list.getSize())]
    sigpdfs_names = [p.GetName() for p in sigpdfs_list]

    signals = np.zeros((len(bkgpdfs_names), len(sigpdfs_names))) 
    annotazioni = np.empty(signals.shape, dtype=object)
    for j in range(len(sigpdfs_names)):
        for i in range(len(bkgpdfs_names)):
            n, nerr = fit(bkgpdfs_names[i]+"_"+sigpdfs_names[j], mass, data, out_dir, bkgpdfs_list[i], sigpdfs_list[j])
            signals[i, j]=n
            annotazioni[i, j] = f"{n:.2f} ± {nerr:.2f}"

    plt.figure(figsize=(6, 15))
    print("Max_sig: ", signals.max())
    print("Min_sig: ", signals.min())
    sns.heatmap(signals, annot=annotazioni, fmt='', cmap='coolwarm', linewidths=0.5, linecolor='white', cbar=True, xticklabels=sigpdfs_names, yticklabels=bkgpdfs_names)
    plt.title('')
    plt.savefig(out_dir+'/signals.png', format='png', dpi=300)
        
    