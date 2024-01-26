from ROOT import RDataFrame, gROOT, EnableImplicitMT
gROOT.SetBatch(True)
EnableImplicitMT()
import matplotlib.pyplot as plt
import numpy as np
import math, os, draw_utilities
from progress.bar import Bar

class ROOTDrawer(draw_utilities.ROOTDrawer):
    pass


if __name__ == "__main__":
    rdf_data = RDataFrame("FinalTree", "../Analysis/FinalFiles/Analyzed_Data_2022.root")
    rdf_data = rdf_data.Filter("abs(Quadruplet_Mass-5.366)>0.15")
    evt_data = rdf_data.Count().GetValue()
    print(evt_data)
    rdf_MC = RDataFrame("FinalTree", "../Analysis/FinalFiles/Analyzed_Data_BsJPsiPhi.root")
    evt_MC = rdf_MC.Count().GetValue()
    out_dir="AMS_plot"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    muon_id = ["isGlobal", "isPF", "isLoose", "isMedium", "isTight", "isSoft", "isTracker"]

    """
    Nsel = len(muon_id)**4
    bar = Bar('Processing', max=Nsel)
    
    AMS = []
    selections = []
    for i in muon_id:
        for j in muon_id:
            for k in muon_id:
                for w in muon_id:
                    sel = i+"[0]+"+j+"[1]+"+k+"[2]+"+w+"[3] == 4"
                    nbkg = rdf_data.Filter(sel).Count().GetValue()
                    nbkg = nbkg/evt_data
                    nsig = rdf_MC.Filter(sel).Count().GetValue()
                    nsig = nsig/evt_MC
                    AMS.append(math.sqrt(2*((nsig+nbkg)*math.log(1+nsig/nbkg) - nsig)))
                    selections.append(sel)
                    bar.next()

    OUT: isMedium[0]+isMedium[1]+isMedium[2]+isMedium[3] == 4

    Nsel = len(muon_id)**2
    bar = Bar('Processing', max=Nsel)
    AMS = []
    selections = []
    for i in muon_id:
        for j in muon_id:
            sel = "("+i+"[0]+"+i+"[1]+"+i+"[2]+"+i+"[3] == 4) && ("+j+"[0]+"+j+"[1]+"+j+"[2]+"+j+"[3] == 4)"
            nbkg = rdf_data.Filter(sel).Count().GetValue()
            nbkg = nbkg/evt_data
            nsig = rdf_MC.Filter(sel).Count().GetValue()
            nsig = nsig/evt_MC
            AMS.append(math.sqrt(2*((nsig+nbkg)*math.log(1+nsig/nbkg) - nsig)))
            selections.append(sel)
            bar.next()

    OUT: (isGlobal[0]+isGlobal[1]+isGlobal[2]+isGlobal[3] == 4) && (isSoft[0]+isSoft[1]+isSoft[2]+isSoft[3] == 4)
    

    Nsel = 16*len(muon_id)**2
    bar = Bar('Processing', max=Nsel)
    AMS = []
    selections = []
    for i in muon_id:
        for j in muon_id:
            if(j!=i):
                for k in ["1","2","3","4"]:
                    for h in ["1","2","3","4"]:
                        sel = "("+i+"[0]+"+i+"[1]+"+i+"[2]+"+i+"[3] == "+k+") && ("+j+"[0]+"+j+"[1]+"+j+"[2]+"+j+"[3] == "+h+")"
                        nbkg = rdf_data.Filter(sel).Count().GetValue()
                        nbkg = nbkg/evt_data
                        nsig = rdf_MC.Filter(sel).Count().GetValue()
                        nsig = nsig/evt_MC
                        if(nbkg!=0):
                            AMS.append(math.sqrt(2*((nsig+nbkg)*math.log(1+nsig/nbkg) - nsig)))
                            selections.append(sel)
                        else:
                            print("nbkg==0 sel: ",sel)
                        bar.next()
            else:
                for w in range(16):
                    bar.next()

    OUT: (isGlobal[0]+isGlobal[1]+isGlobal[2]+isGlobal[3] == 4) && (isSoft[0]+isSoft[1]+isSoft[2]+isSoft[3] == 4)

    """
    Nsel = len(muon_id)**2
    bar = Bar('Processing', max=Nsel)
    AMS = []
    selections = []
    for i in muon_id:
        for j in muon_id:
            sel = "("+i+"[0]+"+i+"[1]+"+i+"[2]+"+i+"[3] == 4) || ("+j+"[0]+"+j+"[1]+"+j+"[2]+"+j+"[3] == 4)"
            sel = "("+sel+") && (BsJPsiPhi_sel_OS1>0 || BsJPsiPhi_sel_OS2>0)"
            nbkg = rdf_data.Filter(sel).Count().GetValue()
            nbkg = nbkg/evt_data
            nsig = rdf_MC.Filter(sel).Count().GetValue()
            nsig = nsig/evt_MC
            AMS.append(math.sqrt(2*((nsig+nbkg)*math.log(1+nsig/nbkg) - nsig)))
            selections.append(sel)
            bar.next()

    #OUT: (isMedium[0]+isMedium[1]+isMedium[2]+isMedium[3] == 4) || (isTight[0]+isTight[1]+isTight[2]+isTight[3] == 4)

    
    bar.finish()
    best_sel = selections[AMS.index(max(AMS))]
    print(best_sel)
    with open('output.txt', 'w') as file:
        file.write(best_sel)
                    
    
