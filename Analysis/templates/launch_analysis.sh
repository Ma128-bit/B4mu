#!/bin/sh
# Usage:
#    launch_analysis.sh <index>

helpstring="Usage:
launch_analysis.sh [index]"
index=$1

# Check inputs
if [ -z ${1+x} ]; then
  echo -e ${helpstring}
  return
fi

export LC_TIME="en_US.UTF-8"
current_datetime=$(date "+%a %b %d %H:%M:%S %Y")
echo "$current_datetime -- Starting Job"

echo "Hostname: $(hostname)"
source /cvmfs/sft.cern.ch/lcg/views/LCG_107/x86_64-el9-gcc14-opt/setup.sh
cd PATH
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#cmsenv

python3 Analizer.py --index $index --delta DELTAVAL --directory_IN INPUT_DIR --directory_OUT OUTPUT_DIR --isMC TRUEFALSE --analysis_type ANALYSISTYPE
