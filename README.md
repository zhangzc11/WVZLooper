# WVZLooper

## Quick start

    # Installing the code
    git clone --recurse-submodule git@github.com:sgnoohc/WVZLooper.git
    cd WVZLooper
    source setup.sh
    make clean
    make -j

    # Running the code
    ./Analysis.exe <(for i in $(ls /nfs-7/userdata/phchang/babies/WVZ2016_v0.0.9/); do echo ${i/.root/}; done) WVZ2016_v0.0.9 baseline_y2016_0520
    ./Analysis.exe <(for i in $(ls /nfs-7/userdata/phchang/babies/WVZ2017_v0.0.9/); do echo ${i/.root/}; done) WVZ2017_v0.0.9 baseline_y2017_0520
    ./Analysis.exe <(for i in $(ls /nfs-7/userdata/phchang/babies/WVZ2018_v0.0.9/); do echo ${i/.root/}; done) WVZ2018_v0.0.9 baseline_y2018_0520

    # Hadding the histogram outputs by each year
    sh scripts/hadd.sh WVZ2016_v0.0.9 baseline_y2016_0520 # The last two arguments must match the last two arguments from previous command
    sh scripts/hadd.sh WVZ2017_v0.0.9 baseline_y2017_0520 # The last two arguments must match the last two arguments from previous command
    sh scripts/hadd.sh WVZ2018_v0.0.9 baseline_y2018_0520 # The last two arguments must match the last two arguments from previous command

    # Hadding the entire Run 2 into a single histograms (NOTE: Must be ran after each year has alreday been hadded)
    sh scripts/haddallyears.sh WVZ2016_v0.0.9 baseline_y2016_0520 WVZ2017_v0.0.9 baseline_y2017_0520 WVZ2018_v0.0.9 baseline_y2018_0520

    # Plotting the output histograms by each year
    python scripts/makeplot.py WVZ2016_v0.0.9 baseline_y2016_0520 # The last two arguments must match the last two arguments from previous command
    python scripts/makeplot.py WVZ2017_v0.0.9 baseline_y2017_0520 # The last two arguments must match the last two arguments from previous command
    python scripts/makeplot.py WVZ2018_v0.0.9 baseline_y2018_0520 # The last two arguments must match the last two arguments from previous command

    # Plotting the output histograms of all year
    python scripts/makeplot.py WVZ2016_v0.0.9_WVZ2017_v0.0.9_WVZ2018_v0.0.9 baseline_y2016_0520_baseline_y2017_0520_baseline_y2018_0520 # Basically the tags are just concatenated with "_"

    # Outputs by each year
    # plots/WVZ2016_v0.0.5/baseline_y2016_0520/
    # plots/WVZ2017_v0.0.5/baseline_y2017_0520/
    # plots/WVZ2018_v0.0.5/baseline_y2018_0520/

    # Outputs of all year combined
    # plots/WVZ2016_v0.0.9_WVZ2017_v0.0.9_WVZ2018_v0.0.9/baseline_y2016_0520_baseline_y2017_0520_baseline_y2018_0520/
