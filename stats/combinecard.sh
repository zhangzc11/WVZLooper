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
  echo "  -s    sample_set             (e.g. -s WVZ2016_v0.1.6)"
  echo "  -t    tag                    (e.g. -t y2016_test)"
  echo
  exit
}

# Command-line opts
while getopts ":s:t:h" OPTION; do
  case $OPTION in
    s) SAMPLESET=${OPTARG};;
    t) TAG=${OPTARG};;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${SAMPLESET} ]; then usage; fi
if [ -z ${TAG}  ]; then usage; fi

# to shift away the parsed options
shift $(($OPTIND - 1))

# Verbose
date
echo "================================================"
echo "$(basename $0) $*"
echo "$(basename $0) $*" >> $DIR/.$(basename $0).history
echo "------------------------------------------------"
echo "SAMPLESET      : ${SAMPLESET}"
echo "TAG            : ${TAG}"
echo "================================================"

combineCards.py \
    emu1=stats/${SAMPLESET}/${TAG}/emu_datacard_bin1.txt \
    emu2=stats/${SAMPLESET}/${TAG}/emu_datacard_bin2.txt \
    emu3=stats/${SAMPLESET}/${TAG}/emu_datacard_bin3.txt \
    emu4=stats/${SAMPLESET}/${TAG}/emu_datacard_bin4.txt \
    emu5=stats/${SAMPLESET}/${TAG}/emu_datacard_bin5.txt \
    offz=stats/${SAMPLESET}/${TAG}/offz_datacard_bin1.txt  > stats/${SAMPLESET}/${TAG}/stat.txt
