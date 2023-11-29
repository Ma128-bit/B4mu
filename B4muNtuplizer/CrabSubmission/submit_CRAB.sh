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
declare -a Pre_E_MC22=("Dataset_prova1" "Dataset_prova2")
declare -a Post_E_MC22=("/Bd4Mu_13p6TeV-pythia8_Run3/mbuonsan-130X_mcRun3_2022_realistic_postEE_v6_Bd4Mu_MINIAODSIM-1998bbcdca3ce14ea15a9b06075ab84e/USER" "/Bs4Mu_13p6TeV-pythia8_Run3/mbuonsan-130X_mcRun3_2022_realistic_postEE_v6_Bs4Mu_MINIAODSIM-1998bbcdca3ce14ea15a9b06075ab84e/USER")

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
      MC_pre)
        globaltag="130X_mcRun3_2022_realistic_v5"
        datasets=("${Pre_E_MC22[@]}")
        input_type="global"
        ;;
      MC_post)
        globaltag="130X_mcRun3_2022_realistic_postEE_v6"
        datasets=("${Post_E_MC22[@]}")
        input_type="phys03"
        ;;
      *)
        echo "Error: The era is incorrect."
        exit 1
        ;;
    esac
fi

voms-proxy-init --valid 192:00 --voms cms

if [[ "$era" != *"MC"* ]]; then
    mkdir -p "${year}_era${era}"
    echo "Data ${year} - era ${era} is selected"
    path="${directory}/${year}_era${era}/PatAndTree_cfg.py"
    cp "${path_to_skim_file}/run_Data2022_PatAndTree_cfg.py" "$path"
    sed -i "s#124X_dataRun3_v14#${globaltag}#g" "${year}_era${era}/PatAndTree_cfg.py"
    for i in {0..7}; do
        cp templates/CRAB_template.py "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#YEAR#${year}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#ERANAME#${era}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#NUMBER#${i}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#DATASET_ID#${Data_ID[${i}]}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#GOLDEN_JSON_PATH#${golden_json}#g" "${year}_era${era}/CRAB_stream_${i}.py"
        crab submit -c "${year}_era${era}/CRAB_stream_${i}.py"
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
        cp templates/CRAB_template_MC.py "${year}_${era}/CRAB_MC_${j}.py"
        sed -i "s#YEAR#${year}#g" "${year}_${era}/CRAB_MC_${j}.py"
        sed -i "s#ERANAME#${era}#g" "${year}_${era}/CRAB_MC_${j}.py"
        sed -i "s#MC_DATASET#${i}#g" "${year}_${era}/CRAB_MC_${j}.py"
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" "${year}_${era}/CRAB_MC_${j}.py"
        sed -i "s#INPUT_TYPE#${input_type}#g" "${year}_${era}/CRAB_MC_${j}.py"
        crab submit -c "${year}_${era}/CRAB_MC_${j}.py"
        echo "${era} - $j submitted!"
        ((j++))
        sleep 2
    done

fi




