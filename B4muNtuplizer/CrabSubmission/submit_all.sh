declare -a Era_2022=("F" "G")
declare -a Era_2023=("B" "C-v1" "C-v2" "C-v3" "C-v4" "D-v1" "D-v2")

source resubmit_CRAB.sh MC_pre 2022
source resubmit_CRAB.sh MC_pre 2023
source resubmit_CRAB.sh MC_post 2022
source resubmit_CRAB.sh MC_post 2023

for e in "${Era_2022[@]}"; do
	source submit_CRAB.sh ${e} 2022
	sleep 1 
done

for e in "${Era_2023[@]}"; do
        source submit_CRAB.sh ${e} 2023
        sleep 1
done
