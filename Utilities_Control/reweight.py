from ROOT import gROOT, TH1F, TChain, gDirectory, TFile,  TObject
gROOT.SetBatch(True)
import sys, os, subprocess, argparse
from array import array
import shutil

binning_dict = {
    "FlightDistBS_SV_Significance": "(1200,0,600)",
    "mu2_bs_dxy_sig": "(100,-100,100)",
    "Quadruplet_Eta": "(200,-2.5,2.5)",
    "vtx_prob": "(200, 0.01, 1)",
    "bdt": "(50,0,1)"
}

 
def save_histograms(histograms, output_file):
    output = TFile(output_file, "RECREATE")
    for histogram in histograms:
        histogram.Write()
    output.Close()

def load_histograms(file):
    histograms = []
    histogram_names = []
    for key in file.GetListOfKeys():
        obj = key.ReadObj()
        if isinstance(obj, TH1F):
            histograms.append(obj.Clone())
            histogram_names.append(obj.GetName())
    return histograms, histogram_names
    
def reweight(file_name, varname, name):
    print("ADDW NAME: ", name)
    if not os.path.exists("Weight"):
        subprocess.run(["mkdir", "Weight"])
    
    # Data ALL
    data = TChain("FinalTree")
    data.Add(file_name)

    binning = binning_dict[varname]
    numbers = binning.strip("()").split(",")
    numbers = [float(x) if x.isdigit() else float(x) for x in numbers]

    if name == "1":
        wadd_ = "*bdt_reweight_0"
        hname = "h_ratio_1"
    elif name == "2":
        wadd_ = "*bdt_reweight_0*bdt_reweight_1"
        hname = "h_ratio_2"
    else:
        wadd_ = ""
        hname = "h_ratio_0"

    print(hname)
    data.Draw(varname + ">>hdata_sig" + binning, "nsigBs_sw"+wadd_+"*(isMC==0 && RefittedSV_Mass_eq>5.2 && RefittedSV_Mass_eq<5.7)")
    hdata_sig = TH1F(gDirectory.Get("hdata_sig" ))
    data.Draw(varname + ">>hMC_sig" + binning, "nsigBs_sw"+wadd_+"*weight_pileUp*ctau_weight_central*(isMC>0)")
    hMC_sig = TH1F(gDirectory.Get("hMC_sig"))

    hMC_sig.Scale(1 / hMC_sig.Integral(1,int(numbers[0])))
    hdata_sig.Scale(1 / hdata_sig.Integral(1,int(numbers[0])))

    h_x_ratio = hdata_sig.Clone(hname)
    h_x_ratio.Sumw2()
    h_x_ratio.Divide(hMC_sig)

    return h_x_ratio


def addw(file_name, varname, h_x_ratio, name):
    # Open the original file and get the TTree
    input_file = TFile(file_name, "UPDATE")
    tree = input_file.Get("FinalTree")

    # Create a new branch in the TTree
    weight = array('f', [0])
    new_branch = tree.Branch("bdt_reweight_"+name, weight, "bdt_reweight/F")

    # Loop over the entries in the TTree and fill the new branch
    for entry in tree:
        x_value = getattr(entry, varname)
        ismc = getattr(entry, "isMC")
        if ismc == 0:
            weight[0] = 1
            new_branch.Fill()
        else:
            bin_number = h_x_ratio.FindBin(x_value)
            weight[0] = h_x_ratio.GetBinContent(bin_number)
            new_branch.Fill()

    # Write the updated TTree to the file
    tree.Write("FinalTree", TObject.kOverwrite)
    input_file.Close()

def addw_one(file_name, name):
    # Open the original file and get the TTree
    input_file = TFile(file_name, "UPDATE")
    tree = input_file.Get("FinalTree")

    # Create a new branch in the TTree
    weight = array('f', [0])
    new_branch = tree.Branch("bdt_reweight_"+name, weight, "bdt_reweight/F")

    # Loop over the entries in the TTree and fill the new branch
    for entry in tree:
        weight[0] = 1
        new_branch.Fill()

    # Write the updated TTree to the file
    tree.Write("FinalTree", TObject.kOverwrite)
    input_file.Close()

