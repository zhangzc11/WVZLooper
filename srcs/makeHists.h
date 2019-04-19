#ifndef _MAKE_HISTS_H_
#define _MAKE_HISTS_H_
#include <iostream>
class TH1D;
class TH2D;
class TH2F;
class TH3F;
class TFile;
class TProfile;
class TProfile2D;
class makeHists
{
 public:
  TFile *hf;
//*********Oiginal histograms***********//

    TH1D *plot_lepton_Mll[3];
    TH1D *plot_lepton_missingET[3];
    TH1D *plot_lepton_Pt[3][8];
    TH1D *plot_lepton_Eta[3][8];
    TH1D *plot_nj[3];
    TH1D *plot_ht[3];
    TH1D *plot_St[3];
    TH1D *plot_lepton_relIsoEA[3][8];
    TH1D *plot_lepton_sip3d[3][8];
    TH1D *plot_lepton_ip[3][8];
    TH1D *plot_lepton_dxy[3][8];
    TH1D *plot_lepton_dz[3][8];
    TH1D *plot_lepton_mva[3][8];
    TH1D *plot_Mjet_DeltaR[3];
    TH1D *plot_Mjet_LeadPt[3];

    TH1D *plot_dilep_good_Mll[6];
    TH1D *plot_dilep_good_Mll_em;
    TH1D *plot_dilep_good_Mll_onZ;
    TH1D *plot_dilep_good_Mll_offZ;
    TH1D *plot_dilep_fake_Mll[6];
    TH1D *plot_dilep_fake_Mll_em;
    TH1D *plot_dilep_fake_Mll_onZ;
    TH1D *plot_dilep_fake_Mll_offZ;
    TH1D *plot_dilep_rest_Mll[6];
    TH1D *plot_dilep_rest_Mll_em;
    TH1D *plot_dilep_rest_Mll_onZ;
    TH1D *plot_dilep_rest_Mll_offZ;

    TH1D *plot_dilep_good_relIso;
    TH1D *plot_dilep_fake_relIso;



//  TH1D *plot_zboson_mass_seed_muon[16];
//  TH1D *plot_zboson_mass_seed_antimuon[16];
//  TH1D *plot_zboson_mass_forward_muon[16];
//  TH1D *plot_zboson_mass_forward_antimuon[16];

//  TH1D *plot_zboson_mass;
//  TH1D *plot_zboson_mass_forward;
//  TH1D *plot_zboson_mass_backward;

//*************************************//
  void bookHists(const char* fName);
  void saveHists();

};

#endif
