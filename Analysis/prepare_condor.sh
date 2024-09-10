# !/bin/sh
# Usage:
#    prepare_condor.sh <Era> <Year> <Analysis_type> <Delta>

helpstring="Usage:
prepare_condor.sh [Era] [Year] [Analysis_type] [Delta]"
era=$1
year=$2
Analysis_type=$3
delta=$4

# Check inputs
if [ -z ${4+x} ]; then
    echo -e ${helpstring}
    return
fi

if [ "${Analysis_type}" == "B2mu2K" ]; then
    Ana_temp="B2Mu2K"
elif [ "${Analysis_type}" == "B2muKpi" ]; then
    Ana_temp="B2Mu2K"
elif [ "${Analysis_type}" == "B4mu" ]; then
    Ana_temp="B4Mu"
else
    echo "Error: The Analysis_type is incorrect."
    return
fi

file_directory="/lustre/cms/store/user/mbuonsan"

declare -a C_2022=("240731_160611" "240731_160630" "240731_160644" "240731_162051" "240731_160721" "240731_160745" "240731_160804" "240731_160818")
declare -a D_v1_2022=("240731_160834" "240731_160859" "240731_160917" "240731_160933" "240731_160948" "240731_161012" "240731_161027" "240731_161042")
declare -a D_v2_2022=("240731_161059" "240731_161122" "240731_161136" "240731_161149" "240731_161205" "240731_161223" "240731_161240" "240731_161253")
declare -a E_2022=("240731_161312" "240731_161332" "240731_161352" "240731_161411" "240731_161426" "240731_161440" "240731_161506" "240731_161522")
declare -a F_2022=("240731_153720" "240731_153734" "240731_153753" "240731_153810" "240731_153823" "240731_153837" "240731_153854" "240731_153916")
declare -a G_2022=("240731_153931" "240731_153945" "240731_153959" "240731_154014" "240731_154037" "240731_154052" "240731_154111" "240731_154127")

declare -a B_2023=("240731_154536" "240731_154557" "240731_154619" "240731_154643" "240731_154707" "240731_154721" "240731_154741" "240731_154756")
declare -a C_v1_2023=("240731_154810" "240731_154827" "240731_154847" "240731_154902" "240731_154915" "240731_154931" "240731_154944" "240731_155009")
declare -a C_v2_2023=("240731_155026" "240731_155044" "240731_155100" "240731_155117" "240731_155133" "240731_155149" "240731_155206" "240731_155229")
declare -a C_v3_2023=("240731_155245" "240731_155304" "240731_155320" "240731_155344" "240731_155401" "240731_155427" "240731_155454" "240731_155509")
declare -a C_v4_2023=("240731_155534" "240731_155557" "240731_155613" "240731_155630" "240731_155654" "240731_155714" "240731_155729" "240731_155745")
declare -a D_v1_2023=("240731_155815" "240731_155830" "240731_155844" "240731_155858" "240731_155923" "240731_155938" "240731_155952" "240731_160008")
declare -a D_v2_2023=("240731_160031" "240731_160049" "240731_160104" "240731_160117" "240731_160138" "240731_160158" "240731_160213" "240731_160227")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/240731_153207" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/240731_153225" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/240731_153250")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/240731_153311" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/240731_153325" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/240731_153342")

declare -a MC23_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/240731_153505" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/240731_153523" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/240731_153546")
declare -a MC23_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/240731_153610" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/240731_153630" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/240731_153650")

declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu" "BsJPsiPhi")

declare -a Control_C_2022=("240603_215841" "240603_215858" "240603_215917" "240603_220014" "240603_220041" "240603_220119" "240603_220145" "240603_220236")
declare -a Control_D_v1_2022=("240603_220353" "240603_220435" "240603_220556" "240603_220657" "240603_220806" "240603_220903" "240603_220957" "240603_221031")
declare -a Control_D_v2_2022=("240603_221217" "240603_221327" "240603_221411" "240603_221446" "240603_221519" "240603_221606" "240603_221638" "240603_221706")
declare -a Control_E_2022=("240603_222837" "240603_222900" "240603_222926" "240603_223006" "240603_223038" "240603_223109" "240603_223142" "240603_223218")
declare -a Control_F_2022=("240603_152518" "240603_152549" "240603_152622" "240603_152653" "240603_152723" "240603_152753" "240603_152827" "240603_153011")
declare -a Control_G_2022=("240603_153233" "240603_153311" "240603_153342" "240603_153411" "240603_153437" "240603_153507" "240603_153548" "240603_153618")

