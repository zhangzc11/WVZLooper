#include "wvztree.h"
wvztree wvz;

void wvztree::Init(TTree *tree) {
  gen_V_p4_branch = tree->GetBranch("gen_V_p4");
  if (gen_V_p4_branch) gen_V_p4_branch->SetAddress(&gen_V_p4_);
  gen_lep_p4_branch = tree->GetBranch("gen_lep_p4");
  if (gen_lep_p4_branch) gen_lep_p4_branch->SetAddress(&gen_lep_p4_);
  gen_child_p4_branch = tree->GetBranch("gen_child_p4");
  if (gen_child_p4_branch) gen_child_p4_branch->SetAddress(&gen_child_p4_);
  gen_part_p4_branch = tree->GetBranch("gen_part_p4");
  if (gen_part_p4_branch) gen_part_p4_branch->SetAddress(&gen_part_p4_);
  lep_p4_branch = tree->GetBranch("lep_p4");
  if (lep_p4_branch) lep_p4_branch->SetAddress(&lep_p4_);
  met_p4_branch = tree->GetBranch("met_p4");
  if (met_p4_branch) met_p4_branch->SetAddress(&met_p4_);
  jets_p4_branch = tree->GetBranch("jets_p4");
  if (jets_p4_branch) jets_p4_branch->SetAddress(&jets_p4_);
  jets_cen_p4_branch = tree->GetBranch("jets_cen_p4");
  if (jets_cen_p4_branch) jets_cen_p4_branch->SetAddress(&jets_cen_p4_);
  rawjets_p4_branch = tree->GetBranch("rawjets_p4");
  if (rawjets_p4_branch) rawjets_p4_branch->SetAddress(&rawjets_p4_);

  tree->SetMakeClass(1);

  run_branch = tree->GetBranch("run");
  if (run_branch) run_branch->SetAddress(&run_);
  lumi_branch = tree->GetBranch("lumi");
  if (lumi_branch) lumi_branch->SetAddress(&lumi_);
  evt_branch = tree->GetBranch("evt");
  if (evt_branch) evt_branch->SetAddress(&evt_);
  isData_branch = tree->GetBranch("isData");
  if (isData_branch) isData_branch->SetAddress(&isData_);
  evt_scale1fb_branch = tree->GetBranch("evt_scale1fb");
  if (evt_scale1fb_branch) evt_scale1fb_branch->SetAddress(&evt_scale1fb_);
  genps_weight_branch = tree->GetBranch("genps_weight");
  if (genps_weight_branch) genps_weight_branch->SetAddress(&genps_weight_);
  xsec_br_branch = tree->GetBranch("xsec_br");
  if (xsec_br_branch) xsec_br_branch->SetAddress(&xsec_br_);
  evt_passgoodrunlist_branch = tree->GetBranch("evt_passgoodrunlist");
  if (evt_passgoodrunlist_branch) evt_passgoodrunlist_branch->SetAddress(&evt_passgoodrunlist_);
  CMS4path_branch = tree->GetBranch("CMS4path");
  if (CMS4path_branch) CMS4path_branch->SetAddress(&CMS4path_);
  CMS4index_branch = tree->GetBranch("CMS4index");
  if (CMS4index_branch) CMS4index_branch->SetAddress(&CMS4index_);
  weight_fr_r1_f1_branch = tree->GetBranch("weight_fr_r1_f1");
  if (weight_fr_r1_f1_branch) weight_fr_r1_f1_branch->SetAddress(&weight_fr_r1_f1_);
  weight_fr_r1_f2_branch = tree->GetBranch("weight_fr_r1_f2");
  if (weight_fr_r1_f2_branch) weight_fr_r1_f2_branch->SetAddress(&weight_fr_r1_f2_);
  weight_fr_r1_f0p5_branch = tree->GetBranch("weight_fr_r1_f0p5");
  if (weight_fr_r1_f0p5_branch) weight_fr_r1_f0p5_branch->SetAddress(&weight_fr_r1_f0p5_);
  weight_fr_r2_f1_branch = tree->GetBranch("weight_fr_r2_f1");
  if (weight_fr_r2_f1_branch) weight_fr_r2_f1_branch->SetAddress(&weight_fr_r2_f1_);
  weight_fr_r2_f2_branch = tree->GetBranch("weight_fr_r2_f2");
  if (weight_fr_r2_f2_branch) weight_fr_r2_f2_branch->SetAddress(&weight_fr_r2_f2_);
  weight_fr_r2_f0p5_branch = tree->GetBranch("weight_fr_r2_f0p5");
  if (weight_fr_r2_f0p5_branch) weight_fr_r2_f0p5_branch->SetAddress(&weight_fr_r2_f0p5_);
  weight_fr_r0p5_f1_branch = tree->GetBranch("weight_fr_r0p5_f1");
  if (weight_fr_r0p5_f1_branch) weight_fr_r0p5_f1_branch->SetAddress(&weight_fr_r0p5_f1_);
  weight_fr_r0p5_f2_branch = tree->GetBranch("weight_fr_r0p5_f2");
  if (weight_fr_r0p5_f2_branch) weight_fr_r0p5_f2_branch->SetAddress(&weight_fr_r0p5_f2_);
  weight_fr_r0p5_f0p5_branch = tree->GetBranch("weight_fr_r0p5_f0p5");
  if (weight_fr_r0p5_f0p5_branch) weight_fr_r0p5_f0p5_branch->SetAddress(&weight_fr_r0p5_f0p5_);
  weight_pdf_up_branch = tree->GetBranch("weight_pdf_up");
  if (weight_pdf_up_branch) weight_pdf_up_branch->SetAddress(&weight_pdf_up_);
  weight_pdf_down_branch = tree->GetBranch("weight_pdf_down");
  if (weight_pdf_down_branch) weight_pdf_down_branch->SetAddress(&weight_pdf_down_);
  weight_alphas_down_branch = tree->GetBranch("weight_alphas_down");
  if (weight_alphas_down_branch) weight_alphas_down_branch->SetAddress(&weight_alphas_down_);
  weight_alphas_up_branch = tree->GetBranch("weight_alphas_up");
  if (weight_alphas_up_branch) weight_alphas_up_branch->SetAddress(&weight_alphas_up_);
  HLT_DoubleMu_branch = tree->GetBranch("HLT_DoubleMu");
  if (HLT_DoubleMu_branch) HLT_DoubleMu_branch->SetAddress(&HLT_DoubleMu_);
  HLT_DoubleEl_branch = tree->GetBranch("HLT_DoubleEl");
  if (HLT_DoubleEl_branch) HLT_DoubleEl_branch->SetAddress(&HLT_DoubleEl_);
  HLT_MuEG_branch = tree->GetBranch("HLT_MuEG");
  if (HLT_MuEG_branch) HLT_MuEG_branch->SetAddress(&HLT_MuEG_);
  pass_duplicate_ee_em_mm_branch = tree->GetBranch("pass_duplicate_ee_em_mm");
  if (pass_duplicate_ee_em_mm_branch) pass_duplicate_ee_em_mm_branch->SetAddress(&pass_duplicate_ee_em_mm_);
  pass_duplicate_mm_em_ee_branch = tree->GetBranch("pass_duplicate_mm_em_ee");
  if (pass_duplicate_mm_em_ee_branch) pass_duplicate_mm_em_ee_branch->SetAddress(&pass_duplicate_mm_em_ee_);
  gen_ht_branch = tree->GetBranch("gen_ht");
  if (gen_ht_branch) gen_ht_branch->SetAddress(&gen_ht_);
  gen_V_pt_branch = tree->GetBranch("gen_V_pt");
  if (gen_V_pt_branch) gen_V_pt_branch->SetAddress(&gen_V_pt_);
  gen_V_eta_branch = tree->GetBranch("gen_V_eta");
  if (gen_V_eta_branch) gen_V_eta_branch->SetAddress(&gen_V_eta_);
  gen_V_phi_branch = tree->GetBranch("gen_V_phi");
  if (gen_V_phi_branch) gen_V_phi_branch->SetAddress(&gen_V_phi_);
  gen_V_mass_branch = tree->GetBranch("gen_V_mass");
  if (gen_V_mass_branch) gen_V_mass_branch->SetAddress(&gen_V_mass_);
  gen_V_id_branch = tree->GetBranch("gen_V_id");
  if (gen_V_id_branch) gen_V_id_branch->SetAddress(&gen_V_id_);
  gen_lep_pt_branch = tree->GetBranch("gen_lep_pt");
  if (gen_lep_pt_branch) gen_lep_pt_branch->SetAddress(&gen_lep_pt_);
  gen_lep_eta_branch = tree->GetBranch("gen_lep_eta");
  if (gen_lep_eta_branch) gen_lep_eta_branch->SetAddress(&gen_lep_eta_);
  gen_lep_phi_branch = tree->GetBranch("gen_lep_phi");
  if (gen_lep_phi_branch) gen_lep_phi_branch->SetAddress(&gen_lep_phi_);
  gen_lep_mass_branch = tree->GetBranch("gen_lep_mass");
  if (gen_lep_mass_branch) gen_lep_mass_branch->SetAddress(&gen_lep_mass_);
  gen_lep_id_branch = tree->GetBranch("gen_lep_id");
  if (gen_lep_id_branch) gen_lep_id_branch->SetAddress(&gen_lep_id_);
  gen_child_pt_branch = tree->GetBranch("gen_child_pt");
  if (gen_child_pt_branch) gen_child_pt_branch->SetAddress(&gen_child_pt_);
  gen_child_eta_branch = tree->GetBranch("gen_child_eta");
  if (gen_child_eta_branch) gen_child_eta_branch->SetAddress(&gen_child_eta_);
  gen_child_phi_branch = tree->GetBranch("gen_child_phi");
  if (gen_child_phi_branch) gen_child_phi_branch->SetAddress(&gen_child_phi_);
  gen_child_mass_branch = tree->GetBranch("gen_child_mass");
  if (gen_child_mass_branch) gen_child_mass_branch->SetAddress(&gen_child_mass_);
  gen_child_id_branch = tree->GetBranch("gen_child_id");
  if (gen_child_id_branch) gen_child_id_branch->SetAddress(&gen_child_id_);
  gen_part_pt_branch = tree->GetBranch("gen_part_pt");
  if (gen_part_pt_branch) gen_part_pt_branch->SetAddress(&gen_part_pt_);
  gen_part_eta_branch = tree->GetBranch("gen_part_eta");
  if (gen_part_eta_branch) gen_part_eta_branch->SetAddress(&gen_part_eta_);
  gen_part_phi_branch = tree->GetBranch("gen_part_phi");
  if (gen_part_phi_branch) gen_part_phi_branch->SetAddress(&gen_part_phi_);
  gen_part_mass_branch = tree->GetBranch("gen_part_mass");
  if (gen_part_mass_branch) gen_part_mass_branch->SetAddress(&gen_part_mass_);
  gen_part_id_branch = tree->GetBranch("gen_part_id");
  if (gen_part_id_branch) gen_part_id_branch->SetAddress(&gen_part_id_);
  gen_part_mother_id_branch = tree->GetBranch("gen_part_mother_id");
  if (gen_part_mother_id_branch) gen_part_mother_id_branch->SetAddress(&gen_part_mother_id_);
  gen_part_grandma_id_branch = tree->GetBranch("gen_part_grandma_id");
  if (gen_part_grandma_id_branch) gen_part_grandma_id_branch->SetAddress(&gen_part_grandma_id_);
  gen_part_status_branch = tree->GetBranch("gen_part_status");
  if (gen_part_status_branch) gen_part_status_branch->SetAddress(&gen_part_status_);
  VHchannel_branch = tree->GetBranch("VHchannel");
  if (VHchannel_branch) VHchannel_branch->SetAddress(&VHchannel_);
  Higgschannel_branch = tree->GetBranch("Higgschannel");
  if (Higgschannel_branch) Higgschannel_branch->SetAddress(&Higgschannel_);
  nGenTauClean_branch = tree->GetBranch("nGenTauClean");
  if (nGenTauClean_branch) nGenTauClean_branch->SetAddress(&nGenTauClean_);
  nGenTau_branch = tree->GetBranch("nGenTau");
  if (nGenTau_branch) nGenTau_branch->SetAddress(&nGenTau_);
  hasTau_branch = tree->GetBranch("hasTau");
  if (hasTau_branch) hasTau_branch->SetAddress(&hasTau_);
  nLightLep_branch = tree->GetBranch("nLightLep");
  if (nLightLep_branch) nLightLep_branch->SetAddress(&nLightLep_);
  firstgoodvertex_branch = tree->GetBranch("firstgoodvertex");
  if (firstgoodvertex_branch) firstgoodvertex_branch->SetAddress(&firstgoodvertex_);
  nvtx_branch = tree->GetBranch("nvtx");
  if (nvtx_branch) nvtx_branch->SetAddress(&nvtx_);
  nTrueInt_branch = tree->GetBranch("nTrueInt");
  if (nTrueInt_branch) nTrueInt_branch->SetAddress(&nTrueInt_);
  lep_pt_branch = tree->GetBranch("lep_pt");
  if (lep_pt_branch) lep_pt_branch->SetAddress(&lep_pt_);
  lep_eta_branch = tree->GetBranch("lep_eta");
  if (lep_eta_branch) lep_eta_branch->SetAddress(&lep_eta_);
  lep_etaSC_branch = tree->GetBranch("lep_etaSC");
  if (lep_etaSC_branch) lep_etaSC_branch->SetAddress(&lep_etaSC_);
  lep_phi_branch = tree->GetBranch("lep_phi");
  if (lep_phi_branch) lep_phi_branch->SetAddress(&lep_phi_);
  lep_energy_branch = tree->GetBranch("lep_energy");
  if (lep_energy_branch) lep_energy_branch->SetAddress(&lep_energy_);
  lep_mva_branch = tree->GetBranch("lep_mva");
  if (lep_mva_branch) lep_mva_branch->SetAddress(&lep_mva_);
  lep_mvaIso_branch = tree->GetBranch("lep_mvaIso");
  if (lep_mvaIso_branch) lep_mvaIso_branch->SetAddress(&lep_mvaIso_);
  lep_relIso04DB_branch = tree->GetBranch("lep_relIso04DB");
  if (lep_relIso04DB_branch) lep_relIso04DB_branch->SetAddress(&lep_relIso04DB_);
  lep_relIso03EA_branch = tree->GetBranch("lep_relIso03EA");
  if (lep_relIso03EA_branch) lep_relIso03EA_branch->SetAddress(&lep_relIso03EA_);
  lep_relIso03EAv4_branch = tree->GetBranch("lep_relIso03EAv4");
  if (lep_relIso03EAv4_branch) lep_relIso03EAv4_branch->SetAddress(&lep_relIso03EAv4_);
  lep_relIso03EAwLep_branch = tree->GetBranch("lep_relIso03EAwLep");
  if (lep_relIso03EAwLep_branch) lep_relIso03EAwLep_branch->SetAddress(&lep_relIso03EAwLep_);
  lep_ip3d_branch = tree->GetBranch("lep_ip3d");
  if (lep_ip3d_branch) lep_ip3d_branch->SetAddress(&lep_ip3d_);
  lep_sip3d_branch = tree->GetBranch("lep_sip3d");
  if (lep_sip3d_branch) lep_sip3d_branch->SetAddress(&lep_sip3d_);
  lep_dxy_branch = tree->GetBranch("lep_dxy");
  if (lep_dxy_branch) lep_dxy_branch->SetAddress(&lep_dxy_);
  lep_dz_branch = tree->GetBranch("lep_dz");
  if (lep_dz_branch) lep_dz_branch->SetAddress(&lep_dz_);
  lep_mc_motherid_branch = tree->GetBranch("lep_mc_motherid");
  if (lep_mc_motherid_branch) lep_mc_motherid_branch->SetAddress(&lep_mc_motherid_);
  lep_mc_id_branch = tree->GetBranch("lep_mc_id");
  if (lep_mc_id_branch) lep_mc_id_branch->SetAddress(&lep_mc_id_);
  lep_motherIdv2_branch = tree->GetBranch("lep_motherIdv2");
  if (lep_motherIdv2_branch) lep_motherIdv2_branch->SetAddress(&lep_motherIdv2_);
  lep_idx_branch = tree->GetBranch("lep_idx");
  if (lep_idx_branch) lep_idx_branch->SetAddress(&lep_idx_);
  lep_id_branch = tree->GetBranch("lep_id");
  if (lep_id_branch) lep_id_branch->SetAddress(&lep_id_);
  lep_isTightPOG_branch = tree->GetBranch("lep_isTightPOG");
  if (lep_isTightPOG_branch) lep_isTightPOG_branch->SetAddress(&lep_isTightPOG_);
  lep_isMediumPOG_branch = tree->GetBranch("lep_isMediumPOG");
  if (lep_isMediumPOG_branch) lep_isMediumPOG_branch->SetAddress(&lep_isMediumPOG_);
  lep_isMVAwp80NoIsoPOG_branch = tree->GetBranch("lep_isMVAwp80NoIsoPOG");
  if (lep_isMVAwp80NoIsoPOG_branch) lep_isMVAwp80NoIsoPOG_branch->SetAddress(&lep_isMVAwp80NoIsoPOG_);
  lep_isMVAwp90NoIsoPOG_branch = tree->GetBranch("lep_isMVAwp90NoIsoPOG");
  if (lep_isMVAwp90NoIsoPOG_branch) lep_isMVAwp90NoIsoPOG_branch->SetAddress(&lep_isMVAwp90NoIsoPOG_);
  lep_isMVAwpLooseNoIsoPOG_branch = tree->GetBranch("lep_isMVAwpLooseNoIsoPOG");
  if (lep_isMVAwpLooseNoIsoPOG_branch) lep_isMVAwpLooseNoIsoPOG_branch->SetAddress(&lep_isMVAwpLooseNoIsoPOG_);
  lep_isMVAwp80IsoPOG_branch = tree->GetBranch("lep_isMVAwp80IsoPOG");
  if (lep_isMVAwp80IsoPOG_branch) lep_isMVAwp80IsoPOG_branch->SetAddress(&lep_isMVAwp80IsoPOG_);
  lep_isMVAwp90IsoPOG_branch = tree->GetBranch("lep_isMVAwp90IsoPOG");
  if (lep_isMVAwp90IsoPOG_branch) lep_isMVAwp90IsoPOG_branch->SetAddress(&lep_isMVAwp90IsoPOG_);
  lep_isMVAwpLooseIsoPOG_branch = tree->GetBranch("lep_isMVAwpLooseIsoPOG");
  if (lep_isMVAwpLooseIsoPOG_branch) lep_isMVAwpLooseIsoPOG_branch->SetAddress(&lep_isMVAwpLooseIsoPOG_);
  lep_isMVAwpHZZIsoPOG_branch = tree->GetBranch("lep_isMVAwpHZZIsoPOG");
  if (lep_isMVAwpHZZIsoPOG_branch) lep_isMVAwpHZZIsoPOG_branch->SetAddress(&lep_isMVAwpHZZIsoPOG_);
  lep_isCutBasedNoIsoVetoPOG_branch = tree->GetBranch("lep_isCutBasedNoIsoVetoPOG");
  if (lep_isCutBasedNoIsoVetoPOG_branch) lep_isCutBasedNoIsoVetoPOG_branch->SetAddress(&lep_isCutBasedNoIsoVetoPOG_);
  lep_isCutBasedNoIsoLoosePOG_branch = tree->GetBranch("lep_isCutBasedNoIsoLoosePOG");
  if (lep_isCutBasedNoIsoLoosePOG_branch) lep_isCutBasedNoIsoLoosePOG_branch->SetAddress(&lep_isCutBasedNoIsoLoosePOG_);
  lep_isCutBasedNoIsoMediumPOG_branch = tree->GetBranch("lep_isCutBasedNoIsoMediumPOG");
  if (lep_isCutBasedNoIsoMediumPOG_branch) lep_isCutBasedNoIsoMediumPOG_branch->SetAddress(&lep_isCutBasedNoIsoMediumPOG_);
  lep_isCutBasedNoIsoTightPOG_branch = tree->GetBranch("lep_isCutBasedNoIsoTightPOG");
  if (lep_isCutBasedNoIsoTightPOG_branch) lep_isCutBasedNoIsoTightPOG_branch->SetAddress(&lep_isCutBasedNoIsoTightPOG_);
  lep_isCutBasedIsoVetoPOG_branch = tree->GetBranch("lep_isCutBasedIsoVetoPOG");
  if (lep_isCutBasedIsoVetoPOG_branch) lep_isCutBasedIsoVetoPOG_branch->SetAddress(&lep_isCutBasedIsoVetoPOG_);
  lep_isCutBasedIsoLoosePOG_branch = tree->GetBranch("lep_isCutBasedIsoLoosePOG");
  if (lep_isCutBasedIsoLoosePOG_branch) lep_isCutBasedIsoLoosePOG_branch->SetAddress(&lep_isCutBasedIsoLoosePOG_);
  lep_isCutBasedIsoMediumPOG_branch = tree->GetBranch("lep_isCutBasedIsoMediumPOG");
  if (lep_isCutBasedIsoMediumPOG_branch) lep_isCutBasedIsoMediumPOG_branch->SetAddress(&lep_isCutBasedIsoMediumPOG_);
  lep_isCutBasedIsoTightPOG_branch = tree->GetBranch("lep_isCutBasedIsoTightPOG");
  if (lep_isCutBasedIsoTightPOG_branch) lep_isCutBasedIsoTightPOG_branch->SetAddress(&lep_isCutBasedIsoTightPOG_);
  met_pt_branch = tree->GetBranch("met_pt");
  if (met_pt_branch) met_pt_branch->SetAddress(&met_pt_);
  met_phi_branch = tree->GetBranch("met_phi");
  if (met_phi_branch) met_phi_branch->SetAddress(&met_phi_);
  met_up_pt_branch = tree->GetBranch("met_up_pt");
  if (met_up_pt_branch) met_up_pt_branch->SetAddress(&met_up_pt_);
  met_up_phi_branch = tree->GetBranch("met_up_phi");
  if (met_up_phi_branch) met_up_phi_branch->SetAddress(&met_up_phi_);
  met_dn_pt_branch = tree->GetBranch("met_dn_pt");
  if (met_dn_pt_branch) met_dn_pt_branch->SetAddress(&met_dn_pt_);
  met_dn_phi_branch = tree->GetBranch("met_dn_phi");
  if (met_dn_phi_branch) met_dn_phi_branch->SetAddress(&met_dn_phi_);
  met_gen_pt_branch = tree->GetBranch("met_gen_pt");
  if (met_gen_pt_branch) met_gen_pt_branch->SetAddress(&met_gen_pt_);
  met_gen_phi_branch = tree->GetBranch("met_gen_phi");
  if (met_gen_phi_branch) met_gen_phi_branch->SetAddress(&met_gen_phi_);
  passesMETfiltersRun2_branch = tree->GetBranch("passesMETfiltersRun2");
  if (passesMETfiltersRun2_branch) passesMETfiltersRun2_branch->SetAddress(&passesMETfiltersRun2_);
  jets_pt_branch = tree->GetBranch("jets_pt");
  if (jets_pt_branch) jets_pt_branch->SetAddress(&jets_pt_);
  jets_eta_branch = tree->GetBranch("jets_eta");
  if (jets_eta_branch) jets_eta_branch->SetAddress(&jets_eta_);
  jets_phi_branch = tree->GetBranch("jets_phi");
  if (jets_phi_branch) jets_phi_branch->SetAddress(&jets_phi_);
  jets_mass_branch = tree->GetBranch("jets_mass");
  if (jets_mass_branch) jets_mass_branch->SetAddress(&jets_mass_);
  jets_btag_score_branch = tree->GetBranch("jets_btag_score");
  if (jets_btag_score_branch) jets_btag_score_branch->SetAddress(&jets_btag_score_);
  jets_cen_pt_branch = tree->GetBranch("jets_cen_pt");
  if (jets_cen_pt_branch) jets_cen_pt_branch->SetAddress(&jets_cen_pt_);
  jets_cen_eta_branch = tree->GetBranch("jets_cen_eta");
  if (jets_cen_eta_branch) jets_cen_eta_branch->SetAddress(&jets_cen_eta_);
  jets_cen_phi_branch = tree->GetBranch("jets_cen_phi");
  if (jets_cen_phi_branch) jets_cen_phi_branch->SetAddress(&jets_cen_phi_);
  jets_cen_mass_branch = tree->GetBranch("jets_cen_mass");
  if (jets_cen_mass_branch) jets_cen_mass_branch->SetAddress(&jets_cen_mass_);
  rawjets_pt_branch = tree->GetBranch("rawjets_pt");
  if (rawjets_pt_branch) rawjets_pt_branch->SetAddress(&rawjets_pt_);
  rawjets_eta_branch = tree->GetBranch("rawjets_eta");
  if (rawjets_eta_branch) rawjets_eta_branch->SetAddress(&rawjets_eta_);
  rawjets_phi_branch = tree->GetBranch("rawjets_phi");
  if (rawjets_phi_branch) rawjets_phi_branch->SetAddress(&rawjets_phi_);
  rawjets_mass_branch = tree->GetBranch("rawjets_mass");
  if (rawjets_mass_branch) rawjets_mass_branch->SetAddress(&rawjets_mass_);
  rawjets_JEC_branch = tree->GetBranch("rawjets_JEC");
  if (rawjets_JEC_branch) rawjets_JEC_branch->SetAddress(&rawjets_JEC_);
  rawjets_passJetID_branch = tree->GetBranch("rawjets_passJetID");
  if (rawjets_passJetID_branch) rawjets_passJetID_branch->SetAddress(&rawjets_passJetID_);
  rawjets_isORwLep_branch = tree->GetBranch("rawjets_isORwLep");
  if (rawjets_isORwLep_branch) rawjets_isORwLep_branch->SetAddress(&rawjets_isORwLep_);
  nj_branch = tree->GetBranch("nj");
  if (nj_branch) nj_branch->SetAddress(&nj_);
  nj_up_branch = tree->GetBranch("nj_up");
  if (nj_up_branch) nj_up_branch->SetAddress(&nj_up_);
  nj_dn_branch = tree->GetBranch("nj_dn");
  if (nj_dn_branch) nj_dn_branch->SetAddress(&nj_dn_);
  nb_branch = tree->GetBranch("nb");
  if (nb_branch) nb_branch->SetAddress(&nb_);
  nb_up_branch = tree->GetBranch("nb_up");
  if (nb_up_branch) nb_up_branch->SetAddress(&nb_up_);
  nb_dn_branch = tree->GetBranch("nb_dn");
  if (nb_dn_branch) nb_dn_branch->SetAddress(&nb_dn_);
  nbmed_branch = tree->GetBranch("nbmed");
  if (nbmed_branch) nbmed_branch->SetAddress(&nbmed_);
  ht_branch = tree->GetBranch("ht");
  if (ht_branch) ht_branch->SetAddress(&ht_);
  nj_cen_branch = tree->GetBranch("nj_cen");
  if (nj_cen_branch) nj_cen_branch->SetAddress(&nj_cen_);
  nj_cen_up_branch = tree->GetBranch("nj_cen_up");
  if (nj_cen_up_branch) nj_cen_up_branch->SetAddress(&nj_cen_up_);
  nj_cen_dn_branch = tree->GetBranch("nj_cen_dn");
  if (nj_cen_dn_branch) nj_cen_dn_branch->SetAddress(&nj_cen_dn_);
  weight_btagsf_branch = tree->GetBranch("weight_btagsf");
  if (weight_btagsf_branch) weight_btagsf_branch->SetAddress(&weight_btagsf_);
  weight_btagsf_heavy_DN_branch = tree->GetBranch("weight_btagsf_heavy_DN");
  if (weight_btagsf_heavy_DN_branch) weight_btagsf_heavy_DN_branch->SetAddress(&weight_btagsf_heavy_DN_);
  weight_btagsf_heavy_UP_branch = tree->GetBranch("weight_btagsf_heavy_UP");
  if (weight_btagsf_heavy_UP_branch) weight_btagsf_heavy_UP_branch->SetAddress(&weight_btagsf_heavy_UP_);
  weight_btagsf_light_DN_branch = tree->GetBranch("weight_btagsf_light_DN");
  if (weight_btagsf_light_DN_branch) weight_btagsf_light_DN_branch->SetAddress(&weight_btagsf_light_DN_);
  weight_btagsf_light_UP_branch = tree->GetBranch("weight_btagsf_light_UP");
  if (weight_btagsf_light_UP_branch) weight_btagsf_light_UP_branch->SetAddress(&weight_btagsf_light_UP_);

  tree->SetMakeClass(0);
}

