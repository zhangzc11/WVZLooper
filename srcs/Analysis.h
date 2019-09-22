#ifndef Analysis_h
#define Analysis_h

#include "TApplication.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TComplex.h"
#include "TEnv.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TH3F.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TSystem.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <unistd.h>
#include <vector>
#include "TVector3.h"

#include "makeHists.h"
#include "rooutil.h"
#include "wvztree.h"
#include "hzzangles.h"

// MET MC Correction
#include "METCorrectionHandler.h"

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
    bool doFakeEst;
    bool doSyst;
    bool doSkim;
    std::vector<LV> leptons;
    std::vector<int> lep_veto_idxs;
    std::vector<int> lep_tight_idxs;
    std::vector<int> lep_nom_idxs;
    std::vector<int> lep_notnom_idxs;
    std::vector<int> lep_fakeable_idxs;
    int nNotNomLeptons;
    int nTightLeptons;
    int nFakeableLeptons;
    int nVetoLeptons;
    int lep_Veto_idx1;
    int lep_Veto_idx2;
    int lep_Veto_idx3;
    int lep_Veto_idx4;
    int lep_ZCand_idx1;
    int lep_ZCand_idx2;
    int lep_Z2Cand_idx1;
    int lep_Z2Cand_idx2;
    int nNominalLeptons;
    int lep_Nom_idx1;
    int lep_Nom_idx2;
    int lep_Nom_idx3;
    int lep_2ndZCand_idx1;
    int lep_2ndZCand_idx2;
    int lep_WCand_idx1;
    int nDFOS;
    int lep_FakeCand_idx1;
    int lep_FakeCand_idx2;
    int lep_NonFakeCand_idx;
    int lep_VetoButNotNom_idx;
    int lep_Fakeable_idx;
    int lep_DYX_FakeCand_idx;
    int lep_DYX_DYCand_idx1;
    int lep_DYX_DYCand_idx2;
    int lep_Fakeable_MCmatched_idx;
    int lep_5Lep_Z1_idx1;
    int lep_5Lep_Z1_idx2;
    int lep_5Lep_Z2_idx1;
    int lep_5Lep_Z2_idx2;
    int lep_5Lep_W_idx;
    LV dilepZCand;
    LV dilepNominal;
    TString output_tfile_name;
    int year;

    // Scale factors
    RooUtil::HistMap* histmap_2016_elec_reco_highpt_sf;
    RooUtil::HistMap* histmap_2016_elec_reco_lowpt_sf;
    RooUtil::HistMap* histmap_2016_elec_medium_sf;
    RooUtil::HistMap* histmap_2016_elec_veto_sf;
    RooUtil::HistMap* histmap_2017_elec_reco_highpt_sf;
    RooUtil::HistMap* histmap_2017_elec_reco_lowpt_sf;
    RooUtil::HistMap* histmap_2017_elec_medium_sf;
    RooUtil::HistMap* histmap_2017_elec_veto_sf;
    RooUtil::HistMap* histmap_2018_elec_reco_sf;
    RooUtil::HistMap* histmap_2018_elec_medium_sf;
    RooUtil::HistMap* histmap_2018_elec_veto_sf;
    RooUtil::HistMap* histmap_2016_muon_BCDEF_id_sf;
    RooUtil::HistMap* histmap_2016_muon_BCDEF_id_lowpt_sf;
    RooUtil::HistMap* histmap_2016_muon_BCDEF_tightiso_sf;
    RooUtil::HistMap* histmap_2016_muon_BCDEF_looseiso_sf;
    RooUtil::HistMap* histmap_2016_muon_GH_id_sf;
    RooUtil::HistMap* histmap_2016_muon_GH_id_lowpt_sf;
    RooUtil::HistMap* histmap_2016_muon_GH_tightiso_sf;
    RooUtil::HistMap* histmap_2016_muon_GH_looseiso_sf;
    RooUtil::HistMap* histmap_2017_muon_id_sf;
    RooUtil::HistMap* histmap_2017_muon_id_lowpt_sf;
    RooUtil::HistMap* histmap_2017_muon_tightiso_sf;
    RooUtil::HistMap* histmap_2017_muon_looseiso_sf;
    RooUtil::HistMap* histmap_2018_muon_id_sf;
    RooUtil::HistMap* histmap_2018_muon_id_lowpt_sf;
    RooUtil::HistMap* histmap_2018_muon_tightiso_sf;
    RooUtil::HistMap* histmap_2018_muon_looseiso_sf;
    RooUtil::HistMap* histmap_2016_fake_rate_el;
    RooUtil::HistMap* histmap_2016_fake_rate_mu;
    RooUtil::HistMap* histmap_2017_fake_rate_el;
    RooUtil::HistMap* histmap_2017_fake_rate_mu;
    RooUtil::HistMap* histmap_2018_fake_rate_el;
    RooUtil::HistMap* histmap_2018_fake_rate_mu;

    // MET MC Correction for
    METCorrectionHandler metcorrector;
    METObject metobj;
    METObject metobj_corrected;

    // TVector3 objects are kept here in order to speed up. (why? cause ROOT IS STUPID)
    // TVector3 lep1, lep2, metv3, zeta, zetaunit, sum, sum_vis;
    LV lep1, lep2, metv3, zeta, zetaunit, sum, sum_vis;

    // Looper
    RooUtil::Looper<wvztree>* looper;
    TTree* t;
    TFile* BDTinputFile;
    RooUtil::TTreeX* tx;

