from ROOT import gROOT, TCanvas, TChain, EnableImplicitMT, RooRealVar, RooArgSet, RooDataSet, RooJohnson, RooFit, RDataFrame, gInterpreter, RooExtendPdf, TFile
from ROOT import RooWorkspace, RooArgList, RooAddPdf, RooExponential
import argparse, json

gROOT.SetBatch(True)
EnableImplicitMT()

gInterpreter.Declare("""
    double new_weight(int isMC, double weight, double w_Bs, double w_Bd){
        if(isMC==1) return w_Bs*weight;
        if(isMC==2) return w_Bd*weight;
        else return 1;
    }
""")

def load_info_from_json(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    return json_file["TreeName"], json_file["Mass_var"], json_file["MC_id"], json_file["weight"], json_file["Branches4sel"]

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

def plotMC(model, dataset, name="test.png"):
    can = TCanvas()
    can.SetCanvasSize(1000,800)
    plot = mass.frame()
    plot.SetTitle("Plot of "+name)
    dataset.plotOn(plot, RooFit.Range("sig"))
    model.paramOn(plot, RooFit.Layout(0.6, 0.9, 0.9))
    model.plotOn(plot, RooFit.Range("sig"), RooFit.LineColor(2) )
    plot.Draw()
    can.Update()
    can.SaveAs(name)
    del can
    del plot

def plot(modelB, modelS, datasetB, datasetS, name="test.png"):
    can = TCanvas()
    can.SetCanvasSize(1000,800)
    plot = mass.frame()
    plot.SetTitle("Plot of "+name)
    datasetB.plotOn(plot, binning, RooFit.MarkerColor(0), RooFit.LineColor(0) )
    modelB.plotOn( plot, RooFit.NormRange(fit_range), RooFit.Range(fit_range), RooFit.LineColor(4))
    datasetS.plotOn(plot, RooFit.Range("sig"), RooFit.MarkerColor(0), RooFit.LineColor(0) )
    modelS.plotOn(plot, RooFit.Range("sig"), RooFit.LineColor(2) )
    datasetB.plotOn( plot, RooFit.CutRange(fit_range), binning )
    plot.Draw()
    can.Update()
    can.SaveAs(name)
    del can
    del plot

if __name__ == "__main__":
    # Import info from user
    parser = argparse.ArgumentParser(description="config and root file")
    parser.add_argument("--config", type=str, help="config name")
    #parser.add_argument("--root_file", type=str, help="config name")
    args = parser.parse_args()
    configfile = args.config
    #root_file = args.root_file
    blinded = True
    root_file = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/XGBoost/BDT_results/_20240604-141204/TrainedDataset_20240604.root"

    tree_name, mass_name, mcid_name, weight, branches = load_info_from_json(configfile)

    BRBs = "1.0e-9"
    BRBd = "0"
    rdf = RDataFrame(tree_name, root_file)
    rdf = rdf.Define("weight_BR", "new_weight("+mcid_name+","+weight+","+BRBs+","+BRBd+")")
    rdf.Snapshot(tree_name, "RootFiles/Dataset.root")
    del rdf

    tree = TChain(tree_name)
    tree.AddFile("RootFiles/Dataset.root")  

    mass = RooRealVar(mass_name, mass_name, 4.0, 7.0)
    roomc = RooRealVar(mcid_name, mcid_name, 0, 2)
    weight_b = RooRealVar("weight_BR", "weight_BR", 0. ,5.0e+6)
    real_vars = []
    for branch in branches.keys():
        real_vars.append(RooRealVar(branch, branch, branches[branch][0], branches[branch][1]))

    variables = RooArgSet(mass)
    variables.add(roomc)
    variables.add(weight_b)
    for var in real_vars:
        variables.add(var)

    binning = RooFit.Binning(60, 4.,7.)
    mass.setRange("loSB", 4., 5. )
    mass.setRange("hiSB", 5.5, 7. )
    mass.setRange("sig", 5., 5.5 )
    mass.setRange("full", 4., 7. )
    
    fit_range = "loSB,hiSB"
    
    categories = category_sel(configfile)

    for cat in categories.keys():
        # Load DataSets
        data = RooDataSet('data', 'data', tree, variables, "isMC==0 && "+categories[cat])
        MCBs = RooDataSet('MCBs', 'MCBs', tree, variables, "isMC==1 &&"+categories[cat], "weight_BR")
        MCBd = RooDataSet('MCBd', 'MCBd', tree, variables, "isMC==2 && "+categories[cat], "weight_BR")
        MC_all = RooDataSet('MC_all', 'MC_all', tree, variables, "(isMC==2 || isMC==1) && "+categories[cat], "weight_BR")

        # Signals Models and Fits
        muBs = RooRealVar("muBs", "muBs", 5.37, 4.1, 6.80);
        lambdaBs = RooRealVar("lambdaBs", "lambdaBs", 0.04, 0.001, 1.)
        gammaBs = RooRealVar("gammaBs", "gammaBs", 0.3, -6.0, 6.0)
        deltaBs = RooRealVar("deltaBs", "deltaBs", 1.0, 0.01, 5.0)
        JS_Bs = RooJohnson("JS_Bs", "JS_Bs", mass, muBs, lambdaBs, gammaBs, deltaBs)

        eventsBs = RooRealVar("eventsBs", "eventsBs", 1.0, 0.0, 10.0)
        model_Bs = RooExtendPdf("model_Bs", "model_Bs", JS_Bs, eventsBs)
        if(BRBs!="0"):
            model_Bs.fitTo(MCBs, RooFit.SumW2Error(True), RooFit.PrintLevel(-1))
        else:
            model_Bs.setVal(0)
            model_Bs.setConstant(True)
        

        muBd = RooRealVar("muBd", "muBd", 5.37, 4.1, 6.80);
        lambdaBd = RooRealVar("lambdaBd", "lambdaBd", 0.04, 0.001, 1.)
        gammaBd = RooRealVar("gammaBd", "gammaBd", 0.3, -6.0, 6.0)
        deltaBd = RooRealVar("deltaBd", "deltaBd", 1.0, 0.01, 5.0)
        JS_Bd = RooJohnson("JS_Bd", "JS_Bd", mass, muBd, lambdaBd, gammaBd, deltaBd)

        eventsBd = RooRealVar("eventsBd", "eventsBd", 1.0, 0.0, 10.0)
        model_Bd = RooExtendPdf("model_Bd", "model_Bd", JS_Bd, eventsBd)
        if(BRBd!="0"):
            model_Bd.fitTo(MCBd, RooFit.SumW2Error(True), RooFit.PrintLevel(-1))
        else:
            eventsBd.setVal(0)
            eventsBd.setConstant(True)

        # Save Fit results/plots
        bs_nevt = eventsBs.getVal()
        bd_nevt = eventsBd.getVal()
        print("Bs events: ", bs_nevt)
        print("Bd events: ", bd_nevt)

        plotMC(model_Bs, MCBs, name="Plots/Fit_"+cat+"_MC_Bs.png")
        plotMC(model_Bd, MCBd, name="Plots/Fit_"+cat+"_MC_Bd.png")

        ratio = RooRealVar("ratio", "ratio", bs_nevt/(bs_nevt + bd_nevt))
        sig_model = RooAddPdf("sig_model", "sig_model", RooArgList(JS_Bs,  JS_Bd), ratio)
        #sig_model.fitTo(MC_all, RooFit.SumW2Error(True), RooFit.PrintLevel(-1))

        # Bkg Model and Fit:
        alpha = RooRealVar("#alpha", "alpha", -0.9, -10, 10)
        exp = RooExponential("exp", "exp", mass, alpha)
        eventsbkg = RooRealVar("eventsbkg", "eventsbkg", 10.0, 0.0, 200.0)
        exp_model = RooExtendPdf("exp_model", "exp_model", exp, eventsbkg)
        exp_model.fitTo(data, RooFit.Range(fit_range),  RooFit.PrintLevel(-1))
        print("Bkg events: ", eventsbkg.getVal())

        # Save Plot:
        plotData(exp_model, data, name="Plots/Fit_"+cat+"_data.png")
        plot(exp_model, sig_model, data, MC_all, name="Plots/Fit_"+cat+"_dataMC.png")

        # Create workspace
        output = TFile.Open("Workspaces/workspace_"+cat+".root","recreate")
        print("Creating workspace")
        w = RooWorkspace('w')

        w.factory(mass_name+"[%f, %f]" % (4, 7))
        w.factory("Exponential::bkg("+mass_name+", alpha[%f])" % (alpha.getVal()))

        w.factory("muBs[%f]" % muBs.getVal())
        w.factory("lambdaBs[%f,%f,%f]" % (lambdaBs.getVal(), lambdaBs.getVal()-0.001, lambdaBs.getVal()+0.001))
        w.factory("gammaBs[%f,%f,%f]" % (gammaBs.getVal(), gammaBs.getVal()-0.001, gammaBs.getVal()+0.001))
        w.factory("deltaBs[%f,%f,%f]" % (deltaBs.getVal(), deltaBs.getVal()-0.001, deltaBs.getVal()+0.001))

        w.factory("muBd[%f]" % muBd.getVal())
        w.factory("lambdaBd[%f,%f,%f]" % (lambdaBd.getVal(), lambdaBd.getVal()-0.001, lambdaBd.getVal()+0.001))
        w.factory("gammaBd[%f,%f,%f]" % (gammaBd.getVal(), gammaBd.getVal()-0.001, gammaBd.getVal()+0.001))
        w.factory("deltaBd[%f,%f,%f]" % (deltaBd.getVal(), deltaBd.getVal()-0.001, deltaBd.getVal()+0.001))

        if(BRBd=="0"):
            w.factory("RooJohnson::sig("+mass_name+", muBs, lambdaBs, gammaBs, deltaBs)")
        elif(BRBs=="0"):
            w.factory("RooJohnson::sig("+mass_name+", muBd, lambdaBd, gammaBd, deltaBd)")
        else:
            w.factory("ratio[%f]" % ratio.getVal())
            w.factory("RooJohnson::JS_Bs("+mass_name+", muBs, lambdaBs, gammaBs, deltaBs)")
            w.factory("RooJohnson::JS_Bd("+mass_name+", muBd, lambdaBd, gammaBd, deltaBd)")
            w.factory("RooAddPdf::sig(JS_Bs, JS_Bd, ratio)")

        it = w.allVars().createIterator()
        all_vars = [it.Next() for _ in range(w.allVars().getSize())]
        for var in all_vars:
            if var.GetName() in ["muBs", "lambdaBs", "gammaBs", "deltaBs", "muBd", "lambdaBd", "gammaBd", "deltaBd", "ratio"]:
                var.setConstant(True)

        reducedData = data.reduce(RooArgSet(mass))
        reducedData.SetName("data_obs")
        getattr(w,'import')(reducedData)
        w.Print()
        w.Write()
        output.Close()

        # dump the text datacard
        with open("Datacards/datacard"+cat+".txt", 'w') as card:
            card.write(
            '''
imax 1 number of bins
jmax * number of processes minus 1
kmax * number of nuisance parameters
--------------------------------------------------------------------------------
shapes background    {inclusive}       ../Workspaces/workspace_{inclusive}.root w:bkg
shapes signal        {inclusive}       ../Workspaces/workspace_{inclusive}.root w:sig
shapes data_obs      {inclusive}       ../Workspaces/workspace_{inclusive}.root w:data_obs
--------------------------------------------------------------------------------
bin               {inclusive}
observation       {obs:d}
--------------------------------------------------------------------------------
bin                                     {inclusive}           {inclusive}
process                                 signal              background
process                                 0                   1
rate                                    {signal:.4f}        {bkg:.4f}
--------------------------------------------------------------------------------
lumi          lnN                       1.025               -   
--------------------------------------------------------------------------------
bkg        rateParam                 {inclusive}        background      1.
alpha      param   {slopeval:.4f} {slopeerr:.4f}
            '''.format(
                    inclusive = cat,
                    obs      = data.numEntries() if blinded==False else -1, # number of observed events
                    signal   = bs_nevt + bd_nevt, # number of EXPECTED signal events, INCLUDES the a priori normalisation. Combine fit results will be in terms of signal strength relative to this inistial normalisation
                    bkg      = eventsbkg.getVal(), # number of expected background events **over the full mass range** using the exponential funciton fitted in the sidebands 
                    slopeval = alpha.getVal(), 
                    slopeerr = alpha.getError(),
                    )
        )
        del w
        del output
        del exp_model
        del sig_model
        del model_Bd
        del model_Bs
        del data
        del MCBs
        del MCBd
"""
unc_1         lnN                       1.05                -   
unc_2         lnN                       1.2                 -   
--------------------------------------------------------------------------------
bkg        rateParam                 {inclusive}        background      1.
alpha      param   {slopeval:.4f} {slopeerr:.4f}
"""