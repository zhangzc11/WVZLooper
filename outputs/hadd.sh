hadd -f zz.root MC_zz_*_powheg_1_results.root MC_ggzz*.root 
hadd -f ttz.root MC_ttz_ll_mll1_amcatnlo_1_results.root MC_ttz_llvv_mll10_amcatnlo_1_results.root
hadd -f twz.root MC_twz_ll_madgraph_1_results.root
hadd -f wz.root MC_wz_3lv_amcatnlo_1_results.root
#hadd -f sig.root MC_www_amcatnlo_1_results.root MC_wwz_amcatnlo_1_results.root MC_wzz_amcatnlo_1_results.root MC_zzz_amcatnlo_1_results.root MC_wh_ww_amcatnlo_1_results.root MC_wh_zz_amcatnlo_1_results.root MC_zh_ww_amcatnlo_1_results.root MC_zh_zz_amcatnlo_1_results.root
hadd -f sig.root MC_www_amcatnlo_1_results.root MC_wwz_amcatnlo_1_results.root MC_wzz_amcatnlo_1_results.root MC_zzz_amcatnlo_1_results.root MC_wh_zz_amcatnlo_1_results.root MC_zh_ww_amcatnlo_1_results.root MC_zh_zz_amcatnlo_1_results.root
hadd -f wwz.root MC_wwz_amcatnlo_1_results.root MC_zh_ww_amcatnlo_1_results.root
#hadd -f www.root MC_www_amcatnlo_1_results.root MC_wh_ww_amcatnlo_1_results.root
hadd -f www.root MC_www_amcatnlo_1_results.root 
hadd -f wzz.root MC_wzz_amcatnlo_1_results.root MC_wh_zz_amcatnlo_1_results.root
hadd -f zzz.root MC_zzz_amcatnlo_1_results.root MC_zh_zz_amcatnlo_1_results.root
hadd -f zh_wwz.root MC_zh_ww_amcatnlo_1_results.root
#hadd -f wh_www.root MC_wh_ww_amcatnlo_1_results.root
hadd -f wh_wzz.root MC_wh_zz_amcatnlo_1_results.root
hadd -f zh_zzz.root MC_zh_zz_amcatnlo_1_results.root
hadd -f nonh_wwz.root MC_wwz_amcatnlo_1_results.root
hadd -f nonh_www.root MC_www_amcatnlo_1_results.root
hadd -f nonh_wzz.root MC_wzz_amcatnlo_1_results.root
hadd -f nonh_zzz.root MC_zzz_amcatnlo_1_results.root
hadd -f dyttbar.root MC_dy_m1050_madgraph_1_results.root MC_dy_m50_madgraph_1_results.root MC_ttbar_dilep_madgraph_1_results.root MC_zg_llg_amcatnlo_1_results.root
hadd -f rare.root MC_sts_4f_leptonic_madgraph_1_results.root MC_tZq_ll_madgraph_1_results.root MC_ttw_lnu_amcatnlo_1_results.root MC_tw_antitopnofullhad_powheg_1_results.root MC_tw_topnofullhad_powheg_1_results.root MC_wwg_amcatnlo_1_results.root MC_wzg_amcatnlo_1_results.root
hadd -f higgs.root MC_ggh_hzz4l_powheg_1_results.root MC_vh_nonbbwwzz_amcatnlo_1_results.root MC_tth_nonbb_powheg_1_results.root
hadd -k -f data.root data_*.root
