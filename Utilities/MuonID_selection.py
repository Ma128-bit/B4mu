from ROOT import RDataFrame, gROOT
gROOT.SetBatch(True)
import matplotlib.pyplot as plt
import numpy as np
import math, os, draw_utilities
from progress.bar import Bar

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass
    

if __name__ == "__main__":
    rdf_data = RDataFrame("FinalTree", "../Analysis/FinalFiles/Analyzed_Data_2022.root")
    rdf_data = rdf_data.Filter("abs(Quadruplet_Mass-5.366)>0.15")
    evt_data = rdf_data.Count()
    print(evt_data)
    rdf_MC = RDataFrame("FinalTree", "../Analysis/FinalFiles/Analyzed_Data_BsJPsiPhi.root")
    evt_MC = rdf_MC.Count()
    out_dir="AMS_plot"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    muon_id = ["isGlobal", "isPF", "isLoose", "isMedium", "isTight", "isSoft", "isTracker"]

    Nsel = len(muon_id)**4
    bar = Bar('Processing', max=Nsel)
    
    AMS = []
    selections = []
    for i in muon_id:
        for j in muon_id:
            for k in muon_id:
                for w in muon_id:
                    sel = i+"[0]+"+j+"[1]+"+k+"[2]+"+w+"[3] == 4"
                    nbkg = rdf_data.Filter(sel).Count()
                    nbkg = nbkg/evt_data
                    nsig = rdf_MC.Filter(sel).Count()
                    nsig = nsig/evt_MC
                    AMS.append(math.sqrt(2*((nsig+nbkg)*math.log(1+nsig/nbkg) - nsig)))
                    selections.append(sel)
                    bar.next()

    bar.finish()
    best_sel = selections[AMS.index(max(AMS))]
    print(best_sel)
                    
    
