#!/bin/bash

usage()
{
    echo "Usage:"
    echo ""
    echo "   $ $0 ntuple_version tagname"
    echo ""
    echo ""
    exit
}

if [ -z $1 ]; then usage; fi
if [ -z $2 ]; then usage; fi

if [ ! -d plots/$1/$2 ]; then
    echo ">>> Error! cannot find outputs/$1/$2"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

python rooutil/yield_for_keynote.py plots/$1/$2/yield/ChannelOffZHighMET__Yield.txt > offz.txt
python rooutil/yield_for_keynote.py plots/$1/$2/yield/ChannelEMuHighMT__Yield.txt > emu.txt
python rooutil/yield_for_keynote.py plots/$1/$2/yield/ChannelOnZ__Yield.txt > onz.txt
python rooutil/yield_for_keynote.py plots/$1/$2/yield/ChannelBTagEMu__Yield.txt > btag.txt

##head -n1 emu.txt > summary.txt
tail -n2 emu.txt | head -n1 > summary_${1}_${2}.txt
tail -n2 offz.txt | head -n1 >> summary_${1}_${2}.txt
tail -n2 onz.txt | head -n1 >> summary_${1}_${2}.txt
tail -n2 btag.txt | head -n1 >> summary_${1}_${2}.txt
