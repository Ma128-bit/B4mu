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

declare -a C_2022=("240510_165042" "240510_165106" "240510_165131" "240510_165201" "240513_071428" "240510_165312" "240510_165338" "240510_165359")
declare -a D_v1_2022=("240510_165439" "240510_165503" "240510_165529" "240510_165558" "240510_165619" "240510_165640" "240510_165703" "240510_165730")
declare -a D_v2_2022=("240510_165750" "240510_165812" "240510_165842" "240510_165903" "240510_165927" "240510_165957" "240510_170024" "240510_170046")
declare -a E_2022=("240510_170119" "240510_170136" "240510_170223" "240510_170248" "240510_170311" "240510_170331" "240510_170359" "240510_170421")
declare -a F_2022=("240510_161215" "240510_161232" "240510_161248" "240510_161306" "240510_161324" "240510_161340" "240510_161400" "240510_161417")
declare -a G_2022=("240510_161437" "240510_161453" "240510_161512" "240514_091456" "240510_161548" "240510_161605" "240510_161623" "240510_161642")

declare -a B_2023=("240510_161701" "240510_161719" "240510_161737" "240510_161758" "240510_161816" "240510_161832" "240510_161851" "240510_161915")
declare -a C_v1_2023=("240510_161940" "240510_162004" "240510_162042" "240510_162106" "240510_162136" "240510_162159" "240510_162219" "240510_162237")
declare -a C_v2_2023=("240510_162301" "240510_162323" "240510_162340" "240510_162402" "240510_162426" "240510_162448" "240510_162508" "240510_162529")
declare -a C_v3_2023=("240510_162558" "240510_162616" "240510_162635" "240510_162702" "240510_162721" "240510_162743" "240510_162823" "240510_162845")
declare -a C_v4_2023=("240510_162904" "240510_162929" "240510_162948" "240510_163016" "240510_163040" "240510_163059" "240510_163119" "240510_163136")
declare -a D_v1_2023=("240510_163550" "240510_163616" "240510_163639" "240510_163706" "240510_163729" "240510_163747" "240510_163805" "240510_163825")
declare -a D_v2_2023=("240510_163205" "240510_163224" "240510_163243" "240510_163301" "240510_163325" "240510_163349" "240510_163408" "240510_163434")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/240510_160836" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/240510_160859" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/240510_160913")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/240510_161029" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/240510_161050" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/240510_161105")

declare -a MC23_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/240510_160935" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/240510_160952" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/240510_161008")
declare -a MC23_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/240510_161122" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/240510_161138" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/240510_161158")

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


