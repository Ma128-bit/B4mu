# !/bin/sh
# Usage:
#    prepare_and_submit_MC_new.sh <Year> <Delta>

helpstring="Usage:
prepare_and_submit_MC.sh [Year] [Delta]"

year=$1
delta=$2
analysis_type="B2mu2K"

# Check inputs
if [ -z ${2+x} ]; then
    echo -e ${helpstring}
    return
fi

declare -a MC_=("MC_pre_new" "MC_post_new")
declare -a MC_24=("MC_new")

declare -a MC_B2mu2K22=("B2mu2K_new" "B2mu2K_new_ext")
declare -a MC_B2mu2K23=("B2mu2K_new")

if [ "${year}" == "2022" ]; then
  MC_id=("${MC_B2mu2K22[@]}")
  MC_name=("${MC_[@]}")
elif [ "${year}" == "2023" ]; then
  MC_id=("${MC_B2mu2K23[@]}")
  MC_name=("${MC_[@]}")
elif [ "${year}" == "2024" ]; then
  MC_id=("${MC_B2mu2K23[@]}")
  MC_name=("${MC_24[@]}")
else
  echo "Error: The year is incorrect."
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
        condor_submit -name tmp submit.condor
        cd ..
    done
    echo ""
    cd ..
    sleep 1
done
cd ..