void wvztree::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  run_isLoaded = false;
  lumi_isLoaded = false;
  evt_isLoaded = false;
  isData_isLoaded = false;
  evt_scale1fb_isLoaded = false;
  genps_weight_isLoaded = false;
  xsec_br_isLoaded = false;
  evt_passgoodrunlist_isLoaded = false;
  CMS4path_isLoaded = false;
  CMS4index_isLoaded = false;
  weight_fr_r1_f1_isLoaded = false;
  weight_fr_r1_f2_isLoaded = false;
  weight_fr_r1_f0p5_isLoaded = false;
  weight_fr_r2_f1_isLoaded = false;
  weight_fr_r2_f2_isLoaded = false;
  weight_fr_r2_f0p5_isLoaded = false;
  weight_fr_r0p5_f1_isLoaded = false;
  weight_fr_r0p5_f2_isLoaded = false;
  weight_fr_r0p5_f0p5_isLoaded = false;
  weight_pdf_up_isLoaded = false;
  weight_pdf_down_isLoaded = false;
  weight_alphas_down_isLoaded = false;
  weight_alphas_up_isLoaded = false;
  HLT_DoubleMu_isLoaded = false;
  HLT_DoubleEl_isLoaded = false;
  HLT_MuEG_isLoaded = false;
  pass_duplicate_ee_em_mm_isLoaded = false;
  pass_duplicate_mm_em_ee_isLoaded = false;
  gen_ht_isLoaded = false;
  gen_V_p4_isLoaded = false;
  gen_V_pt_isLoaded = false;
  gen_V_eta_isLoaded = false;
  gen_V_phi_isLoaded = false;
  gen_V_mass_isLoaded = false;
  gen_V_id_isLoaded = false;
  gen_lep_p4_isLoaded = false;
  gen_lep_pt_isLoaded = false;
  gen_lep_eta_isLoaded = false;
  gen_lep_phi_isLoaded = false;
  gen_lep_mass_isLoaded = false;
  gen_lep_id_isLoaded = false;
  gen_child_p4_isLoaded = false;
  gen_child_pt_isLoaded = false;
  gen_child_eta_isLoaded = false;
  gen_child_phi_isLoaded = false;
  gen_child_mass_isLoaded = false;
  gen_child_id_isLoaded = false;
  gen_part_p4_isLoaded = false;
  gen_part_pt_isLoaded = false;
  gen_part_eta_isLoaded = false;
  gen_part_phi_isLoaded = false;
  gen_part_mass_isLoaded = false;
  gen_part_id_isLoaded = false;
  gen_part_mother_id_isLoaded = false;
  gen_part_grandma_id_isLoaded = false;
  gen_part_status_isLoaded = false;
  VHchannel_isLoaded = false;
  Higgschannel_isLoaded = false;
  nGenTauClean_isLoaded = false;
  nGenTau_isLoaded = false;
  hasTau_isLoaded = false;
  nLightLep_isLoaded = false;
  firstgoodvertex_isLoaded = false;
  nvtx_isLoaded = false;
  nTrueInt_isLoaded = false;
  lep_p4_isLoaded = false;
  lep_pt_isLoaded = false;
  lep_eta_isLoaded = false;
  lep_etaSC_isLoaded = false;
  lep_phi_isLoaded = false;
  lep_energy_isLoaded = false;
  lep_mva_isLoaded = false;
  lep_mvaIso_isLoaded = false;
  lep_relIso04DB_isLoaded = false;
  lep_relIso03EA_isLoaded = false;
  lep_relIso03EAv4_isLoaded = false;
  lep_relIso03EAwLep_isLoaded = false;
  lep_ip3d_isLoaded = false;
  lep_sip3d_isLoaded = false;
  lep_dxy_isLoaded = false;
  lep_dz_isLoaded = false;
  lep_mc_motherid_isLoaded = false;
  lep_mc_id_isLoaded = false;
  lep_motherIdv2_isLoaded = false;
  lep_idx_isLoaded = false;
  lep_id_isLoaded = false;
  lep_isTightPOG_isLoaded = false;
  lep_isMediumPOG_isLoaded = false;
  lep_isMVAwp80NoIsoPOG_isLoaded = false;
  lep_isMVAwp90NoIsoPOG_isLoaded = false;
  lep_isMVAwpLooseNoIsoPOG_isLoaded = false;
  lep_isMVAwp80IsoPOG_isLoaded = false;
  lep_isMVAwp90IsoPOG_isLoaded = false;
  lep_isMVAwpLooseIsoPOG_isLoaded = false;
  lep_isMVAwpHZZIsoPOG_isLoaded = false;
  lep_isCutBasedNoIsoVetoPOG_isLoaded = false;
  lep_isCutBasedNoIsoLoosePOG_isLoaded = false;
  lep_isCutBasedNoIsoMediumPOG_isLoaded = false;
  lep_isCutBasedNoIsoTightPOG_isLoaded = false;
  lep_isCutBasedIsoVetoPOG_isLoaded = false;
  lep_isCutBasedIsoLoosePOG_isLoaded = false;
  lep_isCutBasedIsoMediumPOG_isLoaded = false;
  lep_isCutBasedIsoTightPOG_isLoaded = false;
  met_p4_isLoaded = false;
  met_pt_isLoaded = false;
  met_phi_isLoaded = false;
  met_up_pt_isLoaded = false;
  met_up_phi_isLoaded = false;
  met_dn_pt_isLoaded = false;
  met_dn_phi_isLoaded = false;
  met_gen_pt_isLoaded = false;
  met_gen_phi_isLoaded = false;
  passesMETfiltersRun2_isLoaded = false;
  jets_p4_isLoaded = false;
  jets_pt_isLoaded = false;
  jets_eta_isLoaded = false;
  jets_phi_isLoaded = false;
  jets_mass_isLoaded = false;
  jets_btag_score_isLoaded = false;
  jets_cen_p4_isLoaded = false;
  jets_cen_pt_isLoaded = false;
  jets_cen_eta_isLoaded = false;
  jets_cen_phi_isLoaded = false;
  jets_cen_mass_isLoaded = false;
  rawjets_p4_isLoaded = false;
  rawjets_pt_isLoaded = false;
  rawjets_eta_isLoaded = false;
  rawjets_phi_isLoaded = false;
  rawjets_mass_isLoaded = false;
  rawjets_JEC_isLoaded = false;
  rawjets_passJetID_isLoaded = false;
  rawjets_isORwLep_isLoaded = false;
  nj_isLoaded = false;
  nj_up_isLoaded = false;
  nj_dn_isLoaded = false;
  nb_isLoaded = false;
  nb_up_isLoaded = false;
  nb_dn_isLoaded = false;
  nbmed_isLoaded = false;
  ht_isLoaded = false;
  nj_cen_isLoaded = false;
  nj_cen_up_isLoaded = false;
  nj_cen_dn_isLoaded = false;
  weight_btagsf_isLoaded = false;
  weight_btagsf_heavy_DN_isLoaded = false;
  weight_btagsf_heavy_UP_isLoaded = false;
  weight_btagsf_light_DN_isLoaded = false;
  weight_btagsf_light_UP_isLoaded = false;
}

