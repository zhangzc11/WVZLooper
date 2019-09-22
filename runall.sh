#!/bin/bash

trap "kill 0" EXIT

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
  echo "  -1    Force looper stage     (e.g. -1)"
  echo "  -2    Force hadder stage     (e.g. -2)"
  echo "  -d    plot output directory  (e.g. -d cr)"
  echo "  -p    plot filter pattern    (e.g. -p _cutflow)"
  echo "  -n    nbins                  (e.g. -n)"
  echo "  -u    unblind                (e.g. -u)"
  echo "  -y    yaxis-log              (e.g. -y)"
  echo "  -s    do systematics         (e.g. -s)"
  echo "  -k    do skim tree           (e.g. -k)"
  echo "  -x    xaxis_label            (e.g. -x \"MET [GeV]\")"
  echo "  -r    yaxis_range            (e.g. -r \"[0., 100.]\")"
  echo "  -a    run all year           (e.g. -a)"
  echo "  -c    one combined signal    (e.g. -c)"
  echo "  -i    stack signal           (e.g. -i)"
  echo
  exit
}

# Command-line opts
while getopts ":b:t:v:d:p:n:r:x:12uyskacih" OPTION; do
  case $OPTION in
    b) BASELINE=${OPTARG};;
    t) NTUPLETYPE=${OPTARG};;
    v) NTUPLEVERSION=${OPTARG};;
    d) DIRNAME=${OPTARG};;
    p) PATTERN=${OPTARG};;
    1) FORCELOOPER=true;;
    2) FORCEHADDER=true;;
    n) NBINS=" -n "${OPTARG};;
    x) XAXISLABEL=${OPTARG};;
    r) YAXISRANGE=${OPTARG};;
    u) UNBLIND=" -u";;
    y) YAXISLOG=" -y";;
    s) DOSYST=" -s";;
    k) DOSKIM=" -k";;
    a) RUNALLYEAR=true;;
    c) ONESIGNAL=" -c";;
    i) STACKSIGNAL=" -i";;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${BASELINE} ]; then usage; fi
if [ -z ${NTUPLETYPE}  ]; then usage; fi
if [ -z ${NTUPLEVERSION}  ]; then usage; fi
if [ -z ${FORCELOOPER}  ]; then FORCELOOPER=false; fi
if [ -z ${FORCEHADDER}  ]; then FORCEHADDER=false; fi
if [ -z ${DIRNAME}  ]; then DIRNAME="yield"; fi
if [ -z ${PATTERN}  ]; then PATTERN="ChannelEMuHighMT__Yield,ChannelOffZHighMET__Yield,ChannelOnZ__Yield,ChannelBTagEMu__Yield,FiveLeptonsMT5th__Yield"; fi
if [ -z "${NBINS}" ]; then NBINS=""; fi
if [ -z ${UNBLIND}  ]; then UNBLIND=""; fi
if [ -z ${YAXISLOG}  ]; then YAXISLOG=""; fi
if [ -z ${DOSYST}  ]; then DOSYST=""; fi
if [ -z ${DOSKIM}  ]; then DOSKIM=""; fi
if [ -z "${XAXISLABEL}" ]; then XAXISLABEL=""; fi
if [ -z "${YAXISRANGE}" ]; then YAXISRANGE=""; fi
if [ -z ${RUNALLYEAR}  ]; then RUNALLYEAR=false; fi
if [ -z ${ONESIGNAL}  ]; then ONESIGNAL=""; fi
if [ -z ${STACKSIGNAL}  ]; then STACKSIGNAL=""; fi

# Verbose
date
echo "================================================"
echo "$(basename $0) $*"
echo "$(basename $0) $*" >> $DIR/.$(basename $0).history
echo "------------------------------------------------"
echo "BASELINE       : ${BASELINE}"
echo "NTUPLEVERSION  : ${NTUPLEVERSION}"
echo "NTUPLETYPE     : ${NTUPLETYPE}"
echo "FORCELOOPER    : ${FORCELOOPER}"
echo "FORCEHADDER    : ${FORCEHADDER}"
echo "DIRNAME        : ${DIRNAME}"
echo "PATTERN        : ${PATTERN}"
echo "NBINS          : ${NBINS}"
echo "XAXISLABEL     : ${XAXISLABEL}"
echo "YAXISRANGE     : ${YAXISRANGE}"
echo "UNBLIND        : ${UNBLIND}"
echo "YAXISLOG       : ${YAXISLOG}"
echo "DOSYST         : ${DOSYST}"
echo "DOSKIM         : ${DOSKIM}"
echo "RUNALLYEAR     : ${RUNALLYEAR}"
echo "ONESIGNAL      : ${ONESIGNAL}"
echo "STACKSIGNAL    : ${STACKSIGNAL}"
echo "================================================"

# to shift away the parsed options
shift $(($OPTIND - 1))

# Sanity check that the sample referred exists
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2016_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2016_${NTUPLEVERSION}/ does not exists!"; exit; fi
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2017_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2017_${NTUPLEVERSION}/ does not exists!"; exit; fi
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2018_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}2018_${NTUPLEVERSION}/ does not exists!"; exit; fi

