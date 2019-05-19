# WVZLooper

## Quick start

    git clone --recurse-submodule git@github.com:sgnoohc/WVZLooper.git
    cd WVZLooper
    source setup.sh
    make clean
    make -j
    ./Analysis.exe configs/Filelist_bkg.list MC WVZ2018_v0.0.5 run1 # "run1" is the tag of this specific loop run
    sh scripts/hadd.sh WVZ2018_v0.0.5 run1 # The last two arguments must match the last two arguments from previous command
    python scripts/makeplot.py WVZ2018_v0.0.5 run1 # The last two arguments must match the last two arguments from previous command
    # outputs are in plots/WVZ2018_v0.0.5/run1/

