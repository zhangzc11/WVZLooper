// -*- C++ -*-
// This is a header file generated with the command:
// makeCMS3ClassFiles("/nfs-7/userdata/phchang/babies/WVZ2017_v0.1.12.4/wwz_amcatnlo_1.root", "t", "wvztree", "tas", "wvz")

#ifndef wvztree_H
#define wvztree_H

#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class wvztree {
 private: 
 protected: 
  unsigned int index;
  int      run_;
  TBranch *run_branch;
  bool     run_isLoaded;
  int      lumi_;
  TBranch *lumi_branch;
  bool     lumi_isLoaded;
  unsigned long long evt_;
  TBranch *evt_branch;
  bool     evt_isLoaded;
  int      isData_;
  TBranch *isData_branch;
  bool     isData_isLoaded;
  float    evt_scale1fb_;
  TBranch *evt_scale1fb_branch;
  bool     evt_scale1fb_isLoaded;
  float    genps_weight_;
  TBranch *genps_weight_branch;
  bool     genps_weight_isLoaded;
  float    xsec_br_;
  TBranch *xsec_br_branch;
  bool     xsec_br_isLoaded;
  int      evt_passgoodrunlist_;
  TBranch *evt_passgoodrunlist_branch;
  bool     evt_passgoodrunlist_isLoaded;
  TString *CMS4path_;
  TBranch *CMS4path_branch;
  bool     CMS4path_isLoaded;
  int      CMS4index_;
  TBranch *CMS4index_branch;
  bool     CMS4index_isLoaded;
  float    weight_fr_r1_f1_;
  TBranch *weight_fr_r1_f1_branch;
  bool     weight_fr_r1_f1_isLoaded;
  float    weight_fr_r1_f2_;
  TBranch *weight_fr_r1_f2_branch;
  bool     weight_fr_r1_f2_isLoaded;
  float    weight_fr_r1_f0p5_;
  TBranch *weight_fr_r1_f0p5_branch;
  bool     weight_fr_r1_f0p5_isLoaded;
  float    weight_fr_r2_f1_;
  TBranch *weight_fr_r2_f1_branch;
  bool     weight_fr_r2_f1_isLoaded;
  float    weight_fr_r2_f2_;
  TBranch *weight_fr_r2_f2_branch;
  bool     weight_fr_r2_f2_isLoaded;
  float    weight_fr_r2_f0p5_;
  TBranch *weight_fr_r2_f0p5_branch;
  bool     weight_fr_r2_f0p5_isLoaded;
  float    weight_fr_r0p5_f1_;
  TBranch *weight_fr_r0p5_f1_branch;
  bool     weight_fr_r0p5_f1_isLoaded;
  float    weight_fr_r0p5_f2_;
  TBranch *weight_fr_r0p5_f2_branch;
  bool     weight_fr_r0p5_f2_isLoaded;
  float    weight_fr_r0p5_f0p5_;
  TBranch *weight_fr_r0p5_f0p5_branch;
  bool     weight_fr_r0p5_f0p5_isLoaded;
  float    weight_pdf_up_;
  TBranch *weight_pdf_up_branch;
  bool     weight_pdf_up_isLoaded;
  float    weight_pdf_down_;
  TBranch *weight_pdf_down_branch;
  bool     weight_pdf_down_isLoaded;
  float    weight_alphas_down_;
  TBranch *weight_alphas_down_branch;
  bool     weight_alphas_down_isLoaded;
  float    weight_alphas_up_;
  TBranch *weight_alphas_up_branch;
  bool     weight_alphas_up_isLoaded;
  int      HLT_DoubleMu_;
  TBranch *HLT_DoubleMu_branch;
  bool     HLT_DoubleMu_isLoaded;
  int      HLT_DoubleEl_;
  TBranch *HLT_DoubleEl_branch;
  bool     HLT_DoubleEl_isLoaded;
  int      HLT_MuEG_;
  TBranch *HLT_MuEG_branch;
  bool     HLT_MuEG_isLoaded;
  int      pass_duplicate_ee_em_mm_;
  TBranch *pass_duplicate_ee_em_mm_branch;
  bool     pass_duplicate_ee_em_mm_isLoaded;
  int      pass_duplicate_mm_em_ee_;
  TBranch *pass_duplicate_mm_em_ee_branch;
  bool     pass_duplicate_mm_em_ee_isLoaded;
  float    gen_ht_;
  TBranch *gen_ht_branch;
  bool     gen_ht_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *gen_V_p4_;
  TBranch *gen_V_p4_branch;
  bool     gen_V_p4_isLoaded;
  vector<float> *gen_V_pt_;
  TBranch *gen_V_pt_branch;
  bool     gen_V_pt_isLoaded;
  vector<float> *gen_V_eta_;
  TBranch *gen_V_eta_branch;
  bool     gen_V_eta_isLoaded;
  vector<float> *gen_V_phi_;
  TBranch *gen_V_phi_branch;
  bool     gen_V_phi_isLoaded;
  vector<float> *gen_V_mass_;
  TBranch *gen_V_mass_branch;
  bool     gen_V_mass_isLoaded;
  vector<int> *gen_V_id_;
  TBranch *gen_V_id_branch;
  bool     gen_V_id_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *gen_lep_p4_;
  TBranch *gen_lep_p4_branch;
  bool     gen_lep_p4_isLoaded;
  vector<float> *gen_lep_pt_;
  TBranch *gen_lep_pt_branch;
  bool     gen_lep_pt_isLoaded;
  vector<float> *gen_lep_eta_;
  TBranch *gen_lep_eta_branch;
  bool     gen_lep_eta_isLoaded;
  vector<float> *gen_lep_phi_;
  TBranch *gen_lep_phi_branch;
  bool     gen_lep_phi_isLoaded;
  vector<float> *gen_lep_mass_;
  TBranch *gen_lep_mass_branch;
  bool     gen_lep_mass_isLoaded;
  vector<int> *gen_lep_id_;
  TBranch *gen_lep_id_branch;
  bool     gen_lep_id_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *gen_part_p4_;
  TBranch *gen_part_p4_branch;
  bool     gen_part_p4_isLoaded;
  vector<float> *gen_part_pt_;
  TBranch *gen_part_pt_branch;
  bool     gen_part_pt_isLoaded;
  vector<float> *gen_part_eta_;
  TBranch *gen_part_eta_branch;
  bool     gen_part_eta_isLoaded;
  vector<float> *gen_part_phi_;
  TBranch *gen_part_phi_branch;
  bool     gen_part_phi_isLoaded;
  vector<float> *gen_part_mass_;
  TBranch *gen_part_mass_branch;
  bool     gen_part_mass_isLoaded;
  vector<int> *gen_part_id_;
  TBranch *gen_part_id_branch;
  bool     gen_part_id_isLoaded;
  vector<int> *gen_part_mother_id_;
  TBranch *gen_part_mother_id_branch;
  bool     gen_part_mother_id_isLoaded;
  vector<int> *gen_part_grandma_id_;
  TBranch *gen_part_grandma_id_branch;
  bool     gen_part_grandma_id_isLoaded;
  vector<int> *gen_part_status_;
  TBranch *gen_part_status_branch;
  bool     gen_part_status_isLoaded;
  int      VHchannel_;
  TBranch *VHchannel_branch;
  bool     VHchannel_isLoaded;
  int      Higgschannel_;
  TBranch *Higgschannel_branch;
  bool     Higgschannel_isLoaded;
  int      nGenTauClean_;
  TBranch *nGenTauClean_branch;
  bool     nGenTauClean_isLoaded;
  int      nGenTau_;
  TBranch *nGenTau_branch;
  bool     nGenTau_isLoaded;
  int      hasTau_;
  TBranch *hasTau_branch;
  bool     hasTau_isLoaded;
  int      nLightLep_;
  TBranch *nLightLep_branch;
  bool     nLightLep_isLoaded;
  int      firstgoodvertex_;
  TBranch *firstgoodvertex_branch;
  bool     firstgoodvertex_isLoaded;
  int      nvtx_;
  TBranch *nvtx_branch;
  bool     nvtx_isLoaded;
  int      nTrueInt_;
  TBranch *nTrueInt_branch;
  bool     nTrueInt_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *lep_p4_;
  TBranch *lep_p4_branch;
  bool     lep_p4_isLoaded;
  vector<float> *lep_pt_;
  TBranch *lep_pt_branch;
  bool     lep_pt_isLoaded;
  vector<float> *lep_eta_;
  TBranch *lep_eta_branch;
  bool     lep_eta_isLoaded;
  vector<float> *lep_etaSC_;
  TBranch *lep_etaSC_branch;
  bool     lep_etaSC_isLoaded;
  vector<float> *lep_phi_;
  TBranch *lep_phi_branch;
  bool     lep_phi_isLoaded;
  vector<float> *lep_energy_;
  TBranch *lep_energy_branch;
  bool     lep_energy_isLoaded;
  vector<float> *lep_mva_;
  TBranch *lep_mva_branch;
  bool     lep_mva_isLoaded;
  vector<float> *lep_mvaIso_;
  TBranch *lep_mvaIso_branch;
  bool     lep_mvaIso_isLoaded;
  vector<float> *lep_relIso04DB_;
  TBranch *lep_relIso04DB_branch;
  bool     lep_relIso04DB_isLoaded;
  vector<float> *lep_relIso03EA_;
  TBranch *lep_relIso03EA_branch;
  bool     lep_relIso03EA_isLoaded;
  vector<float> *lep_relIso03EAv4_;
  TBranch *lep_relIso03EAv4_branch;
  bool     lep_relIso03EAv4_isLoaded;
  vector<float> *lep_relIso03EAwLep_;
  TBranch *lep_relIso03EAwLep_branch;
  bool     lep_relIso03EAwLep_isLoaded;
  vector<float> *lep_ip3d_;
  TBranch *lep_ip3d_branch;
  bool     lep_ip3d_isLoaded;
  vector<float> *lep_sip3d_;
  TBranch *lep_sip3d_branch;
  bool     lep_sip3d_isLoaded;
  vector<float> *lep_dxy_;
  TBranch *lep_dxy_branch;
  bool     lep_dxy_isLoaded;
  vector<float> *lep_dz_;
  TBranch *lep_dz_branch;
  bool     lep_dz_isLoaded;
  vector<int> *lep_mc_motherid_;
  TBranch *lep_mc_motherid_branch;
  bool     lep_mc_motherid_isLoaded;
  vector<int> *lep_mc_id_;
  TBranch *lep_mc_id_branch;
  bool     lep_mc_id_isLoaded;
  vector<int> *lep_motherIdv2_;
  TBranch *lep_motherIdv2_branch;
  bool     lep_motherIdv2_isLoaded;
  vector<int> *lep_idx_;
  TBranch *lep_idx_branch;
  bool     lep_idx_isLoaded;
  vector<int> *lep_id_;
  TBranch *lep_id_branch;
  bool     lep_id_isLoaded;
  vector<int> *lep_isTightPOG_;
  TBranch *lep_isTightPOG_branch;
  bool     lep_isTightPOG_isLoaded;
  vector<int> *lep_isMediumPOG_;
  TBranch *lep_isMediumPOG_branch;
  bool     lep_isMediumPOG_isLoaded;
  vector<int> *lep_isMVAwp80NoIsoPOG_;
  TBranch *lep_isMVAwp80NoIsoPOG_branch;
  bool     lep_isMVAwp80NoIsoPOG_isLoaded;
  vector<int> *lep_isMVAwp90NoIsoPOG_;
  TBranch *lep_isMVAwp90NoIsoPOG_branch;
  bool     lep_isMVAwp90NoIsoPOG_isLoaded;
  vector<int> *lep_isMVAwpLooseNoIsoPOG_;
  TBranch *lep_isMVAwpLooseNoIsoPOG_branch;
  bool     lep_isMVAwpLooseNoIsoPOG_isLoaded;
  vector<int> *lep_isMVAwp80IsoPOG_;
  TBranch *lep_isMVAwp80IsoPOG_branch;
  bool     lep_isMVAwp80IsoPOG_isLoaded;
  vector<int> *lep_isMVAwp90IsoPOG_;
  TBranch *lep_isMVAwp90IsoPOG_branch;
  bool     lep_isMVAwp90IsoPOG_isLoaded;
  vector<int> *lep_isMVAwpLooseIsoPOG_;
  TBranch *lep_isMVAwpLooseIsoPOG_branch;
  bool     lep_isMVAwpLooseIsoPOG_isLoaded;
  vector<int> *lep_isMVAwpHZZIsoPOG_;
  TBranch *lep_isMVAwpHZZIsoPOG_branch;
  bool     lep_isMVAwpHZZIsoPOG_isLoaded;
  vector<int> *lep_isCutBasedNoIsoVetoPOG_;
  TBranch *lep_isCutBasedNoIsoVetoPOG_branch;
  bool     lep_isCutBasedNoIsoVetoPOG_isLoaded;
  vector<int> *lep_isCutBasedNoIsoLoosePOG_;
  TBranch *lep_isCutBasedNoIsoLoosePOG_branch;
  bool     lep_isCutBasedNoIsoLoosePOG_isLoaded;
  vector<int> *lep_isCutBasedNoIsoMediumPOG_;
  TBranch *lep_isCutBasedNoIsoMediumPOG_branch;
  bool     lep_isCutBasedNoIsoMediumPOG_isLoaded;
  vector<int> *lep_isCutBasedNoIsoTightPOG_;
  TBranch *lep_isCutBasedNoIsoTightPOG_branch;
  bool     lep_isCutBasedNoIsoTightPOG_isLoaded;
  vector<int> *lep_isCutBasedIsoVetoPOG_;
  TBranch *lep_isCutBasedIsoVetoPOG_branch;
  bool     lep_isCutBasedIsoVetoPOG_isLoaded;
  vector<int> *lep_isCutBasedIsoLoosePOG_;
  TBranch *lep_isCutBasedIsoLoosePOG_branch;
  bool     lep_isCutBasedIsoLoosePOG_isLoaded;
  vector<int> *lep_isCutBasedIsoMediumPOG_;
  TBranch *lep_isCutBasedIsoMediumPOG_branch;
  bool     lep_isCutBasedIsoMediumPOG_isLoaded;
  vector<int> *lep_isCutBasedIsoTightPOG_;
  TBranch *lep_isCutBasedIsoTightPOG_branch;
  bool     lep_isCutBasedIsoTightPOG_isLoaded;
  ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > *met_p4_;
  TBranch *met_p4_branch;
  bool     met_p4_isLoaded;
  float    met_pt_;
  TBranch *met_pt_branch;
  bool     met_pt_isLoaded;
  float    met_phi_;
  TBranch *met_phi_branch;
  bool     met_phi_isLoaded;
  float    met_up_pt_;
  TBranch *met_up_pt_branch;
  bool     met_up_pt_isLoaded;
  float    met_up_phi_;
  TBranch *met_up_phi_branch;
  bool     met_up_phi_isLoaded;
  float    met_dn_pt_;
  TBranch *met_dn_pt_branch;
  bool     met_dn_pt_isLoaded;
  float    met_dn_phi_;
  TBranch *met_dn_phi_branch;
  bool     met_dn_phi_isLoaded;
  float    met_gen_pt_;
  TBranch *met_gen_pt_branch;
  bool     met_gen_pt_isLoaded;
  float    met_gen_phi_;
  TBranch *met_gen_phi_branch;
  bool     met_gen_phi_isLoaded;
  int      passesMETfiltersRun2_;
  TBranch *passesMETfiltersRun2_branch;
  bool     passesMETfiltersRun2_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_p4_;
  TBranch *jets_p4_branch;
  bool     jets_p4_isLoaded;
  vector<float> *jets_pt_;
  TBranch *jets_pt_branch;
  bool     jets_pt_isLoaded;
  vector<float> *jets_eta_;
  TBranch *jets_eta_branch;
  bool     jets_eta_isLoaded;
  vector<float> *jets_phi_;
  TBranch *jets_phi_branch;
  bool     jets_phi_isLoaded;
  vector<float> *jets_mass_;
  TBranch *jets_mass_branch;
  bool     jets_mass_isLoaded;
  vector<float> *jets_btag_score_;
  TBranch *jets_btag_score_branch;
  bool     jets_btag_score_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *jets_cen_p4_;
  TBranch *jets_cen_p4_branch;
  bool     jets_cen_p4_isLoaded;
  vector<float> *jets_cen_pt_;
  TBranch *jets_cen_pt_branch;
  bool     jets_cen_pt_isLoaded;
  vector<float> *jets_cen_eta_;
  TBranch *jets_cen_eta_branch;
  bool     jets_cen_eta_isLoaded;
  vector<float> *jets_cen_phi_;
  TBranch *jets_cen_phi_branch;
  bool     jets_cen_phi_isLoaded;
  vector<float> *jets_cen_mass_;
  TBranch *jets_cen_mass_branch;
  bool     jets_cen_mass_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > *rawjets_p4_;
  TBranch *rawjets_p4_branch;
  bool     rawjets_p4_isLoaded;
  vector<float> *rawjets_pt_;
  TBranch *rawjets_pt_branch;
  bool     rawjets_pt_isLoaded;
  vector<float> *rawjets_eta_;
  TBranch *rawjets_eta_branch;
  bool     rawjets_eta_isLoaded;
  vector<float> *rawjets_phi_;
  TBranch *rawjets_phi_branch;
  bool     rawjets_phi_isLoaded;
  vector<float> *rawjets_mass_;
  TBranch *rawjets_mass_branch;
  bool     rawjets_mass_isLoaded;
  vector<float> *rawjets_JEC_;
  TBranch *rawjets_JEC_branch;
  bool     rawjets_JEC_isLoaded;
  vector<int> *rawjets_passJetID_;
  TBranch *rawjets_passJetID_branch;
  bool     rawjets_passJetID_isLoaded;
  vector<int> *rawjets_isORwLep_;
  TBranch *rawjets_isORwLep_branch;
  bool     rawjets_isORwLep_isLoaded;
  int      nj_;
  TBranch *nj_branch;
  bool     nj_isLoaded;
  int      nj_up_;
  TBranch *nj_up_branch;
  bool     nj_up_isLoaded;
  int      nj_dn_;
  TBranch *nj_dn_branch;
  bool     nj_dn_isLoaded;
  int      nb_;
  TBranch *nb_branch;
  bool     nb_isLoaded;
  int      nb_up_;
  TBranch *nb_up_branch;
  bool     nb_up_isLoaded;
  int      nb_dn_;
  TBranch *nb_dn_branch;
  bool     nb_dn_isLoaded;
  int      nbmed_;
  TBranch *nbmed_branch;
  bool     nbmed_isLoaded;
  float    ht_;
  TBranch *ht_branch;
  bool     ht_isLoaded;
  int      nj_cen_;
  TBranch *nj_cen_branch;
  bool     nj_cen_isLoaded;
  int      nj_cen_up_;
  TBranch *nj_cen_up_branch;
  bool     nj_cen_up_isLoaded;
  int      nj_cen_dn_;
  TBranch *nj_cen_dn_branch;
  bool     nj_cen_dn_isLoaded;
  float    weight_btagsf_;
  TBranch *weight_btagsf_branch;
  bool     weight_btagsf_isLoaded;
  float    weight_btagsf_heavy_DN_;
  TBranch *weight_btagsf_heavy_DN_branch;
  bool     weight_btagsf_heavy_DN_isLoaded;
  float    weight_btagsf_heavy_UP_;
  TBranch *weight_btagsf_heavy_UP_branch;
  bool     weight_btagsf_heavy_UP_isLoaded;
  float    weight_btagsf_light_DN_;
  TBranch *weight_btagsf_light_DN_branch;
  bool     weight_btagsf_light_DN_isLoaded;
  float    weight_btagsf_light_UP_;
  TBranch *weight_btagsf_light_UP_branch;
  bool     weight_btagsf_light_UP_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const int &isData();
  const float &evt_scale1fb();
  const float &genps_weight();
  const float &xsec_br();
  const int &evt_passgoodrunlist();
  const TString &CMS4path();
  const int &CMS4index();
  const float &weight_fr_r1_f1();
  const float &weight_fr_r1_f2();
  const float &weight_fr_r1_f0p5();
  const float &weight_fr_r2_f1();
  const float &weight_fr_r2_f2();
  const float &weight_fr_r2_f0p5();
  const float &weight_fr_r0p5_f1();
  const float &weight_fr_r0p5_f2();
  const float &weight_fr_r0p5_f0p5();
  const float &weight_pdf_up();
  const float &weight_pdf_down();
  const float &weight_alphas_down();
  const float &weight_alphas_up();
  const int &HLT_DoubleMu();
  const int &HLT_DoubleEl();
  const int &HLT_MuEG();
  const int &pass_duplicate_ee_em_mm();
  const int &pass_duplicate_mm_em_ee();
  const float &gen_ht();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_V_p4();
  const vector<float> &gen_V_pt();
  const vector<float> &gen_V_eta();
  const vector<float> &gen_V_phi();
  const vector<float> &gen_V_mass();
  const vector<int> &gen_V_id();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_lep_p4();
  const vector<float> &gen_lep_pt();
  const vector<float> &gen_lep_eta();
  const vector<float> &gen_lep_phi();
  const vector<float> &gen_lep_mass();
  const vector<int> &gen_lep_id();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_part_p4();
  const vector<float> &gen_part_pt();
  const vector<float> &gen_part_eta();
  const vector<float> &gen_part_phi();
  const vector<float> &gen_part_mass();
  const vector<int> &gen_part_id();
  const vector<int> &gen_part_mother_id();
  const vector<int> &gen_part_grandma_id();
  const vector<int> &gen_part_status();
  const int &VHchannel();
  const int &Higgschannel();
  const int &nGenTauClean();
  const int &nGenTau();
  const int &hasTau();
  const int &nLightLep();
  const int &firstgoodvertex();
  const int &nvtx();
  const int &nTrueInt();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &lep_p4();
  const vector<float> &lep_pt();
  const vector<float> &lep_eta();
  const vector<float> &lep_etaSC();
  const vector<float> &lep_phi();
  const vector<float> &lep_energy();
  const vector<float> &lep_mva();
  const vector<float> &lep_mvaIso();
  const vector<float> &lep_relIso04DB();
  const vector<float> &lep_relIso03EA();
  const vector<float> &lep_relIso03EAv4();
  const vector<float> &lep_relIso03EAwLep();
  const vector<float> &lep_ip3d();
  const vector<float> &lep_sip3d();
  const vector<float> &lep_dxy();
  const vector<float> &lep_dz();
  const vector<int> &lep_mc_motherid();
  const vector<int> &lep_mc_id();
  const vector<int> &lep_motherIdv2();
  const vector<int> &lep_idx();
  const vector<int> &lep_id();
  const vector<int> &lep_isTightPOG();
  const vector<int> &lep_isMediumPOG();
  const vector<int> &lep_isMVAwp80NoIsoPOG();
  const vector<int> &lep_isMVAwp90NoIsoPOG();
  const vector<int> &lep_isMVAwpLooseNoIsoPOG();
  const vector<int> &lep_isMVAwp80IsoPOG();
  const vector<int> &lep_isMVAwp90IsoPOG();
  const vector<int> &lep_isMVAwpLooseIsoPOG();
  const vector<int> &lep_isMVAwpHZZIsoPOG();
  const vector<int> &lep_isCutBasedNoIsoVetoPOG();
  const vector<int> &lep_isCutBasedNoIsoLoosePOG();
  const vector<int> &lep_isCutBasedNoIsoMediumPOG();
  const vector<int> &lep_isCutBasedNoIsoTightPOG();
  const vector<int> &lep_isCutBasedIsoVetoPOG();
  const vector<int> &lep_isCutBasedIsoLoosePOG();
  const vector<int> &lep_isCutBasedIsoMediumPOG();
  const vector<int> &lep_isCutBasedIsoTightPOG();
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &met_p4();
  const float &met_pt();
  const float &met_phi();
  const float &met_up_pt();
  const float &met_up_phi();
  const float &met_dn_pt();
  const float &met_dn_phi();
  const float &met_gen_pt();
  const float &met_gen_phi();
  const int &passesMETfiltersRun2();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_p4();
  const vector<float> &jets_pt();
  const vector<float> &jets_eta();
  const vector<float> &jets_phi();
  const vector<float> &jets_mass();
  const vector<float> &jets_btag_score();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_cen_p4();
  const vector<float> &jets_cen_pt();
  const vector<float> &jets_cen_eta();
  const vector<float> &jets_cen_phi();
  const vector<float> &jets_cen_mass();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &rawjets_p4();
  const vector<float> &rawjets_pt();
  const vector<float> &rawjets_eta();
  const vector<float> &rawjets_phi();
  const vector<float> &rawjets_mass();
  const vector<float> &rawjets_JEC();
  const vector<int> &rawjets_passJetID();
  const vector<int> &rawjets_isORwLep();
  const int &nj();
  const int &nj_up();
  const int &nj_dn();
  const int &nb();
  const int &nb_up();
  const int &nb_dn();
  const int &nbmed();
  const float &ht();
  const int &nj_cen();
  const int &nj_cen_up();
  const int &nj_cen_dn();
  const float &weight_btagsf();
  const float &weight_btagsf_heavy_DN();
  const float &weight_btagsf_heavy_UP();
  const float &weight_btagsf_light_DN();
  const float &weight_btagsf_light_UP();

  static void progress(int nEventsTotal, int nEventsChain);
};

