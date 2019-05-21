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
#include "TSystem.h"

#include <tuple>
#include <unistd.h>

#include "wvztree.h"
#include "rooutil.h"

using namespace std;
//class: the main class for functions;
class Analysis
{
public:

    TTree      *fTTree;

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
    std::vector<int> lep_veto_idxs;
    int nVetoLeptons;
    int lep_ZCand_idx1;
    int lep_ZCand_idx2;
    int nNominalLeptons;
    int lep_Nom_idx1;
    int lep_Nom_idx2;
    int lep_Nom_idx3;
    int lep_2ndZCand_idx1;
    int lep_2ndZCand_idx2;
    int lep_WCand_idx1;
    TLorentzVector dilepZCand;
    TLorentzVector dilepNominal;
    TString output_tfile_name;
    int year;

    // Scale factors
    RooUtil::HistMap* histmap_purwegt;

//*******functions********//
    Analysis(const char* ifileName, const char* RootName);
    virtual ~Analysis();
    virtual void  Initial(const char* RootName, int RootNumber);
    virtual void  Loop(const char* NtupleVersion, const char* TagName);
    virtual void  End(int RootNumber);
    virtual void  Finish(int RootNumber);
    virtual void  Output();

//*******plots*******//
    makeHists* myhists;

    void readLeptons();

    void selectVetoLeptons();
    void selectZCandLeptons();
    void selectNominalLeptons();
    void select2ndZCandAndWCandLeptons();
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
    bool IsTwoOSLeptonEvent();
    bool FindZCandLeptons();
    bool FindTwoOSNominalLeptons();

    bool Cut4LepLeptonPt();
    bool CutHLT();
    bool Cut4LepLowMll();
    bool Cut4LepBVeto();

    bool IsChannelEMu();
    bool IsChannelOnZ();
    bool IsChannelOffZ();
    bool Is2ndOnZFiveLepton();
    bool Is5thNominal();
    bool IsNjetGeq2();

    bool ChannelEMuHighMll();
    bool ChannelOffZHighMET();

    float VarMll();
    float VarMET();
    float VarNvtx();
    float VarMll2ndZ();
    float VarMT5th();
    float VarRelIso5th();
    float VarPt5th();
    float VarNjet();
    float VarMll2l();

};
#endif

#ifdef Analysis_C
Analysis::Analysis(const char* ifileName, const char* RootName)
{

    // Create histograms
    myhists = new makeHists();

    TString TypeName = TString(RootName).Contains("Run201") ? "DATA" : "MC";

    output_tfile_name = TypeName + "_" + (TString)RootName +  "_results.root";

    // myhists->bookHists(output_tfile_name);

}

