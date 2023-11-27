#!/bin/bash

# Verifica che siano forniti 5 argomenti
if [ $# -ne 2 ]; then
  echo "Usage: $0 <era> <year>"
  exit 1
fi

# Assegna i nomi dei file ai singoli argomenti
era=$1
year=$2

directory="$PWD"
echo "pwd: $directory"
home_dir=$(dirname "$(dirname "$directory")/CrabSubmission")
echo "Home dir: $home_dir"
path_to_skim_file="${home_dir}/SkimTools/test"

declare -a C_2022=("Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1")
declare -a D-v1_2022=("Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1" "Run2022D-PromptReco-v1")
declare -a D-v2_2022=("Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2" "Run2022D-PromptReco-v2")
declare -a E_2022=("Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1" "Run2022E-PromptReco-v1")
declare -a F_2022=("Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1" "Run2022F-22Sep2023-v1")
declare -a G_2022=("Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v2" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1" "Run2022G-22Sep2023-v1")
declare -a Pre_E_MC=()
declare -a Post_E_MC=()

if [ "${year}" == "2022" ]; then
    case "$era" in
      C)
        echo "Era C."
        Data_ID=("${C_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      D-v1)
        echo "Era D-v1."
        Data_ID=("${D-v1_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      D-v2)
        echo "Era D-v2."
        Data_ID=("${D-v2_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      E)
        echo "Era E."
        Data_ID=("${E_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      F)
        echo "Era F."
        Data_ID=("${F_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      G)
        echo "Era G."
        Data_ID=("${G_2022[@]}")
        globaltag=""
        golden_json=
        ;;
      MC_pre)
        echo "MC ${era}."
        globaltag=""
        datasets=("${Pre_E_MC[@]}")
        ;;
      MC_post)
        echo "MC ${era}."
        globaltag=""
        datasets=("${Post_E_MC[@]}")
        ;;
      *)
        echo "Errore: era non corretta."
        exit 1
        ;;
    esac
fi

if [[ "$era" != *"MC_"* ]]; then
    mkdir -p "${year}_era${era}"
    path="${directory}/${year}_era${era}/PatAndTree_cfg.py"
    cp "${path_to_skim_file}/run_Data2022_PatAndTree_cfg.py" "$path"
    sed -i "s#124X_dataRun3_v14#${globaltag}#g" "${year}_era${era}/PatAndTree_cfg.py"
    for i in {0..7}; do
        echo "Stream $i"
        cp templates/CRAB_template.py "${year}_era${era}/CRAB_stream_${i}.py"
        sed -i "s#YEAR#${year}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#ERANAME#${era}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#NUMBER#${i}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#DATASET_ID#${Data_ID[${i}]}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#GOLDEN_JSON_PATH#${golden_json}#g" "${year}_era${era}/CRAD_stream_${i}.py"
        
        
    done
else
    mkdir -p "${year}_${era}"
    echo "\${era} contiene 'MC_'."
    path="${directory}/${year}_${era}/PatAndTree_cfg.py"
    cp "${path_to_skim_file}/run_MC2022_PatAndTree_cfg.py" "$path"
    sed -i "s#130X_mcRun3_2022_realistic_postEE_v6#${globaltag}#g" "${year}_${era}/PatAndTree_cfg.py"

fi




