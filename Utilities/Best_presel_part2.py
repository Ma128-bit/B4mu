import numpy as np
import pandas as pd
import math
import ROOT

df = pd.read_csv("Best_cut/best_pre_sel_2022control.csv")

var = {
    "vtx_prob": [0.0011+i*0.0002 for i in range(10)],
    "Cos2d_PV_SV": [0.7+i/31 for i in range(10)],
    "FlightDistBS_SV_Significance": [1+i/3 for i in range(10)]
}

histogram = ROOT.TH3F("S/sigma", "S/sigma; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     10, -0.5, 9.5, 10, -0.5, 9.5, 10, -0.5, 9.5)

histogram2 = ROOT.TH3F("AMS", "AMS; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     10, -0.5, 9.5, 10, -0.5, 9.5, 10, -0.5, 9.5)

maxim = 0
x, y, z = 0, 0, 0
for index, row in df.iterrows():
    #Ssigma=0
    #AMS = row["sig3sigma"]/math.sqrt(row["bkg3sigma"])
    Ssigma = row["mean_nsig"]/row["sigma_nsig"]
    histogram.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, Ssigma)
    AMS=math.sqrt(2*((row["mean_nsig"]+row["mean_nbkg"])*math.log(1+row["mean_nsig"]/row["mean_nbkg"]) - row["mean_nsig"]))
    histogram2.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, AMS)
    if(Ssigma>maxim):
        maxim = Ssigma
        x = int(row["ID12"])
        y = int(row["ID3"])
        z = int(row["ID4"])
        print("MAX:",x,y,z, Ssigma)
    

canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
histogram.Draw("colz")

print("MAX:",var["vtx_prob"][x],var["Cos2d_PV_SV"][y],var["FlightDistBS_SV_Significance"][z])
root_file = ROOT.TFile("istogramma.root", "RECREATE")
histogram.Write()
histogram2.Write()
root_file.Close()