void wvztree::LoadAllBranches() {
  // load all branches
  if (run_branch != 0) run();
  if (lumi_branch != 0) lumi();
  if (evt_branch != 0) evt();
  if (isData_branch != 0) isData();
  if (evt_scale1fb_branch != 0) evt_scale1fb();
  if (genps_weight_branch != 0) genps_weight();
  if (xsec_br_branch != 0) xsec_br();
  if (evt_passgoodrunlist_branch != 0) evt_passgoodrunlist();
  if (CMS4path_branch != 0) CMS4path();
  if (CMS4index_branch != 0) CMS4index();
  if (weight_fr_r1_f1_branch != 0) weight_fr_r1_f1();
  if (weight_fr_r1_f2_branch != 0) weight_fr_r1_f2();
  if (weight_fr_r1_f0p5_branch != 0) weight_fr_r1_f0p5();
  if (weight_fr_r2_f1_branch != 0) weight_fr_r2_f1();
  if (weight_fr_r2_f2_branch != 0) weight_fr_r2_f2();
  if (weight_fr_r2_f0p5_branch != 0) weight_fr_r2_f0p5();
  if (weight_fr_r0p5_f1_branch != 0) weight_fr_r0p5_f1();
  if (weight_fr_r0p5_f2_branch != 0) weight_fr_r0p5_f2();
  if (weight_fr_r0p5_f0p5_branch != 0) weight_fr_r0p5_f0p5();
  if (weight_pdf_up_branch != 0) weight_pdf_up();
  if (weight_pdf_down_branch != 0) weight_pdf_down();
  if (weight_alphas_down_branch != 0) weight_alphas_down();
  if (weight_alphas_up_branch != 0) weight_alphas_up();
  if (HLT_DoubleMu_branch != 0) HLT_DoubleMu();
  if (HLT_DoubleEl_branch != 0) HLT_DoubleEl();
  if (HLT_MuEG_branch != 0) HLT_MuEG();
  if (pass_duplicate_ee_em_mm_branch != 0) pass_duplicate_ee_em_mm();
  if (pass_duplicate_mm_em_ee_branch != 0) pass_duplicate_mm_em_ee();
  if (gen_ht_branch != 0) gen_ht();
  if (gen_V_p4_branch != 0) gen_V_p4();
  if (gen_V_pt_branch != 0) gen_V_pt();
  if (gen_V_eta_branch != 0) gen_V_eta();
  if (gen_V_phi_branch != 0) gen_V_phi();
  if (gen_V_mass_branch != 0) gen_V_mass();
  if (gen_V_id_branch != 0) gen_V_id();
  if (gen_lep_p4_branch != 0) gen_lep_p4();
  if (gen_lep_pt_branch != 0) gen_lep_pt();
  if (gen_lep_eta_branch != 0) gen_lep_eta();
  if (gen_lep_phi_branch != 0) gen_lep_phi();
  if (gen_lep_mass_branch != 0) gen_lep_mass();
  if (gen_lep_id_branch != 0) gen_lep_id();
  if (gen_child_p4_branch != 0) gen_child_p4();
  if (gen_child_pt_branch != 0) gen_child_pt();
  if (gen_child_eta_branch != 0) gen_child_eta();
  if (gen_child_phi_branch != 0) gen_child_phi();
  if (gen_child_mass_branch != 0) gen_child_mass();
  if (gen_child_id_branch != 0) gen_child_id();
  if (gen_part_p4_branch != 0) gen_part_p4();
  if (gen_part_pt_branch != 0) gen_part_pt();
  if (gen_part_eta_branch != 0) gen_part_eta();
  if (gen_part_phi_branch != 0) gen_part_phi();
  if (gen_part_mass_branch != 0) gen_part_mass();
  if (gen_part_id_branch != 0) gen_part_id();
  if (gen_part_mother_id_branch != 0) gen_part_mother_id();
  if (gen_part_grandma_id_branch != 0) gen_part_grandma_id();
  if (gen_part_status_branch != 0) gen_part_status();
  if (VHchannel_branch != 0) VHchannel();
  if (Higgschannel_branch != 0) Higgschannel();
  if (nGenTauClean_branch != 0) nGenTauClean();
  if (nGenTau_branch != 0) nGenTau();
  if (hasTau_branch != 0) hasTau();
  if (nLightLep_branch != 0) nLightLep();
  if (firstgoodvertex_branch != 0) firstgoodvertex();
  if (nvtx_branch != 0) nvtx();
  if (nTrueInt_branch != 0) nTrueInt();
  if (lep_p4_branch != 0) lep_p4();
  if (lep_pt_branch != 0) lep_pt();
  if (lep_eta_branch != 0) lep_eta();
  if (lep_etaSC_branch != 0) lep_etaSC();
  if (lep_phi_branch != 0) lep_phi();
  if (lep_energy_branch != 0) lep_energy();
  if (lep_mva_branch != 0) lep_mva();
  if (lep_mvaIso_branch != 0) lep_mvaIso();
  if (lep_relIso04DB_branch != 0) lep_relIso04DB();
  if (lep_relIso03EA_branch != 0) lep_relIso03EA();
  if (lep_relIso03EAv4_branch != 0) lep_relIso03EAv4();
  if (lep_relIso03EAwLep_branch != 0) lep_relIso03EAwLep();
  if (lep_ip3d_branch != 0) lep_ip3d();
  if (lep_sip3d_branch != 0) lep_sip3d();
  if (lep_dxy_branch != 0) lep_dxy();
  if (lep_dz_branch != 0) lep_dz();
  if (lep_mc_motherid_branch != 0) lep_mc_motherid();
  if (lep_mc_id_branch != 0) lep_mc_id();
  if (lep_motherIdv2_branch != 0) lep_motherIdv2();
  if (lep_idx_branch != 0) lep_idx();
  if (lep_id_branch != 0) lep_id();
  if (lep_isTightPOG_branch != 0) lep_isTightPOG();
  if (lep_isMediumPOG_branch != 0) lep_isMediumPOG();
  if (lep_isMVAwp80NoIsoPOG_branch != 0) lep_isMVAwp80NoIsoPOG();
  if (lep_isMVAwp90NoIsoPOG_branch != 0) lep_isMVAwp90NoIsoPOG();
  if (lep_isMVAwpLooseNoIsoPOG_branch != 0) lep_isMVAwpLooseNoIsoPOG();
  if (lep_isMVAwp80IsoPOG_branch != 0) lep_isMVAwp80IsoPOG();
  if (lep_isMVAwp90IsoPOG_branch != 0) lep_isMVAwp90IsoPOG();
  if (lep_isMVAwpLooseIsoPOG_branch != 0) lep_isMVAwpLooseIsoPOG();
  if (lep_isMVAwpHZZIsoPOG_branch != 0) lep_isMVAwpHZZIsoPOG();
  if (lep_isCutBasedNoIsoVetoPOG_branch != 0) lep_isCutBasedNoIsoVetoPOG();
  if (lep_isCutBasedNoIsoLoosePOG_branch != 0) lep_isCutBasedNoIsoLoosePOG();
  if (lep_isCutBasedNoIsoMediumPOG_branch != 0) lep_isCutBasedNoIsoMediumPOG();
  if (lep_isCutBasedNoIsoTightPOG_branch != 0) lep_isCutBasedNoIsoTightPOG();
  if (lep_isCutBasedIsoVetoPOG_branch != 0) lep_isCutBasedIsoVetoPOG();
  if (lep_isCutBasedIsoLoosePOG_branch != 0) lep_isCutBasedIsoLoosePOG();
  if (lep_isCutBasedIsoMediumPOG_branch != 0) lep_isCutBasedIsoMediumPOG();
  if (lep_isCutBasedIsoTightPOG_branch != 0) lep_isCutBasedIsoTightPOG();
  if (met_p4_branch != 0) met_p4();
  if (met_pt_branch != 0) met_pt();
  if (met_phi_branch != 0) met_phi();
  if (met_up_pt_branch != 0) met_up_pt();
  if (met_up_phi_branch != 0) met_up_phi();
  if (met_dn_pt_branch != 0) met_dn_pt();
  if (met_dn_phi_branch != 0) met_dn_phi();
  if (met_gen_pt_branch != 0) met_gen_pt();
  if (met_gen_phi_branch != 0) met_gen_phi();
  if (passesMETfiltersRun2_branch != 0) passesMETfiltersRun2();
  if (jets_p4_branch != 0) jets_p4();
  if (jets_pt_branch != 0) jets_pt();
  if (jets_eta_branch != 0) jets_eta();
  if (jets_phi_branch != 0) jets_phi();
  if (jets_mass_branch != 0) jets_mass();
  if (jets_btag_score_branch != 0) jets_btag_score();
  if (jets_cen_p4_branch != 0) jets_cen_p4();
  if (jets_cen_pt_branch != 0) jets_cen_pt();
  if (jets_cen_eta_branch != 0) jets_cen_eta();
  if (jets_cen_phi_branch != 0) jets_cen_phi();
  if (jets_cen_mass_branch != 0) jets_cen_mass();
  if (rawjets_p4_branch != 0) rawjets_p4();
  if (rawjets_pt_branch != 0) rawjets_pt();
  if (rawjets_eta_branch != 0) rawjets_eta();
  if (rawjets_phi_branch != 0) rawjets_phi();
  if (rawjets_mass_branch != 0) rawjets_mass();
  if (rawjets_JEC_branch != 0) rawjets_JEC();
  if (rawjets_passJetID_branch != 0) rawjets_passJetID();
  if (rawjets_isORwLep_branch != 0) rawjets_isORwLep();
  if (nj_branch != 0) nj();
  if (nj_up_branch != 0) nj_up();
  if (nj_dn_branch != 0) nj_dn();
  if (nb_branch != 0) nb();
  if (nb_up_branch != 0) nb_up();
  if (nb_dn_branch != 0) nb_dn();
  if (nbmed_branch != 0) nbmed();
  if (ht_branch != 0) ht();
  if (nj_cen_branch != 0) nj_cen();
  if (nj_cen_up_branch != 0) nj_cen_up();
  if (nj_cen_dn_branch != 0) nj_cen_dn();
  if (weight_btagsf_branch != 0) weight_btagsf();
  if (weight_btagsf_heavy_DN_branch != 0) weight_btagsf_heavy_DN();
  if (weight_btagsf_heavy_UP_branch != 0) weight_btagsf_heavy_UP();
  if (weight_btagsf_light_DN_branch != 0) weight_btagsf_light_DN();
  if (weight_btagsf_light_UP_branch != 0) weight_btagsf_light_UP();
}

