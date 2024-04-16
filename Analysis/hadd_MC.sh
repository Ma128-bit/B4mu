# !/bin/sh
# Usage:
#    prepare_and_submit_ALL.sh <Year>

helpstring="Usage:
prepare_and_submit_ALL.sh [Year] [Analysis_type]"

year=$1
analysis_type=$2

# Check inputs
if [ -z ${2+x} ]; then
    echo -e ${helpstring}
    return
fi


if [ ! -d "FinalFiles_${analysis_type}" ]; then
    mkdir -p "FinalFiles_${analysis_type}"
fi

hadd FinalFiles_${analysis_type}/Analyzed_MC_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/${analysis_type}/Analyzed_Data_*.root

if [${analysis_type}=="B2mu2K"]; then
    hadd FinalFiles_${analysis_type}/Analyzed_MC_Kpi_with_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/B2muKpi/Analyzed_Data_*.root
elif [${analysis_type}=="B2muKpi"]; then
    hadd FinalFiles_${analysis_type}/Analyzed_MC_2K_with_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/B2mu2K/Analyzed_Data_*.root
fi