declare -a Control_B_2023=("240603_153715" "240603_153800" "240603_153847" "240603_153930" "240603_154008" "240603_154045" "240603_154127" "240603_154213")
declare -a Control_C_v1_2023=("240603_154805" "240603_154843" "240603_154928" "240603_155008" "240603_155047" "240603_155132" "240603_155213" "240603_155303")
declare -a Control_C_v2_2023=("240603_155506" "240603_155530" "240603_155600" "240603_155633" "240603_155705" "240603_155743" "240603_155810" "240603_155837")
declare -a Control_C_v3_2023=("240603_175813" "240603_175829" "240603_175844" "240603_175858" "240603_175914" "240603_175928" "240603_175944" "240603_175959")
declare -a Control_C_v4_2023=("240603_180749" "240603_180804" "240603_180820" "240603_180837" "240603_180853" "240603_180919" "240603_180937" "240603_180954")
declare -a Control_D_v1_2023=("240603_181222" "240603_181239" "240603_181255" "240603_181310" "240603_181326" "240603_181341" "240603_181357" "240603_181412")
declare -a Control_D_v2_2023=("240603_183924" "240603_183940" "240603_183956" "240603_184013" "240603_184028" "240603_184044" "240603_184059" "240603_184115")

declare -a MC22_B2mu2trk_pre=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bs2mu2K_Mini/240603_193559" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bd2muKpi_Mini/240603_193615")
declare -a MC22_B2mu2trk_post=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bs2mu2K_Mini/240603_193940" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bd2muKpi_Mini/240603_193955")
declare -a B2mu2trk_MC_label=("B2mu2K" "B2muKpi")

if [ "${Ana_temp}" == "B4Mu" ]; then
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
            datasets=("${MC22_B4mu_pre[@]}")
            label=("${B4mu_MC_label[@]}")
            ;;
          MC_post)
            datasets=("${MC22_B4mu_post[@]}")
            label=("${B4mu_MC_label[@]}")
            ;;
          *)
            echo "Error: The era is incorrect."
            return
            ;;
        esac
    elif [ "${year}" == "2023" ]; then
        case "$era" in
          B)
            datasets=("${B_2023[@]}")
            ;;
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
          MC_pre)
            datasets=("${MC23_B4mu_pre[@]}")
            label=("${B4mu_MC_label[@]}")
            ;;
          MC_post)
            datasets=("${MC23_B4mu_post[@]}")
            label=("${B4mu_MC_label[@]}")
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
elif [ "${Ana_temp}" == "B2Mu2K" ]; then
    if [ "${year}" == "2022" ]; then
        case "$era" in
          C)
            datasets=("${Control_C_2022[@]}")
            ;;
          D-v1)
            datasets=("${Control_D_v1_2022[@]}")
            ;;
          D-v2)
            datasets=("${Control_D_v2_2022[@]}")
            ;;
          E)
            datasets=("${Control_E_2022[@]}")
            ;;
          F)
            datasets=("${Control_F_2022[@]}")
            ;;
          G)
            datasets=("${Control_G_2022[@]}")
            ;;
          MC_pre)
            datasets=("${MC22_B2mu2trk_pre[@]}")
            label=("${B2mu2trk_MC_label[@]}")
            ;;
          MC_post)
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
          B)
            datasets=("${Control_B_2023[@]}")
            ;;
          C-v1)
            datasets=("${Control_C_v1_2023[@]}")
            ;;
          C-v2)
            datasets=("${Control_C_v2_2023[@]}")
            ;;
          C-v3)
            datasets=("${Control_C_v3_2023[@]}")
            ;;
          C-v4)
            datasets=("${Control_C_v4_2023[@]}")
            ;;
          D-v1)
            datasets=("${Control_D_v1_2023[@]}")
            ;;
          D-v2)
            datasets=("${Control_D_v2_2023[@]}")
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
        ndir=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/" | wc -l)
        tot=$(find "${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}" -type f | wc -l)
        #wtot=$(du -sBG "${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}" | awk '{print $1}' | sed 's/G//')
        #w0=$(echo "$delta * 0.05 / 1" | bc)
        #number_of_splits=$(((${wtot} / ${w0}) + 1))
        #delta=$(((${tot} / ${number_of_splits}) + 1))
        #tot=0
        #for j in $(seq 0 $((ndir - 1))); do
        #    nfiles=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/000${j}/" | wc -l)
        #    tot=$((tot + nfiles))
        #done
        #echo "nfiles=${tot}"
        number_of_splits=$(((${tot} / ${delta}) + 1))
        echo "queue ${number_of_splits}" >> "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        sed -i "s#PATH#${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/submit.condor"
        
        cp templates/launch_analysis.sh "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}"
        sed -i "s#PATH#${home_directory}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#DELTAVAL#${delta}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#INPUT_DIR#${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#OUTPUT_DIR#${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#TRUEFALSE#0#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
        sed -i "s#ANALYSISTYPE#${Analysis_type}#g" "${home_directory}/${Analysis_type}/${year}_era${era}/stream_${i}/launch_analysis.sh"
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
        if [[ "$i" == *"BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen"* ]]; then
            sed -i "s#TRUEFALSE#2#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        else
            sed -i "s#TRUEFALSE#1#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        fi
        sed -i "s#ANALYSISTYPE#${Analysis_type}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        
        echo -n "."
        ((j++))
        sleep 1
    done
fi
echo " Done!"