#
#
# Run the jobs
#
#
if ${FORCELOOPER} || [ ! -d outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE} ]; then
    echo "Running the looper..."
    sh ./run.sh ${DOSYST} ${DOSKIM} -y 2016 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2016_${BASELINE} &
fi
if ${FORCELOOPER} || [ ! -d outputs/${NTUPLETYPE}2017_${NTUPLEVERSION}/y2017_${BASELINE} ]; then
    echo "Running the looper..."
    sh ./run.sh ${DOSYST} ${DOSKIM} -y 2017 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2017_${BASELINE} &
fi
if ${FORCELOOPER} || [ ! -d outputs/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE} ]; then
    echo "Running the looper..."
    sh ./run.sh ${DOSYST} ${DOSKIM} -y 2018 -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -T y2018_${BASELINE} &
fi
wait

#
#
# Hadding the histogram outputs by each year
#
#
if ${FORCEHADDER} || [ ! -e outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE}/sig.root ]; then
    ls outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE}/sig.root
    echo "Running the hadder..."
    sh ./scripts/hadd.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} ${DOSKIM} # The last two arguments must match the last two arguments from previous command
fi
if ${FORCEHADDER} || [ ! -e outputs/${NTUPLETYPE}2017_${NTUPLEVERSION}/y2017_${BASELINE}/sig.root ]; then
    ls outputs/${NTUPLETYPE}2017_${NTUPLEVERSION}/y2017_${BASELINE}/sig.root
    echo "Running the hadder..."
    sh ./scripts/hadd.sh ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} ${DOSKIM} # The last two arguments must match the last two arguments from previous command
fi
if ${FORCEHADDER} || [ ! -e outputs/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE}/sig.root ]; then
    ls outputs/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE}/sig.root
    echo "Running the hadder..."
    sh ./scripts/hadd.sh ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE} ${DOSKIM} # The last two arguments must match the last two arguments from previous command
fi

#
#
# Hadding the entire Run 2 into a single histograms (NOTE: Must be ran after each year has alreday been hadded)
#
#
if ${FORCEHADDER} || [ ! -f outputs/${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION}/y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE}/sig.root ]; then
    sh ./scripts/haddallyears.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE}
fi

if [ -n ${DIRNAME} ] && [ -n ${PATTERN} ]; then

    if [ ! -f outputs/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE}/sig.root ]; then
        echo "Does looper output files exist? for example, 'outputs/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE}/sig.root' does not exist!"
        echo "Try re-running the loooper"
        echo "sh runall.sh -b ${BASELINE} -t ${NTUPLETYPE} -v ${NTUPLEVERSION} -1 -2"
        exit
    fi

    if ${RUNALLYEAR}; then
        # Plotting the output histograms by each year
        python ./scripts/plot.py ${STACKSIGNAL} ${ONESIGNAL} -r "${YAXISRANGE}" -x "${XAXISLABEL}" ${YAXISLOG} ${UNBLIND} -s ${NTUPLETYPE}2016_${NTUPLEVERSION} -t y2016_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
        python ./scripts/plot.py ${STACKSIGNAL} ${ONESIGNAL} -r "${YAXISRANGE}" -x "${XAXISLABEL}" ${YAXISLOG} ${UNBLIND} -s ${NTUPLETYPE}2017_${NTUPLEVERSION} -t y2017_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
        python ./scripts/plot.py ${STACKSIGNAL} ${ONESIGNAL} -r "${YAXISRANGE}" -x "${XAXISLABEL}" ${YAXISLOG} ${UNBLIND} -s ${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2018_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # The last two arguments must match the last two arguments from previous command
    fi

    # Plotting the output histograms of all year
    echo python ./scripts/plot.py ${STACKSIGNAL} ${ONESIGNAL} -r "${YAXISRANGE}" -x "${XAXISLABEL}" ${YAXISLOG} ${UNBLIND} -s ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # Basically the tags are just concatenated with "_"
    python ./scripts/plot.py ${STACKSIGNAL} ${ONESIGNAL} -r "${YAXISRANGE}" -x "${XAXISLABEL}" ${YAXISLOG} ${UNBLIND} -s ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} -d ${DIRNAME} -p ${PATTERN} ${NBINS} # Basically the tags are just concatenated with "_"

fi

if [[ ${PATTERN} == *"ChannelEMuHighMT__Yield,ChannelOffZHighMET__Yield,ChannelOnZ__Yield,ChannelBTagEMu__Yield"* ]] && [[ ${NTUPLETYPE} == *"WVZ"* ]]; then

    # pretty_print each year
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2016_${NTUPLEVERSION} y2016_${BASELINE} # Basically the tags are just concatenated with "_"
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2017_${NTUPLEVERSION} y2017_${BASELINE} # Basically the tags are just concatenated with "_"
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2018_${NTUPLEVERSION} y2018_${BASELINE} # Basically the tags are just concatenated with "_"

    # Printing in to text format for easy keynote export
    sh ./scripts/yield_print.sh ${NTUPLETYPE}2016_${NTUPLEVERSION}_${NTUPLETYPE}2017_${NTUPLEVERSION}_${NTUPLETYPE}2018_${NTUPLEVERSION} y2016_${BASELINE}_y2017_${BASELINE}_y2018_${BASELINE} # Basically the tags are just concatenated with "_"
fi
