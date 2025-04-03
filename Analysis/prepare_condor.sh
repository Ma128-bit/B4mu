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

declare -a C_2022=("241216_133239" "241216_133250" "241216_133302" "241216_133313" "241216_133326" "241216_133337" "241216_133349" "241216_133401")
declare -a D_v1_2022=("241216_133413" "241216_133425" "241216_133436" "241216_133447" "241216_133457" "241216_133508" "241216_133518" "241216_133529")
declare -a D_v2_2022=("241216_133541" "241216_133553" "241216_133604" "241216_133615" "241216_133626" "241216_133637" "241216_133648" "241216_133659")
declare -a E_2022=("241216_133712" "241216_133724" "241216_133736" "241216_133747" "241216_133757" "241216_133808" "241216_133818" "241216_133828")
declare -a F_2022=("241216_131033" "241216_131044" "241216_131056" "241216_131108" "241216_131118" "241216_131130" "241216_131144" "241216_131155")
declare -a G_2022=("241216_131208" "241216_131218" "241216_131229" "241216_131240" "241216_131252" "241216_131303" "241216_131314" "241216_131324")

declare -a B_2023=("241216_131345" "241216_131355" "241216_131405" "241216_131416" "241216_131426" "241216_131436" "241216_131448" "241216_131458")
declare -a C_v1_2023=("241216_131509" "241216_131520" "241216_131531" "241216_131541" "241216_131552" "241216_131603" "241216_131613" "241216_131622")
declare -a C_v2_2023=("241216_131634" "241216_131644" "241216_131654" "241216_131704" "241216_131714" "241216_131725" "241216_131736" "241216_131747")
declare -a C_v3_2023=("241216_131759" "241216_131810" "241216_131820" "241216_131831" "241216_131842" "241216_131853" "241216_131904" "241216_131914")
declare -a C_v4_2023=("241216_131925" "241216_131935" "241216_131946" "241216_131956" "241216_132007" "241216_132017" "241216_132030" "241216_132040")
declare -a D_v1_2023=("241216_132052" "241216_132107" "241216_132118" "241216_132128" "241216_132138" "241216_132148" "241216_132158" "241216_132209")
declare -a D_v2_2023=("241216_132221" "241216_132231" "241216_132241" "241216_132251" "241216_132301" "241216_132311" "241216_132322" "241216_132332")

declare -a B_2024=("241216_152926" "241216_152937" "241216_152948" "241216_153000" "241216_153012" "241216_153024" "241216_153035" "241216_153048")
declare -a C_2024=("241216_153102" "241216_153114" "241216_153125" "241216_153137" "241216_153148" "241216_153200" "241216_153212" "241216_153225")
declare -a D_2024=("241216_153237" "241216_153248" "241216_153301" "241216_153315" "241216_153328" "241216_153340" "241216_153352" "241216_153404")
declare -a E_v1_2024=("241216_153418" "241216_153429" "241216_153441" "241216_153453" "241216_153505" "241216_153517" "241216_153528" "241216_153539")
declare -a E_v2_2024=("241216_153552" "241216_153603" "241216_153615" "241216_153627" "241216_153638" "241216_153650" "241216_153701" "241216_153711")
declare -a F_2024=("241216_153725" "241216_153735" "241216_153747" "241216_153800" "241216_153810" "241216_153822" "241216_153833" "241216_153844")
declare -a G_2024=("241216_153858" "241216_153909" "241216_153920" "241216_153930" "241216_153941" "241216_153952" "241216_154003" "241216_154014")
declare -a H_2024=("241216_154028" "241216_154040" "241216_154051" "241216_154104" "241216_154115" "241216_154129" "241216_154140" "241216_154151")
declare -a I_v1_2024=("241216_154203" "241216_154213" "241216_154224" "241216_154236" "241216_154246" "241216_154258" "241216_154309" "241216_154320")
declare -a I_v2_2024=("241216_154332" "241216_154343" "241216_154353" "241216_154404" "241216_154414" "241216_154426" "241216_154438" "241216_154449")

declare -a MC22_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bd_Mini/241216_132654" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_pre_Bs_Mini/241216_132705" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_pre_BsJPsiPhi_Mini/241216_132716")
declare -a MC22_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bd_Mini/241216_132729" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2022_MC_post_Bs_Mini/241216_132739" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2022_MC_post_BsJPsiPhi_Mini/241216_132750")

declare -a MC23_B4mu_pre=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bd_Mini/241216_132540" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_pre_Bs_Mini/241216_132550" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_pre_BsJPsiPhi_Mini/241216_132600")
declare -a MC23_B4mu_post=("BdTo4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bd_Mini/241216_132613" "Bs0To4Mu_FourMuonFilter_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2023_MC_post_Bs_Mini/241216_132623" "BsToJpsiPhi_JMM_PhiMM_MuFilter_SoftQCDnonD_TuneCP5_13p6TeV-pythia8-evtgen/SkimB4Mu_2023_MC_post_BsJPsiPhi_Mini/241216_132635")