def change_bdt_w(file_name, year):
    # Open the original file and get the TTree
    input_file = TFile(file_name, "UPDATE")
    tree = input_file.Get("FinalTree")

    # Create a new branch in the TTree
    bdt_weight = array('d', [0])
    new_branch = tree.Branch("bdt_weight2", bdt_weight, "bdt_weight2/D")

    # Loop over the entries in the TTree and fill the new branch
    for entry in tree:
        value0 = getattr(entry, "bdt_reweight_0")
        value1 = getattr(entry, "bdt_reweight_1")
        weight = getattr(entry, "bdt_weight")
        if year=="2024":
            value2 = getattr(entry, "bdt_reweight_2")
            bdt_weight[0] = weight * value0 * value1 * value2
        else:
            bdt_weight[0] = weight * value0 * value1
        if bdt_weight[0] < 0:
            bdt_weight[0] = 0
        new_branch.Fill()

    # Write the updated TTree to the file
    tree.Write("FinalTree", TObject.kOverwrite)
    input_file.Close()

        
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("--file", type=str, required=True, help="file path")
    parser.add_argument("--compute", action='store_true', help="")
    parser.add_argument("--new_bdtw", action='store_true', help="")
    args = parser.parse_args()
    compute = args.compute
    file_path = args.file
    new_bdtw = args.new_bdtw
    base_path = file_path.rsplit('.root', 1)[0]

    if compute == True:
        shutil.copy(file_path, base_path+"_rw.root")
        h_x_ratio = reweight(base_path+"_rw.root", "FlightDistBS_SV_Significance", "0")
        addw(base_path+"_rw.root", "FlightDistBS_SV_Significance", h_x_ratio, "0")
        h_x_ratio_2 = reweight(base_path+"_rw.root", "Quadruplet_Eta", "1")
        addw(base_path+"_rw.root", "Quadruplet_Eta", h_x_ratio_2, "1")
        if "2022" in file_path:
            addw_one(base_path+"_rw.root", "2")
            save_histograms([h_x_ratio, h_x_ratio_2], "Weight/weights_2022.root")
        elif "2023" in file_path:
            addw_one(base_path+"_rw.root", "2")
            save_histograms([h_x_ratio, h_x_ratio_2], "Weight/weights_2023.root")
        elif "2024" in file_path:
            h_x_ratio_3 = reweight(base_path+"_rw.root", "vtx_prob", "2")
            addw(base_path+"_rw.root", "vtx_prob", h_x_ratio_3, "2")
            save_histograms([h_x_ratio, h_x_ratio_2, h_x_ratio_3], "Weight/weights_2024.root")
        else:
            print("Wrong year")
            exit()
    else:
        shutil.copy(file_path, base_path+"_rw.root")
        if "2022" in file_path:
            year = "2022"
            file = TFile("Weight/weights_2022.root", "READ")
        elif "2023" in file_path:
            year = "2023"
            file = TFile("Weight/weights_2023.root", "READ")
        elif "2024" in file_path:
            year = "2024"
            file = TFile("Weight/weights_2024.root", "READ")
        else:
            print("No weights found")
            exit()
        histograms, hnames = load_histograms(file)
        for name in hnames:
            if "h_ratio_" in name:
                name = name.replace("h_ratio_", "")
                addw(base_path+"_rw.root", "FlightDistBS_SV_Significance" if name == "0" else "Quadruplet_Eta" if name == "1" else "vtx_prob", histograms[hnames.index("h_ratio_" + name)], name)
        if year!="2024":
            addw_one(base_path+"_rw.root", "2")
        file.Close()
        if new_bdtw:
            change_bdt_w(base_path+"_rw.root", year)
        
        
        