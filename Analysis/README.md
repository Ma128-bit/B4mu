# Analysis code for the B&rarr;4mu search at Run 3

## Run analysis on an entire year with condor (ONLY Data):
```
source prepare_and_submit_ALL.sh [year] [Analysis_type] [delta]
```
*  [year] is the year (`2022` or `2023`);
*  [Analysis_type] `B4mu`, `B2mu2K` or `B2muKpi`;
*  [Delta] is the number of input files per submission

Then:
```
source hadd_ALL.sh [year] [Analysis_type]
```
Example:`source prepare_and_submit_ALL.sh 2022 B4mu 300`
<p>&nbsp;</p>


## Run analysis on an era with condor (Data and MC):
```
source prepare_condor.sh [era] [year] [Analysis_type] [delta]
```
*  [era] is the era (`C, D-v1, D-v2, E, F, G, MC_pre, MC_post` for 2022 `C-v1, C-v2, C-v3, C-v4, D-v1, D-v2` for 2023)
*  [year] is the year (`2022` or `2023`);
*  [Analysis_type] `B4mu`, `B2mu2K` or `B2muKpi`;
*  [Delta] is the number of input files per submission

**FOR DATA ONLY**:
```
cd [Analysis_type]/[year]_era[era] 
source submit_era.sh
```
Then:
```
source hadd_era.sh
```
**FOR MC**:
```
cd [Analysis_type]/[year]_[era]/[MC_lable]
submit submit.condor with condor
merge the output with hadd
```
<p>&nbsp;</p>

## Run analysis on a specific subset of data

```
python3 Analizer.py --index [ID] --delta [Delta] --directory_IN [Input_dir] --directory_OUT [Output_dir] --isMC [isMC] --Analysis_type [Analysis_type]
```
*  [Input_dir] is the directory with the root files;
*  [Output_dir] is the directory where the files will be saved;
*  [Delta] is the number of input files;
*  [ID] is used to select input file root with number between [ID]x[Delta] and ([ID]+1)x[Delta];
*  [isMC] is 0 for data and 1 for MC;
*  [Analysis_type] `B4mu`, `B2mu2K` or `B2muKpi`

<p>&nbsp;</p>