declare -a MC24_B4mu=("BdTo4Mu_Fil-FourMuon_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2024_MC_Bd_Mini/250225_150503" "Bs0To4Mu_Fil-FourMuon_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2024_MC_Bs_Mini/250225_150514" "BsToJpsiPhi-JMM-PhiMM_Fil-Mu_Par-SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB4Mu_2024_MC_BsJPsiPhi_Mini/250225_150528")

declare -a B4mu_MC_label=("Bd_4mu" "Bs_4mu" "BsJPsiPhi")

declare -a Control_C_2022=("241216_134021" "241216_134030" "241216_134038" "241216_134046" "241216_134053" "241216_134101" "241216_134110" "241216_134117")
declare -a Control_D_v1_2022=("241216_134128" "241216_134136" "241216_134144" "241216_134151" "241216_134158" "241216_134207" "241216_134214" "241216_134222")
declare -a Control_D_v2_2022=("241216_134232" "241216_134240" "241216_134248" "241216_134255" "241216_134303" "241216_134310" "241216_134319" "241216_134327")
declare -a Control_E_2022=("241216_134337" "241216_134345" "241216_134352" "241216_134400" "241216_134408" "241216_134416" "241216_134424" "241216_134431")
declare -a Control_F_2022=("241216_142458" "241216_142507" "241216_142514" "241216_142522" "241216_142530" "241216_142538" "241216_142546" "241216_142554")
declare -a Control_G_2022=("241216_142607" "241216_142614" "241216_142622" "241216_142630" "241216_142637" "241216_142645" "241216_142653" "241216_142702")

declare -a Control_B_2023=("241216_142801" "241216_142808" "241216_142816" "241216_142824" "241216_142832" "241216_142840" "241216_142848" "241216_142856")
declare -a Control_C_v1_2023=("241216_142906" "241216_142913" "241216_142921" "241216_142929" "241216_142937" "241216_142945" "241216_142953" "241216_143001")
declare -a Control_C_v2_2023=("241216_143011" "241216_143019" "241216_143027" "241216_143035" "241216_143042" "241216_143050" "241216_143058" "241216_143105")
declare -a Control_C_v3_2023=("241216_143116" "241216_143124" "241216_143131" "241216_143139" "241216_143147" "241216_143155" "241216_143202" "241216_143210")
declare -a Control_C_v4_2023=("241216_143220" "241216_143227" "241216_143235" "241216_143242" "241216_143250" "241216_143258" "241216_143305" "241216_143312")
declare -a Control_D_v1_2023=("241216_143325" "241216_143333" "241216_143341" "241216_143348" "241216_143356" "241216_143404" "241216_143413" "241216_143421")
declare -a Control_D_v2_2023=("241216_143432" "241216_143440" "241216_143449" "241216_143457" "241216_143507" "241216_143515" "241216_143524" "241216_143533")

declare -a Control_B_2024=("250227_182458" "250227_182517" "250227_182527" "250227_182537" "250227_182550" "250227_182600" "250227_182608" "250227_182620")
declare -a Control_C_2024=("250227_182633" "250227_182642" "250227_182656" "250227_182717" "250227_182731" "250227_182742" "250227_182807" "250227_182825")
declare -a Control_D_2024=("250227_182842" "250227_182906" "250227_182919" "250227_182933" "250227_182944" "250227_182953" "250227_183003" "250227_183016")
declare -a Control_E_v1_2024=("250227_183035" "250227_183048" "250227_183058" "250227_183111" "250227_183128" "250227_183140" "250227_183157" "250227_183207")
declare -a Control_E_v2_2024=("250227_183223" "250227_183234" "250227_183246" "250227_183300" "250227_183308" "250227_183318" "250227_183328" "250227_183336")
declare -a Control_F_2024=("250227_183354" "250227_183403" "250227_183414" "250227_183436" "250227_183445" "250227_183455" "250227_183506" "250227_183518")
declare -a Control_G_2024=("250331_155707" "250331_155715" "250331_155748" "250331_155848" "250331_155928" "250331_160041" "250331_160135" "250331_160252")
declare -a Control_H_2024=("250227_183740" "250227_183808" "250227_183820" "250227_183830" "250227_183841" "250227_183904" "250227_183918" "250227_183931")
declare -a Control_I_v1_2024=("250227_183946" "250227_183958" "250227_184009" "250227_184017" "250227_184027" "250227_184039" "250227_184050" "250227_184101")
declare -a Control_I_v2_2024=("250227_184124" "250227_184156" "250227_184206" "250227_184220" "250227_184230" "250227_184246" "250227_184300" "250303_183401")

declare -a MC22_B2mu2trk_pre=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_pre_Bs2mu2K_Mini/241122_122443" "BdToJpsiKstar_BMuonFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_pre_Bd2muKpi_Mini/241122_122449")
declare -a MC22_B2mu2trk_post=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_post_Bs2mu2K_Mini/241122_122459" "BdToJpsiKstar_BMuonFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_post_Bd2muKpi_Mini/241122_122505")

