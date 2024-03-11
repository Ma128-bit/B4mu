#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Usage: $0 <era> <year>"
  return
fi

era=$1
year=$2

directory="$PWD"
#echo "pwd: $directory"
home_dir=$(dirname "$(dirname "$directory")/CrabSubmission")
#echo "Home dir: $home_dir"
path_to_skim_file="${home_dir}/SkimTools/test"

declare -a C_2022=("Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1")
declare -a D_v1_2022=("Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1")
declare -a D_v2_2022=("Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2")
declare -a E_2022=("Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1")
declare -a F_2022=("Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1")
declare -a G_2022=("Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v2" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1")

declare -a B_2023=("Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v2" "Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v1" "Run2023B-22Sep2023-v1")
declare -a C_v1_2023=("Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v2" "Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v1" "Run2023C-22Sep2023_v1-v1")
declare -a C_v2_2023=("Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1" "Run2023C-22Sep2023_v2-v1")
declare -a C_v3_2023=("Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1" "Run2023C-22Sep2023_v3-v1")
declare -a C_v4_2023=("Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v2" "Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v1" "Run2023C-22Sep2023_v4-v1")
declare -a D_v1_2023=("Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1" "Run2023D-22Sep2023_v1-v1")
declare -a D_v2_2023=("Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v2" "Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v1" "Run2023D-22Sep2023_v2-v1")

declare -a MC22_B2mu2trk_pre=("/BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/Run3Summer22MiniAODv4-130X_mcRun3_2022_realistic_v5-v2/MINIAODSIM")
declare -a MC22_B2mu2trk_post=("/BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/Run3Summer22EEMiniAODv4-130X_mcRun3_2022_realistic_postEE_v6-v2/MINIAODSIM")
declare -a B4mu_MC_label=("Bs2mu2K")


if [ "${year}" == "2022" ]; then
    case "$era" in
      C)
        Data_ID=("${C_2022[@]}")
        globaltag="124X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions22/Cert_Collisions2022_eraC_355862_357482_Golden.json"
        ;;
      D-v1)
        Data_ID=("${D_v1_2022[@]}")
        globaltag="124X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions22/Cert_Collisions2022_eraD_357538_357900_Golden.json"
        ;;
      D-v2)
        Data_ID=("${D_v2_2022[@]}")
        globaltag="124X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions22/Cert_Collisions2022_eraD_357538_357900_Golden.json"
        ;;
      E)
        Data_ID=("${E_2022[@]}")
        globaltag="124X_dataRun3_Prompt_v10"
        golden_json="Collisions22/Cert_Collisions2022_eraE_359022_360331_Golden.json"
        ;;
      F)
        Data_ID=("${F_2022[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions22/Cert_Collisions2022_eraF_360390_362167_Golden.json"
        ;;
      G)
        Data_ID=("${G_2022[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions22/Cert_Collisions2022_eraG_362433_362760_Golden.json"
        ;;
      MC_B4mu_pre)
        globaltag="130X_mcRun3_2022_realistic_v5"
        datasets=("${MC22_B4mu_pre[@]}")
        label=("${B4mu_MC_label[@]}")
        input_type="global"
        ;;
      MC_B4mu_post)
        globaltag="130X_mcRun3_2022_realistic_postEE_v6"
        datasets=("${MC22_B4mu_post[@]}")
        label=("${B4mu_MC_label[@]}")
        input_type="global"
        ;;
      *)
        echo "Error: The era is incorrect."
        return
        ;;
    esac
elif [ "${year}" == "2023" ]; then
    case "$era" in
      B)
        Data_ID=("${B_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraB_366403_367079_Golden.json"
        ;;
      C-v1)
        Data_ID=("${C_v1_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraC_367095_368823_Golden.json"
        ;;
      C-v2)
        Data_ID=("${C_v2_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraC_367095_368823_Golden.json"
        ;;
      C-v3)
        Data_ID=("${C_v3_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraC_367095_368823_Golden.json"
        ;;
      C-v4)
        Data_ID=("${C_v4_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraC_367095_368823_Golden.json"
        ;;
      D-v1)
        Data_ID=("${D_v1_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraD_369803_370790_Golden.json"
        ;;
      D-v2)
        Data_ID=("${D_v2_2023[@]}")
        globaltag="130X_dataRun3_PromptAnalysis_v1"
        golden_json="Collisions23/Cert_Collisions2023_eraD_369803_370790_Golden.json"
        ;;
      *)
        echo "Error: The era is incorrect."
        return
        ;;
    esac
else
    echo "Error: The year is incorrect."
    return
fi

#voms-proxy-init --valid 192:00 --voms cms

if [[ "$era" != *"MC"* ]]; then
    mkdir -p "${year}_era${era}"
    echo "Data ${year} - era ${era} is selected"
    path="${directory}/${year}_era${era}/PatAndTree_cfg.py"
    cp "${path_to_skim_file}/run_Data2022_PatAndTree_cfg.py" "$path"
    sed -i "s#124X_dataRun3_v14#${globaltag}#g" "${year}_era${era}/PatAndTree_cfg.py"
    cp templates/report.sh "${year}_era${era}/report.sh"
    cp templates/status.sh "${year}_era${era}/status.sh"
    cp templates/resubmit.sh "${year}_era${era}/resubmit.sh"
    cd "${year}_era${era}"
    sed -i "s#YEAR#${year}#g" *.sh
    sed -i "s#ERANAME#${era}#g" *.sh
    cd ..
    cp templates/submit.sh "${year}_era${era}/submit.sh"
    for i in {0..7}; do
        cp templates/CRAB_template.py "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#YEAR#${year}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#ERANAME#${era}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#NUMBER#${i}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#DATASET_ID#${Data_ID[${i}]}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#GOLDEN_JSON_PATH#${golden_json}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        cd "${year}_era${era}"
        crab submit -c "CRAB_stream_${i}.py"
        cd ..
        echo "Stream $i submitted!"
        sleep 2
    done
else
    mkdir -p "${year}_${era}"
    echo "${era} - ${year} is selected"
    path="${directory}/${year}_${era}/PatAndTree_cfg.py"
    cp "${path_to_skim_file}/run_MC2022_PatAndTree_cfg.py" "$path"
    sed -i "s#130X_mcRun3_2022_realistic_postEE_v6#${globaltag}#g" "${year}_${era}/PatAndTree_cfg.py"
    j=0
    for i in "${datasets[@]}"; do
        cp templates/CRAB_template_MC.py "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#YEAR#${year}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#ERANAME#${era}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#MC_DATASET#${i}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#INPUT_TYPE#${input_type}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        sed -i "s#B_TYPE#${label[${j}]}#g" "${year}_${era}/CRAB_MC_${label[${j}]}.py"
        cd "${year}_${era}"
        crab submit -c "CRAB_MC_${label[${j}]}.py"
        cd ..
        echo "${era} - $j submitted!"
        ((j++))
        sleep 2
    done

fi




