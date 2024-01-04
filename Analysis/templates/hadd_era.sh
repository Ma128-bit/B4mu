#!/bin/sh
# Usage:
#    hadd_era.sh

echo "Hadd YEARNAME -- era ERANAME:"
hadd Analyzed_Data_YEARNAME_Era_ERANAME.root stream_*/Analyzed_Data_*.root
echo "Done YEARNAME -- era ERANAME!"
