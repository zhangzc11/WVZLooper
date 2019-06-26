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
  echo "  -v    Ntuple version         (e.g. -v v0.0.9, v0.1.0, or etc.)"
  echo
  exit
}

# Command-line opts
while getopts ":b:t:v:h" OPTION; do
  case $OPTION in
    b) BASELINE=${OPTARG};;
    t) NTUPLETYPE=${OPTARG};;
    v) NTUPLEVERSION=${OPTARG};;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${BASELINE} ]; then usage; fi
if [ -z ${NTUPLETYPE}  ]; then usage; fi
if [ -z ${NTUPLEVERSION}  ]; then usage; fi

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
echo "================================================"

sh $DIR/combinecard.sh -s ${NTUPLETYPE}2016_${NTUPLEVERSION} -t y2016_${BASELINE}
sh $DIR/combinecard.sh -s ${NTUPLETYPE}2017_${NTUPLEVERSION} -t y2017_${BASELINE}
sh $DIR/combinecard.sh -s ${NTUPLETYPE}2018_${NTUPLEVERSION} -t y2018_${BASELINE}

combineCards.py \
    y2016=stats/${NTUPLETYPE}2016_${NTUPLEVERSION}/y2016_${BASELINE}/stat.txt \
    y2017=stats/${NTUPLETYPE}2017_${NTUPLEVERSION}/y2017_${BASELINE}/stat.txt \
    y2018=stats/${NTUPLETYPE}2018_${NTUPLEVERSION}/y2018_${BASELINE}/stat.txt > stats/stat.txt
