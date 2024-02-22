# !/bin/sh
# Usage:
#    prepare_condor.sh <Era> <Year>

helpstring="Usage:
prepare_condor.sh [Era] [Year] [Delta]"
era=$1
year=$2
delta=$3

# Check inputs
if [ -z ${3+x} ]; then
    echo -e ${helpstring}
    return
fi

file_directory="/lustre/cms/store/user/mbuonsan"

declare -a C_2022=("240202_220729" "240202_220749" "240202_220808" "240202_220825" "240202_220842" "240202_220901" "240202_220919" "240202_220941")
declare -a D_v1_2022=("240202_221016" "240202_221032" "240202_221053" "240202_221117" "240202_221137" "240202_221153" "240202_221209" "240202_221227")
declare -a D_v2_2022=("240202_221253" "240202_221315" "240202_221333" "240202_221350" "240202_221406" "240202_221424" "240202_221444" "240202_221504")
declare -a E_2022=("240202_221528" "240202_221544" "240202_221603" "240202_221626" "240202_221648" "240202_221711" "240202_221733" "240202_221754")
declare -a F_2022=("240202_214033" "240202_214056" "240202_214123" "240202_214144" "240202_214203" "240202_214220" "240202_214241" "240202_214301")
declare -a G_2022=("240202_214330" "240202_214350" "240202_214408" "240202_214425" "240202_214445" "240202_214510" "240202_214530" "240202_214548")

declare -a C_v1_2023=("240202_214619" "240202_214642" "240202_214706" "240202_214725" "240202_214742" "240202_214801" "240202_214819" "240202_214840")
declare -a C_v2_2023=("240202_214911" "240202_214928" "240202_214943" "240202_215001" "240202_215021" "240202_215041" "240202_215101" "240202_215118")
declare -a C_v3_2023=("240202_215212" "240202_215234" "240202_215303" "240202_215324" "240202_215342" "240202_215358" "240202_215419" "240202_215438")
declare -a C_v4_2023=("240202_215603" "240202_215622" "240202_215648" "240202_215706" "240202_215722" "240202_215737" "240202_215753" "240202_215815")
declare -a D_v1_2023=("240202_215948" "240202_220024" "240202_220041" "240202_220058" "240202_220115" "240202_220134" "240202_220152" "240202_220214")
declare -a D_v2_2023=("240202_220243" "240202_220259" "240202_220320" "240202_220339" "240202_220358" "240202_220415" "240202_220433" "240202_220451")

declare -a MC22_B4mu_pre=("Dataset_prova1" "Dataset_prova2")
declare -a MC22_B4mu_post=("" "")
declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu")

declare -a MC22_BsJPsiPhi_pre=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_pre_BsJPsiPhi_Mini/240221_224821")
#declare -a MC22_BsJPsiPhi_post=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_post_BsJPsiPhi_Mini/240129_201539")
declare -a MC22_BsJPsiPhi_post=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_post_BsJPsiPhi_Mini/240221_224937")

declare -a BsJPsiPhi_MC_label=("BsJPsiPhi")


if [ "${year}" == "2022" ]; then
    case "$era" in
      C)
        datasets=("${C_2022[@]}")
        ;;
      D-v1)
        datasets=("${D_v1_2022[@]}")
        ;;
      D-v2)
        datasets=("${D_v2_2022[@]}")
        ;;
      E)
        datasets=("${E_2022[@]}")
        ;;
      F)
        datasets=("${F_2022[@]}")
        ;;
      G)
        datasets=("${G_2022[@]}")
        ;;
      MC_B4mu_pre)
        datasets=("${MC22_B4mu_pre[@]}")
        label=("${B4mu_MC_label[@]}")
        ;;
      MC_B4mu_post)
        datasets=("${MC22_B4mu_post[@]}")
        label=("${B4mu_MC_label[@]}")
        ;;
      MC_BsJPsiPhi_pre)
        datasets=("${MC22_BsJPsiPhi_pre[@]}")
        label=("${BsJPsiPhi_MC_label[@]}")
        ;;
      MC_BsJPsiPhi_post)
        datasets=("${MC22_BsJPsiPhi_post[@]}")
        label=("${BsJPsiPhi_MC_label[@]}")
        ;;
      *)
        echo "Error: The era is incorrect."
        return
        ;;
    esac