declare -a MC23_B2mu2trk_pre=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_pre_Bs2mu2K_Mini/241122_122536" "BdToJpsiKstar_BMuonFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_pre_Bd2muKpi_Mini/241122_122543")
declare -a MC23_B2mu2trk_post=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_post_Bs2mu2K_Mini/241122_122554" "BdToJpsiKstar_BMuonFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_post_Bd2muKpi_Mini/241122_122600")

declare -a B2mu2trk_MC_label=("B2mu2K" "B2muKpi")

declare -a MC22_B2mu2K_pre_new=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_pre_new_Bs2mu2K_Mini/241216_145104" "BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_pre_new_ext_Bs2mu2K_Mini/241216_145915")
declare -a MC22_B2mu2K_post_new=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_post_new_Bs2mu2K_Mini/241216_145130" "BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2022_MC_post_new_ext_Bs2mu2K_Mini/241216_145859")
declare -a B2mu2K22_MC_label=("B2mu2K_new" "B2mu2K_new_ext")

declare -a MC23_B2mu2K_pre_new=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_pre_new_Bs2mu2K_Mini/241216_144957")
declare -a MC23_B2mu2K_post_new=("BsToJPsiPhi_JPsiToMuMu_PhiToKK_EtaPtFilter_SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2023_MC_post_new_Bs2mu2K_Mini/241216_145017")

declare -a MC24_B2mu2K_new=("BsToJPsiPhi-JPsiToMuMu-PhiToKK_Fil-EtaPt_Par-SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen/SkimB2Mu2K_2024_MC_Bs2mu2K_Mini/250227_182435")

declare -a B2mu2K23_MC_label=("B2mu2K_new")

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
    elif [ "${year}" == "2024" ]; then
        case "$era" in
          B)
            datasets=("${B_2024[@]}")
            ;;
          C)
            datasets=("${C_2024[@]}")
            ;;
          D)
            datasets=("${D_2024[@]}")
            ;;
          E-v1)
            datasets=("${E_v1_2024[@]}")
            ;;
          E-v2)
            datasets=("${E_v2_2024[@]}")
            ;;
          F)
            datasets=("${F_2024[@]}")
            ;;
          G)
            datasets=("${G_2024[@]}")
            ;;
          H)
            datasets=("${H_2024[@]}")
            ;;
          I-v1)
            datasets=("${I_v1_2024[@]}")
            ;;
          I-v2)
            datasets=("${I_v2_2024[@]}")
            ;;
          MC)
            datasets=("${MC24_B4mu[@]}")
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
          MC_pre_new)
            datasets=("${MC22_B2mu2K_pre_new[@]}")
            label=("${B2mu2K22_MC_label[@]}")
            ;;
          MC_post_new)
            datasets=("${MC22_B2mu2K_post_new[@]}")
            label=("${B2mu2K22_MC_label[@]}")
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
          MC_pre)
            datasets=("${MC23_B2mu2trk_pre[@]}")
            label=("${B2mu2trk_MC_label[@]}")
            ;;
          MC_post)
            datasets=("${MC23_B2mu2trk_post[@]}")
            label=("${B2mu2trk_MC_label[@]}")
            ;;
          MC_pre_new)
            datasets=("${MC23_B2mu2K_pre_new[@]}")
            label=("${B2mu2K23_MC_label[@]}")
            ;;
          MC_post_new)
            datasets=("${MC23_B2mu2K_post_new[@]}")
            label=("${B2mu2K23_MC_label[@]}")
            ;;
          *)
            echo "Error: The era is incorrect."
            return
            ;;
        esac
    elif [ "${year}" == "2024" ]; then
        case "$era" in
          B)
            datasets=("${Control_B_2024[@]}")
            ;;
          C)
            datasets=("${Control_C_2024[@]}")
            ;;
          D)
            datasets=("${Control_D_2024[@]}")
            ;;
          E-v1)
            datasets=("${Control_E_v1_2024[@]}")
            ;;
          E-v2)
            datasets=("${Control_E_v2_2024[@]}")
            ;;
          F)
            datasets=("${Control_F_2024[@]}")
            ;;
          G)
            datasets=("${Control_G_2024[@]}")
            ;;
          H)
            datasets=("${Control_H_2024[@]}")
            ;;
          I-v1)
            datasets=("${Control_I_v1_2024[@]}")
            ;;
          I-v2)
            datasets=("${Control_I_v2_2024[@]}")
            ;;
          MC_new)
            datasets=("${MC24_B2mu2K_new[@]}")
            label=("${B2mu2K23_MC_label[@]}")
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
        elif [[ "$i" == *"BsToJpsiPhi-JMM-PhiMM_Fil-Mu_Par-SoftQCDnonD_TuneCP5_13p6TeV_pythia8-evtgen"* ]]; then
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


