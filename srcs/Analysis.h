#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include "TH1F.h"
#include <iostream>
#include <vector>
#include "makeHists.h"
#include <fstream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include "TH3F.h"
#include <TRandom3.h>
#include <TMinuit.h>
#include <TApplication.h>
#include "TEnv.h"
#include <TComplex.h>
#include "TH2D.h"

#include <tuple>

using namespace std;
//class: the main class for functions;
class Analysis
{
public:

    TTree      *fChain;

//*******variables for tree*******//
    //mc global


    Int_t           run;
    Int_t           lumi;
    ULong64_t       evt;
    Int_t           isData;
    Float_t         evt_scale1fb;
    Float_t         genps_weight;
    Float_t         xsec_br;
    Int_t           evt_passgoodrunlist;
    TString         *CMS4path;
    Int_t           CMS4index;
    Float_t         weight_fr_r1_f1;
    Float_t         weight_fr_r1_f2;
    Float_t         weight_fr_r1_f0p5;
    Float_t         weight_fr_r2_f1;
    Float_t         weight_fr_r2_f2;
    Float_t         weight_fr_r2_f0p5;
    Float_t         weight_fr_r0p5_f1;
    Float_t         weight_fr_r0p5_f2;
    Float_t         weight_fr_r0p5_f0p5;
    Float_t         weight_pdf_up;
    Float_t         weight_pdf_down;
    Float_t         weight_alphas_down;
    Float_t         weight_alphas_up;
    Int_t           HLT_DoubleMu;
    Int_t           HLT_DoubleEl;
    Int_t           HLT_MuEG;
    Int_t           pass_duplicate_ee_em_mm;
    Int_t           pass_duplicate_mm_em_ee;
    Float_t         gen_ht;
    Int_t           firstgoodvertex;
    Int_t           nvtx;
    Int_t           nTrueInt;
    vector<float>   *lep_pt = 0;
    vector<float>   *lep_eta = 0;
    vector<float>   *lep_phi = 0;
    vector<float>   *lep_energy = 0;
    vector<float>   *lep_mva = 0;
    vector<float>   *lep_relIso03EA = 0;
    vector<float>   *lep_relIso03EAwLep = 0;
    vector<float>   *lep_ip3d = 0;
    vector<float>   *lep_sip3d = 0;
    vector<float>   *lep_dxy = 0;
    vector<float>   *lep_dz = 0;
    vector<int>     *lep_mc_id = 0;
    vector<int>     *lep_motherIdv2 = 0;
    vector<int>     *lep_idx = 0;
    vector<int>     *lep_id = 0;
    vector<int>     *lep_isTightPOG = 0;
    vector<int>     *lep_isMediumPOG = 0;

    Float_t         met_pt;
    Float_t         met_phi;
    Float_t         met_up_pt;
    Float_t         met_up_phi;
    Float_t         met_dn_pt;
    Float_t         met_dn_phi;
    Float_t         met_gen_pt;
    Float_t         met_gen_phi;
    vector<float>   *jets_pt = 0;
    vector<float>   *jets_eta = 0;
    vector<float>   *jets_phi = 0;
    vector<float>   *jets_mass = 0;
    Int_t           nj;
    Int_t           nb;
    Int_t           nbmed;
    Float_t         ht;
    Float_t         weight_btagsf;
    Float_t         weight_btagsf_heavy_DN;
    Float_t         weight_btagsf_heavy_UP;
    Float_t         weight_btagsf_light_DN;
    Float_t         weight_btagsf_light_UP;


    Float_t         lumi_weight = 41.3;

    bool isRead;
    bool isDamaged;
    TLorentzVector leptons[10];
    int nVetoLeptons;
    int lep_ZCand_idx1;
    int lep_ZCand_idx2;
    int nNominalLeptons;
    int lep_Nom_idx1;
    int lep_Nom_idx2;
    int lep_Nom_idx3;
    TLorentzVector dilepZCand;
    TLorentzVector dilepNominal;
    TString output_tfile_name;

//*******functions********//
    Analysis(const char* ifileName, const char* TypeName, const char* RootName);
    virtual ~Analysis();
    virtual void  Initial(const char* RootName, int RootNumber);
    virtual void  Loop(const char* TypeName);
    virtual void  End(int RootNumber);
    virtual void  Finish(int RootNumber);
    virtual void  Output();

//*******plots*******//
    makeHists* myhists;

