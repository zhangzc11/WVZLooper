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
  echo "  -y    year                   (e.g. -y 2016)"
  echo "  -t    Ntuple type            (e.g. -t WVZMVA, WVZ, or Trilep etc.)"
  echo "  -v    Ntuple version         (e.g. -v 0.0.9, 0.1.0, or etc.)"
  echo "  -T    tag                    (e.g. -T test1)"
  echo "  -F    do fake                (e.g. -F)"
  echo
  exit
}

# Command-line opts
while getopts ":y:t:v:T:Fh" OPTION; do
  case $OPTION in
    y) YEAR=${OPTARG};;
    t) NTUPLETYPE=${OPTARG};;
    v) NTUPLEVERSION=${OPTARG};;
    T) TAG=${OPTARG};;
    F) FAKE=FAKE;;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${YEAR} ]; then usage; fi
if [ -z ${NTUPLETYPE}  ]; then usage; fi
if [ -z ${NTUPLEVERSION}  ]; then usage; fi
if [ -z ${TAG}  ]; then usage; fi

# to shift away the parsed options
shift $(($OPTIND - 1))

# Verbose
date
echo "================================================"
echo "$(basename $0) $*"
echo "$(basename $0) $*" >> $DIR/.$(basename $0).history
echo "------------------------------------------------"
echo "YEAR           : ${YEAR}"
echo "NTUPLEVERSION  : ${NTUPLEVERSION}"
echo "NTUPLETYPE     : ${NTUPLETYPE}"
echo "TAG            : ${TAG}"
echo "FAKE           : ${FAKE}"
echo "================================================"

# Sanity check that the sample referred exists
if [ ! -d /nfs-7/userdata/phchang/babies/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/ ]; then echo "Asked to run over the ${NTUPLEVERSION} of ${NTUPLETYPE} but /nfs-7/userdata/phchang/babies/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/ does not exists!"; exit; fi

echo " Will only show progress for ZZ to 4L sample. since that is the bottle neck processing"

rm -f .jobs_${YEAR}_${NTUPLEVERSION}_${NTUPLETYPE}_${TAG}_${FAKE}.txt
for i in $(ls -r /nfs-7/userdata/phchang/babies/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/); do
    if [[ $i == *"zz_4l_powheg"* ]]; then
        echo ./Analysis.exe ${i} ${NTUPLETYPE}${YEAR}_${NTUPLEVERSION} ${TAG} ${FAKE}" | tee outputs/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/${TAG}/${FAKE}${i}.log"  >> .jobs_${YEAR}_${NTUPLEVERSION}_${NTUPLETYPE}_${TAG}_${FAKE}.txt
    else
        echo ./Analysis.exe ${i} ${NTUPLETYPE}${YEAR}_${NTUPLEVERSION} ${TAG} ${FAKE}"> outputs/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/${TAG}/${FAKE}${i}.log"  >> .jobs_${YEAR}_${NTUPLEVERSION}_${NTUPLETYPE}_${TAG}_${FAKE}.txt
    fi
done

mkdir -p outputs/${NTUPLETYPE}${YEAR}_${NTUPLEVERSION}/${TAG}
sh rooutil/xargs.sh -n 6 .jobs_${YEAR}_${NTUPLEVERSION}_${NTUPLETYPE}_${TAG}_${FAKE}.txt
