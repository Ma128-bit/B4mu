# !/bin/sh
# Usage:
#    prepare_and_submit_ALL.sh <Year> <Delta>

helpstring="Usage:
prepare_and_submit_ALL.sh [Year] [Delta]"

year=$1
delta=$2

# Check inputs
if [ -z ${2+x} ]; then
    echo -e ${helpstring}
    return
fi

declare -a Era_2022=("C" "D-v1" "D-v2" "E" "F" "G")
declare -a Era_2023=("C-v1" "C-v2" "C-v3" "C-v4" "D-v1" "D-v2")

if [ "${year}" == "2022" ]; then
  eras=("${Era_2022[@]}")
elif [ "${year}" == "2022" ]; then
  eras=("${Era_2023[@]}")
else
  echo "Error: The year is incorrect."
fi

for e in "${eras[@]}"; do
    source prepare_condor.sh $e $year $delta
done

echo "End preparation ... beginning submission"
sleep 2

for e in "${eras[@]}"; do
    cd "${year}_era${e}"
    source submit_era.sh
    echo ""
    cd ..
    sleep 1
done