//*******functions********//
    Analysis(const char* ifileName, const char* RootName);
    virtual ~Analysis();
    virtual void  Initial(const char* RootName, int RootNumber);
    virtual void  Loop(const char* NtupleVersion, const char* TagName, bool dosyst, bool doskim);
    virtual void  End(int RootNumber);
    virtual void  Finish(int RootNumber);
    virtual void  Output();

//*******plots*******//
    makeHists* myhists;

    void setDoSkim(bool=true);
    void createNewBranches();
    void fillSkimTree();

    void loadScaleFactors();

    void readLeptons();

    void selectVetoLeptons();
    void selectZCandLeptons();
    void selectNominalLeptons();
    void select2ndZCandLeptons();
    void selectTightLeptons();
    void selectFakeStudyLeptons();
    void select2ndZCandAndWCandLeptons();
    void selectVetoButNotNomLeptons();
    void selectFakeableLeptons();
    void select5LepLeptons();
    void sortLeptonIndex();
    void setDilepMasses();
    void correctMET();

    float EventWeight();
    float LeptonScaleFactor(int=0, int=0);
    float LeptonScaleFactorZZ4l();
    float LeptonScaleFactor5Lep();
    float LeptonScaleFactorv1();
    float IndividualLeptonScaleFactor(int, bool, int=0, int=0);
    float FakeFactor();
    float BTagSF();

    bool passZCandLeptonID(int idx);
    bool passZCandElectronID(int idx);
    bool passZCandMuonID(int idx);

    bool passNominalLeptonID(int idx);
    bool passNominalElectronID(int idx);
    bool passNominalMuonID(int idx);

    bool passVetoLeptonID(int idx);
    bool passVetoElectronID(int idx);
    bool passVetoMuonID(int idx);

    bool passFakeableLeptonID(int idx);
    bool passFakeableElectronID(int idx);
    bool passFakeableMuonID(int idx);

    bool CutGenFilter();
    bool Is3LeptonEvent();
    bool Is3LeptonTTZEvent();
    bool Is4LeptonEvent();
    bool Is4LeptonFakeValidationEvents();
    bool Is3LeptonFakeValidationEvents();
    bool Is5LeptonEvent();
    bool IsTwoOSLeptonEvent();
    bool FindZCandLeptons();
    bool FindTwoOSNominalLeptons();
    bool FindTwoOSZ2Leptons();
    bool FindOSOneNomOneVbntLeptons();
    bool FindOSOneNomOneNotNomLeptons();
    bool IsEMuPlusX();
    bool IsDYPlusX();

    bool Cut4LepLeptonPt(bool=false);
    bool CutZZ4LepLeptonPt();
    bool CutHLT();
    bool Cut4LepLowMll(bool=false);
    bool Cut4LepBVeto(int=0);
    bool Cut4LepBTag(int=0);
    bool CutHighMT(int=0);
    bool CutHighMET(int=0);
    bool CutHighMTAR(int=0);
    bool CutLowPtZeta(int=0);

    bool IsChannelEMu(bool=false);
    bool IsChannelOnZ(bool=false);
    bool IsChannelOffZ(bool=false);
    bool IsChannelOffZHighMll(bool=false);
    bool IsChannelOffZLowMll(bool=false);
    bool IsChannelHZZ4l();
    bool Is2ndOnZFiveLepton();
    bool Is5thNominal();
    bool IsNjetGeq2();

    bool CutHEMVeto();

    bool ChannelEMuHighMll(bool=false);
    bool ChannelOffZHighMET();

    float VarMll();
    float VarMjj();
    float VarMjjMinDR();
    float VarMET(int=0);
    float VarNvtx();
    float VarMll2ndZ();
    float VarMT(int,int=0);
    float VarMTll(int,int,int=0);
    float VarMT5th(int=0);
    float VarMTNom0(int=0);
    float VarMTNom1(int=0);
    float VarMTMax(int=0);
    float VarMTMin(int=0);
    float VarMTVetoButNotNom(int=0);
    float VarMTFakeable(int=0);
    float VarRelIso5th();
    float VarPt5th();
    float VarNjet();
    float VarNEENoiseJet();
    float VarNfwdjet();
    float VarNb();
    float VarNbmed();
    float VarMll2l();
    float VarNSFOS();
    float VarLepPt(int);
    float VarLepEta(int idx);
    float VarLepPhi(int idx);
    float VarPtll(int, int);
    float VarMll(int, int);
    float VarM4l(int, int, int, int);
    float VarHTLep(int, int, int, int);
    float VarHTLep5();
    float VarMETPhi(int=0);
    float VarTauTauDisc(int=0);
    float VarPtZetaDiff(int=0);
    float VarPtZeta(int=0);
    float VarPtZetaVis(int=0);
    float VarMinDRJetsToLep(int);

    LeptonVectors GetLeptonVectors();

};
#endif

