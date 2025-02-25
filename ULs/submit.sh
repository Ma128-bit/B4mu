#!/bin/sh

helpstring="Usage:
submit.sh [index]"
ind=$1

# Check inputs
if [ -z ${1+x} ]; then
  echo -e ${helpstring}
  return
fi

export LC_TIME="en_US.UTF-8"
current_datetime=$(date "+%a %b %d %H:%M:%S %Y")
echo "$current_datetime -- Starting Job"

echo "Hostname: $(hostname)"
#source /cvmfs/sft.cern.ch/lcg/views/dev3/latest/x86_64-centos7-gcc11-opt/setup.sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /lustrehome/mbuonsante/B_4mu/Combine/CMSSW_14_1_0_pre4/src/B4muLimits
cmsenv

#cp RootFiles/Dataset.root /lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root
mkdir Out_${ind}/Workspaces
mkdir Out_${ind}/Plots
mkdir Out_${ind}/RootFiles
mkdir Out_${ind}/Datacards

python3 CreateDatacards.py --config Out_${ind}/config_scan_${ind}.json --Bs "1.0e-10" --Bd "4.0e-12" --best_cut true --BDT_uncBool true --index $ind --inputfile_loc "/lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root"

cd /lustrehome/mbuonsante/B_4mu/Combine/CMSSW_14_1_0_pre4/src/B4muLimits

cd Out_${ind}/Datacards

pwd

combineCards.py datacardA1.txt datacardA2.txt > datacardA.txt
combineCards.py datacardB1.txt datacardB2.txt > datacardB.txt
combineCards.py datacardC1.txt datacardC2.txt > datacardC.txt

combine -M AsymptoticLimits --cl 0.95 datacardA.txt --run blind >> ../output_combineA.txt
combine -M AsymptoticLimits --cl 0.95 datacardB.txt --run blind >> ../output_combineB.txt
combine -M AsymptoticLimits --cl 0.95 datacardC.txt --run blind >> ../output_combineC.txt

rm /lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root
