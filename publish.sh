#!/bin/bash
#cp $LOCALRT/src/flashgg/Validation/python/output_hlt.root output_hlt.root
echo "tune_html before executing"
sleep 5
./tune_html.sh
root -b make_up.C
export date_string=`date | sed -e "s| |_|g" -e "s|:|_|g" -e "s|,||g" -e "s|(UTC+0100)||g"`
echo $date_string
mkdir ~/www/higgs/runII/$date_string
ls ~/www/higgs/runII/$date_string
cp -rf *.C *.root plots index.html ~/www/higgs/runII/$date_string
echo "http://test-carrillo.web.cern.ch/test-carrillo/higgs/runII/$date_string"
