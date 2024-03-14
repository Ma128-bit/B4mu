# !/bin/sh
# Usage:
#    prepare_condor.sh <Era> <Year> <Analysis_type> <Delta>

helpstring="Usage:
prepare_condor.sh [Era] [Year] [Analysis_type] [Delta]"
era=$1
year=$2
analysis=$3
delta=$4

# Check inputs
if [ -z ${4+x} ]; then
    echo -e ${helpstring}
    return
fi

if [ "${analysis}" == "Bs2mu2k" ]; then
    Analysis_type="B2mu2trk"
elif [ "${analysis}" == "Bd2mukpi" ]; then
    Analysis_type="B2mu2trk"
elif [ "${analysis}" == "B4mu" ]; then
    Analysis_type="B4mu"
else
    echo "Error: The Analysis_type is incorrect."
    return
fi

file_directory="/lustre/cms/store/user/mbuonsan"

declare -a C_2022=("240222_113218" "240222_113247" "240222_113323" "240222_113355" "240222_113426" "240222_113500" "240222_113536" "240222_113608")
declare -a D_v1_2022=("240222_113739" "240222_113806" "240222_113841" "240222_113921" "240222_114004" "240222_114035" "240222_114107" "240222_114132")
declare -a D_v2_2022=("240222_114204" "240222_114228" "240222_114258" "240222_114330" "240222_114359" "240222_114438" "240222_114508" "240222_114538")
declare -a E_2022=("240222_114640" "240222_114709" "240222_114736" "240222_114807" "240222_114835" "240222_114903" "240222_114930" "240222_114958")
declare -a F_2022=("240221_222028" "240221_222042" "240221_222058" "240222_111910" "240222_102944" "240221_222149" "240221_222204" "240221_222219")
declare -a G_2022=("240221_222246" "240221_222308" "240221_222324" "240221_222340" "240221_222356" "240221_222410" "240221_222426" "240221_222442")

declare -a C_v1_2023=("240221_222514" "240221_222529" "240222_103359" "240221_222557" "240221_222615" "240221_222633" "240221_222647" "240221_222702")
declare -a C_v2_2023=("240221_222723" "240221_222740" "240221_222757" "240221_222813" "240221_222827" "240221_222844" "240221_222858" "240221_222912")
declare -a C_v3_2023=("240221_222957" "240221_223012" "240221_223027" "240221_223042" "240221_223058" "240221_223116" "240221_223130" "240221_223145")
declare -a C_v4_2023=("240221_223205" "240221_223219" "240221_223236" "240221_223253" "240221_223309" "240222_111635" "240221_223338" "240221_223355")
declare -a D_v1_2023=("240221_223853" "240221_223908" "240222_103703" "240222_103751" "240222_103825" "240221_224014" "240221_224028" "240221_224042")
declare -a D_v2_2023=("240221_224146" "240221_224200" "240221_224215" "240221_224232" "240221_224250" "240221_224304" "240221_224318" "240221_224332")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_B4mu_pre_Bd_Mini/240229_131755" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_B4mu_pre_Bs_Mini/240229_131808")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_B4mu_post_Bd_Mini/240229_131939" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_B4mu_post_Bs_Mini/240229_131952")
declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu")

declare -a MC22_BsJPsiPhi_pre=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_pre_BsJPsiPhi_Mini/240221_224821")
declare -a MC22_BsJPsiPhi_post=("BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_BsJPsiPhi_post_BsJPsiPhi_Mini/240221_224937")
declare -a BsJPsiPhi_MC_label=("BsJPsiPhi")

declare -a MC22_B2mu2trk_pre=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bs2mu2K_Mini/240311_160420" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bd2muKpi_Mini/240311_160443")
declare -a MC22_B2mu2trk_post=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bs2mu2K_Mini/240311_160529" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bd2muKpi_Mini/240311_160548")
declare -a B2mu2trk_MC_label=("Bs2mu2K" "Bd2muKpi")

if [ "${Analysis_type}" == "B4mu" ]; then
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
elif [ "${Analysis_type}" == "B2mu2trk" ]; then
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
          MC_B2mu2trk_pre)
            datasets=("${MC22_B2mu2trk_pre[@]}")
            label=("${B2mu2trk_MC_label[@]}")
            ;;
          MC_B2mu2trk_post)
            datasets=("${MC22_B2mu2trk_post[@]}")
            label=("${B2mu2trk_MC_label[@]}")
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
fi


home_directory="$PWD"

if [[ "$era" != *"MC"* ]]; then
    if [ ! -d "${home_directory}/${Analysis_type}/${year}_era${era}" ]; then
        mkdir -p "${home_directory}/${Analysis_type}/${year}_era${era}"
    fi
    echo "Data ${year} - era ${era} is selected"
    cp templates/submit_era.sh "${home_directory}/${Analysis_type}/${year}_era${era}"
    cp templates/hadd_era.sh "${home_directory}/${Analysis_type}/${year}_era${era}"
    sed -i "s#YEARNAME#${year}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/submit_era.sh"
    sed -i "s#ERANAME#${era}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/submit_era.sh"
    sed -i "s#YEARNAME#${year}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/hadd_era.sh"
    sed -i "s#ERANAME#${era}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/hadd_era.sh"

    for i in {0..7}; do
        if [ ! -d "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}" ]; then
            mkdir -p "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}"
            mkdir -p "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/log"
        fi

        cp templates/submit.condor "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}"
        ndir=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/" | wc -l)
        tot=0
        for j in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/000${j}/" | wc -l)
            tot=$((tot + nfiles))
        done
        #echo "nfiles=${tot}"
        number_of_splits=$(((${tot} / ${delta}) + 1))
        echo "queue ${number_of_splits}" >> "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        sed -i "s#PATH#${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        
        cp templates/launch_analysis.sh "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}"
        sed -i "s#PATH#${home_directory}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#DELTAVAL#${delta}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#INPUT_DIR#${file_directory}/ParkingDoubleMuonLowMass${i}/SkimB4Mu_${year}era${era}_stream${i}_Mini/${datasets[${i}]}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#OUTPUT_DIR#${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#TRUEFALSE#0#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#ANALYSISTYPE#${analysis}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        
        echo -n "."
        sleep 1
    done
else
    if [ ! -d "${home_directory}/${Analysis_type}/${year}_${era}" ]; then
        mkdir -p "${home_directory}/${Analysis_type}/${year}_${era}"
    fi
    echo "Data ${year} - ${era} is selected"
    j=0
    for i in "${datasets[@]}"; do
        if [ ! -d "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}" ]; then
            mkdir -p "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}"
            mkdir -p "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/log"
        fi
        cp templates/submit.condor "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}"
        ndir=$(ls "${file_directory}/${i}/" | wc -l)
        tot=0
        for k in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/${i}/000${k}/" | wc -l)
            tot=$((tot + nfiles))
        done
        number_of_splits=$(((${tot} / ${delta}) + 1))
        echo "queue ${number_of_splits}" >> "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/submit.condor"
        sed -i "s#PATH#${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/submit.condor"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/submit.condor"
        
        cp templates/launch_analysis.sh "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}"
        sed -i "s#PATH#${home_directory}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#DELTAVAL#${delta}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#INPUT_DIR#${file_directory}/${i}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#OUTPUT_DIR#${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#TRUEFALSE#1#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#ANALYSISTYPE#${analysis}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        
        echo -n "."
        ((j++))
        sleep 1
    done
fi
echo " Done!"


