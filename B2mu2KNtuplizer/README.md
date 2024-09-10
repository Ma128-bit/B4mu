# Ntuplizer code for the B&rarr;4mu search at Run 3

## Submit all era in a year:
```
submitAllJobs.sh [Year] [MCflag]
```
* `[year]` = `2022` or  `2023`: `[MCflag]` = `true or false`

## Run ntuplizer on a full dataset:
```
cd CrabSubmission
source submit_CRAB.sh [era] [year] 
```
**For DATA:**

* `[year]` = `2022` : `[era]` = `C, D-v1, D-v2, E, F, G`
* `[year]` = `2023` : `[era]` = `C-v1, C-v2, C-v3, C-v4, D-v1, D-v2`

**For MC:**

* `[year]` = `2022` or  `2023`: `[era]` = `MC_pre, MC_post`

<p>&nbsp;</p>

## Run ntuplizer on few root files:

`cd SkimTools/test`

For data: `cmsRun run_Data2022_PatAndTree_cfg.py`

For MC: `cmsRun run_MC2022_PatAndTree_cfg.py`

<p>&nbsp;</p>