#ifdef Analysis_C
Analysis::Analysis(const char* ifileName, const char* RootName)
{

    // Create histograms
    myhists = new makeHists();

    TString TypeName = TString(RootName).Contains("Run201") ? "DATA" : "MC";

    output_tfile_name = TypeName + "_" + (TString)RootName +  "_results.root";

    // Initializing
    looper = 0;

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
    // delete fTTree->GetCurrentFile();
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

//===========================================================================
// Needed for sorting leptons by pt
//===========================================================================
struct MyLepton
{
    int idx;
    double pt;
    MyLepton(int idx_, float pt_) { idx = idx_; pt = pt_; }
};

struct less_than_key
{
    inline bool operator() (const MyLepton& struct1, const MyLepton& struct2)
    {
        return (struct1.pt > struct2.pt);
    }
};

//_______________________________________________________________________________________________________
class TheoryWeight
{
    public:
        RooUtil::HistMap* histmap_neventsinfile;
        float nominal_;
        float pdfup_;
        float pdfdn_;
        float alsup_;
        float alsdn_;
        float qsqup_;
        float qsqdn_;
        TheoryWeight() : histmap_neventsinfile(0), nominal_(1), pdfup_(1), pdfdn_(1), alsup_(1), alsdn_(1), qsqup_(1), qsqdn_(1) {}
        void setFile(TString fname)
        {
            histmap_neventsinfile = new RooUtil::HistMap(fname + ":h_neventsinfile");
            nominal_ = histmap_neventsinfile->hist->GetBinContent(1+1);
            pdfup_   = histmap_neventsinfile->hist->GetBinContent(1+10);
            pdfdn_   = histmap_neventsinfile->hist->GetBinContent(1+11);
            alsup_   = histmap_neventsinfile->hist->GetBinContent(1+13);
            alsdn_   = histmap_neventsinfile->hist->GetBinContent(1+12);
            qsqup_   = histmap_neventsinfile->hist->GetBinContent(1+5);
            qsqdn_   = histmap_neventsinfile->hist->GetBinContent(1+9);
        }
        float& nominal() { return nominal_; }
        float& pdfup()   { return pdfup_  ; }
        float& pdfdn()   { return pdfdn_  ; }
        float& alsup()   { return alsup_  ; }
        float& alsdn()   { return alsdn_  ; }
        float& qsqup()   { return qsqup_  ; }
        float& qsqdn()   { return qsqdn_  ; }
};

extern TheoryWeight theoryweight;

using namespace std;
#endif
