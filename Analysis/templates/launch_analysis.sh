# !/bin/sh
# Usage:
#    prepare_condor.sh <index>

helpstring="Usage:
prepare_condor.sh [index]"
index=$1

# Check inputs
if [ -z ${1+x} ]; then
  echo -e ${helpstring}
  return
fi

source /cvmfs/sft.cern.ch/lcg/views/dev3/latest/x86_64-centos7-gcc11-opt/setup.sh
python3 PATH/Analizer.py --index $index --delta DELTAVAL --directory_IN INPUT_DIR --directory_OUT OUTPUT_DIR