    void readLeptons();

    void selectVetoLeptons();
    void selectZCandLeptons();
    void selectNominalLeptons();
    void sortLeptonIndex();
    void setDilepMasses();

    float EventWeight();

    bool passZCandLeptonID(int idx);
    bool passZCandElectronID(int idx);
    bool passZCandMuonID(int idx);

    bool passNominalLeptonID(int idx);
    bool passNominalElectronID(int idx);
    bool passNominalMuonID(int idx);

    bool passVetoLeptonID(int idx);
    bool passVetoElectronID(int idx);
    bool passVetoMuonID(int idx);

    bool Is4LeptonEvent();
    bool Is5LeptonEvent();
    bool FindZCandLeptons();
    bool FindTwoOSNominalLeptons();

    bool Cut4LepLeptonPt();
    bool Cut4LepLowMll();
    bool Cut4LepBVeto();

    bool IsChannelEMu();
    bool IsChannelOnZ();
    bool IsChannelOffZ();

    bool ChannelEMuHighMll();

    float VarMll();

};
#endif

#ifdef Analysis_C
Analysis::Analysis(const char* ifileName, const char* TypeName, const char* RootName)
{

    // Create histograms
    myhists = new makeHists();

    output_tfile_name = (TString)TypeName + "_" + (TString)RootName +  "_results.root";

    // myhists->bookHists(output_tfile_name);

}

void Analysis::End(int RootNumber)
{

    //output: free the current file
    cout << "**Runing: Free Rootfile " << RootNumber << endl;
    if (!fChain)
    {
        //output: reading file failed
        cout << "XXXX**Runing: Big Error! No file load!" << endl;
        return;
    }
    delete fChain->GetCurrentFile();
}


Analysis::~Analysis()
{
}

void Analysis::Finish(int RootNumber)
{

    //save histogram
    cout << "**Runing: " << RootNumber << " rootfiles finished" << endl;
    // if (myhists)
    // {
    //     myhists->saveHists();
    //     delete myhists;
    // }
    cout << "**Runing: histogram saved" << endl;
}

void Analysis::Output()
{
    cout << endl;
    cout << endl;
}

