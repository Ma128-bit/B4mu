# !/bin/sh
# Usage:
#    prepare_and_submit_ALL.sh <Year>

helpstring="Usage:
prepare_and_submit_ALL.sh [Year] [Analysis_type] [Label]"

year=$1
analysis_type=$2
label=$3

# Check inputs
if [ -z ${3+x} ]; then
    echo -e ${helpstring}
    return
fi


if [ ! -d "FinalFiles_${analysis_type}_${label}" ]; then
    mkdir -p "FinalFiles_${analysis_type}_${label}"
fi
if [ "${analysis_type}" == "B4mu" ]; then
    hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_Bd_4mu_${year}.root ${analysis_type}/${year}_MC*/Bd_4mu/Analyzed_Data_*.root
    hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_Bs_4mu_${year}.root ${analysis_type}/${year}_MC*/Bs_4mu/Analyzed_Data_*.root
    hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_BsJPsiPhi_${year}.root ${analysis_type}/${year}_MC*/BsJPsiPhi/Analyzed_Data_*.root
#else
    #hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/${analysis_type}/Analyzed_Data_*.root
fi

if [ "${analysis_type}" == "B2mu2K" ]; then
    hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*_new*/${analysis_type}*/Analyzed_Data_*.root
    #hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_Kpi_with_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/B2muKpi/Analyzed_Data_*.root
elif [ "${analysis_type}" == "B2muKpi" ]; then
    hadd FinalFiles_${analysis_type}_${label}/Analyzed_MC_2K_with_${analysis_type}_${year}.root ${analysis_type}/${year}_MC_p*/B2mu2K/Analyzed_Data_*.root
fi
