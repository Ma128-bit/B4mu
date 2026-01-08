#!/bin/bash

if [ -z "$1" ]; then
  echo "Uso: $0 <path_to_best_model_directory>"
  return
fi

TARGET_DIR="$1"

if [ ! -d "BDT_results/$TARGET_DIR" ]; then
  echo "Errore: la directory BDT_results/$TARGET_DIR non esiste."
  return
fi

ABS_PATH=$(realpath "BDT_results/$TARGET_DIR")
PARENT_DIR=$(dirname "$ABS_PATH")

ln -sfn "$ABS_PATH" "$PARENT_DIR/test_bdt"

echo "Creato alias simbolico: $PARENT_DIR/test_bdt → $ABS_PATH"