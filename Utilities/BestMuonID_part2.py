import numpy as np
import pandas as pd
import ROOT, math

type = "Fit"
df = pd.read_csv("MuonID_plots/DFbestID_2022signal"+type+"_v2.csv")

histogram = ROOT.TH3F("S/sigma", "S/sigma; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     9, -0.5, 8.5, 9, -0.5, 8.5, 9, -0.5, 8.5)

histogram2 = ROOT.TH3F("AMS", "AMS; vtx_prob; Cos2d_PV_SV; FlightDistBS_SV_Significance",
                     9, -0.5, 8.5, 9, -0.5, 8.5, 9, -0.5, 8.5)

maxim = 0
x, y, z = 0, 0, 0
for index, row in df.iterrows():
    #Ssigma=0
    #AMS = row["sig3sigma"]/math.sqrt(row["bkg3sigma"])
    Ssigma = row["mean_nsig"]/row["sigma_nsig"]
    histogram.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, Ssigma)
    S = row["mean_nsig"]
    B = row["mean_nbkg"]
    AMS=math.sqrt(2*((S+B)*math.log(1+S/B) - S))
    histogram2.SetBinContent(int(row["ID12"])+1, int(row["ID3"])+1, int(row["ID4"])+1, AMS)
    if(Ssigma>maxim):
        #if ((int(row["ID12"]) == int(row["ID3"])) and (int(row["ID12"]) == int(row["ID4"]))):
        maxim = Ssigma
        x = int(row["ID12"])
        y = int(row["ID3"])
        z = int(row["ID4"])
        print("MAX:",x,y,z, Ssigma)
    

canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
histogram.Draw("colz")

root_file = ROOT.TFile("istogramma.root", "RECREATE")
histogram.Write()
histogram2.Write()
root_file.Close()
