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

declare -a C_2022=("240109_094521" "240109_094534" "240109_094548" "240109_094603" "240109_094618" "240109_094632" "240109_094646" "240109_094700")
declare -a D_v1_2022=("240109_094748" "240109_094807" "240109_094821" "240109_094834" "240109_094849" "240109_094902" "240109_094916" "240109_094929")
declare -a D_v2_2022=("240109_094947" "240109_095001" "240109_095016" "240109_095032" "240109_095045" "240109_095059" "240109_095113" "240109_095126")
declare -a E_2022=("240109_095146" "240109_095200" "240109_095214" "240109_095229" "240109_095243" "240109_095258" "240109_095313" "240109_095328")
declare -a F_2022=("240109_090914" "240109_090926" "240109_090939" "240109_090952" "240109_091004" "240109_091017" "240109_091029" "240109_091042")
declare -a G_2022=("240109_091157" "240109_091209" "240109_091222" "240109_091234" "240109_091247" "240109_091300" "240109_091313" "240109_091325")

declare -a C_v1_2023=("240109_091351" "240109_091404" "240109_091417" "240109_091429" "240109_091443" "240109_091456" "240109_091511" "240109_091525")
declare -a C_v2_2023=("240109_091612" "240109_091626" "240109_091641" "240109_091655" "240109_091711" "240109_091728" "240109_091742" "240109_091755")
declare -a C_v3_2023=("240109_092227" "240109_092241" "240109_092256" "240109_092310" "240109_092324" "240109_092338" "240109_092351" "240109_092405")
declare -a C_v4_2023=("240109_092455" "240109_092509" "240109_092523" "240109_092537" "240109_092551" "240109_092603" "240109_092617" "240109_092630")
declare -a D_v1_2023=("240109_092702" "240109_092715" "240109_092727" "240109_092740" "240109_092752" "240109_092805" "240109_092818" "240109_092831")
declare -a D_v2_2023=("240109_092903" "240109_092916" "240109_092928" "240109_092941" "240109_092954" "240109_093007" "240109_093019" "240109_093032")

declare -a MC22_B4mu_pre=("Dataset_prova1" "Dataset_prova2")
declare -a MC22_B4mu_post=("" "")
declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu")

declare -a MC22_BsJPsiPhi_pre=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_pre_BsJPsiPhi_Mini/240112_141848")
declare -a MC22_BsJPsiPhi_post=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_post_BsJPsiPhi_Mini/240112_140733")
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
        sed -i "s#TRUEFALSE#False#g" "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"
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
        for j in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/${i}/000${j}/" | wc -l)
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
        sed -i "s#TRUEFALSE#True#g" "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        chmod a+x "${home_directory}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        
        echo -n "."
        ((j++))
        sleep 1
    done
fi
echo " Done!"


