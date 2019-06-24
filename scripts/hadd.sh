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

if [ -z $1 ]; then usage; fi
if [ -z $2 ]; then usage; fi

if [ ! -d outputs/$1/$2 ]; then
    echo ">>> Error! cannot find outputs/$1/$2"
    echo ">>> Check your ntuple_version and tagname that you've provided!"
    exit
fi

cd outputs/$1/$2

# If the sample set is dilepton set use a different hadding scheme
if [[ $1 == *"Dilep"* ]]; then
    hadd -f dy.root MC_dy_m50_madgraph_1_results.root
    hadd -f top.root MC_ttbar_dilep_madgraph_1_results.root
    hadd -f wj.root MC_wjets_incl_madgraph_1_results.root
    hadd -f ww.root MC_ww_2l_powheg_1_results.root
    hadd -f data.root DATA_data_Run*mm*.root
elif [[ $1 == *"WVZ"* ]] || [[ $1 == *"Trilep"* ]]; then
    hadd -f zz.root MC_zz_4l_powheg_1_results.root MC_zz_2l2q_powheg_1_results.root MC_zz_2l2v_powheg_1_results.root MC_ggzz_*.root
    hadd -f ttz.root MC_ttz_ll_mll1_amcatnlo_1_results.root MC_ttz_llvv_mll10_amcatnlo_1_results.root
    hadd -f twz.root MC_twz_ll_madgraph_1_results.root
    if [[ $1 == *"0.1.1"* ]]; then # v0.1.1 has wjets and wgamma
        hadd -f wz.root MC_wz_3lv_amcatnlo_1_results.root MC_wz_2l2q_*root
    else
        hadd -f wz.root MC_wz_3lv_amcatnlo_1_results.root
    fi
    hadd -f sig.root MC_www_amcatnlo_1_results.root MC_wwz_amcatnlo_1_results.root MC_wzz_amcatnlo_1_results.root MC_zzz_amcatnlo_1_results.root MC_wh_ww_amcatnlo_1_results.root MC_wh_zz_amcatnlo_1_results.root MC_zh_ww_amcatnlo_1_results.root MC_zh_zz_amcatnlo_1_results.root
    hadd -f wwz.root MC_wwz_amcatnlo_1_results.root MC_zh_ww_amcatnlo_1_results.root
    hadd -f www.root MC_www_amcatnlo_1_results.root MC_wh_ww_amcatnlo_1_results.root
    hadd -f wzz.root MC_wzz_amcatnlo_1_results.root MC_wh_zz_amcatnlo_1_results.root
    hadd -f zzz.root MC_zzz_amcatnlo_1_results.root MC_zh_zz_amcatnlo_1_results.root
    hadd -f zh_wwz.root MC_zh_ww_amcatnlo_1_results.root
    hadd -f wh_www.root MC_wh_ww_amcatnlo_1_results.root
    hadd -f wh_wzz.root MC_wh_zz_amcatnlo_1_results.root
    hadd -f zh_zzz.root MC_zh_zz_amcatnlo_1_results.root
    hadd -f nonh_wwz.root MC_wwz_amcatnlo_1_results.root
    hadd -f nonh_www.root MC_www_amcatnlo_1_results.root
    hadd -f nonh_wzz.root MC_wzz_amcatnlo_1_results.root
    hadd -f nonh_zzz.root MC_zzz_amcatnlo_1_results.root
    #hadd -f other.root MC_dy_m1050_madgraph_1_results.root MC_dy_m50_madgraph_1_results.root MC_ggh_hzz4l_powheg_1_results.root MC_sts_4f_leptonic_madgraph_1_results.root MC_tZq_ll_madgraph_1_results.root MC_ttbar_amcatnlo_1_results.root MC_ttw_lnu_amcatnlo_1_results.root MC_tw_antitopnofullhad_powheg_1_results.root MC_tw_topnofullhad_powheg_1_results.root MC_vbsww_madgraph_1_results.root MC_wjets_incl_madgraph_1_results.root MC_ww_incl_pythia_1_results.root MC_wwg_amcatnlo_1_results.root MC_wzg_amcatnlo_1_results.root
    hadd -f dyttbar.root MC_dy_m1050_madgraph_1_results.root MC_dy_m50_madgraph_1_results.root MC_ttbar_dilep_madgraph_1_results.root MC_zg_llg_amcatnlo_1_results.root
    hadd -f ttbar.root MC_ttbar_dilep_madgraph_1_results.root
    hadd -f dy.root MC_dy_m1050_madgraph_1_results.root MC_dy_m50_madgraph_1_results.root MC_zg_llg_amcatnlo_1_results.root
    if [[ $1 == *"0.0.5"* ]]; then # older samples had vbsww and ww_inclusive
        hadd -f rare.root MC_sts_4f_leptonic_madgraph_1_results.root MC_tZq_ll_madgraph_1_results.root MC_ttw_lnu_amcatnlo_1_results.root MC_tw_antitopnofullhad_powheg_1_results.root MC_tw_topnofullhad_powheg_1_results.root MC_ww_incl_pythia_1_results.root MC_wwg_amcatnlo_1_results.root MC_wzg_amcatnlo_1_results.root MC_vbsww_madgraph_1_results.root
    elif [[ $1 == *"0.1.1"* ]]; then # v0.1.1 has wjets and wgamma
        hadd -f rare.root MC_sts_4f_leptonic_madgraph_1_results.root MC_tZq_ll_madgraph_1_results.root MC_ttw_lnu_amcatnlo_1_results.root MC_tw_antitopnofullhad_powheg_1_results.root MC_tw_topnofullhad_powheg_1_results.root MC_ww_2l_powheg_1_results.root MC_wwg_amcatnlo_1_results.root MC_wzg_amcatnlo_1_results.root MC_wjets_incl_madgraph_1_results.root MC_wg_*_1_results.root MC_stt_*_1_results.root
    else # newer samples don't have vbsww and ww is dilepton filtered
        hadd -f rare.root MC_sts_4f_leptonic_madgraph_1_results.root MC_tZq_ll_madgraph_1_results.root MC_ttw_lnu_amcatnlo_1_results.root MC_tw_antitopnofullhad_powheg_1_results.root MC_tw_topnofullhad_powheg_1_results.root MC_ww_2l_powheg_1_results.root MC_wwg_amcatnlo_1_results.root MC_wzg_amcatnlo_1_results.root
    fi
    hadd -f higgs.root MC_ggh_hzz4l_powheg_1_results.root MC_vh_nonbbwwzz_amcatnlo_1_results.root MC_tth_nonbb_powheg_1_results.root
    hadd -f data.root DATA_data_Run*.root
    hadd -f ddfake.root FAKE_*.root
fi

cd - > /dev/null
