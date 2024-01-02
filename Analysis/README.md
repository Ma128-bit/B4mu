# Analysis code for the B&rarr;4mu search at Run 3

## Run analysis on a specific subset of date with

```
python3 Analizer.py --index [ID] --delta [Delta] --directory_IN [Input_dir] --directory_OUT [Output_dir]
```
*  [Input_dir] is the directory with the root files;
*  [Output_dir] is the directory where the files will be saved;
*  [Delta] is the number of input files
*  [ID] is used to select input file root with number between [ID]*[Delta] and ([ID]+1)*[Delta]
<p>&nbsp;</p>
