#!/bin/bash

# Verifica che siano forniti 5 argomenti
if [ $# -ne 2 ]; then
  echo "Usage: $0 <era> <year>"
  exit 1
fi

# Assegna i nomi dei file ai singoli argomenti
era=$1
year=$2

if [ "${year}" == "2022" ]; then
    case "$era" in
      C|D-v1|D-v2)
        echo "Hai scelto B."
        # Fai qualcosa per la scelta B
        ;;
      E)
        echo "Hai scelto C."
        # Fai qualcosa per la scelta C
        ;;
      F|G)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      MC_pre)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      MC_post)
        echo "Hai scelto K."
        # Fai qualcosa per la scelta K
        ;;
      *)
        echo "Errore: era non corretta."
        exit 1
        ;;
    esac
fi


# Modifica i file con sed
sed -i 's/vecchia_stringa/nuova_stringa/g' "$file1" "$file2" "$file3" "$file4" "$file5"

echo "Modifica completata con successo nei file: $file1 $file2 $file3 $file4 $file5."