#ifndef __CINT__
extern wvztree wvz;
#endif

namespace tas {
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const int &isData();
  const float &evt_scale1fb();
  const float &genps_weight();
  const float &xsec_br();
  const int &evt_passgoodrunlist();
  const TString &CMS4path();
  const int &CMS4index();
  const float &weight_fr_r1_f1();
  const float &weight_fr_r1_f2();
  const float &weight_fr_r1_f0p5();
  const float &weight_fr_r2_f1();
  const float &weight_fr_r2_f2();
  const float &weight_fr_r2_f0p5();
  const float &weight_fr_r0p5_f1();
  const float &weight_fr_r0p5_f2();
  const float &weight_fr_r0p5_f0p5();
  const float &weight_pdf_up();
  const float &weight_pdf_down();
  const float &weight_alphas_down();
  const float &weight_alphas_up();
  const int &HLT_DoubleMu();
  const int &HLT_DoubleEl();
  const int &HLT_MuEG();
  const int &pass_duplicate_ee_em_mm();
  const int &pass_duplicate_mm_em_ee();
  const float &gen_ht();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_V_p4();
  const vector<float> &gen_V_pt();
  const vector<float> &gen_V_eta();
  const vector<float> &gen_V_phi();
  const vector<float> &gen_V_mass();
  const vector<int> &gen_V_id();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_lep_p4();
  const vector<float> &gen_lep_pt();
  const vector<float> &gen_lep_eta();
  const vector<float> &gen_lep_phi();
  const vector<float> &gen_lep_mass();
  const vector<int> &gen_lep_id();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &gen_part_p4();
  const vector<float> &gen_part_pt();
  const vector<float> &gen_part_eta();
  const vector<float> &gen_part_phi();
  const vector<float> &gen_part_mass();
  const vector<int> &gen_part_id();
  const vector<int> &gen_part_mother_id();
  const vector<int> &gen_part_grandma_id();
  const vector<int> &gen_part_status();
  const int &VHchannel();
  const int &Higgschannel();
  const int &nGenTauClean();
  const int &nGenTau();
  const int &hasTau();
  const int &nLightLep();
  const int &firstgoodvertex();
  const int &nvtx();
  const int &nTrueInt();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &lep_p4();
  const vector<float> &lep_pt();
  const vector<float> &lep_eta();
  const vector<float> &lep_etaSC();
  const vector<float> &lep_phi();
  const vector<float> &lep_energy();
  const vector<float> &lep_mva();
  const vector<float> &lep_mvaIso();
  const vector<float> &lep_relIso04DB();
  const vector<float> &lep_relIso03EA();
  const vector<float> &lep_relIso03EAv4();
  const vector<float> &lep_relIso03EAwLep();
  const vector<float> &lep_ip3d();
  const vector<float> &lep_sip3d();
  const vector<float> &lep_dxy();
  const vector<float> &lep_dz();
  const vector<int> &lep_mc_motherid();
  const vector<int> &lep_mc_id();
  const vector<int> &lep_motherIdv2();
  const vector<int> &lep_idx();
  const vector<int> &lep_id();
  const vector<int> &lep_isTightPOG();
  const vector<int> &lep_isMediumPOG();
  const vector<int> &lep_isMVAwp80NoIsoPOG();
  const vector<int> &lep_isMVAwp90NoIsoPOG();
  const vector<int> &lep_isMVAwpLooseNoIsoPOG();
  const vector<int> &lep_isMVAwp80IsoPOG();
  const vector<int> &lep_isMVAwp90IsoPOG();
  const vector<int> &lep_isMVAwpLooseIsoPOG();
  const vector<int> &lep_isMVAwpHZZIsoPOG();
  const vector<int> &lep_isCutBasedNoIsoVetoPOG();
  const vector<int> &lep_isCutBasedNoIsoLoosePOG();
  const vector<int> &lep_isCutBasedNoIsoMediumPOG();
  const vector<int> &lep_isCutBasedNoIsoTightPOG();
  const vector<int> &lep_isCutBasedIsoVetoPOG();
  const vector<int> &lep_isCutBasedIsoLoosePOG();
  const vector<int> &lep_isCutBasedIsoMediumPOG();
  const vector<int> &lep_isCutBasedIsoTightPOG();
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &met_p4();
  const float &met_pt();
  const float &met_phi();
  const float &met_up_pt();
  const float &met_up_phi();
  const float &met_dn_pt();
  const float &met_dn_phi();
  const float &met_gen_pt();
  const float &met_gen_phi();
  const int &passesMETfiltersRun2();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_p4();
  const vector<float> &jets_pt();
  const vector<float> &jets_eta();
  const vector<float> &jets_phi();
  const vector<float> &jets_mass();
  const vector<float> &jets_btag_score();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &jets_cen_p4();
  const vector<float> &jets_cen_pt();
  const vector<float> &jets_cen_eta();
  const vector<float> &jets_cen_phi();
  const vector<float> &jets_cen_mass();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > > &rawjets_p4();
  const vector<float> &rawjets_pt();
  const vector<float> &rawjets_eta();
  const vector<float> &rawjets_phi();
  const vector<float> &rawjets_mass();
  const vector<float> &rawjets_JEC();
  const vector<int> &rawjets_passJetID();
  const vector<int> &rawjets_isORwLep();
  const int &nj();
  const int &nj_up();
  const int &nj_dn();
  const int &nb();
  const int &nb_up();
  const int &nb_dn();
  const int &nbmed();
  const float &ht();
  const int &nj_cen();
  const int &nj_cen_up();
  const int &nj_cen_dn();
  const float &weight_btagsf();
  const float &weight_btagsf_heavy_DN();
  const float &weight_btagsf_heavy_UP();
  const float &weight_btagsf_light_DN();
  const float &weight_btagsf_light_UP();
}
#endif
