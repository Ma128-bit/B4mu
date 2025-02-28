# !/bin/sh
# Usage:
#    prepare_and_submit_MC.sh <Year> <Analysis_type> <Delta>

helpstring="Usage:
prepare_and_submit_MC.sh [Year] [Analysis_type] [Delta]"

year=$1
analysis_type=$2
delta=$3

# Check inputs
if [ -z ${3+x} ]; then
    echo -e ${helpstring}
    return
fi

declare -a MC_name2022=("MC_pre" "MC_post")
declare -a MC_name2024=("MC")

if [ "${year}" == "2022" ]; then
  MC_name=("${MC_name2022[@]}")
elif [ "${year}" == "2023" ]; then
  MC_name=("${MC_name2022[@]}")
elif [ "${year}" == "2024" ]; then
  MC_name=("${MC_name2024[@]}")
else
  echo "Error: The year is incorrect."
  return
fi

declare -a MC_B4mu=("Bd_4mu" "Bs_4mu" "BsJPsiPhi")
declare -a MC_B2mu2K=("B2mu2K")
declare -a MC_B2muKpi=("B2muKpi")

if [ "${analysis_type}" == "B4mu" ]; then
  MC_id=("${MC_B4mu[@]}")
elif [ "${analysis_type}" == "B2mu2K" ]; then
  MC_id=("${MC_B2mu2K[@]}")
elif [ "${analysis_type}" == "B2muKpi" ]; then
  MC_id=("${MC_B2muKpi[@]}")
else
  echo "Error: The analysis_type is incorrect."
  return
fi

for e in "${MC_name[@]}"; do
    source prepare_condor.sh $e $year $analysis_type $delta
done

echo "End preparation ... beginning submission"
sleep 1

cd $analysis_type
for e in "${MC_name[@]}"; do
    cd "${year}_${e}"
    for id in "${MC_id[@]}"; do
        cd "${id}"
        condor_submit -name ettore submit.condor
        cd ..
    done
    echo ""
    cd ..
    sleep 1
done
cd ..
