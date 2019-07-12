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

## Extrapolation Uncertainties

The extrapolation uncertainties for control regions were obatined from the following script.  
Make sure the directory paths in the python script is pointing to the right places.  

    python scripts/extrapolation_uncertainty.py

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

## Fake validations

For fake validations one uses the trilepton filtered baby

    sh runall.sh -b baseline_0709            -t Trilep -v v0.1.11

### For plotting fake validation stuff

It follows similar structure

    sh runall.sh -b baseline_0709            -t Trilep -v v0.1.11 -d fakevr -p DYPlusXFakeMuMR__lepFakeCand2relIso03EA -u

### For comparing WZ v. DY or ttbar

To draw the WZ v. DY ttbar comparison plots use the following script.  
Make sure the directory pahts in the python script is pointing to the right places.  

    python scripts/compare_fakes.py