const int &wvztree::run() {
  if (not run_isLoaded) {
    if (run_branch != 0) {
      run_branch->GetEntry(index);
    } else {
      printf("branch run_branch does not exist!\n");
      exit(1);
    }
    run_isLoaded = true;
  }
  return run_;
}

const int &wvztree::lumi() {
  if (not lumi_isLoaded) {
    if (lumi_branch != 0) {
      lumi_branch->GetEntry(index);
    } else {
      printf("branch lumi_branch does not exist!\n");
      exit(1);
    }
    lumi_isLoaded = true;
  }
  return lumi_;
}

const unsigned long long &wvztree::evt() {
  if (not evt_isLoaded) {
    if (evt_branch != 0) {
      evt_branch->GetEntry(index);
    } else {
      printf("branch evt_branch does not exist!\n");
      exit(1);
    }
    evt_isLoaded = true;
  }
  return evt_;
}

const int &wvztree::isData() {
  if (not isData_isLoaded) {
    if (isData_branch != 0) {
      isData_branch->GetEntry(index);
    } else {
      printf("branch isData_branch does not exist!\n");
      exit(1);
    }
    isData_isLoaded = true;
  }
  return isData_;
}

const float &wvztree::evt_scale1fb() {
  if (not evt_scale1fb_isLoaded) {
    if (evt_scale1fb_branch != 0) {
      evt_scale1fb_branch->GetEntry(index);
    } else {
      printf("branch evt_scale1fb_branch does not exist!\n");
      exit(1);
    }
    evt_scale1fb_isLoaded = true;
  }
  return evt_scale1fb_;
}

const float &wvztree::genps_weight() {
  if (not genps_weight_isLoaded) {
    if (genps_weight_branch != 0) {
      genps_weight_branch->GetEntry(index);
    } else {
      printf("branch genps_weight_branch does not exist!\n");
      exit(1);
    }
    genps_weight_isLoaded = true;
  }
  return genps_weight_;
}

const float &wvztree::xsec_br() {
  if (not xsec_br_isLoaded) {
    if (xsec_br_branch != 0) {
      xsec_br_branch->GetEntry(index);
    } else {
      printf("branch xsec_br_branch does not exist!\n");
      exit(1);
    }
    xsec_br_isLoaded = true;
  }
  return xsec_br_;
}

const int &wvztree::evt_passgoodrunlist() {
  if (not evt_passgoodrunlist_isLoaded) {
    if (evt_passgoodrunlist_branch != 0) {
      evt_passgoodrunlist_branch->GetEntry(index);
    } else {
      printf("branch evt_passgoodrunlist_branch does not exist!\n");
      exit(1);
    }
    evt_passgoodrunlist_isLoaded = true;
  }
  return evt_passgoodrunlist_;
}

const TString &wvztree::CMS4path() {
  if (not CMS4path_isLoaded) {
    if (CMS4path_branch != 0) {
      CMS4path_branch->GetEntry(index);
    } else {
      printf("branch CMS4path_branch does not exist!\n");
      exit(1);
    }
    CMS4path_isLoaded = true;
  }
  return *CMS4path_;
}

const int &wvztree::CMS4index() {
  if (not CMS4index_isLoaded) {
    if (CMS4index_branch != 0) {
      CMS4index_branch->GetEntry(index);
    } else {
      printf("branch CMS4index_branch does not exist!\n");
      exit(1);
    }
    CMS4index_isLoaded = true;
  }
  return CMS4index_;
}