void Analysis::End(int RootNumber)
{

    //output: free the current file
    cout << "**Runing: Free Rootfile " << RootNumber << endl;
    if (!fTTree)
    {
        //output: reading file failed
        cout << "XXXX**Runing: Big Error! No file load!" << endl;
        return;
    }
    delete fTTree->GetCurrentFile();
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
    fTTree = tree;
    //output: got a new rootfile
    cout << "**Runing: Starting Rootfile " << RootNumber << endl;
    isRead = true;
    // double filenumber;
    // filenumber = fTTree->GetEntries();
    // if (filenumber == 0)
    //     isRead = false;
    //*****set address****//
    CMS4path = 0;
    fTTree->SetBranchAddress("run", &run);
    fTTree->SetBranchAddress("lumi", &lumi);
    fTTree->SetBranchAddress("evt", &evt);
    fTTree->SetBranchAddress("isData", &isData);
    fTTree->SetBranchAddress("evt_scale1fb", &evt_scale1fb);
    fTTree->SetBranchAddress("genps_weight", &genps_weight);
    fTTree->SetBranchAddress("xsec_br", &xsec_br);
    fTTree->SetBranchAddress("CMS4path",&CMS4path);
    fTTree->SetBranchAddress("CMS4index", &CMS4index);
    fTTree->SetBranchAddress("weight_fr_r1_f1", &weight_fr_r1_f1);
    fTTree->SetBranchAddress("weight_fr_r1_f2", &weight_fr_r1_f2);
    fTTree->SetBranchAddress("weight_fr_r1_f0p5", &weight_fr_r1_f0p5);
    fTTree->SetBranchAddress("weight_fr_r2_f1", &weight_fr_r2_f1);
    fTTree->SetBranchAddress("weight_fr_r2_f2", &weight_fr_r2_f2);
    fTTree->SetBranchAddress("weight_fr_r2_f0p5", &weight_fr_r2_f0p5);
    fTTree->SetBranchAddress("weight_fr_r0p5_f1", &weight_fr_r0p5_f1);
    fTTree->SetBranchAddress("weight_fr_r0p5_f2", &weight_fr_r0p5_f2);
    fTTree->SetBranchAddress("weight_fr_r0p5_f0p5", &weight_fr_r0p5_f0p5);
    fTTree->SetBranchAddress("weight_pdf_up", &weight_pdf_up);
    fTTree->SetBranchAddress("weight_pdf_down", &weight_pdf_down);
    fTTree->SetBranchAddress("weight_alphas_down", &weight_alphas_down);
    fTTree->SetBranchAddress("weight_alphas_up", &weight_alphas_up);
    fTTree->SetBranchAddress("HLT_DoubleMu", &HLT_DoubleMu);
    fTTree->SetBranchAddress("HLT_DoubleEl", &HLT_DoubleEl);
    fTTree->SetBranchAddress("HLT_MuEG", &HLT_MuEG);
    fTTree->SetBranchAddress("pass_duplicate_ee_em_mm", &pass_duplicate_ee_em_mm);
    fTTree->SetBranchAddress("pass_duplicate_mm_em_ee", &pass_duplicate_mm_em_ee);
    fTTree->SetBranchAddress("gen_ht", &gen_ht);
    fTTree->SetBranchAddress("firstgoodvertex", &firstgoodvertex);
    fTTree->SetBranchAddress("nvtx", &nvtx);
    fTTree->SetBranchAddress("nTrueInt", &nTrueInt);
    fTTree->SetBranchAddress("lep_pt", &lep_pt);
    fTTree->SetBranchAddress("lep_eta", &lep_eta);
    fTTree->SetBranchAddress("lep_phi", &lep_phi);
    fTTree->SetBranchAddress("lep_energy", &lep_energy);
    fTTree->SetBranchAddress("lep_mva", &lep_mva);
    fTTree->SetBranchAddress("lep_relIso03EA", &lep_relIso03EA);
    fTTree->SetBranchAddress("lep_relIso03EAwLep", &lep_relIso03EAwLep);
    fTTree->SetBranchAddress("lep_ip3d", &lep_ip3d);
    fTTree->SetBranchAddress("lep_sip3d", &lep_sip3d);
    fTTree->SetBranchAddress("lep_dxy", &lep_dxy);
    fTTree->SetBranchAddress("lep_dz", &lep_dz);
    fTTree->SetBranchAddress("lep_mc_id", &lep_mc_id);
    fTTree->SetBranchAddress("lep_motherIdv2", &lep_motherIdv2);
    fTTree->SetBranchAddress("lep_idx", &lep_idx);
    fTTree->SetBranchAddress("lep_id", &lep_id);
    fTTree->SetBranchAddress("lep_isTightPOG", &lep_isTightPOG);
    fTTree->SetBranchAddress("lep_isMediumPOG", &lep_isMediumPOG);
    fTTree->SetBranchAddress("met_pt", &met_pt);
    fTTree->SetBranchAddress("met_phi", &met_phi);
    fTTree->SetBranchAddress("met_up_pt", &met_up_pt);
    fTTree->SetBranchAddress("met_up_phi", &met_up_phi);
    fTTree->SetBranchAddress("met_dn_pt", &met_dn_pt);
    fTTree->SetBranchAddress("met_dn_phi", &met_dn_phi);
    fTTree->SetBranchAddress("met_gen_pt", &met_gen_pt);
    fTTree->SetBranchAddress("met_gen_phi", &met_gen_phi);
    fTTree->SetBranchAddress("jets_pt", &jets_pt);
    fTTree->SetBranchAddress("jets_eta", &jets_eta);
    fTTree->SetBranchAddress("jets_phi", &jets_phi);
    fTTree->SetBranchAddress("jets_mass", &jets_mass);
    fTTree->SetBranchAddress("nj", &nj);
    fTTree->SetBranchAddress("nb", &nb);
    fTTree->SetBranchAddress("nbmed", &nbmed);
    fTTree->SetBranchAddress("ht", &ht);
    fTTree->SetBranchAddress("weight_btagsf", &weight_btagsf);
    fTTree->SetBranchAddress("weight_btagsf_heavy_DN", &weight_btagsf_heavy_DN);
    fTTree->SetBranchAddress("weight_btagsf_heavy_UP", &weight_btagsf_heavy_UP);
    fTTree->SetBranchAddress("weight_btagsf_light_DN", &weight_btagsf_light_DN);
    fTTree->SetBranchAddress("weight_btagsf_light_UP", &weight_btagsf_light_UP);
}

#endif
