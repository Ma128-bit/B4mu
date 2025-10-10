from ROOT import TFile, TH1F
import json

def category_sel(configfile):
    with open(configfile, 'r') as fp:
        json_file = json.loads(fp.read())
    categories_list = json_file["categories"]
    category = {cat: "" for cat in categories_list}
    for c in categories_list:
        category[c] = json_file[c]
    print(category)
    return category

file = TFile.Open("../Utilities_Control/ROOTFiles_24_01_25/AllB2mu2K_sPlot_rw_bdt.root")
tree = file.Get("FinalTree")

categories = category_sel("/lustrehome/mbuonsante/B_4mu/Combine/CMSSW_14_1_0_pre4/src/B4muLimits/configs/config_20_01_25_10%.json")

for c in categories.keys():
    data_hist = TH1F("data_hist", "Data Histogram", 100, 0, 10)
    mc_hist = TH1F("mc_hist", "MC Histogram", 100, 0, 10)
    data_histpost = TH1F("data_histpost", "Data Histogram", 100, 0, 10)
    mc_histpost = TH1F("mc_histpost", "MC Histogram", 100, 0, 10)

    if "A" in c:
        tree.Draw("RefittedSV_Mass>>data_hist", "nsigBs_sw*(isMC==0 && category==0 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
        tree.Draw("RefittedSV_Mass>>mc_hist", "nsigBs_sw*weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && category==0 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
    elif "B" in c:
        tree.Draw("RefittedSV_Mass>>data_hist", "nsigBs_sw*(isMC==0 && category==1 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
        tree.Draw("RefittedSV_Mass>>mc_hist", "nsigBs_sw*weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && category==1 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
    else:
        tree.Draw("RefittedSV_Mass>>data_hist", "nsigBs_sw*(isMC==0 && category==2 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
        tree.Draw("RefittedSV_Mass>>mc_hist", "nsigBs_sw*weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && category==2 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
    
    data_events_0 = data_hist.Integral()
    mc_events_0 = mc_hist.Integral()
    
    cat = categories[c].replace("bdt_cv","bdt")
    tree.Draw("RefittedSV_Mass>>data_histpost", "nsigBs_sw*(isMC==0 && (" + cat + ") && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7)")
    tree.Draw("RefittedSV_Mass>>mc_histpost", "nsigBs_sw*weight*bdt_reweight_0*bdt_reweight_1*bdt_reweight_2*(isMC>0 && RefittedSV_Mass>5.2 && RefittedSV_Mass<5.7 && (" + cat + "))")
    
    data_events = data_histpost.Integral()
    mc_events = mc_histpost.Integral()
    
    print((data_events/data_events_0)/(mc_events/mc_events_0))
    del data_hist
    del mc_hist
    del data_histpost
    del mc_histpost