const float &wvztree::weight_fr_r1_f1() {
  if (not weight_fr_r1_f1_isLoaded) {
    if (weight_fr_r1_f1_branch != 0) {
      weight_fr_r1_f1_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r1_f1_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r1_f1_isLoaded = true;
  }
  return weight_fr_r1_f1_;
}

const float &wvztree::weight_fr_r1_f2() {
  if (not weight_fr_r1_f2_isLoaded) {
    if (weight_fr_r1_f2_branch != 0) {
      weight_fr_r1_f2_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r1_f2_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r1_f2_isLoaded = true;
  }
  return weight_fr_r1_f2_;
}

const float &wvztree::weight_fr_r1_f0p5() {
  if (not weight_fr_r1_f0p5_isLoaded) {
    if (weight_fr_r1_f0p5_branch != 0) {
      weight_fr_r1_f0p5_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r1_f0p5_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r1_f0p5_isLoaded = true;
  }
  return weight_fr_r1_f0p5_;
}

const float &wvztree::weight_fr_r2_f1() {
  if (not weight_fr_r2_f1_isLoaded) {
    if (weight_fr_r2_f1_branch != 0) {
      weight_fr_r2_f1_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r2_f1_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r2_f1_isLoaded = true;
  }
  return weight_fr_r2_f1_;
}

const float &wvztree::weight_fr_r2_f2() {
  if (not weight_fr_r2_f2_isLoaded) {
    if (weight_fr_r2_f2_branch != 0) {
      weight_fr_r2_f2_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r2_f2_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r2_f2_isLoaded = true;
  }
  return weight_fr_r2_f2_;
}

const float &wvztree::weight_fr_r2_f0p5() {
  if (not weight_fr_r2_f0p5_isLoaded) {
    if (weight_fr_r2_f0p5_branch != 0) {
      weight_fr_r2_f0p5_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r2_f0p5_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r2_f0p5_isLoaded = true;
  }
  return weight_fr_r2_f0p5_;
}

const float &wvztree::weight_fr_r0p5_f1() {
  if (not weight_fr_r0p5_f1_isLoaded) {
    if (weight_fr_r0p5_f1_branch != 0) {
      weight_fr_r0p5_f1_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r0p5_f1_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r0p5_f1_isLoaded = true;
  }
  return weight_fr_r0p5_f1_;
}

const float &wvztree::weight_fr_r0p5_f2() {
  if (not weight_fr_r0p5_f2_isLoaded) {
    if (weight_fr_r0p5_f2_branch != 0) {
      weight_fr_r0p5_f2_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r0p5_f2_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r0p5_f2_isLoaded = true;
  }
  return weight_fr_r0p5_f2_;
}

const float &wvztree::weight_fr_r0p5_f0p5() {
  if (not weight_fr_r0p5_f0p5_isLoaded) {
    if (weight_fr_r0p5_f0p5_branch != 0) {
      weight_fr_r0p5_f0p5_branch->GetEntry(index);
    } else {
      printf("branch weight_fr_r0p5_f0p5_branch does not exist!\n");
      exit(1);
    }
    weight_fr_r0p5_f0p5_isLoaded = true;
  }
  return weight_fr_r0p5_f0p5_;
}

const float &wvztree::weight_pdf_up() {
  if (not weight_pdf_up_isLoaded) {
    if (weight_pdf_up_branch != 0) {
      weight_pdf_up_branch->GetEntry(index);
    } else {
      printf("branch weight_pdf_up_branch does not exist!\n");
      exit(1);
    }
    weight_pdf_up_isLoaded = true;
  }
  return weight_pdf_up_;
}

const float &wvztree::weight_pdf_down() {
  if (not weight_pdf_down_isLoaded) {
    if (weight_pdf_down_branch != 0) {
      weight_pdf_down_branch->GetEntry(index);
    } else {
      printf("branch weight_pdf_down_branch does not exist!\n");
      exit(1);
    }
    weight_pdf_down_isLoaded = true;
  }
  return weight_pdf_down_;
}

const float &wvztree::weight_alphas_down() {
  if (not weight_alphas_down_isLoaded) {
    if (weight_alphas_down_branch != 0) {
      weight_alphas_down_branch->GetEntry(index);
    } else {
      printf("branch weight_alphas_down_branch does not exist!\n");
      exit(1);
    }
    weight_alphas_down_isLoaded = true;
  }
  return weight_alphas_down_;
}

const float &wvztree::weight_alphas_up() {
  if (not weight_alphas_up_isLoaded) {
    if (weight_alphas_up_branch != 0) {
      weight_alphas_up_branch->GetEntry(index);
    } else {
      printf("branch weight_alphas_up_branch does not exist!\n");
      exit(1);
    }
    weight_alphas_up_isLoaded = true;
  }
  return weight_alphas_up_;
}

const int &wvztree::HLT_DoubleMu() {
  if (not HLT_DoubleMu_isLoaded) {
    if (HLT_DoubleMu_branch != 0) {
      HLT_DoubleMu_branch->GetEntry(index);
    } else {
      printf("branch HLT_DoubleMu_branch does not exist!\n");
      exit(1);
    }
    HLT_DoubleMu_isLoaded = true;
  }
  return HLT_DoubleMu_;
}

const int &wvztree::HLT_DoubleEl() {
  if (not HLT_DoubleEl_isLoaded) {
    if (HLT_DoubleEl_branch != 0) {
      HLT_DoubleEl_branch->GetEntry(index);
    } else {
      printf("branch HLT_DoubleEl_branch does not exist!\n");
      exit(1);
    }
    HLT_DoubleEl_isLoaded = true;
  }
  return HLT_DoubleEl_;
}

const int &wvztree::HLT_MuEG() {
  if (not HLT_MuEG_isLoaded) {
    if (HLT_MuEG_branch != 0) {
      HLT_MuEG_branch->GetEntry(index);
    } else {
      printf("branch HLT_MuEG_branch does not exist!\n");
      exit(1);
    }
    HLT_MuEG_isLoaded = true;
  }
  return HLT_MuEG_;
}

const int &wvztree::pass_duplicate_ee_em_mm() {
  if (not pass_duplicate_ee_em_mm_isLoaded) {
    if (pass_duplicate_ee_em_mm_branch != 0) {
      pass_duplicate_ee_em_mm_branch->GetEntry(index);
    } else {
      printf("branch pass_duplicate_ee_em_mm_branch does not exist!\n");
      exit(1);
    }
    pass_duplicate_ee_em_mm_isLoaded = true;
  }
  return pass_duplicate_ee_em_mm_;
}

const int &wvztree::pass_duplicate_mm_em_ee() {
  if (not pass_duplicate_mm_em_ee_isLoaded) {
    if (pass_duplicate_mm_em_ee_branch != 0) {
      pass_duplicate_mm_em_ee_branch->GetEntry(index);
    } else {
      printf("branch pass_duplicate_mm_em_ee_branch does not exist!\n");
      exit(1);
    }
    pass_duplicate_mm_em_ee_isLoaded = true;
  }
  return pass_duplicate_mm_em_ee_;
}

const float &wvztree::gen_ht() {
  if (not gen_ht_isLoaded) {
    if (gen_ht_branch != 0) {
      gen_ht_branch->GetEntry(index);
    } else {
      printf("branch gen_ht_branch does not exist!\n");
      exit(1);
    }
    gen_ht_isLoaded = true;
  }
  return gen_ht_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::gen_V_p4() {
  if (not gen_V_p4_isLoaded) {
    if (gen_V_p4_branch != 0) {
      gen_V_p4_branch->GetEntry(index);
    } else {
      printf("branch gen_V_p4_branch does not exist!\n");
      exit(1);
    }
    gen_V_p4_isLoaded = true;
  }
  return *gen_V_p4_;
}

const vector<float> &wvztree::gen_V_pt() {
  if (not gen_V_pt_isLoaded) {
    if (gen_V_pt_branch != 0) {
      gen_V_pt_branch->GetEntry(index);
    } else {
      printf("branch gen_V_pt_branch does not exist!\n");
      exit(1);
    }
    gen_V_pt_isLoaded = true;
  }
  return *gen_V_pt_;
}

const vector<float> &wvztree::gen_V_eta() {
  if (not gen_V_eta_isLoaded) {
    if (gen_V_eta_branch != 0) {
      gen_V_eta_branch->GetEntry(index);
    } else {
      printf("branch gen_V_eta_branch does not exist!\n");
      exit(1);
    }
    gen_V_eta_isLoaded = true;
  }
  return *gen_V_eta_;
}

const vector<float> &wvztree::gen_V_phi() {
  if (not gen_V_phi_isLoaded) {
    if (gen_V_phi_branch != 0) {
      gen_V_phi_branch->GetEntry(index);
    } else {
      printf("branch gen_V_phi_branch does not exist!\n");
      exit(1);
    }
    gen_V_phi_isLoaded = true;
  }
  return *gen_V_phi_;
}

const vector<float> &wvztree::gen_V_mass() {
  if (not gen_V_mass_isLoaded) {
    if (gen_V_mass_branch != 0) {
      gen_V_mass_branch->GetEntry(index);
    } else {
      printf("branch gen_V_mass_branch does not exist!\n");
      exit(1);
    }
    gen_V_mass_isLoaded = true;
  }
  return *gen_V_mass_;
}

const vector<int> &wvztree::gen_V_id() {
  if (not gen_V_id_isLoaded) {
    if (gen_V_id_branch != 0) {
      gen_V_id_branch->GetEntry(index);
    } else {
      printf("branch gen_V_id_branch does not exist!\n");
      exit(1);
    }
    gen_V_id_isLoaded = true;
  }
  return *gen_V_id_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::gen_lep_p4() {
  if (not gen_lep_p4_isLoaded) {
    if (gen_lep_p4_branch != 0) {
      gen_lep_p4_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_p4_branch does not exist!\n");
      exit(1);
    }
    gen_lep_p4_isLoaded = true;
  }
  return *gen_lep_p4_;
}

const vector<float> &wvztree::gen_lep_pt() {
  if (not gen_lep_pt_isLoaded) {
    if (gen_lep_pt_branch != 0) {
      gen_lep_pt_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_pt_branch does not exist!\n");
      exit(1);
    }
    gen_lep_pt_isLoaded = true;
  }
  return *gen_lep_pt_;
}

const vector<float> &wvztree::gen_lep_eta() {
  if (not gen_lep_eta_isLoaded) {
    if (gen_lep_eta_branch != 0) {
      gen_lep_eta_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_eta_branch does not exist!\n");
      exit(1);
    }
    gen_lep_eta_isLoaded = true;
  }
  return *gen_lep_eta_;
}

const vector<float> &wvztree::gen_lep_phi() {
  if (not gen_lep_phi_isLoaded) {
    if (gen_lep_phi_branch != 0) {
      gen_lep_phi_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_phi_branch does not exist!\n");
      exit(1);
    }
    gen_lep_phi_isLoaded = true;
  }
  return *gen_lep_phi_;
}

const vector<float> &wvztree::gen_lep_mass() {
  if (not gen_lep_mass_isLoaded) {
    if (gen_lep_mass_branch != 0) {
      gen_lep_mass_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_mass_branch does not exist!\n");
      exit(1);
    }
    gen_lep_mass_isLoaded = true;
  }
  return *gen_lep_mass_;
}

const vector<int> &wvztree::gen_lep_id() {
  if (not gen_lep_id_isLoaded) {
    if (gen_lep_id_branch != 0) {
      gen_lep_id_branch->GetEntry(index);
    } else {
      printf("branch gen_lep_id_branch does not exist!\n");
      exit(1);
    }
    gen_lep_id_isLoaded = true;
  }
  return *gen_lep_id_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::gen_child_p4() {
  if (not gen_child_p4_isLoaded) {
    if (gen_child_p4_branch != 0) {
      gen_child_p4_branch->GetEntry(index);
    } else {
      printf("branch gen_child_p4_branch does not exist!\n");
      exit(1);
    }
    gen_child_p4_isLoaded = true;
  }
  return *gen_child_p4_;
}

const vector<float> &wvztree::gen_child_pt() {
  if (not gen_child_pt_isLoaded) {
    if (gen_child_pt_branch != 0) {
      gen_child_pt_branch->GetEntry(index);
    } else {
      printf("branch gen_child_pt_branch does not exist!\n");
      exit(1);
    }
    gen_child_pt_isLoaded = true;
  }
  return *gen_child_pt_;
}

const vector<float> &wvztree::gen_child_eta() {
  if (not gen_child_eta_isLoaded) {
    if (gen_child_eta_branch != 0) {
      gen_child_eta_branch->GetEntry(index);
    } else {
      printf("branch gen_child_eta_branch does not exist!\n");
      exit(1);
    }
    gen_child_eta_isLoaded = true;
  }
  return *gen_child_eta_;
}

const vector<float> &wvztree::gen_child_phi() {
  if (not gen_child_phi_isLoaded) {
    if (gen_child_phi_branch != 0) {
      gen_child_phi_branch->GetEntry(index);
    } else {
      printf("branch gen_child_phi_branch does not exist!\n");
      exit(1);
    }
    gen_child_phi_isLoaded = true;
  }
  return *gen_child_phi_;
}

const vector<float> &wvztree::gen_child_mass() {
  if (not gen_child_mass_isLoaded) {
    if (gen_child_mass_branch != 0) {
      gen_child_mass_branch->GetEntry(index);
    } else {
      printf("branch gen_child_mass_branch does not exist!\n");
      exit(1);
    }
    gen_child_mass_isLoaded = true;
  }
  return *gen_child_mass_;
}

const vector<int> &wvztree::gen_child_id() {
  if (not gen_child_id_isLoaded) {
    if (gen_child_id_branch != 0) {
      gen_child_id_branch->GetEntry(index);
    } else {
      printf("branch gen_child_id_branch does not exist!\n");
      exit(1);
    }
    gen_child_id_isLoaded = true;
  }
  return *gen_child_id_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::gen_part_p4() {
  if (not gen_part_p4_isLoaded) {
    if (gen_part_p4_branch != 0) {
      gen_part_p4_branch->GetEntry(index);
    } else {
      printf("branch gen_part_p4_branch does not exist!\n");
      exit(1);
    }
    gen_part_p4_isLoaded = true;
  }
  return *gen_part_p4_;
}

const vector<float> &wvztree::gen_part_pt() {
  if (not gen_part_pt_isLoaded) {
    if (gen_part_pt_branch != 0) {
      gen_part_pt_branch->GetEntry(index);
    } else {
      printf("branch gen_part_pt_branch does not exist!\n");
      exit(1);
    }
    gen_part_pt_isLoaded = true;
  }
  return *gen_part_pt_;
}

const vector<float> &wvztree::gen_part_eta() {
  if (not gen_part_eta_isLoaded) {
    if (gen_part_eta_branch != 0) {
      gen_part_eta_branch->GetEntry(index);
    } else {
      printf("branch gen_part_eta_branch does not exist!\n");
      exit(1);
    }
    gen_part_eta_isLoaded = true;
  }
  return *gen_part_eta_;
}

const vector<float> &wvztree::gen_part_phi() {
  if (not gen_part_phi_isLoaded) {
    if (gen_part_phi_branch != 0) {
      gen_part_phi_branch->GetEntry(index);
    } else {
      printf("branch gen_part_phi_branch does not exist!\n");
      exit(1);
    }
    gen_part_phi_isLoaded = true;
  }
  return *gen_part_phi_;
}

const vector<float> &wvztree::gen_part_mass() {
  if (not gen_part_mass_isLoaded) {
    if (gen_part_mass_branch != 0) {
      gen_part_mass_branch->GetEntry(index);
    } else {
      printf("branch gen_part_mass_branch does not exist!\n");
      exit(1);
    }
    gen_part_mass_isLoaded = true;
  }
  return *gen_part_mass_;
}

const vector<int> &wvztree::gen_part_id() {
  if (not gen_part_id_isLoaded) {
    if (gen_part_id_branch != 0) {
      gen_part_id_branch->GetEntry(index);
    } else {
      printf("branch gen_part_id_branch does not exist!\n");
      exit(1);
    }
    gen_part_id_isLoaded = true;
  }
  return *gen_part_id_;
}

const vector<int> &wvztree::gen_part_mother_id() {
  if (not gen_part_mother_id_isLoaded) {
    if (gen_part_mother_id_branch != 0) {
      gen_part_mother_id_branch->GetEntry(index);
    } else {
      printf("branch gen_part_mother_id_branch does not exist!\n");
      exit(1);
    }
    gen_part_mother_id_isLoaded = true;
  }
  return *gen_part_mother_id_;
}

const vector<int> &wvztree::gen_part_grandma_id() {
  if (not gen_part_grandma_id_isLoaded) {
    if (gen_part_grandma_id_branch != 0) {
      gen_part_grandma_id_branch->GetEntry(index);
    } else {
      printf("branch gen_part_grandma_id_branch does not exist!\n");
      exit(1);
    }
    gen_part_grandma_id_isLoaded = true;
  }
  return *gen_part_grandma_id_;
}

const vector<int> &wvztree::gen_part_status() {
  if (not gen_part_status_isLoaded) {
    if (gen_part_status_branch != 0) {
      gen_part_status_branch->GetEntry(index);
    } else {
      printf("branch gen_part_status_branch does not exist!\n");
      exit(1);
    }
    gen_part_status_isLoaded = true;
  }
  return *gen_part_status_;
}

const int &wvztree::VHchannel() {
  if (not VHchannel_isLoaded) {
    if (VHchannel_branch != 0) {
      VHchannel_branch->GetEntry(index);
    } else {
      printf("branch VHchannel_branch does not exist!\n");
      exit(1);
    }
    VHchannel_isLoaded = true;
  }
  return VHchannel_;
}

const int &wvztree::Higgschannel() {
  if (not Higgschannel_isLoaded) {
    if (Higgschannel_branch != 0) {
      Higgschannel_branch->GetEntry(index);
    } else {
      printf("branch Higgschannel_branch does not exist!\n");
      exit(1);
    }
    Higgschannel_isLoaded = true;
  }
  return Higgschannel_;
}

const int &wvztree::nGenTauClean() {
  if (not nGenTauClean_isLoaded) {
    if (nGenTauClean_branch != 0) {
      nGenTauClean_branch->GetEntry(index);
    } else {
      printf("branch nGenTauClean_branch does not exist!\n");
      exit(1);
    }
    nGenTauClean_isLoaded = true;
  }
  return nGenTauClean_;
}

const int &wvztree::nGenTau() {
  if (not nGenTau_isLoaded) {
    if (nGenTau_branch != 0) {
      nGenTau_branch->GetEntry(index);
    } else {
      printf("branch nGenTau_branch does not exist!\n");
      exit(1);
    }
    nGenTau_isLoaded = true;
  }
  return nGenTau_;
}

const int &wvztree::hasTau() {
  if (not hasTau_isLoaded) {
    if (hasTau_branch != 0) {
      hasTau_branch->GetEntry(index);
    } else {
      printf("branch hasTau_branch does not exist!\n");
      exit(1);
    }
    hasTau_isLoaded = true;
  }
  return hasTau_;
}

const int &wvztree::nLightLep() {
  if (not nLightLep_isLoaded) {
    if (nLightLep_branch != 0) {
      nLightLep_branch->GetEntry(index);
    } else {
      printf("branch nLightLep_branch does not exist!\n");
      exit(1);
    }
    nLightLep_isLoaded = true;
  }
  return nLightLep_;
}

const int &wvztree::firstgoodvertex() {
  if (not firstgoodvertex_isLoaded) {
    if (firstgoodvertex_branch != 0) {
      firstgoodvertex_branch->GetEntry(index);
    } else {
      printf("branch firstgoodvertex_branch does not exist!\n");
      exit(1);
    }
    firstgoodvertex_isLoaded = true;
  }
  return firstgoodvertex_;
}

const int &wvztree::nvtx() {
  if (not nvtx_isLoaded) {
    if (nvtx_branch != 0) {
      nvtx_branch->GetEntry(index);
    } else {
      printf("branch nvtx_branch does not exist!\n");
      exit(1);
    }
    nvtx_isLoaded = true;
  }
  return nvtx_;
}

const int &wvztree::nTrueInt() {
  if (not nTrueInt_isLoaded) {
    if (nTrueInt_branch != 0) {
      nTrueInt_branch->GetEntry(index);
    } else {
      printf("branch nTrueInt_branch does not exist!\n");
      exit(1);
    }
    nTrueInt_isLoaded = true;
  }
  return nTrueInt_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::lep_p4() {
  if (not lep_p4_isLoaded) {
    if (lep_p4_branch != 0) {
      lep_p4_branch->GetEntry(index);
    } else {
      printf("branch lep_p4_branch does not exist!\n");
      exit(1);
    }
    lep_p4_isLoaded = true;
  }
  return *lep_p4_;
}

const vector<float> &wvztree::lep_pt() {
  if (not lep_pt_isLoaded) {
    if (lep_pt_branch != 0) {
      lep_pt_branch->GetEntry(index);
    } else {
      printf("branch lep_pt_branch does not exist!\n");
      exit(1);
    }
    lep_pt_isLoaded = true;
  }
  return *lep_pt_;
}

const vector<float> &wvztree::lep_eta() {
  if (not lep_eta_isLoaded) {
    if (lep_eta_branch != 0) {
      lep_eta_branch->GetEntry(index);
    } else {
      printf("branch lep_eta_branch does not exist!\n");
      exit(1);
    }
    lep_eta_isLoaded = true;
  }
  return *lep_eta_;
}

const vector<float> &wvztree::lep_etaSC() {
  if (not lep_etaSC_isLoaded) {
    if (lep_etaSC_branch != 0) {
      lep_etaSC_branch->GetEntry(index);
    } else {
      printf("branch lep_etaSC_branch does not exist!\n");
      exit(1);
    }
    lep_etaSC_isLoaded = true;
  }
  return *lep_etaSC_;
}

const vector<float> &wvztree::lep_phi() {
  if (not lep_phi_isLoaded) {
    if (lep_phi_branch != 0) {
      lep_phi_branch->GetEntry(index);
    } else {
      printf("branch lep_phi_branch does not exist!\n");
      exit(1);
    }
    lep_phi_isLoaded = true;
  }
  return *lep_phi_;
}

const vector<float> &wvztree::lep_energy() {
  if (not lep_energy_isLoaded) {
    if (lep_energy_branch != 0) {
      lep_energy_branch->GetEntry(index);
    } else {
      printf("branch lep_energy_branch does not exist!\n");
      exit(1);
    }
    lep_energy_isLoaded = true;
  }
  return *lep_energy_;
}

const vector<float> &wvztree::lep_mva() {
  if (not lep_mva_isLoaded) {
    if (lep_mva_branch != 0) {
      lep_mva_branch->GetEntry(index);
    } else {
      printf("branch lep_mva_branch does not exist!\n");
      exit(1);
    }
    lep_mva_isLoaded = true;
  }
  return *lep_mva_;
}

const vector<float> &wvztree::lep_mvaIso() {
  if (not lep_mvaIso_isLoaded) {
    if (lep_mvaIso_branch != 0) {
      lep_mvaIso_branch->GetEntry(index);
    } else {
      printf("branch lep_mvaIso_branch does not exist!\n");
      exit(1);
    }
    lep_mvaIso_isLoaded = true;
  }
  return *lep_mvaIso_;
}

const vector<float> &wvztree::lep_relIso04DB() {
  if (not lep_relIso04DB_isLoaded) {
    if (lep_relIso04DB_branch != 0) {
      lep_relIso04DB_branch->GetEntry(index);
    } else {
      printf("branch lep_relIso04DB_branch does not exist!\n");
      exit(1);
    }
    lep_relIso04DB_isLoaded = true;
  }
  return *lep_relIso04DB_;
}

const vector<float> &wvztree::lep_relIso03EA() {
  if (not lep_relIso03EA_isLoaded) {
    if (lep_relIso03EA_branch != 0) {
      lep_relIso03EA_branch->GetEntry(index);
    } else {
      printf("branch lep_relIso03EA_branch does not exist!\n");
      exit(1);
    }
    lep_relIso03EA_isLoaded = true;
  }
  return *lep_relIso03EA_;
}

const vector<float> &wvztree::lep_relIso03EAv4() {
  if (not lep_relIso03EAv4_isLoaded) {
    if (lep_relIso03EAv4_branch != 0) {
      lep_relIso03EAv4_branch->GetEntry(index);
    } else {
      printf("branch lep_relIso03EAv4_branch does not exist!\n");
      exit(1);
    }
    lep_relIso03EAv4_isLoaded = true;
  }
  return *lep_relIso03EAv4_;
}

const vector<float> &wvztree::lep_relIso03EAwLep() {
  if (not lep_relIso03EAwLep_isLoaded) {
    if (lep_relIso03EAwLep_branch != 0) {
      lep_relIso03EAwLep_branch->GetEntry(index);
    } else {
      printf("branch lep_relIso03EAwLep_branch does not exist!\n");
      exit(1);
    }
    lep_relIso03EAwLep_isLoaded = true;
  }
  return *lep_relIso03EAwLep_;
}

const vector<float> &wvztree::lep_ip3d() {
  if (not lep_ip3d_isLoaded) {
    if (lep_ip3d_branch != 0) {
      lep_ip3d_branch->GetEntry(index);
    } else {
      printf("branch lep_ip3d_branch does not exist!\n");
      exit(1);
    }
    lep_ip3d_isLoaded = true;
  }
  return *lep_ip3d_;
}

const vector<float> &wvztree::lep_sip3d() {
  if (not lep_sip3d_isLoaded) {
    if (lep_sip3d_branch != 0) {
      lep_sip3d_branch->GetEntry(index);
    } else {
      printf("branch lep_sip3d_branch does not exist!\n");
      exit(1);
    }
    lep_sip3d_isLoaded = true;
  }
  return *lep_sip3d_;
}

const vector<float> &wvztree::lep_dxy() {
  if (not lep_dxy_isLoaded) {
    if (lep_dxy_branch != 0) {
      lep_dxy_branch->GetEntry(index);
    } else {
      printf("branch lep_dxy_branch does not exist!\n");
      exit(1);
    }
    lep_dxy_isLoaded = true;
  }
  return *lep_dxy_;
}

const vector<float> &wvztree::lep_dz() {
  if (not lep_dz_isLoaded) {
    if (lep_dz_branch != 0) {
      lep_dz_branch->GetEntry(index);
    } else {
      printf("branch lep_dz_branch does not exist!\n");
      exit(1);
    }
    lep_dz_isLoaded = true;
  }
  return *lep_dz_;
}

const vector<int> &wvztree::lep_mc_motherid() {
  if (not lep_mc_motherid_isLoaded) {
    if (lep_mc_motherid_branch != 0) {
      lep_mc_motherid_branch->GetEntry(index);
    } else {
      printf("branch lep_mc_motherid_branch does not exist!\n");
      exit(1);
    }
    lep_mc_motherid_isLoaded = true;
  }
  return *lep_mc_motherid_;
}

const vector<int> &wvztree::lep_mc_id() {
  if (not lep_mc_id_isLoaded) {
    if (lep_mc_id_branch != 0) {
      lep_mc_id_branch->GetEntry(index);
    } else {
      printf("branch lep_mc_id_branch does not exist!\n");
      exit(1);
    }
    lep_mc_id_isLoaded = true;
  }
  return *lep_mc_id_;
}

const vector<int> &wvztree::lep_motherIdv2() {
  if (not lep_motherIdv2_isLoaded) {
    if (lep_motherIdv2_branch != 0) {
      lep_motherIdv2_branch->GetEntry(index);
    } else {
      printf("branch lep_motherIdv2_branch does not exist!\n");
      exit(1);
    }
    lep_motherIdv2_isLoaded = true;
  }
  return *lep_motherIdv2_;
}

const vector<int> &wvztree::lep_idx() {
  if (not lep_idx_isLoaded) {
    if (lep_idx_branch != 0) {
      lep_idx_branch->GetEntry(index);
    } else {
      printf("branch lep_idx_branch does not exist!\n");
      exit(1);
    }
    lep_idx_isLoaded = true;
  }
  return *lep_idx_;
}

const vector<int> &wvztree::lep_id() {
  if (not lep_id_isLoaded) {
    if (lep_id_branch != 0) {
      lep_id_branch->GetEntry(index);
    } else {
      printf("branch lep_id_branch does not exist!\n");
      exit(1);
    }
    lep_id_isLoaded = true;
  }
  return *lep_id_;
}

const vector<int> &wvztree::lep_isTightPOG() {
  if (not lep_isTightPOG_isLoaded) {
    if (lep_isTightPOG_branch != 0) {
      lep_isTightPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isTightPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isTightPOG_isLoaded = true;
  }
  return *lep_isTightPOG_;
}

const vector<int> &wvztree::lep_isMediumPOG() {
  if (not lep_isMediumPOG_isLoaded) {
    if (lep_isMediumPOG_branch != 0) {
      lep_isMediumPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMediumPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMediumPOG_isLoaded = true;
  }
  return *lep_isMediumPOG_;
}

const vector<int> &wvztree::lep_isMVAwp80NoIsoPOG() {
  if (not lep_isMVAwp80NoIsoPOG_isLoaded) {
    if (lep_isMVAwp80NoIsoPOG_branch != 0) {
      lep_isMVAwp80NoIsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwp80NoIsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwp80NoIsoPOG_isLoaded = true;
  }
  return *lep_isMVAwp80NoIsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwp90NoIsoPOG() {
  if (not lep_isMVAwp90NoIsoPOG_isLoaded) {
    if (lep_isMVAwp90NoIsoPOG_branch != 0) {
      lep_isMVAwp90NoIsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwp90NoIsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwp90NoIsoPOG_isLoaded = true;
  }
  return *lep_isMVAwp90NoIsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwpLooseNoIsoPOG() {
  if (not lep_isMVAwpLooseNoIsoPOG_isLoaded) {
    if (lep_isMVAwpLooseNoIsoPOG_branch != 0) {
      lep_isMVAwpLooseNoIsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwpLooseNoIsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwpLooseNoIsoPOG_isLoaded = true;
  }
  return *lep_isMVAwpLooseNoIsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwp80IsoPOG() {
  if (not lep_isMVAwp80IsoPOG_isLoaded) {
    if (lep_isMVAwp80IsoPOG_branch != 0) {
      lep_isMVAwp80IsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwp80IsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwp80IsoPOG_isLoaded = true;
  }
  return *lep_isMVAwp80IsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwp90IsoPOG() {
  if (not lep_isMVAwp90IsoPOG_isLoaded) {
    if (lep_isMVAwp90IsoPOG_branch != 0) {
      lep_isMVAwp90IsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwp90IsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwp90IsoPOG_isLoaded = true;
  }
  return *lep_isMVAwp90IsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwpLooseIsoPOG() {
  if (not lep_isMVAwpLooseIsoPOG_isLoaded) {
    if (lep_isMVAwpLooseIsoPOG_branch != 0) {
      lep_isMVAwpLooseIsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwpLooseIsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwpLooseIsoPOG_isLoaded = true;
  }
  return *lep_isMVAwpLooseIsoPOG_;
}

const vector<int> &wvztree::lep_isMVAwpHZZIsoPOG() {
  if (not lep_isMVAwpHZZIsoPOG_isLoaded) {
    if (lep_isMVAwpHZZIsoPOG_branch != 0) {
      lep_isMVAwpHZZIsoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isMVAwpHZZIsoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isMVAwpHZZIsoPOG_isLoaded = true;
  }
  return *lep_isMVAwpHZZIsoPOG_;
}

const vector<int> &wvztree::lep_isCutBasedNoIsoVetoPOG() {
  if (not lep_isCutBasedNoIsoVetoPOG_isLoaded) {
    if (lep_isCutBasedNoIsoVetoPOG_branch != 0) {
      lep_isCutBasedNoIsoVetoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedNoIsoVetoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedNoIsoVetoPOG_isLoaded = true;
  }
  return *lep_isCutBasedNoIsoVetoPOG_;
}

const vector<int> &wvztree::lep_isCutBasedNoIsoLoosePOG() {
  if (not lep_isCutBasedNoIsoLoosePOG_isLoaded) {
    if (lep_isCutBasedNoIsoLoosePOG_branch != 0) {
      lep_isCutBasedNoIsoLoosePOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedNoIsoLoosePOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedNoIsoLoosePOG_isLoaded = true;
  }
  return *lep_isCutBasedNoIsoLoosePOG_;
}

const vector<int> &wvztree::lep_isCutBasedNoIsoMediumPOG() {
  if (not lep_isCutBasedNoIsoMediumPOG_isLoaded) {
    if (lep_isCutBasedNoIsoMediumPOG_branch != 0) {
      lep_isCutBasedNoIsoMediumPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedNoIsoMediumPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedNoIsoMediumPOG_isLoaded = true;
  }
  return *lep_isCutBasedNoIsoMediumPOG_;
}

const vector<int> &wvztree::lep_isCutBasedNoIsoTightPOG() {
  if (not lep_isCutBasedNoIsoTightPOG_isLoaded) {
    if (lep_isCutBasedNoIsoTightPOG_branch != 0) {
      lep_isCutBasedNoIsoTightPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedNoIsoTightPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedNoIsoTightPOG_isLoaded = true;
  }
  return *lep_isCutBasedNoIsoTightPOG_;
}

const vector<int> &wvztree::lep_isCutBasedIsoVetoPOG() {
  if (not lep_isCutBasedIsoVetoPOG_isLoaded) {
    if (lep_isCutBasedIsoVetoPOG_branch != 0) {
      lep_isCutBasedIsoVetoPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedIsoVetoPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedIsoVetoPOG_isLoaded = true;
  }
  return *lep_isCutBasedIsoVetoPOG_;
}

const vector<int> &wvztree::lep_isCutBasedIsoLoosePOG() {
  if (not lep_isCutBasedIsoLoosePOG_isLoaded) {
    if (lep_isCutBasedIsoLoosePOG_branch != 0) {
      lep_isCutBasedIsoLoosePOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedIsoLoosePOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedIsoLoosePOG_isLoaded = true;
  }
  return *lep_isCutBasedIsoLoosePOG_;
}

const vector<int> &wvztree::lep_isCutBasedIsoMediumPOG() {
  if (not lep_isCutBasedIsoMediumPOG_isLoaded) {
    if (lep_isCutBasedIsoMediumPOG_branch != 0) {
      lep_isCutBasedIsoMediumPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedIsoMediumPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedIsoMediumPOG_isLoaded = true;
  }
  return *lep_isCutBasedIsoMediumPOG_;
}

const vector<int> &wvztree::lep_isCutBasedIsoTightPOG() {
  if (not lep_isCutBasedIsoTightPOG_isLoaded) {
    if (lep_isCutBasedIsoTightPOG_branch != 0) {
      lep_isCutBasedIsoTightPOG_branch->GetEntry(index);
    } else {
      printf("branch lep_isCutBasedIsoTightPOG_branch does not exist!\n");
      exit(1);
    }
    lep_isCutBasedIsoTightPOG_isLoaded = true;
  }
  return *lep_isCutBasedIsoTightPOG_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &wvztree::met_p4() {
  if (not met_p4_isLoaded) {
    if (met_p4_branch != 0) {
      met_p4_branch->GetEntry(index);
    } else {
      printf("branch met_p4_branch does not exist!\n");
      exit(1);
    }
    met_p4_isLoaded = true;
  }
  return *met_p4_;
}

const float &wvztree::met_pt() {
  if (not met_pt_isLoaded) {
    if (met_pt_branch != 0) {
      met_pt_branch->GetEntry(index);
    } else {
      printf("branch met_pt_branch does not exist!\n");
      exit(1);
    }
    met_pt_isLoaded = true;
  }
  return met_pt_;
}

const float &wvztree::met_phi() {
  if (not met_phi_isLoaded) {
    if (met_phi_branch != 0) {
      met_phi_branch->GetEntry(index);
    } else {
      printf("branch met_phi_branch does not exist!\n");
      exit(1);
    }
    met_phi_isLoaded = true;
  }
  return met_phi_;
}

const float &wvztree::met_up_pt() {
  if (not met_up_pt_isLoaded) {
    if (met_up_pt_branch != 0) {
      met_up_pt_branch->GetEntry(index);
    } else {
      printf("branch met_up_pt_branch does not exist!\n");
      exit(1);
    }
    met_up_pt_isLoaded = true;
  }
  return met_up_pt_;
}

const float &wvztree::met_up_phi() {
  if (not met_up_phi_isLoaded) {
    if (met_up_phi_branch != 0) {
      met_up_phi_branch->GetEntry(index);
    } else {
      printf("branch met_up_phi_branch does not exist!\n");
      exit(1);
    }
    met_up_phi_isLoaded = true;
  }
  return met_up_phi_;
}

const float &wvztree::met_dn_pt() {
  if (not met_dn_pt_isLoaded) {
    if (met_dn_pt_branch != 0) {
      met_dn_pt_branch->GetEntry(index);
    } else {
      printf("branch met_dn_pt_branch does not exist!\n");
      exit(1);
    }
    met_dn_pt_isLoaded = true;
  }
  return met_dn_pt_;
}

const float &wvztree::met_dn_phi() {
  if (not met_dn_phi_isLoaded) {
    if (met_dn_phi_branch != 0) {
      met_dn_phi_branch->GetEntry(index);
    } else {
      printf("branch met_dn_phi_branch does not exist!\n");
      exit(1);
    }
    met_dn_phi_isLoaded = true;
  }
  return met_dn_phi_;
}

const float &wvztree::met_gen_pt() {
  if (not met_gen_pt_isLoaded) {
    if (met_gen_pt_branch != 0) {
      met_gen_pt_branch->GetEntry(index);
    } else {
      printf("branch met_gen_pt_branch does not exist!\n");
      exit(1);
    }
    met_gen_pt_isLoaded = true;
  }
  return met_gen_pt_;
}

const float &wvztree::met_gen_phi() {
  if (not met_gen_phi_isLoaded) {
    if (met_gen_phi_branch != 0) {
      met_gen_phi_branch->GetEntry(index);
    } else {
      printf("branch met_gen_phi_branch does not exist!\n");
      exit(1);
    }
    met_gen_phi_isLoaded = true;
  }
  return met_gen_phi_;
}

const int &wvztree::passesMETfiltersRun2() {
  if (not passesMETfiltersRun2_isLoaded) {
    if (passesMETfiltersRun2_branch != 0) {
      passesMETfiltersRun2_branch->GetEntry(index);
    } else {
      printf("branch passesMETfiltersRun2_branch does not exist!\n");
      exit(1);
    }
    passesMETfiltersRun2_isLoaded = true;
  }
  return passesMETfiltersRun2_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::jets_p4() {
  if (not jets_p4_isLoaded) {
    if (jets_p4_branch != 0) {
      jets_p4_branch->GetEntry(index);
    } else {
      printf("branch jets_p4_branch does not exist!\n");
      exit(1);
    }
    jets_p4_isLoaded = true;
  }
  return *jets_p4_;
}

const vector<float> &wvztree::jets_pt() {
  if (not jets_pt_isLoaded) {
    if (jets_pt_branch != 0) {
      jets_pt_branch->GetEntry(index);
    } else {
      printf("branch jets_pt_branch does not exist!\n");
      exit(1);
    }
    jets_pt_isLoaded = true;
  }
  return *jets_pt_;
}

const vector<float> &wvztree::jets_eta() {
  if (not jets_eta_isLoaded) {
    if (jets_eta_branch != 0) {
      jets_eta_branch->GetEntry(index);
    } else {
      printf("branch jets_eta_branch does not exist!\n");
      exit(1);
    }
    jets_eta_isLoaded = true;
  }
  return *jets_eta_;
}

const vector<float> &wvztree::jets_phi() {
  if (not jets_phi_isLoaded) {
    if (jets_phi_branch != 0) {
      jets_phi_branch->GetEntry(index);
    } else {
      printf("branch jets_phi_branch does not exist!\n");
      exit(1);
    }
    jets_phi_isLoaded = true;
  }
  return *jets_phi_;
}

const vector<float> &wvztree::jets_mass() {
  if (not jets_mass_isLoaded) {
    if (jets_mass_branch != 0) {
      jets_mass_branch->GetEntry(index);
    } else {
      printf("branch jets_mass_branch does not exist!\n");
      exit(1);
    }
    jets_mass_isLoaded = true;
  }
  return *jets_mass_;
}

const vector<float> &wvztree::jets_btag_score() {
  if (not jets_btag_score_isLoaded) {
    if (jets_btag_score_branch != 0) {
      jets_btag_score_branch->GetEntry(index);
    } else {
      printf("branch jets_btag_score_branch does not exist!\n");
      exit(1);
    }
    jets_btag_score_isLoaded = true;
  }
  return *jets_btag_score_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::jets_cen_p4() {
  if (not jets_cen_p4_isLoaded) {
    if (jets_cen_p4_branch != 0) {
      jets_cen_p4_branch->GetEntry(index);
    } else {
      printf("branch jets_cen_p4_branch does not exist!\n");
      exit(1);
    }
    jets_cen_p4_isLoaded = true;
  }
  return *jets_cen_p4_;
}

const vector<float> &wvztree::jets_cen_pt() {
  if (not jets_cen_pt_isLoaded) {
    if (jets_cen_pt_branch != 0) {
      jets_cen_pt_branch->GetEntry(index);
    } else {
      printf("branch jets_cen_pt_branch does not exist!\n");
      exit(1);
    }
    jets_cen_pt_isLoaded = true;
  }
  return *jets_cen_pt_;
}

const vector<float> &wvztree::jets_cen_eta() {
  if (not jets_cen_eta_isLoaded) {
    if (jets_cen_eta_branch != 0) {
      jets_cen_eta_branch->GetEntry(index);
    } else {
      printf("branch jets_cen_eta_branch does not exist!\n");
      exit(1);
    }
    jets_cen_eta_isLoaded = true;
  }
  return *jets_cen_eta_;
}

const vector<float> &wvztree::jets_cen_phi() {
  if (not jets_cen_phi_isLoaded) {
    if (jets_cen_phi_branch != 0) {
      jets_cen_phi_branch->GetEntry(index);
    } else {
      printf("branch jets_cen_phi_branch does not exist!\n");
      exit(1);
    }
    jets_cen_phi_isLoaded = true;
  }
  return *jets_cen_phi_;
}

const vector<float> &wvztree::jets_cen_mass() {
  if (not jets_cen_mass_isLoaded) {
    if (jets_cen_mass_branch != 0) {
      jets_cen_mass_branch->GetEntry(index);
    } else {
      printf("branch jets_cen_mass_branch does not exist!\n");
      exit(1);
    }
    jets_cen_mass_isLoaded = true;
  }
  return *jets_cen_mass_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &wvztree::rawjets_p4() {
  if (not rawjets_p4_isLoaded) {
    if (rawjets_p4_branch != 0) {
      rawjets_p4_branch->GetEntry(index);
    } else {
      printf("branch rawjets_p4_branch does not exist!\n");
      exit(1);
    }
    rawjets_p4_isLoaded = true;
  }
  return *rawjets_p4_;
}

const vector<float> &wvztree::rawjets_pt() {
  if (not rawjets_pt_isLoaded) {
    if (rawjets_pt_branch != 0) {
      rawjets_pt_branch->GetEntry(index);
    } else {
      printf("branch rawjets_pt_branch does not exist!\n");
      exit(1);
    }
    rawjets_pt_isLoaded = true;
  }
  return *rawjets_pt_;
}

const vector<float> &wvztree::rawjets_eta() {
  if (not rawjets_eta_isLoaded) {
    if (rawjets_eta_branch != 0) {
      rawjets_eta_branch->GetEntry(index);
    } else {
      printf("branch rawjets_eta_branch does not exist!\n");
      exit(1);
    }
    rawjets_eta_isLoaded = true;
  }
  return *rawjets_eta_;
}

const vector<float> &wvztree::rawjets_phi() {
  if (not rawjets_phi_isLoaded) {
    if (rawjets_phi_branch != 0) {
      rawjets_phi_branch->GetEntry(index);
    } else {
      printf("branch rawjets_phi_branch does not exist!\n");
      exit(1);
    }
    rawjets_phi_isLoaded = true;
  }
  return *rawjets_phi_;
}

const vector<float> &wvztree::rawjets_mass() {
  if (not rawjets_mass_isLoaded) {
    if (rawjets_mass_branch != 0) {
      rawjets_mass_branch->GetEntry(index);
    } else {
      printf("branch rawjets_mass_branch does not exist!\n");
      exit(1);
    }
    rawjets_mass_isLoaded = true;
  }
  return *rawjets_mass_;
}

const vector<float> &wvztree::rawjets_JEC() {
  if (not rawjets_JEC_isLoaded) {
    if (rawjets_JEC_branch != 0) {
      rawjets_JEC_branch->GetEntry(index);
    } else {
      printf("branch rawjets_JEC_branch does not exist!\n");
      exit(1);
    }
    rawjets_JEC_isLoaded = true;
  }
  return *rawjets_JEC_;
}

const vector<int> &wvztree::rawjets_passJetID() {
  if (not rawjets_passJetID_isLoaded) {
    if (rawjets_passJetID_branch != 0) {
      rawjets_passJetID_branch->GetEntry(index);
    } else {
      printf("branch rawjets_passJetID_branch does not exist!\n");
      exit(1);
    }
    rawjets_passJetID_isLoaded = true;
  }
  return *rawjets_passJetID_;
}

const vector<int> &wvztree::rawjets_isORwLep() {
  if (not rawjets_isORwLep_isLoaded) {
    if (rawjets_isORwLep_branch != 0) {
      rawjets_isORwLep_branch->GetEntry(index);
    } else {
      printf("branch rawjets_isORwLep_branch does not exist!\n");
      exit(1);
    }
    rawjets_isORwLep_isLoaded = true;
  }
  return *rawjets_isORwLep_;
}

const int &wvztree::nj() {
  if (not nj_isLoaded) {
    if (nj_branch != 0) {
      nj_branch->GetEntry(index);
    } else {
      printf("branch nj_branch does not exist!\n");
      exit(1);
    }
    nj_isLoaded = true;
  }
  return nj_;
}

const int &wvztree::nj_up() {
  if (not nj_up_isLoaded) {
    if (nj_up_branch != 0) {
      nj_up_branch->GetEntry(index);
    } else {
      printf("branch nj_up_branch does not exist!\n");
      exit(1);
    }
    nj_up_isLoaded = true;
  }
  return nj_up_;
}

const int &wvztree::nj_dn() {
  if (not nj_dn_isLoaded) {
    if (nj_dn_branch != 0) {
      nj_dn_branch->GetEntry(index);
    } else {
      printf("branch nj_dn_branch does not exist!\n");
      exit(1);
    }
    nj_dn_isLoaded = true;
  }
  return nj_dn_;
}

const int &wvztree::nb() {
  if (not nb_isLoaded) {
    if (nb_branch != 0) {
      nb_branch->GetEntry(index);
    } else {
      printf("branch nb_branch does not exist!\n");
      exit(1);
    }
    nb_isLoaded = true;
  }
  return nb_;
}

const int &wvztree::nb_up() {
  if (not nb_up_isLoaded) {
    if (nb_up_branch != 0) {
      nb_up_branch->GetEntry(index);
    } else {
      printf("branch nb_up_branch does not exist!\n");
      exit(1);
    }
    nb_up_isLoaded = true;
  }
  return nb_up_;
}

const int &wvztree::nb_dn() {
  if (not nb_dn_isLoaded) {
    if (nb_dn_branch != 0) {
      nb_dn_branch->GetEntry(index);
    } else {
      printf("branch nb_dn_branch does not exist!\n");
      exit(1);
    }
    nb_dn_isLoaded = true;
  }
  return nb_dn_;
}

const int &wvztree::nbmed() {
  if (not nbmed_isLoaded) {
    if (nbmed_branch != 0) {
      nbmed_branch->GetEntry(index);
    } else {
      printf("branch nbmed_branch does not exist!\n");
      exit(1);
    }
    nbmed_isLoaded = true;
  }
  return nbmed_;
}

const float &wvztree::ht() {
  if (not ht_isLoaded) {
    if (ht_branch != 0) {
      ht_branch->GetEntry(index);
    } else {
      printf("branch ht_branch does not exist!\n");
      exit(1);
    }
    ht_isLoaded = true;
  }
  return ht_;
}

const int &wvztree::nj_cen() {
  if (not nj_cen_isLoaded) {
    if (nj_cen_branch != 0) {
      nj_cen_branch->GetEntry(index);
    } else {
      printf("branch nj_cen_branch does not exist!\n");
      exit(1);
    }
    nj_cen_isLoaded = true;
  }
  return nj_cen_;
}

const int &wvztree::nj_cen_up() {
  if (not nj_cen_up_isLoaded) {
    if (nj_cen_up_branch != 0) {
      nj_cen_up_branch->GetEntry(index);
    } else {
      printf("branch nj_cen_up_branch does not exist!\n");
      exit(1);
    }
    nj_cen_up_isLoaded = true;
  }
  return nj_cen_up_;
}

const int &wvztree::nj_cen_dn() {
  if (not nj_cen_dn_isLoaded) {
    if (nj_cen_dn_branch != 0) {
      nj_cen_dn_branch->GetEntry(index);
    } else {
      printf("branch nj_cen_dn_branch does not exist!\n");
      exit(1);
    }
    nj_cen_dn_isLoaded = true;
  }
  return nj_cen_dn_;
}

const float &wvztree::weight_btagsf() {
  if (not weight_btagsf_isLoaded) {
    if (weight_btagsf_branch != 0) {
      weight_btagsf_branch->GetEntry(index);
    } else {
      printf("branch weight_btagsf_branch does not exist!\n");
      exit(1);
    }
    weight_btagsf_isLoaded = true;
  }
  return weight_btagsf_;
}

const float &wvztree::weight_btagsf_heavy_DN() {
  if (not weight_btagsf_heavy_DN_isLoaded) {
    if (weight_btagsf_heavy_DN_branch != 0) {
      weight_btagsf_heavy_DN_branch->GetEntry(index);
    } else {
      printf("branch weight_btagsf_heavy_DN_branch does not exist!\n");
      exit(1);
    }
    weight_btagsf_heavy_DN_isLoaded = true;
  }
  return weight_btagsf_heavy_DN_;
}

const float &wvztree::weight_btagsf_heavy_UP() {
  if (not weight_btagsf_heavy_UP_isLoaded) {
    if (weight_btagsf_heavy_UP_branch != 0) {
      weight_btagsf_heavy_UP_branch->GetEntry(index);
    } else {
      printf("branch weight_btagsf_heavy_UP_branch does not exist!\n");
      exit(1);
    }
    weight_btagsf_heavy_UP_isLoaded = true;
  }
  return weight_btagsf_heavy_UP_;
}

const float &wvztree::weight_btagsf_light_DN() {
  if (not weight_btagsf_light_DN_isLoaded) {
    if (weight_btagsf_light_DN_branch != 0) {
      weight_btagsf_light_DN_branch->GetEntry(index);
    } else {
      printf("branch weight_btagsf_light_DN_branch does not exist!\n");
      exit(1);
    }
    weight_btagsf_light_DN_isLoaded = true;
  }
  return weight_btagsf_light_DN_;
}

const float &wvztree::weight_btagsf_light_UP() {
  if (not weight_btagsf_light_UP_isLoaded) {
    if (weight_btagsf_light_UP_branch != 0) {
      weight_btagsf_light_UP_branch->GetEntry(index);
    } else {
      printf("branch weight_btagsf_light_UP_branch does not exist!\n");
      exit(1);
    }
    weight_btagsf_light_UP_isLoaded = true;
  }
  return weight_btagsf_light_UP_;
}


void wvztree::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace tas {

const int &run() { return wvz.run(); }
const int &lumi() { return wvz.lumi(); }
const unsigned long long &evt() { return wvz.evt(); }
const int &isData() { return wvz.isData(); }
const float &evt_scale1fb() { return wvz.evt_scale1fb(); }
const float &genps_weight() { return wvz.genps_weight(); }
const float &xsec_br() { return wvz.xsec_br(); }
const int &evt_passgoodrunlist() { return wvz.evt_passgoodrunlist(); }
const TString &CMS4path() { return wvz.CMS4path(); }
const int &CMS4index() { return wvz.CMS4index(); }
const float &weight_fr_r1_f1() { return wvz.weight_fr_r1_f1(); }
const float &weight_fr_r1_f2() { return wvz.weight_fr_r1_f2(); }
const float &weight_fr_r1_f0p5() { return wvz.weight_fr_r1_f0p5(); }
const float &weight_fr_r2_f1() { return wvz.weight_fr_r2_f1(); }
const float &weight_fr_r2_f2() { return wvz.weight_fr_r2_f2(); }
const float &weight_fr_r2_f0p5() { return wvz.weight_fr_r2_f0p5(); }
const float &weight_fr_r0p5_f1() { return wvz.weight_fr_r0p5_f1(); }
const float &weight_fr_r0p5_f2() { return wvz.weight_fr_r0p5_f2(); }
const float &weight_fr_r0p5_f0p5() { return wvz.weight_fr_r0p5_f0p5(); }
const float &weight_pdf_up() { return wvz.weight_pdf_up(); }
const float &weight_pdf_down() { return wvz.weight_pdf_down(); }
const float &weight_alphas_down() { return wvz.weight_alphas_down(); }
const float &weight_alphas_up() { return wvz.weight_alphas_up(); }
const int &HLT_DoubleMu() { return wvz.HLT_DoubleMu(); }
const int &HLT_DoubleEl() { return wvz.HLT_DoubleEl(); }
const int &HLT_MuEG() { return wvz.HLT_MuEG(); }
const int &pass_duplicate_ee_em_mm() { return wvz.pass_duplicate_ee_em_mm(); }
const int &pass_duplicate_mm_em_ee() { return wvz.pass_duplicate_mm_em_ee(); }
const float &gen_ht() { return wvz.gen_ht(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_V_p4() { return wvz.gen_V_p4(); }
const vector<float> &gen_V_pt() { return wvz.gen_V_pt(); }
const vector<float> &gen_V_eta() { return wvz.gen_V_eta(); }
const vector<float> &gen_V_phi() { return wvz.gen_V_phi(); }
const vector<float> &gen_V_mass() { return wvz.gen_V_mass(); }
const vector<int> &gen_V_id() { return wvz.gen_V_id(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_lep_p4() { return wvz.gen_lep_p4(); }
const vector<float> &gen_lep_pt() { return wvz.gen_lep_pt(); }
const vector<float> &gen_lep_eta() { return wvz.gen_lep_eta(); }
const vector<float> &gen_lep_phi() { return wvz.gen_lep_phi(); }
const vector<float> &gen_lep_mass() { return wvz.gen_lep_mass(); }
const vector<int> &gen_lep_id() { return wvz.gen_lep_id(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_child_p4() { return wvz.gen_child_p4(); }
const vector<float> &gen_child_pt() { return wvz.gen_child_pt(); }
const vector<float> &gen_child_eta() { return wvz.gen_child_eta(); }
const vector<float> &gen_child_phi() { return wvz.gen_child_phi(); }
const vector<float> &gen_child_mass() { return wvz.gen_child_mass(); }
const vector<int> &gen_child_id() { return wvz.gen_child_id(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_part_p4() { return wvz.gen_part_p4(); }
const vector<float> &gen_part_pt() { return wvz.gen_part_pt(); }
const vector<float> &gen_part_eta() { return wvz.gen_part_eta(); }
const vector<float> &gen_part_phi() { return wvz.gen_part_phi(); }
const vector<float> &gen_part_mass() { return wvz.gen_part_mass(); }
const vector<int> &gen_part_id() { return wvz.gen_part_id(); }
const vector<int> &gen_part_mother_id() { return wvz.gen_part_mother_id(); }
const vector<int> &gen_part_grandma_id() { return wvz.gen_part_grandma_id(); }
const vector<int> &gen_part_status() { return wvz.gen_part_status(); }
const int &VHchannel() { return wvz.VHchannel(); }
const int &Higgschannel() { return wvz.Higgschannel(); }
const int &nGenTauClean() { return wvz.nGenTauClean(); }
const int &nGenTau() { return wvz.nGenTau(); }
const int &hasTau() { return wvz.hasTau(); }
const int &nLightLep() { return wvz.nLightLep(); }
const int &firstgoodvertex() { return wvz.firstgoodvertex(); }
const int &nvtx() { return wvz.nvtx(); }
const int &nTrueInt() { return wvz.nTrueInt(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &lep_p4() { return wvz.lep_p4(); }
const vector<float> &lep_pt() { return wvz.lep_pt(); }
const vector<float> &lep_eta() { return wvz.lep_eta(); }
const vector<float> &lep_etaSC() { return wvz.lep_etaSC(); }
const vector<float> &lep_phi() { return wvz.lep_phi(); }
const vector<float> &lep_energy() { return wvz.lep_energy(); }
const vector<float> &lep_mva() { return wvz.lep_mva(); }
const vector<float> &lep_mvaIso() { return wvz.lep_mvaIso(); }
const vector<float> &lep_relIso04DB() { return wvz.lep_relIso04DB(); }
const vector<float> &lep_relIso03EA() { return wvz.lep_relIso03EA(); }
const vector<float> &lep_relIso03EAv4() { return wvz.lep_relIso03EAv4(); }
const vector<float> &lep_relIso03EAwLep() { return wvz.lep_relIso03EAwLep(); }
const vector<float> &lep_ip3d() { return wvz.lep_ip3d(); }
const vector<float> &lep_sip3d() { return wvz.lep_sip3d(); }
const vector<float> &lep_dxy() { return wvz.lep_dxy(); }
const vector<float> &lep_dz() { return wvz.lep_dz(); }
const vector<int> &lep_mc_motherid() { return wvz.lep_mc_motherid(); }
const vector<int> &lep_mc_id() { return wvz.lep_mc_id(); }
const vector<int> &lep_motherIdv2() { return wvz.lep_motherIdv2(); }
const vector<int> &lep_idx() { return wvz.lep_idx(); }
const vector<int> &lep_id() { return wvz.lep_id(); }
const vector<int> &lep_isTightPOG() { return wvz.lep_isTightPOG(); }
const vector<int> &lep_isMediumPOG() { return wvz.lep_isMediumPOG(); }
const vector<int> &lep_isMVAwp80NoIsoPOG() { return wvz.lep_isMVAwp80NoIsoPOG(); }
const vector<int> &lep_isMVAwp90NoIsoPOG() { return wvz.lep_isMVAwp90NoIsoPOG(); }
const vector<int> &lep_isMVAwpLooseNoIsoPOG() { return wvz.lep_isMVAwpLooseNoIsoPOG(); }
const vector<int> &lep_isMVAwp80IsoPOG() { return wvz.lep_isMVAwp80IsoPOG(); }
const vector<int> &lep_isMVAwp90IsoPOG() { return wvz.lep_isMVAwp90IsoPOG(); }
const vector<int> &lep_isMVAwpLooseIsoPOG() { return wvz.lep_isMVAwpLooseIsoPOG(); }
const vector<int> &lep_isMVAwpHZZIsoPOG() { return wvz.lep_isMVAwpHZZIsoPOG(); }
const vector<int> &lep_isCutBasedNoIsoVetoPOG() { return wvz.lep_isCutBasedNoIsoVetoPOG(); }
const vector<int> &lep_isCutBasedNoIsoLoosePOG() { return wvz.lep_isCutBasedNoIsoLoosePOG(); }
const vector<int> &lep_isCutBasedNoIsoMediumPOG() { return wvz.lep_isCutBasedNoIsoMediumPOG(); }
const vector<int> &lep_isCutBasedNoIsoTightPOG() { return wvz.lep_isCutBasedNoIsoTightPOG(); }
const vector<int> &lep_isCutBasedIsoVetoPOG() { return wvz.lep_isCutBasedIsoVetoPOG(); }
const vector<int> &lep_isCutBasedIsoLoosePOG() { return wvz.lep_isCutBasedIsoLoosePOG(); }
const vector<int> &lep_isCutBasedIsoMediumPOG() { return wvz.lep_isCutBasedIsoMediumPOG(); }
const vector<int> &lep_isCutBasedIsoTightPOG() { return wvz.lep_isCutBasedIsoTightPOG(); }
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &met_p4() { return wvz.met_p4(); }
const float &met_pt() { return wvz.met_pt(); }
const float &met_phi() { return wvz.met_phi(); }
const float &met_up_pt() { return wvz.met_up_pt(); }
const float &met_up_phi() { return wvz.met_up_phi(); }
const float &met_dn_pt() { return wvz.met_dn_pt(); }
const float &met_dn_phi() { return wvz.met_dn_phi(); }
const float &met_gen_pt() { return wvz.met_gen_pt(); }
const float &met_gen_phi() { return wvz.met_gen_phi(); }
const int &passesMETfiltersRun2() { return wvz.passesMETfiltersRun2(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_p4() { return wvz.jets_p4(); }
const vector<float> &jets_pt() { return wvz.jets_pt(); }
const vector<float> &jets_eta() { return wvz.jets_eta(); }
const vector<float> &jets_phi() { return wvz.jets_phi(); }
const vector<float> &jets_mass() { return wvz.jets_mass(); }
const vector<float> &jets_btag_score() { return wvz.jets_btag_score(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_cen_p4() { return wvz.jets_cen_p4(); }
const vector<float> &jets_cen_pt() { return wvz.jets_cen_pt(); }
const vector<float> &jets_cen_eta() { return wvz.jets_cen_eta(); }
const vector<float> &jets_cen_phi() { return wvz.jets_cen_phi(); }
const vector<float> &jets_cen_mass() { return wvz.jets_cen_mass(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &rawjets_p4() { return wvz.rawjets_p4(); }
const vector<float> &rawjets_pt() { return wvz.rawjets_pt(); }
const vector<float> &rawjets_eta() { return wvz.rawjets_eta(); }
const vector<float> &rawjets_phi() { return wvz.rawjets_phi(); }
const vector<float> &rawjets_mass() { return wvz.rawjets_mass(); }
const vector<float> &rawjets_JEC() { return wvz.rawjets_JEC(); }
const vector<int> &rawjets_passJetID() { return wvz.rawjets_passJetID(); }
const vector<int> &rawjets_isORwLep() { return wvz.rawjets_isORwLep(); }
const int &nj() { return wvz.nj(); }
const int &nj_up() { return wvz.nj_up(); }
const int &nj_dn() { return wvz.nj_dn(); }
const int &nb() { return wvz.nb(); }
const int &nb_up() { return wvz.nb_up(); }
const int &nb_dn() { return wvz.nb_dn(); }
const int &nbmed() { return wvz.nbmed(); }
const float &ht() { return wvz.ht(); }
const int &nj_cen() { return wvz.nj_cen(); }
const int &nj_cen_up() { return wvz.nj_cen_up(); }
const int &nj_cen_dn() { return wvz.nj_cen_dn(); }
const float &weight_btagsf() { return wvz.weight_btagsf(); }
const float &weight_btagsf_heavy_DN() { return wvz.weight_btagsf_heavy_DN(); }
const float &weight_btagsf_heavy_UP() { return wvz.weight_btagsf_heavy_UP(); }
const float &weight_btagsf_light_DN() { return wvz.weight_btagsf_light_DN(); }
const float &weight_btagsf_light_UP() { return wvz.weight_btagsf_light_UP(); }

}

#include "rooutil.cc"
