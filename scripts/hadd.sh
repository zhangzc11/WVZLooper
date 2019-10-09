#!/bin/bash

usage()
{
    echo "Usage:"
    echo ""
    echo "   $ $0 ntuple_version tagname"
    echo ""
    echo ""
    exit
}

PREFIX=MC_
if [ -z $1 ]; then usage; fi
if [ -z $2 ]; then usage; fi
if [ -n "$3" ]; then PREFIX=BDTinputTree_MC_; fi

if [ ! -d outputs/$1/$2 ]; then
    echo ">>> Error! cannot find outputs/$1/$2"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

cd outputs/$1/$2

echo "" > .hadd.cmds.txt

# If the sample set is dilepton set use a different hadding scheme
if [[ $1 == *"Dilep"* ]]; then
    echo "hadd -f dy.root ${PREFIX}dy_m50_madgraph_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f top.root ${PREFIX}ttbar_dilep_madgraph_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f wj.root ${PREFIX}wjets_incl_madgraph_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f ww.root ${PREFIX}ww_2l_powheg_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f data.root DATA_data_Run*mm*.root" >> .hadd.cmds.txt
elif [[ $1 == *"WVZ"* ]] || [[ $1 == *"Trilep"* ]] || [[ $1 == *"TTZ"* ]]; then
    echo "hadd -f zz.root ${PREFIX}zz_4l_powheg_1_results.root ${PREFIX}zz_2l2q_powheg_1_results.root ${PREFIX}zz_2l2v_powheg_1_results.root ${PREFIX}ggzz_*.root" >> .hadd.cmds.txt
    echo "hadd -f ttz.root ${PREFIX}ttz_ll_mll1_amcatnlo_1_results.root ${PREFIX}ttz_llvv_mll10_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f twz.root ${PREFIX}twz_ll_madgraph_1_results.root" >> .hadd.cmds.txt
    if [[ $1 == *"0.1.1"* ]]; then # v0.1.1 has wjets and wgamma
        echo "hadd -f wz.root ${PREFIX}wz_3lv_amcatnlo_1_results.root ${PREFIX}wz_2l2q_*root" >> .hadd.cmds.txt
    else
        echo "hadd -f wz.root ${PREFIX}wz_3lv_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    if [[ $1 == *"WVZ2017_v0.1.12."* ]] || [[ $1 == *"WVZ2018_v0.1.12."* ]] || [[ $1 == *"WVZMVA2017_v0.1.15"* ]] || [[ $1 == *"WVZMVA2018_v0.1.15"* ]]; then
        # echo "hadd -f sig.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_ww_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root ${PREFIX}wwz_4l2v_amcatnlo_1_results.root" >> .hadd.cmds.txt
        echo "hadd -f sig.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_ww_4l_powheg_1_results.root ${PREFIX}ggzh_ww_4l_powheg_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root ${PREFIX}wwz_4l2v_amcatnlo_1_results.root" >> .hadd.cmds.txt
    elif [[ $1 == *"WVZMVA2016_v0.1.15"* ]]; then
        echo "hadd -f sig.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_ww_4l_powheg_1_results.root ${PREFIX}ggzh_ww_4l_powheg_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    else
        echo "hadd -f sig.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_ww_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    # if [[ $1 == *"WVZ2017_v0.1.12."* ]] || [[ $1 == *"WVZ2018_v0.1.12."* ]] || [[ $1 == *"WVZMVA2017_v0.1.15"* ]] || [[ $1 == *"WVZMVA2018_v0.1.15"* ]]; then
    if [[ $1 == *"WVZ2017_v0.1.12."* ]] || [[ $1 == *"WVZ2018_v0.1.12."* ]]; then
       echo "hadd -f wwz.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}zh_ww_amcatnlo_1_results.root ${PREFIX}wwz_4l2v_amcatnlo_1_results.root" >> .hadd.cmds.txt
    elif [[ $1 == *"WVZMVA2017_v0.1.15"* ]] || [[ $1 == *"WVZMVA2018_v0.1.15"* ]]; then
       echo "hadd -f wwz.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}zh_ww_4l_powheg_1_results.root ${PREFIX}ggzh_ww_4l_powheg_1_results.root ${PREFIX}wwz_4l2v_amcatnlo_1_results.root" >> .hadd.cmds.txt
    elif [[ $1 == *"WVZMVA2016_v0.1.15"* ]]; then
       echo "hadd -f wwz.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}zh_ww_4l_powheg_1_results.root ${PREFIX}ggzh_ww_4l_powheg_1_results.root" >> .hadd.cmds.txt
    else
       echo "hadd -f wwz.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}zh_ww_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    echo "hadd -f www.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f wzz.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f zzz.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f zh_wwz.root ${PREFIX}zh_ww_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f wh_www.root ${PREFIX}wh_ww_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f wh_wzz.root ${PREFIX}wh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f zh_zzz.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    if [[ $1 == *"WVZ2017_v0.1.12."* ]] || [[ $1 == *"WVZ2018_v0.1.12."* ]] || [[ $1 == *"WVZMVA2017_v0.1.15"* ]] || [[ $1 == *"WVZMVA2018_v0.1.15"* ]]; then
       echo "hadd -f nonh_wwz.root ${PREFIX}wwz_amcatnlo_1_results.root ${PREFIX}wwz_4l2v_amcatnlo_1_results.root" >> .hadd.cmds.txt
    else
       echo "hadd -f nonh_wwz.root ${PREFIX}wwz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    echo "hadd -f nonh_www.root ${PREFIX}www_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f nonh_wzz.root ${PREFIX}wzz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f nonh_zzz.root ${PREFIX}zzz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    #echo "hadd -f other.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}ggh_hzz4l_powheg_1_results.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttbar_amcatnlo_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}vbsww_madgraph_1_results.root ${PREFIX}wjets_incl_madgraph_1_results.root ${PREFIX}ww_incl_pythia_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f dyttbar.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}ttbar_dilep_madgraph_1_results.root ${PREFIX}zg_llg_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f ttbar.root ${PREFIX}ttbar_dilep_madgraph_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f dy.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}zg_llg_amcatnlo_1_results.root" >> .hadd.cmds.txt
    if [[ $1 == *"0.0.5"* ]]; then # older samples had vbsww and ww_inclusive
        echo "hadd -f rare.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_incl_pythia_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}vbsww_madgraph_1_results.root" >> .hadd.cmds.txt
    elif [[ $1 == *"0.1.1"* ]]; then # v0.1.1 has wjets and wgamma
        echo "hadd -f rare.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}wjets_incl_madgraph_1_results.root ${PREFIX}wg_*_1_results.root ${PREFIX}stt_*_1_results.root" >> .hadd.cmds.txt
    else # newer samples don't have vbsww and ww is dilepton filtered
        echo "hadd -f rare.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    if [[ $1 == *"0.0.5"* ]]; then # older samples had vbsww and ww_inclusive
        echo "hadd -f rarevvv.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_incl_pythia_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}vbsww_madgraph_1_results.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    elif [[ $1 == *"0.1.1"* ]]; then # v0.1.1 has wjets and wgamma
        echo "hadd -f rarevvv.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}wjets_incl_madgraph_1_results.root ${PREFIX}wg_*_1_results.root ${PREFIX}stt_*_1_results.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    else # newer samples don't have vbsww and ww is dilepton filtered
        echo "hadd -f rarevvv.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}www_amcatnlo_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}wh_ww_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    fi
    echo "hadd -f higgs.root ${PREFIX}ggh_hzz4l_powheg_1_results.root ${PREFIX}vh_nonbbwwzz_amcatnlo_1_results.root ${PREFIX}tth_nonbb_powheg_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f data.root DATA_data_Run*.root" >> .hadd.cmds.txt
    # echo "hadd -f data_prehem.root DATA_data_Run201*A*.root DATA_data_Run201*B*.root" >> .hadd.cmds.txt
    # echo "hadd -f ddfake.root FAKE_*.root" >> .hadd.cmds.txt
    echo "hadd -f other.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}ttbar_dilep_madgraph_1_results.root ${PREFIX}zg_llg_amcatnlo_1_results.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}ggh_hzz4l_powheg_1_results.root ${PREFIX}vh_nonbbwwzz_amcatnlo_1_results.root ${PREFIX}tth_nonbb_powheg_1_results.root" >> .hadd.cmds.txt

    echo "hadd -f triother.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}zz_4l_powheg_1_results.root ${PREFIX}zz_2l2q_powheg_1_results.root ${PREFIX}zz_2l2v_powheg_1_results.root ${PREFIX}ggzz_*.root ${PREFIX}ttz_ll_mll1_amcatnlo_1_results.root ${PREFIX}ttz_llvv_mll10_amcatnlo_1_results.root ${PREFIX}twz_ll_madgraph_1_results.root" >> .hadd.cmds.txt

    echo "hadd -f othervvv.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}ttbar_dilep_madgraph_1_results.root ${PREFIX}zg_llg_amcatnlo_1_results.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root ${PREFIX}ggh_hzz4l_powheg_1_results.root ${PREFIX}vh_nonbbwwzz_amcatnlo_1_results.root ${PREFIX}tth_nonbb_powheg_1_results.root ${PREFIX}wzz_amcatnlo_1_results.root ${PREFIX}wh_zz_amcatnlo_1_results.root ${PREFIX}zzz_amcatnlo_1_results.root ${PREFIX}zh_zz_amcatnlo_1_results.root" >> .hadd.cmds.txt
    echo "hadd -f othernoh.root ${PREFIX}dy_m1050_madgraph_1_results.root ${PREFIX}dy_m50_madgraph_1_results.root ${PREFIX}ttbar_dilep_madgraph_1_results.root ${PREFIX}zg_llg_amcatnlo_1_results.root ${PREFIX}sts_4f_leptonic_madgraph_1_results.root ${PREFIX}tZq_ll_madgraph_1_results.root ${PREFIX}ttw_lnu_amcatnlo_1_results.root ${PREFIX}tw_antitopnofullhad_powheg_1_results.root ${PREFIX}tw_topnofullhad_powheg_1_results.root ${PREFIX}ww_2l_powheg_1_results.root ${PREFIX}wwg_amcatnlo_1_results.root ${PREFIX}wzg_amcatnlo_1_results.root" >> .hadd.cmds.txt

fi

sh ../../../rooutil/xargs.sh .hadd.cmds.txt

cd - > /dev/null
