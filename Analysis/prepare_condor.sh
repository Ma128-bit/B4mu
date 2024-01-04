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

declare -a C_2022=("231227_233109" "231227_233117" "231227_233125" "231227_233135" "231227_233144" "231227_233152" "231227_233200" "231227_233209")
declare -a D_v1_2022=("231227_233237" "231227_233245" "231227_233253" "231227_233301" "231227_233308" "231227_233317" "231227_233324" "231227_233332")
declare -a D_v2_2022=("231227_233352" "231227_233359" "231227_233408" "231227_233415" "231227_233423" "231227_233431" "231227_233438" "231227_233446")
declare -a E_2022=("231227_233522" "231227_233529" "231227_233537" "231227_233544" "231227_233553" "231227_233600" "231227_233609" "231227_233617")
declare -a F_2022=("231227_231452" "231227_231459" "231227_231507" "231227_231513" "231227_231521" "231227_231528" "231227_231535" "231227_231542")
declare -a G_2022=("231227_231616" "231227_231623" "231227_231631" "231227_231637" "231227_231645" "231227_231652" "231227_231659" "231227_231706")

declare -a C_v1_2023=("231227_231733" "231227_231740" "231227_231747" "231227_231754" "231227_231801" "231227_231808" "231227_231815" "231227_231822")
declare -a C_v2_2023=("231227_231849" "231227_231856" "231227_231904" "231227_231911" "231227_231918" "231227_231925" "231227_231932" "231227_231940")
declare -a C_v3_2023=("231227_232018" "231227_232025" "231227_232032" "231227_232039" "231227_232046" "231227_232054" "231227_232101" "231227_232108")
declare -a C_v4_2023=("231227_232200" "231227_232206" "231227_232213" "231227_232220" "231227_232227" "231227_232234" "231227_232241" "231227_232248")
declare -a D_v1_2023=("231227_232321" "231227_232328" "231227_232334" "231227_232341" "231227_232348" "231227_232354" "231227_232401" "231227_232408")
declare -a D_v2_2023=("231227_232445" "231227_232452" "231227_232459" "231227_232506" "231227_232513" "231227_232519" "231227_232526" "231227_232533")

declare -a Pre_E_MC22=("Dataset_prova1" "Dataset_prova2")
declare -a Post_E_MC22=("" "")

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
      MC_pre)
        datasets=("${Pre_E_MC22[@]}")
        ;;
      MC_post)
        datasets=("${Post_E_MC22[@]}")
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
        chmod a+x "${home_directory}/${year}_era${era}/stream_${i}/launch_analysis.sh"

        cp templates/submit_era.sh "${home_directory}/${year}_era${era}/stream_${i}"
        cp templates/hadd_era.sh "${home_directory}/${year}_era${era}/stream_${i}"
        sed -i "s#YEARNAME#${year}#g" "${home_directory}/${year}_era${era}/stream_${i}/*_era.sh"
        sed -i "s#ERANAME#${era}#g" "${home_directory}/${year}_era${era}/stream_${i}/*_era.sh"
        sleep 1
    done
fi


