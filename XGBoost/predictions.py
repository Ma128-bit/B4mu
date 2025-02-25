import sys, os, subprocess, json, pickle, uproot, argparse
import numpy as np
import pandas as pd
from xgboost import XGBRegressor, plot_importance
import awkward as ak
from matplotlib import pyplot as plt

branches = [
    "isMC", "lumi", "run", "evt", "nPileUpInt", "PVCollection_Size",
    "Mu1_Pt", "Mu2_Pt", "Mu3_Pt", "Mu4_Pt","Mu1_Eta", "Mu2_Eta", "Mu3_Eta", "Mu4_Eta",
    "Mu1_Phi", "Mu2_Phi", "Mu3_Phi", "Mu4_Phi", "Quadruplet_Mass", "FlightDistBS_SV_Significance",
    "QuadrupletVtx_Chi2", "Quadruplet_Pt", "Quadruplet_Eta", "Quadruplet_Phi",
    "vtx_prob", "mu1_pfreliso03", "mu2_pfreliso03", 
    "mu1_bs_dxy_sig", "mu2_bs_dxy_sig","mu3_bs_dxy_sig", "mu4_bs_dxy_sig",
    "Cos2d_BS_SV", "Cos2d_PV_SV", 
    "RefittedSV_Mass", "RefittedSV_Mass_err"
]


controlKK_branches = ["weight", "RefittedSV_Mass_reso", "ID", "year", "category", "new_ct", "RefittedSV_Mass_eq", "Ditrk_mass", "Dimu_mass", "ID", "year", "weight_pileUp", "ctau_weight_central", "nsigBs_sw", "bdt_reweight_0", "bdt_reweight_1"]
control_branches = ["weight", "RefittedSV_Mass_reso", "ID", "year", "category", "new_ct", "mu3_pfreliso03", "mu4_pfreliso03", "OS1v1_mass_err", "OS2v1_mass_err", "OS1v2_mass_err", "OS2v2_mass_err", "isJPsiPhi", "OS1v1_mass", "OS2v1_mass", "OS1v2_mass", "OS2v2_mass", "Quadruplet_Mass_eq", "weight_err", "weight_pileUp", "weight_pileUp_err", "ctau_weight_central", "NewMassEqation", "bdt_reweight_0", "bdt_reweight_1"]
signal_branches = ["weight", "RefittedSV_Mass_reso", "ID", "year", "category", "new_ct", "mu3_pfreliso03", "mu4_pfreliso03", "OS1v1_mass_err", "OS2v1_mass_err", "OS1v2_mass_err", "OS2v2_mass_err", "isJPsiPhi", "OS1v1_mass", "OS2v1_mass", "OS1v2_mass", "OS2v2_mass", "Quadruplet_Mass_eq",  "weight_err", "weight_pileUp", "weight_pileUp_err", "ctau_weight_central", "ctau_weight_heavy", "ctau_weight_light", "Jpsicut", "phicut", "omegacut", "psi2scut", "bdt_weight", "w_mc", "bdt_reweight_0", "bdt_reweight_1"]

def load_config(config_file):
    """Load info from config file"""
    with open(config_file, 'r') as file:
        json_file = json.load(file)
    output_folder = json_file['output_folder']
    date = json_file['date']
    try:
        label = json_file['label']
    except KeyError:
        label = ""
    out_dir = output_folder+"/"+label+"_"+date
    kfold = json_file['number_of_splits']
    training_variables = json_file['training_variables']
    index_branch = json_file['index_branch']
    return out_dir, kfold, training_variables, index_branch, json_file["category_names"]

def load_data(file_names):
    """Load ROOT data and turn tree into a pd dataframe"""
    trees = []
    for file in file_names:
        print("Loading data from", file)
        f = uproot.open(file)
        tree = f["FinalTree"]
        trees.append(tree.arrays(library="pd"))
    data = pd.concat(trees)
    return data

def category_split(data, config_file):
    with open(config_file, 'r') as file:
        json_file = json.load(file)
    category_sel = json_file["category_sel"]
    data_vec = []
    for cat_i in range(len(category_sel)):
        mask = data.eval(category_sel[cat_i])
        df_temp = data[mask]
        data_vec.append(df_temp)
        del df_temp
        del mask
    return data_vec


def save_data_v2(data, fileName):
    data_v2 = {col: data[col].values for col in data.columns}
    del data
    with uproot.recreate(fileName+".root") as file:
        file["FinalTree"] = data_v2
    del data_v2
    print("File ROOT saved!")

def predict(df, out_dir, training_variables, ifold, cat="Cat"):
    print("Start prediction fold ", ifold)
    X = df[training_variables]
    X = X.values
    model_path = out_dir + "/model_"+cat+"_fold" + str(ifold) + ".pkl"
    with open(model_path, "rb") as file:
        loaded_model = pickle.load(file)
    y_pred = loaded_model.predict(X)

    df[f"bdt_fold{ifold}"] = y_pred
    #df.loc[:, f"bdt_fold{ifold}"] = y_pred
    del y_pred
    del loaded_model
    del X
    return df


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Config File path")
    parser.add_argument("--config", type=str, help="Config File path")
    parser.add_argument("--file_path", type=str, help="Data File path")
    parser.add_argument("--type", type=str, help="Control or signal")
    args = parser.parse_args()
    out_dir, kfold, training_variables, index_branch, categories = load_config(args.config)

    d_temp = load_data([args.file_path])
    if args.type=="wsign":
        d_temp["category"] = 0
    data_vec = category_split(d_temp, args.config)
    data_out = []

    for i, c in enumerate(categories):
        df_temp = data_vec[i].copy()
        
        for j in range(kfold):
            df_temp = predict(df_temp, out_dir, training_variables, j, c)
    
        df_temp["kfoldID"] = (df_temp[index_branch] % kfold).astype(int)
        bdt_scores = (df_temp[index_branch]==0).to_numpy()*(0.0)
        bdt_cv_scores = (df_temp[index_branch]==0).to_numpy()*(0.0)
    
        
        for fold_ in range(kfold):
            branches.append(f"bdt_fold{fold_}")
            bdt_scores += df_temp[f"bdt_fold{fold_}"]/kfold
            bdt_cv_scores += (df_temp["kfoldID"]==(fold_)).to_numpy()*df_temp[f"bdt_fold{fold_}"] 
        
        branches.append("bdt")
        branches.append("bdt_cv")
        
        df_temp['bdt'] = bdt_scores
        df_temp['bdt_cv'] = bdt_cv_scores
        data_out.append(df_temp)
        
    for i, data in enumerate(data_out):
        #print(data)
        if args.type=="Control":
            data = data[list(np.unique(branches+control_branches+training_variables))]
        elif args.type=="wsign":
            data = data[list(np.unique(branches+["Quadruplet_Charge"]))]
        elif args.type=="ControlKK":
            data = data[list(np.unique(branches+controlKK_branches+training_variables))]
        else:
            data = data[list(np.unique(branches+signal_branches+training_variables))]
        
        save_data_v2(data, os.path.splitext(args.file_path)[0]+f"_bdt_c{i}")
