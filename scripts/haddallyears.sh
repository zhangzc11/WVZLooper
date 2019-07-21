#!/bin/bash

usage()
{
    echo "Usage:"
    echo ""
    echo "   $ $0 ntuple_version2016 tagname2016 ntuple_version2017 tagname2017 ntuple_version2018 tagname2018"
    echo ""
    echo ""
    exit
}

if [ -z $1 ]; then usage; fi
if [ -z $2 ]; then usage; fi
if [ -z $3 ]; then usage; fi
if [ -z $4 ]; then usage; fi
if [ -z $5 ]; then usage; fi
if [ -z $6 ]; then usage; fi

if [ ! -d outputs/$1/$2 ]; then
    echo ">>> Error! cannot find outputs/$1/$2"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

if [ ! -d outputs/$3/$4 ]; then
    echo ">>> Error! cannot find outputs/$3/$4"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

if [ ! -d outputs/$5/$6 ]; then
    echo ">>> Error! cannot find outputs/$5/$6"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

mkdir -p outputs/${1}_${3}_${5}/${2}_${4}_${6}

echo "" > .hadd.cmds.txt

if [[ $1 == *"WVZ"* ]] || [[ $1 == *"Trilep"* ]] || [[ $1 == *"TTZ"* ]]; then
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/zz.root       outputs/${1}/${2}/zz.root       outputs/${3}/${4}/zz.root       outputs/${5}/${6}/zz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/ttz.root      outputs/${1}/${2}/ttz.root      outputs/${3}/${4}/ttz.root      outputs/${5}/${6}/ttz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/twz.root      outputs/${1}/${2}/twz.root      outputs/${3}/${4}/twz.root      outputs/${5}/${6}/twz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/wz.root       outputs/${1}/${2}/wz.root       outputs/${3}/${4}/wz.root       outputs/${5}/${6}/wz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/sig.root      outputs/${1}/${2}/sig.root      outputs/${3}/${4}/sig.root      outputs/${5}/${6}/sig.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/wwz.root      outputs/${1}/${2}/wwz.root      outputs/${3}/${4}/wwz.root      outputs/${5}/${6}/wwz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/www.root      outputs/${1}/${2}/www.root      outputs/${3}/${4}/www.root      outputs/${5}/${6}/www.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/wzz.root      outputs/${1}/${2}/wzz.root      outputs/${3}/${4}/wzz.root      outputs/${5}/${6}/wzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/zzz.root      outputs/${1}/${2}/zzz.root      outputs/${3}/${4}/zzz.root      outputs/${5}/${6}/zzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/zh_wwz.root   outputs/${1}/${2}/zh_wwz.root   outputs/${3}/${4}/zh_wwz.root   outputs/${5}/${6}/zh_wwz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/wh_www.root   outputs/${1}/${2}/wh_www.root   outputs/${3}/${4}/wh_www.root   outputs/${5}/${6}/wh_www.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/wh_wzz.root   outputs/${1}/${2}/wh_wzz.root   outputs/${3}/${4}/wh_wzz.root   outputs/${5}/${6}/wh_wzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/zh_zzz.root   outputs/${1}/${2}/zh_zzz.root   outputs/${3}/${4}/zh_zzz.root   outputs/${5}/${6}/zh_zzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/nonh_wwz.root outputs/${1}/${2}/nonh_wwz.root outputs/${3}/${4}/nonh_wwz.root outputs/${5}/${6}/nonh_wwz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/nonh_www.root outputs/${1}/${2}/nonh_www.root outputs/${3}/${4}/nonh_www.root outputs/${5}/${6}/nonh_www.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/nonh_wzz.root outputs/${1}/${2}/nonh_wzz.root outputs/${3}/${4}/nonh_wzz.root outputs/${5}/${6}/nonh_wzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/nonh_zzz.root outputs/${1}/${2}/nonh_zzz.root outputs/${3}/${4}/nonh_zzz.root outputs/${5}/${6}/nonh_zzz.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/dyttbar.root  outputs/${1}/${2}/dyttbar.root  outputs/${3}/${4}/dyttbar.root  outputs/${5}/${6}/dyttbar.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/rare.root     outputs/${1}/${2}/rare.root     outputs/${3}/${4}/rare.root     outputs/${5}/${6}/rare.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/higgs.root    outputs/${1}/${2}/higgs.root    outputs/${3}/${4}/higgs.root    outputs/${5}/${6}/higgs.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/data.root     outputs/${1}/${2}/data.root     outputs/${3}/${4}/data.root     outputs/${5}/${6}/data.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/dy.root       outputs/${1}/${2}/dy.root       outputs/${3}/${4}/dy.root       outputs/${5}/${6}/dy.root      " >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/ttbar.root    outputs/${1}/${2}/ttbar.root    outputs/${3}/${4}/ttbar.root    outputs/${5}/${6}/ttbar.root   " >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/rarevvv.root  outputs/${1}/${2}/rarevvv.root  outputs/${3}/${4}/rarevvv.root  outputs/${5}/${6}/rarevvv.root " >> .hadd.cmds.txt
    # echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/ddfake.root   outputs/${1}/${2}/ddfake.root   outputs/${3}/${4}/ddfake.root   outputs/${5}/${6}/ddfake.root  " >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/other.root    outputs/${1}/${2}/other.root    outputs/${3}/${4}/other.root    outputs/${5}/${6}/other.root   " >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/triother.root outputs/${1}/${2}/triother.root outputs/${3}/${4}/triother.root outputs/${5}/${6}/triother.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/othervvv.root outputs/${1}/${2}/othervvv.root outputs/${3}/${4}/othervvv.root outputs/${5}/${6}/othervvv.root" >> .hadd.cmds.txt
    echo "hadd -f outputs/${1}_${3}_${5}/${2}_${4}_${6}/othernoh.root outputs/${1}/${2}/othernoh.root outputs/${3}/${4}/othernoh.root outputs/${5}/${6}/othernoh.root" >> .hadd.cmds.txt
fi

sh rooutil/xargs.sh .hadd.cmds.txt
