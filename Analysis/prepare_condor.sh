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

declare -a C_2022=("240419_172432" "240419_172449" "240419_172503" "240419_172517" "240419_172530" "240419_172544" "240419_172558" "240419_172612")
declare -a D_v1_2022=("240419_172634" "240419_172649" "240419_172704" "240419_172718" "240419_172733" "240419_172748" "240419_172804" "240419_172818")
declare -a D_v2_2022=("240419_172839" "240419_172853" "240419_172909" "240419_172924" "240419_172942" "240419_172955" "240419_173012" "240419_173026")
declare -a E_2022=("240419_173049" "240419_173105" "240419_173122" "240419_173137" "240419_173154" "240419_173209" "240419_173226" "240419_173241")
declare -a F_2022=("240419_165412" "240419_165425" "240419_165436" "240419_165449" "240419_165501" "240419_165514" "240419_165526" "240419_165539")
declare -a G_2022=("240419_165558" "240419_165611" "240419_165623" "240419_165635" "240419_165646" "240419_165659" "240419_165711" "240419_165723")

declare -a B_2023=("240419_165855" "240419_165907" "240419_165922" "240419_165936" "240419_165950" "240419_170004" "240419_170016" "240419_170029")
declare -a C_v1_2023=("240419_170157" "240419_170214" "240419_170227" "240419_170240" "240419_170257" "240419_170311" "240419_170327" "240419_170341")
declare -a C_v2_2023=("240419_170441" "240419_170455" "240419_170511" "240419_170525" "240419_170538" "240419_170552" "240419_170608" "240419_170622")
declare -a C_v3_2023=("240419_170656" "240419_170712" "240419_170726" "240419_170742" "240419_170757" "240419_170812" "240419_170826" "240419_170841")
declare -a C_v4_2023=("240419_170911" "240419_170926" "240419_170940" "240419_170955" "240419_171008" "240419_171021" "240419_171035" "240419_171049")
declare -a D_v1_2023=("240419_171134" "240419_171148" "240419_171205" "240419_171219" "240419_171233" "240419_171247" "240419_171304" "240419_171323")
declare -a D_v2_2023=("240419_171448" "240419_171502" "240419_171517" "240419_171530" "240419_171544" "240419_171559" "240419_171615" "240419_171627")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/240419_171727" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/240419_171739" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/240419_171811")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/240419_171832" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/240419_171903" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/240419_171936")

declare -a MC23_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/240419_172213" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/240419_172226" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/240419_172240")
declare -a MC23_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/240419_172119" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/240419_172133" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/240419_172146")

declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu" "BsJPsiPhi")

declare -a Control_C_2022=("240325_150034" "240325_150048" "240325_150101" "240325_150115" "240325_150132" "240325_150147" "240325_150201" "240325_150215")
declare -a Control_D_v1_2022=("240325_150236" "240325_150251" "240325_150305" "240325_150320" "240325_150334" "240325_150350" "240325_150404" "240326_115106")
declare -a Control_D_v2_2022=("240325_150555" "240325_150613" "240325_150630" "240325_150647" "240325_150706" "240325_150725" "240325_150743" "240325_150759")
declare -a Control_E_2022=("240325_150848" "240325_150904" "240325_150921" "240325_150938" "240325_150955" "240325_151011" "240325_151026" "240325_151042")
declare -a Control_F_2022=("240325_140458" "240325_140510" "240325_140523" "240325_140538" "240325_140553" "240325_140608" "240325_140622" "240325_140635")
declare -a Control_G_2022=("240325_140819" "240325_140832" "240325_140845" "240325_140859" "240325_140912" "240325_140924" "240325_140937" "240325_140950")

declare -a Control_B_2023=("240325_123033" "240325_123051" "240325_123106" "240325_123121" "240325_123136" "240325_123154" "240325_123215" "240325_123235")
declare -a Control_C_v1_2023=("240325_125312" "240325_125326" "240325_125339" "240325_125352" "240325_125405" "240325_125420" "240325_125434" "240325_125448")
declare -a Control_C_v2_2023=("240326_114255" "240325_125607" "240325_125619" "240325_125631" "240325_125643" "240325_125656" "240325_125707" "240325_125719")
declare -a Control_C_v3_2023=("240326_114422" "240325_125840" "240325_125853" "240325_125907" "240325_125920" "240325_125933" "240325_125946" "240325_125958")
declare -a Control_C_v4_2023=("240325_130127" "240325_130140" "240325_130153" "240325_130205" "240325_130218" "240325_130231" "240325_130246" "240325_130300")
declare -a Control_D_v1_2023=("240325_132534" "240325_132546" "240326_114706" "240325_132611" "240325_132623" "240325_132636" "240325_132649" "240325_132701")
declare -a Control_D_v2_2023=("240325_132915" "240325_132927" "240325_132939" "240325_132959" "240325_133013" "240326_114854" "240325_133038" "240325_133051")

declare -a MC22_B2mu2trk_pre=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bs2mu2K_Mini/240321_102408" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bd2muKpi_Mini/240321_102520")
declare -a MC22_B2mu2trk_post=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bs2mu2K_Mini/240325_102401" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bd2muKpi_Mini/240325_102412")
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
        tot=0
        for j in $(seq 0 $((ndir - 1))); do
            nfiles=$(ls "${file_directory}/ParkingDoubleMuonLowMass${i}/Skim${Ana_temp}_${year}era${era}_stream${i}_Mini/${datasets[${i}]}/000${j}/" | wc -l)
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
        sed -i "s#TRUEFALSE#1#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        sed -i "s#ANALYSISTYPE#${Analysis_type}#g" "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        chmod a+x "${home_directory}/${Analysis_type}/${year}_${era}/${label[${j}]}/launch_analysis.sh"
        
        echo -n "."
        ((j++))
        sleep 1
    done
fi
echo " Done!"


