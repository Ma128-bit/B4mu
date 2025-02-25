#!/bin/sh

helpstring="Usage:
submit.sh [index]"
ind=$1
BsBR=$2
BdBR=$3

# Check inputs
if [ -z ${3+x} ]; then
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

echo ${ind}
#cp RootFiles/Dataset.root /lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root
mkdir Out_${ind}/Workspaces
mkdir Out_${ind}/Plots
mkdir Out_${ind}/RootFiles
mkdir Out_${ind}/Datacards

python3 CreateDatacards.py --config configs/config_20_01_25_10%.json --Bs $BsBR --Bd $BdBR --best_cut true --index $ind --inputfile_loc "/lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root" --multipdf true

cd Out_${ind}/Datacards

combine -M AsymptoticLimits --cl 0.95 datacard_combined.txt --run blind >> ../output_combine.txt

rm /lustre/cms/store/user/mbuonsan/ScanFile/Dataset_${ind}.root

current_datetime=$(date "+%a %b %d %H:%M:%S %Y")
echo "$current_datetime -- Ending Job"