#!/bin/bash

BDTINPUTDIR=/nfs-7/userdata/phchang/babies/BDT_v0.1.12.7/
mkdir -p outputs/
rm -rf outputs/*.root
rm -f .cmd.txt
for i in $(ls ${BDTINPUTDIR}); do
    if [[ $i == *"data.root"* ]]; then
        :
    else
        echo "./doAnalysis -i ${BDTINPUTDIR}$i -t t -o outputs/$i" >> .cmd.txt
    fi
done
xargs.sh .cmd.txt
