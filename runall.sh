#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Help
usage()
{
  echo "ERROR - Usage:"
  echo
  echo "      sh $(basename $0) OPTIONSTRINGS ..."
  echo
  echo "Options with arguments:"
  echo "  -h    Help                   (Display this message)"
  echo "  -b    Baseline tag of github (e.g. -b baseline_0612)"
  echo "  -t    Ntuple type            (e.g. -t WVZMVA, WVZ, or Trilep etc.)"
  echo "  -v    Ntuple version         (e.g. -v 0.0.9, 0.1.0, or etc.)"
  echo "  -f    Force                  (e.g. -f)"
  echo "  -1    skip looper stage      (e.g. -1)"
  echo "  -2    skip hadder stage      (e.g. -2)"
  echo "  -d    plot output directory  (e.g. -d cr)"
  echo "  -p    plot filter pattern    (e.g. -p _cutflow)"
  echo "  -n    nbins                  (e.g. -n)"
  echo "  -u    unblind                (e.g. -u)"
  echo
  exit
}

# Command-line opts
while getopts ":b:t:v:d:p:n:f12uh" OPTION; do
  case $OPTION in
    b) BASELINE=${OPTARG};;
    t) NTUPLETYPE=${OPTARG};;
    v) NTUPLEVERSION=${OPTARG};;
    d) DIRNAME=${OPTARG};;
    p) PATTERN=${OPTARG};;
    f) FORCE=true;;
    1) SKIPLOOPER=true;;
    2) SKIPHADDER=true;;
    n) NBINS=" -n "${OPTARG};;
    u) UNBLIND=" -u";;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${BASELINE} ]; then usage; fi
if [ -z ${NTUPLETYPE}  ]; then usage; fi
if [ -z ${NTUPLEVERSION}  ]; then usage; fi
if [ -z ${FORCE}  ]; then FORCE=false; fi
if [ -z ${SKIPLOOPER}  ]; then SKIPLOOPER=false; fi
if [ -z ${SKIPHADDER}  ]; then SKIPHADDER=false; fi
if [ -z ${DIRNAME}  ]; then DIRNAME="yield"; fi
if [ -z ${PATTERN}  ]; then PATTERN="ChannelEMuHighMT__Yield,ChannelOffZHighMET__Yield,ChannelOnZCR__Yield,ChannelBTagEMuCR__Yield"; fi
if [ -z ${NBINS} ]; then NBINS=""; fi
if [ -z ${UNBLIND}  ]; then UNBLIND=""; fi

# to shift away the parsed options
shift $(($OPTIND - 1))

# Verbose
date
echo "================================================"
echo "$(basename $0) $*"
echo "$(basename $0) $*" >> $DIR/.$(basename $0).history
echo "------------------------------------------------"
echo "BASELINE       : ${BASELINE}"
echo "NTUPLEVERSION  : ${NTUPLEVERSION}"
echo "NTUPLETYPE     : ${NTUPLETYPE}"
echo "FORCE          : ${FORCE}"
echo "SKIPLOOPER     : ${SKIPLOOPER}"
echo "SKIPHADDER     : ${SKIPHADDER}"
echo "DIRNAME        : ${DIRNAME}"
echo "PATTERN        : ${PATTERN}"
echo "NBINS          : ${NBINS}"
echo "UNBLIND        : ${UNBLIND}"
echo "================================================"

# Sanity check that the sample referred exists
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2016_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2016_${NTUPLEVERSION}/ does not exists!"; exit; fi
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2017_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2017_${NTUPLEVERSION}/ does not exists!"; exit; fi
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2018_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2018_${NTUPLEVERSION}/ does not exists!"; exit; fi

if ! ${SKIPLOOPER}; then

    echo "Running the looper..."

    if ! ${FORCE}; then
        # Check whether output already exists
        if [ -d outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} ]; then echo "ERROR: Output outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} already exists! Do you want to overwrite? then re-run with -f option"; exit; fi
        if [ -d outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} ]; then echo "ERROR: Output outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} already exists! Do you want to overwrite? then re-run with -f option"; exit; fi
        if [ -d outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} ]; then echo "ERROR: Output outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} already exists! Do you want to overwrite? then re-run with -f option"; exit; fi
    fi

    # Run the jobs
    sh ./run.sh -y 2016 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2016_${BASELINE} &
    sh ./run.sh -y 2017 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2017_${BASELINE} &
    sh ./run.sh -y 2018 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2018_${BASELINE} &
    # sh ./run.sh -F -y 2016 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2016_${BASELINE} &
    # sh ./run.sh -F -y 2017 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2017_${BASELINE} &
    # sh ./run.sh -F -y 2018 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2018_${BASELINE} &
    wait

fi

if ! ${SKIPHADDER}; then

    echo "Running the hadder..."

    # Hadding the histogram outputs by each year
    sh ./scripts/hadd.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} # The last two arguments must match the last two arguments from previous command
    sh ./scripts/hadd.sh ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} # The last two arguments must match the last two arguments from previous command
    sh ./scripts/hadd.sh ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE} # The last two arguments must match the last two arguments from previous command
    
    # Hadding the entire Run 2 into a single histograms (NOTE: Must be ran after each year has alreday been hadded)
    sh ./scripts/haddallyears.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE}

fi

if [ -n ${DIRNAME} ] && [ -n ${PATTERN} ]; then

    # Plotting the output histograms by each year
    python ./scripts/plot.py ${UNBLIND} -s ${NTUPLETYPE}2016_${NTUPLEVERSION} -t y2016_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
    python ./scripts/plot.py ${UNBLIND} -s ${NTUPLETYPE}2017_${NTUPLEVERSION} -t y2017_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
    python ./scripts/plot.py ${UNBLIND} -s ${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2018_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
    
    # Plotting the output histograms of all year
    python ./scripts/plot.py ${UNBLIND} -s ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # Basically the tags are just concatenated with "_"

fi

if [[ ${PATTERN} == *"PtVarBin"* ]]; then
    if [[ ${NTUPLETYPE} == *"Trilep"* ]]; then
        # Plotting the output histograms by each year
        python ./scripts/fakerate.py ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} # The last two arguments must match the last two arguments from previous command
        python ./scripts/fakerate.py ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} # The last two arguments must match the last two arguments from previous command
        python ./scripts/fakerate.py ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE} # The last two arguments must match the last two arguments from previous command
        
        # Plotting the output histograms of all year
        python ./scripts/fakerate.py ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} # Basically the tags are just concatenated with "_"
    fi
fi

if [[ ${PATTERN} == *"ChannelEMuHighMT__Yield,ChannelOffZHighMET__Yield,ChannelOnZCR__Yield,ChannelBTagEMuCR__Yield"* ]]; then

    # pretty_print each year
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} # Basically the tags are just concatenated with "_"
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} # Basically the tags are just concatenated with "_"
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE} # Basically the tags are just concatenated with "_"

    # Printing in to text format for easy keynote export
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} # Basically the tags are just concatenated with "_"
fi
