# !/bin/sh
# Usage:
#    prepare_and_submit_ALL.sh <Year>

helpstring="Usage:
prepare_and_submit_ALL.sh [Year]"

year=$1
Analysis_type=$2

# Check inputs
if [ -z ${2+x} ]; then
    echo -e ${helpstring}
    return
fi

declare -a Era_2022=("C" "D-v1" "D-v2" "E" "F" "G")
declare -a Era_2023=("C-v1" "C-v2" "C-v3" "C-v4" "D-v1" "D-v2")

if [ "${year}" == "2022" ]; then
  eras=("${Era_2022[@]}")
elif [ "${year}" == "2023" ]; then
  eras=("${Era_2023[@]}")
else
  echo "Error: The year is incorrect."
  return
fi

cd "${Analysis_type}"
for e in "${eras[@]}"; do
    cd "${year}_era${e}"
    source hadd_era.sh
    echo ""
    cd ..
    sleep 1
done
cd ..

if [ ! -d "FinalFiles" ]; then
    mkdir -p "FinalFiles"
fi
hadd FinalFiles/Analyzed_Data_${Analysis_type}_${year}.root ${Analysis_type}/${year}_era*/Analyzed_Data_${year}_Era_*.root
