# Analysis code for the B&rarr;4mu search at Run 3

## Run analysis on an entire year with condor:
```
source prepare_and_submit_ALL.sh [year] [delta]
```
*  [year] is the year;
*  [Delta] is the number of input files per submission

Then:
```
source hadd_ALL.sh
```
Example:`source prepare_and_submit_ALL.sh 2022 300`
<p>&nbsp;</p>


## Run analysis on an era with condor:
```
source prepare_condor.sh [era] [year] [delta]
```
*  [era] is the era;
*  [year] is the year;
*  [Delta] is the number of input files per submission

Then:
```
cd [year]_era[era]
source submit_era.sh
```

Then:
```
source hadd_era.sh
```
<p>&nbsp;</p>

## Run analysis on a specific subset of data

```
python3 Analizer.py --index [ID] --delta [Delta] --directory_IN [Input_dir] --directory_OUT [Output_dir]
```
*  [Input_dir] is the directory with the root files;
*  [Output_dir] is the directory where the files will be saved;
*  [Delta] is the number of input files
*  [ID] is used to select input file root with number between [ID]x[Delta] and ([ID]+1)x[Delta]

<p>&nbsp;</p>
