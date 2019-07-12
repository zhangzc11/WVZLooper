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

#python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelOffZHighMET_cutflow.txt > offz.txt
#python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelOnZNjet_cutflow.txt > onz.txt
#python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelEMuHighMT_cutflow.txt > emu.txt
## python rooutil/yield_for_keynote.py plots/$1/$2/cutflow_detail/ChannelOffZHighMET_cutflow.txt > offz_detail.txt
## python rooutil/yield_for_keynote.py plots/$1/$2/cutflow_detail/ChannelOnZNjet_cutflow.txt > onz_detail.txt
## python rooutil/yield_for_keynote.py plots/$1/$2/cutflow_detail/ChannelEMuHighMll_cutflow.txt > emu_detail.txt

##head -n1 emu.txt > summary.txt
#tail -n4 emu.txt | head -n3 > summary_${1}_${2}.txt
#tail -n3 offz.txt | head -n2 >> summary_${1}_${2}.txt
#tail -n3 onz.txt | head -n2 >> summary_${1}_${2}.txt

python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelOffZHighMET__Yield.txt > offz.txt
python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelOnZ__Yield.txt > onz.txt
python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelEMuHighMT__Yield.txt > emu.txt
python rooutil/yield_for_keynote.py plots/$1/$2/cutflow/ChannelBTagEMu__Yield.txt > btagemu.txt

tail -n2 emu.txt | head -n1 > summary_${1}_${2}.txt
tail -n2 offz.txt | head -n1 >> summary_${1}_${2}.txt
tail -n2 onz.txt | head -n1 >> summary_${1}_${2}.txt
tail -n2 btagemu.txt | head -n1 >> summary_${1}_${2}.txt
