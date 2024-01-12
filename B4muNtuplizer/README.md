# Ntuplizer code for the B&rarr;4mu search at Run 3

## Run ntuplizer on data:
```
cd CrabSubmission
source submit_CRAB.sh [era] [year] 
```
* `[year]` = `2022` : `[era]` = `C, D-v1, D-v2, E, F, G`
* `[year]` = `2023` : `[era]` = `C-v1, C-v2, C-v3, C-v4, D-v1, D-v2`
for data.

* `[year]` = `2022` : `[era]` = `2022_MC_BsJPsiPhi_pre, 2022_MC_BsJPsiPhi_post`
* `[year]` = `2023` : `[era]` = `Not available`
for MC.
<p>&nbsp;</p>

## Run ntuplizer on few root files:
```
cd SkimTools/test
cmsRun run_Data2022_PatAndTree_cfg.py
```
for data.
```
cmsRun run_MC2022_PatAndTree_cfg.py
```
for MC.
<p>&nbsp;</p>
