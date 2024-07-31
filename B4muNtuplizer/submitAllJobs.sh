#!/bin/bash
# Usage:
#    submitAllJobs.sh

helpstring="Usage:
submitAllJobs.sh [Year]"

year=$1

if [ -z ${1+x} ]; then
    echo -e ${helpstring}
    return
fi

declare -a era2022preE=(("C" "D-v1" "D-v2" "E")
declare -a era2022postE=("F" "G")
declare -a era2023=("B" "C-v1" "C-v2" "C-v3" "C-v4" "D-v1" "D-v2")

current_dir=$(pwd)

if [[ "$current_dir" == *"CMSSW_13_"* ]]; then
    if [[ "$year" == "2022" ]]; then
        era=("${era2022postE[@]}")
    elif [[ "$year" == "2023" ]]; then
        era=("${era2023[@]}")
    else
        return
    fi
elif [[ "$current_dir" == *"CMSSW_12_"* ]]; then
    if [[ "$year" == "2022" ]]; then
        era=("${era2022preE[@]}")
    else
        return
    fi
else
    return
fi

cd CrabSubmission
for i in "${era[@]}"; do
    echo "\nData $i"
    source submit_CRAB.sh ${i} 
    sleep 1
done
cd ..
