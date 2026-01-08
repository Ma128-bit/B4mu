import uproot
import numpy as np
import xgboost as xgb
import pandas as pd

# ----------------------------
# Configurazione
# ----------------------------
input_file = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Analysis/FinalFiles_B4mu_09_12_25/Analyzed_Data_B4mu_os_bdt.root"
tree_name = "FinalTree"
model_file = "xgb_model.bin"
reference_file = "/lustrehome/mbuonsante/B_4mu/CMSSW_13_0_13/src/Utilities_organized/ROOTFiles_20_01_25/AllData_rw_bdt_v0.root"  # per il fattore di scala
training_features = [
    "vtx_prob",
    "mu1_pfreliso03",
    "mu2_pfreliso03",
    "FlightDistBS_SV_Significance",
    "mu1_bs_dxy_sig",
    "mu2_bs_dxy_sig",
    "mu3_bs_dxy_sig",
    "mu4_bs_dxy_sig",
    "Cos2d_BS_SV",
    "Quadruplet_Eta",
    "Quadruplet_Pt",
    "RefittedSV_Mass_reso"
]

# ----------------------------
# Leggi modello XGBoost
# ----------------------------
bst = xgb.Booster()
bst.load_model(model_file)

# ----------------------------
# Leggi il ROOT
# ----------------------------
with uproot.open(input_file) as f:
    tree = f[tree_name]
    # Leggi solo le colonne necessarie
    df = tree.arrays(training_features, library="pd")
    df = df[df["RefittedSV_Mass_reso"]>0]
    df = df[df["RefittedSV_Mass_reso"]<0.16]
    # Leggi tutto per riscrivere il ROOT
    df_full = tree.arrays(library="pd")
    df_full = df_full[df_full["RefittedSV_Mass_reso"]>0]
    df_full = df_full[df_full["RefittedSV_Mass_reso"]<0.16]


# ----------------------------
# Prepara DMatrix per predizione
# ----------------------------
dtest = xgb.DMatrix(df)
pA = bst.predict(dtest)          # probabilità di essere "A"
pB = 1.0 - pA                    # probabilità di essere B
weights = pA / pB                # peso iniziale

# ----------------------------
# Fattore di scala generale
# ----------------------------
# Numero eventi pesati
Nweighted = weights.sum()
# Numero eventi in file di riferimento
with uproot.open(reference_file) as f_ref:
    tree_ref = f_ref[tree_name]
    isMC = tree_ref["isMC"].array(library="np")
    Nref = np.sum(isMC == 0)

scale_factor = Nref / Nweighted
print("Nref / Nweighted = ", Nref,"/", Nweighted, "=", scale_factor)
weights *= scale_factor

# ----------------------------
# Aggiungi colonna weight
# ----------------------------
df_full["weight"] = weights

# ----------------------------
# Scrivi nuovo ROOT
# ----------------------------
with uproot.recreate(input_file.replace(".root", "_rw.root")) as f_out:
    f_out[tree_name] = df_full