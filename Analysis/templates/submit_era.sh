#!/bin/sh
# Usage:
#    submit_era.sh

echo "Submit YEARNAME -- era ERANAME:"
for i in {0..7}; do
  echo -n "Stream ${i} "
  condor_submit -name ettore "stream_${i}/submit.condor"
done
echo "Done YEARNAME -- era ERANAME!"