void Analysis::Initial(const char* RootName, int RootNumber)
{
    //link to tree
    TTree *tree = NULL;
    TFile *file = (TFile *)gROOT->GetListOfFiles()->FindObject(RootName);
    if (!file)
        file = new TFile(RootName);
    tree = (TTree *)gDirectory->Get("t");
    if (tree == NULL)
        cout << "No Such Tree!!!" << endl;
    else  cout << "Tree succeeded taking out" << endl;
    fChain = tree;
    //output: got a new rootfile
    cout << "**Runing: Starting Rootfile " << RootNumber << endl;
    isRead = true;
    double filenumber;
    filenumber = fChain->GetEntries();
    if (filenumber == 0)
        isRead = false;
    //*****set address****//
    fChain->SetBranchAddress("run", &run);
    fChain->SetBranchAddress("lumi", &lumi);
    fChain->SetBranchAddress("evt", &evt);
    fChain->SetBranchAddress("isData", &isData);
    fChain->SetBranchAddress("evt_scale1fb", &evt_scale1fb);
    fChain->SetBranchAddress("genps_weight", &genps_weight);
    fChain->SetBranchAddress("xsec_br", &xsec_br);
    fChain->SetBranchAddress("CMS4path",&CMS4path);
    fChain->SetBranchAddress("CMS4index", &CMS4index);
    fChain->SetBranchAddress("weight_fr_r1_f1", &weight_fr_r1_f1);
    fChain->SetBranchAddress("weight_fr_r1_f2", &weight_fr_r1_f2);
    fChain->SetBranchAddress("weight_fr_r1_f0p5", &weight_fr_r1_f0p5);
    fChain->SetBranchAddress("weight_fr_r2_f1", &weight_fr_r2_f1);
    fChain->SetBranchAddress("weight_fr_r2_f2", &weight_fr_r2_f2);
    fChain->SetBranchAddress("weight_fr_r2_f0p5", &weight_fr_r2_f0p5);
    fChain->SetBranchAddress("weight_fr_r0p5_f1", &weight_fr_r0p5_f1);
    fChain->SetBranchAddress("weight_fr_r0p5_f2", &weight_fr_r0p5_f2);
    fChain->SetBranchAddress("weight_fr_r0p5_f0p5", &weight_fr_r0p5_f0p5);
    fChain->SetBranchAddress("weight_pdf_up", &weight_pdf_up);
    fChain->SetBranchAddress("weight_pdf_down", &weight_pdf_down);
    fChain->SetBranchAddress("weight_alphas_down", &weight_alphas_down);
    fChain->SetBranchAddress("weight_alphas_up", &weight_alphas_up);
    fChain->SetBranchAddress("HLT_DoubleMu", &HLT_DoubleMu);
    fChain->SetBranchAddress("HLT_DoubleEl", &HLT_DoubleEl);
    fChain->SetBranchAddress("HLT_MuEG", &HLT_MuEG);
    fChain->SetBranchAddress("pass_duplicate_ee_em_mm", &pass_duplicate_ee_em_mm);
    fChain->SetBranchAddress("pass_duplicate_mm_em_ee", &pass_duplicate_mm_em_ee);
    fChain->SetBranchAddress("gen_ht", &gen_ht);
    fChain->SetBranchAddress("firstgoodvertex", &firstgoodvertex);
    fChain->SetBranchAddress("nvtx", &nvtx);
    fChain->SetBranchAddress("nTrueInt", &nTrueInt);
    fChain->SetBranchAddress("lep_pt", &lep_pt);
    fChain->SetBranchAddress("lep_eta", &lep_eta);
    fChain->SetBranchAddress("lep_phi", &lep_phi);
    fChain->SetBranchAddress("lep_energy", &lep_energy);
    fChain->SetBranchAddress("lep_mva", &lep_mva);
    fChain->SetBranchAddress("lep_relIso03EA", &lep_relIso03EA);
    fChain->SetBranchAddress("lep_relIso03EAwLep", &lep_relIso03EAwLep);
    fChain->SetBranchAddress("lep_ip3d", &lep_ip3d);
    fChain->SetBranchAddress("lep_sip3d", &lep_sip3d);
    fChain->SetBranchAddress("lep_dxy", &lep_dxy);
    fChain->SetBranchAddress("lep_dz", &lep_dz);
    fChain->SetBranchAddress("lep_mc_id", &lep_mc_id);
    fChain->SetBranchAddress("lep_motherIdv2", &lep_motherIdv2);
    fChain->SetBranchAddress("lep_idx", &lep_idx);
    fChain->SetBranchAddress("lep_id", &lep_id);
    fChain->SetBranchAddress("lep_isTightPOG", &lep_isTightPOG);
    fChain->SetBranchAddress("lep_isMediumPOG", &lep_isMediumPOG);
    fChain->SetBranchAddress("met_pt", &met_pt);
    fChain->SetBranchAddress("met_phi", &met_phi);
    fChain->SetBranchAddress("met_up_pt", &met_up_pt);
    fChain->SetBranchAddress("met_up_phi", &met_up_phi);
    fChain->SetBranchAddress("met_dn_pt", &met_dn_pt);
    fChain->SetBranchAddress("met_dn_phi", &met_dn_phi);
    fChain->SetBranchAddress("met_gen_pt", &met_gen_pt);
    fChain->SetBranchAddress("met_gen_phi", &met_gen_phi);
    fChain->SetBranchAddress("jets_pt", &jets_pt);
    fChain->SetBranchAddress("jets_eta", &jets_eta);
    fChain->SetBranchAddress("jets_phi", &jets_phi);
    fChain->SetBranchAddress("jets_mass", &jets_mass);
    fChain->SetBranchAddress("nj", &nj);
    fChain->SetBranchAddress("nb", &nb);
    fChain->SetBranchAddress("nbmed", &nbmed);
    fChain->SetBranchAddress("ht", &ht);
    fChain->SetBranchAddress("weight_btagsf", &weight_btagsf);
    fChain->SetBranchAddress("weight_btagsf_heavy_DN", &weight_btagsf_heavy_DN);
    fChain->SetBranchAddress("weight_btagsf_heavy_UP", &weight_btagsf_heavy_UP);
    fChain->SetBranchAddress("weight_btagsf_light_DN", &weight_btagsf_light_DN);
    fChain->SetBranchAddress("weight_btagsf_light_UP", &weight_btagsf_light_UP);
}

#endif
