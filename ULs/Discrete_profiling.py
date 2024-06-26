from ROOT import gROOT, TCanvas, TChain, EnableImplicitMT, RooRealVar, RooArgSet, RooDataSet, RooGenericPdf, RooFit, TFile
from ROOT import RooWorkspace, RooArgList, RooAddPdf, TLegend, RooChi2Var, RooAbsData, TMath, RooCategory, RooMultiPdf
import argparse, json, os

gROOT.SetBatch(True)
EnableImplicitMT()

def load_info_from_json(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    return json_file["TreeName"], json_file["Mass_var"], json_file["MC_id"], json_file["weight"], json_file["Branches4sel"], json_file["Unblind"]

def category_sel(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    categories_list = json_file["categories"]
    category = {cat: "" for cat in categories_list}
    for c in categories_list:
        category[c] = json_file[c]
    print(category)
    return category

if __name__ == "__main__":
    log = open("discrate_profiling.log", "w")
    # Import info from user
    parser = argparse.ArgumentParser(description="config and root file")
    parser.add_argument("--config", type=str, help="config name")
    parser.add_argument("--worst_fit", type=bool, default=False, help="config name")
    #parser.add_argument("--root_file", type=str, help="config name")
    args = parser.parse_args()
    configfile = args.config
    worst_fit = args.worst_fit
    #root_file = args.root_file
    root_file = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/XGBoost/BDT_results/_20240604-141204/TrainedDataset_20240604.root"

    tree_name, mass_name, mcid_name, weight, branches, isUnblind = load_info_from_json(configfile)

    tree = TChain(tree_name)
    tree.AddFile(root_file)  

    mass = RooRealVar(mass_name, mass_name, 4.0, 7.0)
    roomc = RooRealVar(mcid_name, mcid_name, 0, 2)
    real_vars = []
    for branch in branches.keys():
        real_vars.append(RooRealVar(branch, branch, branches[branch][0], branches[branch][1]))

    variables = RooArgSet(mass)
    variables.add(roomc)
    for var in real_vars:
        variables.add(var)

    binning = RooFit.Binning(60, 4.,7.)
    mass.setRange("loSB", 4., 5.079 )
    mass.setRange("hiSB", 5.566, 7. )
    mass.setRange("sig", 5.079 , 5.566 )
    mass.setRange("full", 4., 7. )
    
    fit_range = "loSB,hiSB"
    
    categories = category_sel(configfile)

    for cat in categories.keys():
        log.write("I'm in %s \n" % cat)
        # Load DataSets
        data = RooDataSet('data', 'data', tree, variables, "isMC==0 && "+categories[cat])
        if isUnblind== False:
            data = data.reduce(RooArgSet(mass),mass_name+">5.566 || "+mass_name+"<5.079" )
        else:
            data = data.reduce(RooArgSet(mass))

        hist = data.binnedClone('histo_'+cat)
        max_order = 4
        max_order = min(max_order, int(hist.sumEntries())-2)
        log.write("Max order: %d \n" % max_order)

        # Create workspace
        pdfs = RooWorkspace('pdfs_'+cat)
        print("Creating workspace")
        getattr(pdfs, 'import')(mass)

        c_powerlaw = RooRealVar("c_PowerLaw_{}".format(cat), "", 1, -100, 100)
        powerlaw = RooGenericPdf("PowerLaw_{}".format(cat), "TMath::Power(@0, @1)", RooArgList(mass, c_powerlaw))
        getattr(pdfs, 'import')(powerlaw)

        pdfs.factory("Exponential::Exponential_{C}({M}, slope_{C}[0, -1000, 100])".format(M=mass_name, C=cat))

        # Bernstein: oder n has n+1 coefficients (starts from constant)
        for i in range(1, max_order+1):
            c_bernstein = '{'+','.join(['c_Bernstein{}{}_{}[.1, 0.0, 1.0]'   .format(i, j, cat) for j in range(i+1)])+'}'
            pdfs.factory('Bernstein::Bernstein{}_{}({}, {})'.format(i, cat, mass_name, c_bernstein))

        # Chebychev: order n has n coefficients (starts from linear)
        for i in range(max_order-1):
            c_chebychev = '{'+','.join(['c_Chebychev{}{}_{}[-10.0, 10.0]'.format(i+1, j, cat) for j in range(i+1)])+'}'
            pdfs.factory('Chebychev::Chebychev{}_{}({}, {})'.format(i+1, cat, mass_name, c_chebychev)) 

        # Polynomial: order n has n coefficients (starts from constant)
        for i in range(1, max_order):
            c_polynomial = '{'+','.join(['c_Polynomial{}{}_{}[-10, 10]'.format(i+1, j, cat) for j in range(i+1)])+'}'
            pdfs.factory('Polynomial::Polynomial{}_{}({}, {})'.format(i, cat, mass_name, c_polynomial)) 

        frame = mass.frame()
        frame.SetTitle(cat)
        data.plotOn(frame, binning)
        
        envelope = RooArgList("envelope")
        
        can = TCanvas()
        leg = TLegend(0.5, 0.6, 0.9, 0.9)
        
        gofmax  = 0
        gofmin = 1000
        bestfit = None
        worstfit = None
        families = ['Bernstein', 'Polynomial', 'Exponential', 'PowerLaw']
        allpdfs_list = RooArgList(pdfs.allPdfs())
        allpdfs_list = [allpdfs_list.at(j) for j in range(allpdfs_list.getSize())]

        converged = 0
        norm = RooRealVar("multipdf_bkg_{}_norm".format(cat), "", 0, 1e+6)
        for j, fam in enumerate(families):
            log.write("> I'm in %s \n" % fam)
            pdf_list = [p for p in allpdfs_list if p.GetName().startswith(fam)]
            mnlls    = []
            for i, pdf in enumerate(pdf_list):
                log.write(">> Pdf: %s \n" % pdf.GetName())
                ext_pdf = RooAddPdf(pdf.GetName()+"_ext", "", RooArgList(pdf), RooArgList(norm))

                if (isUnblind):
                    results = ext_pdf.fitTo(data,  RooFit.Save(True), RooFit.Extended(True))
                else:
                    results = ext_pdf.fitTo(data,  RooFit.Save(True), RooFit.Range(fit_range), RooFit.Extended(True))
                chi2 = RooChi2Var("chi2"+pdf.GetName(), "", ext_pdf, hist, RooFit.DataError(RooAbsData.Expected))
                mnll = results.minNll()+0.5*(i)

                gof_prob = TMath.Prob(chi2.getVal(), int(hist.sumEntries())-pdf.getParameters(data).selectByAttrib("Constant", False).getSize())
                fis_prob = TMath.Prob(2.*(mnlls[-1]-mnll), i-converged) if len(mnlls) else 0
                if results.covQual()==3:
                    mnlls.append(mnll)
                    converged = i

                log.write(">>> %s chi2 %f \n" % (pdf.GetName(), chi2.getVal()) )
                log.write(">>> results.covQual(): %f \n" % results.covQual())
                log.write(">>> fis_prob: %f \n" % fis_prob)
                log.write(">>> gof_prob: %f \n" % gof_prob)

                if (fis_prob < 0.1 and results.covQual()==3) or ("Exponential" in pdf.GetName()):
                #if (fis_prob < 0.1) or ("Exponential" in pdf.GetName()):
                    if gof_prob > gofmax:
                        gofmax = gof_prob
                        bestfit = pdf.GetName()
                    if gof_prob < gofmin:
                        gofmin = gof_prob
                        worstfit = pdf.GetName()

                    envelope.add(pdf)

                    print(">>>", pdf.GetName(), " added to envelope")
                    print("gof_prob:", gof_prob, " fis_prob:", fis_prob, " mnll: ",mnll)
                    ext_pdf.plotOn(frame, RooFit.LineColor(envelope.getSize()), RooFit.Name(pdf.GetName()),
                                RooFit.NormRange('full' if isUnblind else 'loSB,hiSB'),
                                RooFit.Range('full' if isUnblind else 'loSB,hiSB'))
                elif fis_prob >= 0.1:
                    break
                del chi2 
        for pdf in [envelope.at(i) for i in range(envelope.getSize())]:
            if worst_fit==True:
                leg.AddEntry(frame.findObject(pdf.GetName()), pdf.GetName()+" (worstfit)" if worstfit==pdf.GetName() else pdf.GetName(), "l")
            else:
                leg.AddEntry(frame.findObject(pdf.GetName()), pdf.GetName()+" (bestfit)" if bestfit==pdf.GetName() else pdf.GetName(), "l")

        
        frame.GetYaxis().SetLimits(0.0, 1000)
        frame.Draw()
        leg.Draw("SAME")
        can.Update()

        if not os.path.exists("MultiPdfWorkspaces"):
            os.makedirs("MultiPdfWorkspaces")
        if not os.path.exists("MultiPdfPlots"):
            os.makedirs("MultiPdfPlots")

        if isUnblind:
            can.SaveAs("MultiPdfPlots/multipdf_"+cat+"_unblinded.png")
        else:
            can.SaveAs("MultiPdfPlots/multipdf_"+cat+".png")

        roocat = RooCategory("multipdf_bkg_cat_{}".format(cat), "")
        multipdf = RooMultiPdf("multipdf_bkg_{}".format(cat), "", roocat, envelope)
        #indexing Expo in the multipdf. Change line below to switch to "bestfit"
        #roocat.setIndex([envelope.at(i).GetName() for i in range(envelope.getSize())].index('Exponential_{}'.format(cat)))
        roocat.setIndex([envelope.at(i).GetName() for i in range(envelope.getSize())].index(bestfit))

        output = TFile.Open("MultiPdfWorkspaces/workspace_"+cat+".root","recreate")
        print("Creating workspace")
        w = RooWorkspace('w')
        getattr(w, 'import')(envelope)
        getattr(w, 'import')(multipdf)
        getattr(w, 'import')(norm) 
        w.Print()
        w.Write()
        output.Close()
        
        del w
        del output
        del roocat
        del multipdf
        del hist
        del pdfs
        del data

    log.close()