elif [ "${year}" == "2023" ]; then
    case "$era" in
      C-v1)
        datasets=("${C_v1_2023[@]}")
        ;;
      C-v2)
        datasets=("${C_v2_2023[@]}")
        ;;
      C-v3)
        datasets=("${C_v3_2023[@]}")
        ;;
      C-v4)
        datasets=("${C_v4_2023[@]}")
        ;;
      D-v1)
        datasets=("${D_v1_2023[@]}")
        ;;
      D-v2)
        datasets=("${D_v2_2023[@]}")
        ;;
      *)
        echo "Error: The era is incorrect."
        return
        ;;
    esac
else
    echo "Error: The year is incorrect."
    return
fi


home_directory="$PWD"

if [[ "$era" != *"MC"* ]]; then
    if [ ! -d "${home_directory}/${year}_era${era}" ]; then
        mkdir -p "${home_directory}/${year}_era${era}"
    fi
    echo "Data ${year} - era ${era} is selected"
    cp templates/submit_era.sh "${home_directory}/${year}_era${era}"
    cp templates/hadd_era.sh "${home_directory}/${year}_era${era}"
    sed -i "s#YEARNAME#${year}#g" "${home_directory}/${year}_era${era}/submit_era.sh"
    sed -i "s#ERANAME#${era}#g" "${home_directory}/${year}_era${era}/submit_era.sh"
    sed -i "s#YEARNAME#${year}#g" "${home_directory}/${year}_era${era}/hadd_era.sh"
    sed -i "s#ERANAME#${era}#g" "${home_directory}/${year}_era${era}/hadd_era.sh"

    for i in {0..7}; do
        if [ ! -d "${home_directory}/${year}_era${era}/stream_${i}" ]; then
            mkdir -p "${home_directory}/${year}_era${era}/stream_${i}"
            mkdir -p "${home_directory}/${year}_era${era}/stream_${i}/log"
        fi

        cp templates/submit.condor "${home_directory}/${year}_era${era}/stream_${i}"
        ndir=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/" | wc -l)
        tot=0
        for j in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/000${j}/" | wc -l)
            tot=$((tot + nfiles))
        done
        #echo "nfiles=${tot}"
        number_of_splits=$(((${tot} / ${delta}) + 1))
        echo "queue ${number_of_splits}" >> "${home_directory}/${year}_era${era}/stream_${i}/submit.condor"
        sed -i "s#PATH#${home_directory}/${year}_era${era}/stream_${i}#g" "${home_directory}/${year}_era${era}/stream_${i}/submit.condor"
        chmod a+x "${home_directory}/${year}_era${era}/stream_${i}/submit.condor"
        
        cp templates/launch_analysis.sh "${home_directory}/${year}_era${era}/stream_${i}"
        sed -i "s#PATH#${home_directory}#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#DELTAVAL#${delta}#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#INPUT_DIR#${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#OUTPUT_DIR#${home_directory}/${year}_era${era}/stream_${i}#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#TRUEFALSE#0#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        chmod a+x "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        
        echo -n "."
        sleep 1
    done
else
    if [ ! -d "${home_directory}/${year}_${era}" ]; then
        mkdir -p "${home_directory}/${year}_${era}"
    fi
    echo "Data ${year} - ${era} is selected"
    j=0
    for i in "${datasets[@]}"; do
        if [ ! -d "${home_directory}/${year}_${era}/${label[${j}]}" ]; then
            mkdir -p "${home_directory}/${year}_${era}/${label[${j}]}"
            mkdir -p "${home_directory}/${year}_${era}/${label[${j}]}/log"
        fi
        cp templates/submit.condor "${home_directory}/${year}_${era}/${label[${j}]}"
        ndir=$(ls "${file_directory}/${i}/" | wc -l)
        tot=0
        for k in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/${i}/000${k}/" | wc -l)
            tot=$((tot + nfiles))
        done
        number_of_splits=$(((${tot} / ${delta}) + 1))
        echo "queue ${number_of_splits}" >> "${home_directory}/${year}_${era}/${label[${j}]}/submit.condor"
        sed -i "s#PATH#${home_directory}/${year}_${era}/${label[${j}]}#g" "${home_directory}/${year}_${era}/${label[${j}]}/submit.condor"
        chmod a+x "${home_directory}/${year}_${era}/${label[${j}]}/submit.condor"
        
        cp templates/launch_analysis.sh "${home_directory}/${year}_${era}/${label[${j}]}"
        sed -i "s#PATH#${home_directory}#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#DELTAVAL#${delta}#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#INPUT_DIR#${file_directory}/${i}#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#OUTPUT_DIR#${home_directory}/${year}_${era}/${label[${j}]}#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#TRUEFALSE#1#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        chmod a+x "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        
        echo -n "."
        ((j++))
        sleep 1
    done
fi
echo " Done!"


