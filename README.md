# WVZLooper

## Install

    git clone --recurse-submodule git@github.com:sgnoohc/WVZLooper.git
    cd WVZLooper
    source setup.sh
    make clean
    make -j
    ./Analysis.exe configs/Filelist_bkg.list MC
    cd outputs/
    sh hadd.sh
    cd ../
    python scripts/makeplot.py
    # outputs are in plots/
