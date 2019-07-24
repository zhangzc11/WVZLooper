
## Setup

First source setup.sh from the main WVZLooper/ then cd into this directory

    source setup.sh
    cd rooutil/
    make -j
    cd ..
    cd scripts/bdt_looper

First train BDT

    cd tmvabdt
    make
    ./learn.out

Then it creates ouputs

    BDT.root
    dataset/

Go back to the looper

    cd ../
    make -j
    sh run.sh
    python plot.py

