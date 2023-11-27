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
home_dir=$(dirname "$(dirname "$directory")/CrabSubmission")

echo "La parte del percorso prima di 'pippo' è: $parte_prima_di_pippo"

2022_C=("Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1" "Run2022C-PromptReco-v1")

if [ "${year}" == "2022" ]; then
    case "$era" in
      C)
        echo "Era C."
        Data_ID=("${2022_C[@]}")
        path=""
        ;;
      D-v1)
        echo "Hai scelto C."
        # Fai qualcosa per la scelta C
        ;;
      D-v2)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      E)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      F)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      G)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      MC_pre*)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      MC_post*)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      *)
        echo "Errore: era non corretta."
        exit 1
        ;;
    esac
fi

if [[ "$era" != *"MC_"* ]]; then
    mkdir -p "${year}_era${era}"
    for i in {0..7}; do
        echo "Stream $i"
        cp templates/CRAB_template.py "${year}_era${era}/CRAD_stream_${i}.py"
        sed -i "s#YEAR#${year}#g" ${year}_era${era}/CRAD_stream_${i}.py
        sed -i "s#ERANAME#${era}#g" ${year}_era${era}/CRAD_stream_${i}.py
        sed -i "s#NUMBER#${i}#g" ${year}_era${era}/CRAD_stream_${i}.py
        sed -i "s#DATASET_ID#${Data_ID[${i}]}#g" ${year}_era${era}/CRAD_stream_${i}.py
        sed -i "s#FILE_TO_SUBMIT_PATH#${path}#g" ${year}_era${era}/CRAD_stream_${i}.py
        

        
        
    done
else
    mkdir "${year}_MC"
    echo "\$era contiene 'MC_'."
fi




