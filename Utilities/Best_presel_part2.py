import numpy as np
import pandas as pd
import math
import ROOT

df = pd.read_csv("Best_cut/best_pre_sel_2022control.csv")

var = {
    "vtx_prob": [-1],
    "Cos2d_PV_SV": [0],
    "FlightDistBS_SV_Significance": [0, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 5]
}


i, j, k = 1, 1, 10
histogram = ROOT.TH3F("S/sigma", "S/sigma; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     i, -0.5, float(i)-0.5, j, -0.5, float(j)-0.5, k, -0.5, float(k)-0.5)

histogram2 = ROOT.TH3F("AMS", "AMS; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     i, -0.5, float(i)-0.5, j, -0.5, float(j)-0.5, k, -0.5, float(k)-0.5)

maxim = 0
x, y, z = 0, 0, 0
for index, row in df.iterrows():
    S = row["sig3sigma"]+row["mean_nsig"]
    AMS=AMS=math.sqrt(2*((S+row["bkg3sigma"])*math.log(1+S/row["bkg3sigma"]) - S))
    if(row["bkg3sigma"]!=0):
        Ssigma = S/math.sqrt(row["bkg3sigma"])
    else:
        Ssigma = 0
    #Ssigma = row["mean_nsig"]/row["sigma_nsig"]
    histogram.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, Ssigma)
    #AMS=math.sqrt(2*((row["mean_nsig"]+row["mean_nbkg"])*math.log(1+row["mean_nsig"]/row["mean_nbkg"]) - row["mean_nsig"]))
    histogram2.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, AMS)
    if(AMS>maxim):
        maxim = AMS
        x = int(row["ID12"])
        y = int(row["ID3"])
        z = int(row["ID4"])
        print("MAX:",x,y,z, AMS, row["sig3sigma"], row["bkg3sigma"])
    

canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
histogram.Draw("colz")

print("MAX:",var["vtx_prob"][x],var["Cos2d_PV_SV"][y],var["FlightDistBS_SV_Significance"][z])
root_file = ROOT.TFile("istogramma.root", "RECREATE")
histogram.Write()
histogram2.Write()
root_file.Close()
