# WVZLooper

## Quick start

    # Installing the code
    git clone --recurse-submodule git@github.com:sgnoohc/WVZLooper.git
    cd WVZLooper
    source setup.sh
    make clean
    make -j

## Quick run

    sh runall.sh -b baseline_0709            -t WVZ -v v0.1.11
    sh runall.sh -b baseline_0709_with_systs -t WVZ -v v0.1.11 -s

## Plotting

For example,

    sh runall.sh -b baseline_0709 -t WVZ -v 0.1.11 -d myplots -p ChannelEMuHighMT__MllNom -n 5

To plot 5 bin dilepton mass spectrum in emu channel

## Statistical interpretation

    # In another terminal
    source setup.sh
    python scripts/write_datacards.py -t WVZ -v v0.1.11 -b baseline_0709_with_systs

    # In another new terminal
    cd stats/
    source setup_higgs_combine.sh
    cd ../
    sh stats/combine.sh -t WVZ -v v0.1.11 -b baseline_0709_with_systs
    sh stats/doSensitivity.sh stats/stat.txt
    sh stats/doSensitivity.sh stats/WVZ2016_v0.1.11/y2016_baseline_0709_with_systs/stat.txt
    sh stats/doSensitivity.sh stats/WVZ2017_v0.1.11/y2017_baseline_0709_with_systs/stat.txt
    sh stats/doSensitivity.sh stats/WVZ2018_v0.1.11/y2018_baseline_0709_with_systs/stat.txt
