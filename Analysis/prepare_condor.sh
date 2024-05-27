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

declare -a C_2022=("240523_171145" "240523_171210" "240523_171236" "240523_171305" "240523_171336" "240523_171405" "240523_171439" "240523_171514")
declare -a D_v1_2022=("240523_171546" "240523_171619" "240523_171647" "240523_171724" "240523_171802" "240523_171832" "240523_171905" "240523_171931")
declare -a D_v2_2022=("240523_172020" "240523_172051" "240523_172121" "240523_172147" "240523_172215" "240523_172249" "240523_172328" "240523_172357")
declare -a E_2022=("240523_172429" "240523_172458" "240523_172527" "240523_172600" "240523_172641" "240523_172712" "240523_172748" "240523_172815")
declare -a F_2022=("240523_153009" "240523_153043" "240523_153121" "240523_153158" "240523_153227" "240523_153306" "240523_153332" "240523_153359")
declare -a G_2022=("240523_153429" "240523_153453" "240523_153529" "240523_153603" "240523_153722" "240523_153809" "240523_153854" "240523_153942")

declare -a B_2023=("240523_154156" "240523_154231" "240523_154309" "240523_154348" "240523_154439" "240523_154512" "240523_154543" "240523_154620")
declare -a C_v1_2023=("240523_154653" "240523_154724" "240523_154759" "240523_154834" "240523_154913" "240523_154945" "240523_155033" "240523_155107")
declare -a C_v2_2023=("240523_155145" "240523_155223" "240523_155301" "240523_155340" "240523_155419" "240523_155453" "240523_155527" "240523_155604")
declare -a C_v3_2023=("240523_155652" "240523_155730" "240523_155803" "240523_155836" "240523_155911" "240523_155949" "240523_160026" "240523_160056")
declare -a C_v4_2023=("240523_160130" "240523_160218" "240523_160252" "240523_160325" "240523_160404" "240523_160445" "240523_160515" "240523_160602")
declare -a D_v1_2023=("240523_160634" "240523_160705" "240523_160734" "240523_160806" "240523_160838" "240523_160919" "240523_160955" "240523_161022")
declare -a D_v2_2023=("240523_161054" "240523_161119" "240523_161146" "240523_161212" "240523_161246" "240523_161315" "240523_161344" "240523_161410")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/240523_144034" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/240523_144046" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/240523_144059")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/240523_144153" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/240523_144206" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/240523_144218")

declare -a MC23_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/240523_144111" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/240523_144126" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/240523_144140")
declare -a MC23_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/240523_144231" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/240523_144246" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/240523_144258")

declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu" "BsJPsiPhi")

declare -a Control_C_2022=("240422_081145" "240422_081202" "240422_081217" "240422_081231" "240422_081250" "240422_081304" "240422_081320" "240422_081336")
declare -a Control_D_v1_2022=("240422_081423" "240422_081441" "240422_081457" "240422_081515" "240422_081530" "240422_081544" "240422_081557" "240422_081616")
declare -a Control_D_v2_2022=("240422_081654" "240422_081708" "240422_081722" "240422_081748" "240422_081804" "240422_081820" "240422_081834" "240422_081849")
declare -a Control_E_2022=("240422_081916" "240422_082003" "240422_082030" "240422_082115" "240422_082141" "240422_082159" "240422_082212" "240422_082231")
declare -a Control_F_2022=("240421_163014" "240421_163227" "240421_163422" "240421_163548" "240421_163758" "240421_164009" "240421_164205" "240421_164407")
declare -a Control_G_2022=("240422_073638" "240422_073656" "240422_073712" "240422_073730" "240422_073743" "240422_073803" "240422_073821" "240422_073835")

declare -a Control_B_2023=("240422_074737" "240422_074751" "240422_074805" "240422_074824" "240422_074847" "240422_074900" "240422_074915" "240422_074932")
declare -a Control_C_v1_2023=("240422_075000" "240422_075018" "240422_075033" "240422_075048" "240422_075102" "240422_075119" "240422_075139" "240422_075155")
declare -a Control_C_v2_2023=("240422_075219" "240422_075237" "240422_075256" "240422_075315" "240422_075332" "240422_075348" "240422_075407" "240422_075422")
declare -a Control_C_v3_2023=("240422_075938" "240422_075953" "240422_080012" "240422_080032" "240422_080056" "240422_080112" "240422_080126" "240422_080142")
declare -a Control_C_v4_2023=("240422_080323" "240422_080336" "240422_080351" "240422_080406" "240422_080423" "240422_080439" "240422_080454" "240422_080508")
declare -a Control_D_v1_2023=("240422_080537" "240422_080550" "240422_080604" "240422_080619" "240422_080633" "240422_080654" "240422_080708" "240422_080722")
declare -a Control_D_v2_2023=("240422_080740" "240422_080755" "240422_080811" "240422_080826" "240422_080841" "240422_080856" "240422_080919" "240422_080932")

declare -a MC22_B2mu2trk_pre=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bs2mu2K_Mini/240422_082718" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_pre_Bd2muKpi_Mini/240422_082739")
declare -a MC22_B2mu2trk_post=("BstoJpsiPhi_Jpsito2Mu_Phito2K_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bs2mu2K_Mini/240422_082814" "BdtoJpsiKstar_Jpsito2Mu_KstartoKPi_MuFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_B2mu2trk_post_Bd2muKpi_Mini/240422_082906")
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


