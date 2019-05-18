#define Analysis_C
#include "Analysis.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include "TH1D.h"
#include <iostream>
#include <vector>
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
#include <TGraph.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <iomanip>

#include "rooutil.h"

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


using namespace std;

//______________________________________________________________________________________________
void Analysis::Loop(const char* TypeName)
{

    // Sanity checks
    if (isRead == false)
    {
        cout << "this file does not content any events, skip to the next" << endl;
        return;
    }

    // Exit if no fTTree (sanity check)
    if (fTTree == 0)
        return;

    //
    // To set whether this ntuple is data or MC
    //
    TString InputName = TString(TypeName);
    bool isData = false ;
    bool isMC = false;
    if (InputName == "data")
    {
        isData = true;
    }
    else if (InputName == "MC")
    {
        isMC = true;
    }
    else
    {
        cout << "input file type wrong! please input 'data' or 'MC'!" << endl;
    }

    TFile* output_file = new TFile("outputs/" + output_tfile_name, "RECREATE");
    RooUtil::Cutflow cutflow(output_file);
    cutflow.addCut("Weight", [&](){ return 1; }, [&](){ return this->EventWeight(); } ); 
    cutflow.addCutToLastActiveCut("FourLeptons", [&](){ return this->Is4LeptonEvent(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("FindZCandLeptons", [&](){ return this->FindZCandLeptons(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("FindTwoOSNominalLeptons", [&](){ return this->FindTwoOSNominalLeptons(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("Cut4LepLeptonPt", [&](){ return this->Cut4LepLeptonPt(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("Cut4LepLowMll", [&](){ return this->Cut4LepLowMll(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("Cut4LepBVeto", [&](){ return this->Cut4LepBVeto(); }, UNITY ); 

    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelEMu", [&](){ return this->IsChannelEMu(); }, UNITY );
    cutflow.addCutToLastActiveCut("ChannelEMuHighMll", [&](){ return this->ChannelEMuHighMll(); }, UNITY );
    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelOnZ", [&](){ return this->IsChannelOnZ(); }, UNITY );
    cutflow.addCutToLastActiveCut("ChannelOnZNjet", [&](){ return this->IsNjetGeq2(); }, UNITY );
    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelOffZ", [&](){ return this->IsChannelOffZ(); }, UNITY );
    cutflow.addCutToLastActiveCut("ChannelOffZHighMET", [&](){ return this->ChannelOffZHighMET(); }, UNITY );

    cutflow.getCut("Weight");
    cutflow.addCutToLastActiveCut("FiveLeptons", [&](){ return this->Is5LeptonEvent(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("FiveLeptonsMllZ", [&](){ return this->Is2ndOnZFiveLepton(); }, UNITY ); 
    cutflow.addCutToLastActiveCut("FiveLeptonsRelIso5th", [&](){ return this->Is5thNominal(); }, UNITY ); 

    cutflow.getCut("Weight");
    cutflow.addCutToLastActiveCut("TwoOSLeptons", [&](){ return this->IsTwoOSLeptonEvent(); }, UNITY ); 

    cutflow.bookCutflows();

    RooUtil::Histograms histograms;
    histograms.addHistogram("Mll", 180, 0, 300, [&](){ return this->VarMll(); });
    histograms.addHistogram("MET", 180, 0, 300, [&](){ return this->VarMET(); });
    histograms.addHistogram("Mll2ndZ", 180, 0, 300, [&](){ return this->VarMll2ndZ(); });
    histograms.addHistogram("MT5th", 180, 0, 300, [&](){ return this->VarMT5th(); });
    histograms.addHistogram("RelIso5th", 180, 0, 0.4, [&](){ return this->VarRelIso5th(); });
    histograms.addHistogram("Pt5th", 180, 0, 200, [&](){ return this->VarPt5th(); });
    histograms.addHistogram("Njet", 4, 0, 4, [&](){ return this->VarNjet(); });

    cutflow.bookHistograms(histograms);

    // Event Loop (<3 of the code)
    Int_t Nentries = fTTree->GetEntries();
    cout << "there are " << Nentries << " events in Loop" << endl;

    // Looper class to facilitate various things
    TChain* ch = new TChain("t");
    ch->Add(fTTree->GetCurrentFile()->GetName());
    RooUtil::Looper<wvztree> looper(ch, &wvz, -1); // -1 means process 
    while (looper.nextEvent())
    {
        // Once it enters loop it's 1, and then 2, and so on.
        // So need to subtract one and set it to 'ii' so fTTree can load event
        int ii = looper.getNEventsProcessed() - 1; 
        // Load the entry
        fTTree->GetEntry(ii, 0);
        readLeptons();
        selectVetoLeptons();
        selectZCandLeptons();
        selectNominalLeptons();
        select2ndZCandAndWCandLeptons();
        sortLeptonIndex();
        setDilepMasses();
        cutflow.fill();
    }

    // Old way of looping
    // ----------------------------------------------------------------------------------------
    // for (int ii = 0 ; ii < Nentries ; ii++)
    // {
    //     if (isatty(1)) // i.e. when it is running interactively, otherwise no need to let user know what's going on
    //     {
    //         if (ii % 10000 == 0)
    //             printf("\015\033[32m ---> \033[1m\033[31m Processed %d / %d events [%4.1f%%] \033[34m \033[0m\033[32m  <---\033[0m\015 ", ii, Nentries, (float) ii / (float) Nentries);
    //     }
    //     // Load the entry
    //     fTTree->GetEntry(ii, 0);
    //     readLeptons();
    //     selectVetoLeptons();
    //     selectZCandLeptons();
    //     selectNominalLeptons();
    //     select2ndZCandAndWCandLeptons();
    //     sortLeptonIndex();
    //     setDilepMasses();
    //     cutflow.fill();
    // }//end of loop
    // ----------------------------------------------------------------------------------------

    cutflow.saveOutput();


}//end of whole function

//______________________________________________________________________________________________
void Analysis::readLeptons()
{
    for (int jj = 0 ; jj < lep_pt->size(); jj++)
        leptons[jj].SetPtEtaPhiE(((*lep_pt).at(jj)), ((*lep_eta).at(jj)), ((*lep_phi).at(jj)), ((*lep_energy).at(jj)));
}

// Object selection algorithms
//______________________________________________________________________________________________
void Analysis::selectVetoLeptons()
{
    nVetoLeptons = 0;
    lep_veto_idxs.clear();
    for (int kk = 0 ; kk < lep_pt->size() ; kk++)
    {
        if (not passVetoLeptonID(kk))
            continue;
        nVetoLeptons++;
        lep_veto_idxs.push_back(kk);
    }
}

//______________________________________________________________________________________________
void Analysis::selectZCandLeptons()
{
    //================
    // Tagging Z boson
    //================
    // Find the first two particles decay from a Z boson
    lep_ZCand_idx1 = -999;
    lep_ZCand_idx2 = -999;
    bool ifpass; // For tagging Z boson only
    double compare = 10; // Min value of |Mll - MZ|

    // Loop over the leptons and find the Z boson pair
    for (int jj = 0 ; jj < (lep_pt->size() - 1) ; jj ++)
    {

        if (not passZCandLeptonID(jj))
            continue;

        for (int kk = jj + 1 ; kk < lep_pt->size() ; kk++)
        {

            if (not passZCandLeptonID(kk))
                continue;

            dilepZCand = leptons[jj] + leptons[kk];
            int id1 = (*lep_id).at(jj);
            int id2 = (*lep_id).at(kk);
            ifpass = true;
            if (fabs(dilepZCand.M() - 91.1875) > 10)  ifpass = false; // within Z mass window
            if (abs(id1) != abs(id2)) ifpass = false; // same-flavor
            if (id1 == id2)  ifpass = false; // opposite-sign
            if (ifpass && fabs(dilepZCand.M() - 91.1875) > compare) ifpass = false; // if bigger than previous
            if (ifpass)
            {
                compare = fabs(dilepZCand.M() - 91.1876);
                lep_ZCand_idx1 = jj;
                lep_ZCand_idx2 = kk;
            }
        }
    }
}

//______________________________________________________________________________________________
void Analysis::selectNominalLeptons()
{

    // The nominal leptons are the leptons that are not tagged as Z

    // reset
    nNominalLeptons = -999;
    lep_Nom_idx1 = -999;
    lep_Nom_idx2 = -999;
    lep_Nom_idx3 = -999;

    std::vector<int> good_idx; // index of good nominal leptons

    // Loop over the leptons
    for (int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == lep_ZCand_idx1)
            continue;

        if (jj == lep_ZCand_idx2)
            continue;

        if (not passNominalLeptonID(jj))
            continue;

        good_idx.push_back(jj);

    }

    nNominalLeptons = good_idx.size(); // # of good nominal leptons
    if (nNominalLeptons > 0) lep_Nom_idx1 = good_idx[0];
    if (nNominalLeptons > 1) lep_Nom_idx2 = good_idx[1];
    if (nNominalLeptons > 2) lep_Nom_idx3 = good_idx[2];

}

//______________________________________________________________________________________________
void Analysis::select2ndZCandAndWCandLeptons()
{
    // These are the variables to set
    lep_2ndZCand_idx1 = -999;
    lep_2ndZCand_idx2 = -999;
    lep_WCand_idx1 = -999;

    std::vector<int> good_veto_idx; // index of good nominal leptons

    // Loop over the leptons
    for (int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == lep_ZCand_idx1)
            continue;

        if (jj == lep_ZCand_idx2)
            continue;

        if (not passVetoLeptonID(jj))
            continue;

        good_veto_idx.push_back(jj);

    }

    // At this point we must only find 5 total
    if (good_veto_idx.size() != 3)
        return;

    int lep_veto_idx1 = good_veto_idx[0];
    int lep_veto_idx2 = good_veto_idx[1];
    int lep_veto_idx3 = good_veto_idx[2];

    // Assumes lep_Nom_idx1,2,3 are set
    double dM12 = fabs((leptons[lep_veto_idx1] + leptons[lep_veto_idx2]).M() - 91.1876);
    double dM13 = fabs((leptons[lep_veto_idx1] + leptons[lep_veto_idx3]).M() - 91.1876);
    double dM23 = fabs((leptons[lep_veto_idx2] + leptons[lep_veto_idx3]).M() - 91.1876);
    double mindM = std::min(dM12, std::min(dM13, dM23));
    if (mindM == dM12)
    {
        lep_2ndZCand_idx1 = lep_pt->at(lep_veto_idx1) > lep_pt->at(lep_veto_idx2) ? lep_veto_idx1 : lep_veto_idx2;
        lep_2ndZCand_idx2 = lep_pt->at(lep_veto_idx1) > lep_pt->at(lep_veto_idx2) ? lep_veto_idx2 : lep_veto_idx1;
        lep_WCand_idx1 = lep_veto_idx3;
    }
    else if (mindM == dM13)
    {
        lep_2ndZCand_idx1 = lep_pt->at(lep_veto_idx1) > lep_pt->at(lep_veto_idx3) ? lep_veto_idx1 : lep_veto_idx3;
        lep_2ndZCand_idx2 = lep_pt->at(lep_veto_idx1) > lep_pt->at(lep_veto_idx3) ? lep_veto_idx3 : lep_veto_idx1;
        lep_WCand_idx1 = lep_veto_idx2;
    }
    else if (mindM == dM23)
    {
        lep_2ndZCand_idx1 = lep_pt->at(lep_veto_idx2) > lep_pt->at(lep_veto_idx3) ? lep_veto_idx2 : lep_veto_idx3;
        lep_2ndZCand_idx2 = lep_pt->at(lep_veto_idx2) > lep_pt->at(lep_veto_idx3) ? lep_veto_idx3 : lep_veto_idx2;
        lep_WCand_idx1 = lep_veto_idx1;
    }
    else
    {
        RooUtil::error("I should not be here!", "select2ndZCandAndWCandLeptons");
    }
}

//______________________________________________________________________________________________
void Analysis::sortLeptonIndex()
{

    // Sort Nominal leptons
    if (nNominalLeptons == 2)
    {
        int tmp1 = lep_Nom_idx1;
        int tmp2 = lep_Nom_idx2;
        lep_Nom_idx1 = leptons[lep_Nom_idx1].Pt() > leptons[lep_Nom_idx2].Pt() ? tmp1 : tmp2;
        lep_Nom_idx2 = leptons[lep_Nom_idx1].Pt() > leptons[lep_Nom_idx2].Pt() ? tmp2 : tmp1;
    }
    else if (nNominalLeptons == 3)
    {
        std::vector<MyLepton> lepidx;
        lepidx.push_back(MyLepton(lep_Nom_idx1, leptons[lep_Nom_idx1].Pt()));
        lepidx.push_back(MyLepton(lep_Nom_idx2, leptons[lep_Nom_idx2].Pt()));
        lepidx.push_back(MyLepton(lep_Nom_idx3, leptons[lep_Nom_idx3].Pt()));
        std::sort(lepidx.begin(), lepidx.end(), less_than_key());

        lep_Nom_idx1 = lepidx[0].idx;
        lep_Nom_idx2 = lepidx[1].idx;
        lep_Nom_idx3 = lepidx[2].idx;

    }

    // Sort Z lepton indices
    int tmp1 = lep_ZCand_idx1;
    int tmp2 = lep_ZCand_idx2;
    lep_ZCand_idx1 = leptons[lep_ZCand_idx1].Pt() > leptons[lep_ZCand_idx2].Pt() ? lep_ZCand_idx1 : lep_ZCand_idx2;
    lep_ZCand_idx2 = leptons[lep_ZCand_idx1].Pt() > leptons[lep_ZCand_idx2].Pt() ? lep_ZCand_idx2 : lep_ZCand_idx1;

}

//______________________________________________________________________________________________
void Analysis::setDilepMasses()
{
    dilepZCand = leptons[lep_ZCand_idx1] + leptons[lep_ZCand_idx2];
    dilepNominal = leptons[lep_Nom_idx1] + leptons[lep_Nom_idx2];
}

//
//
// Lepton ID functions
//
//

//______________________________________________________________________________________________
bool Analysis::passVetoLeptonID(int idx)
{
    if (abs(lep_id->at(idx)) == 11)
        return passVetoElectronID(idx);
    else /*if (abs(lep_id->at(idx)) == 13)*/
        return passVetoMuonID(idx);
}

//______________________________________________________________________________________________
bool Analysis::passVetoElectronID(int idx)
{

    // - POG MVA Veto (i.e. HZZ w.p.)
    // Already applied at WVZBabyMaker

    // - |eta| < 2.5
    if (not (fabs(lep_eta->at(idx)) < 2.5)) return false;

    // - dz < 0.1
    if (not (fabs(lep_dz->at(idx)) < 0.1)) return false;

    // - dxy < 0.05
    if (not (fabs(lep_dxy->at(idx)) < 0.05)) return false;

    // - RelIso03EA < 0.2 if tagged as Z
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.4)) return false;

    // - |sip3d| < 5
    if (not (fabs(lep_sip3d->at(idx)) < 5)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passVetoMuonID(int idx)
{
    // - POG Loose
    // Already applied at WVZBabyMaker

    // - |eta| < 2.4
    if (not (fabs(lep_eta->at(idx)) < 2.4)) return false;

    // - dz < 0.1
    if (not (fabs(lep_dz->at(idx)) < 0.1)) return false;

    // - dxy < 0.05
    if (not (fabs(lep_dxy->at(idx)) < 0.05)) return false;

    // - RelIso03EA < 0.2 if tagged as Z
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.4)) return false;

    // - |sip3d| < 5
    if (not (fabs(lep_sip3d->at(idx)) < 5)) return false;

    return true;
}

//______________________________________________________________________________________________
bool Analysis::passZCandLeptonID(int idx)
{
    if (abs(lep_id->at(idx)) == 11)
        return passZCandElectronID(idx);
    else /*if (abs(lep_id->at(idx)) == 13)*/
        return passZCandMuonID(idx);
}

//______________________________________________________________________________________________
bool Analysis::passZCandElectronID(int idx)
{

    // Must pass ZCand ID
    if (not (passVetoElectronID(idx))) return false;

    // - RelIso03EA < 0.2 if ZCandElectron
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.2)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passZCandMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passVetoMuonID(idx))) return false;

    // - RelIso03EA < 0.2 if ZCandMuon
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.2)) return false;

    return true;
}

//______________________________________________________________________________________________
bool Analysis::passNominalLeptonID(int idx)
{
    if (abs(lep_id->at(idx)) == 11)
        return passNominalElectronID(idx);
    else /*if (abs(lep_id->at(idx)) == 13)*/
        return passNominalMuonID(idx);
}

//______________________________________________________________________________________________
bool Analysis::passNominalElectronID(int idx)
{
    // Must pass ZCand ID
    if (not (passZCandElectronID(idx))) return false;

    // - RelIso03EA < 0.1 if not tagged as Z
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.1)) return false;

    return true;
}

//______________________________________________________________________________________________
bool Analysis::passNominalMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passZCandMuonID(idx))) return false;

    // - RelIso03EA < 0.1 if not tagged as Z
    if (not (fabs(lep_relIso03EA->at(idx)) < 0.1)) return false;

    return true;
}

//______________________________________________________________________________________________
float Analysis::EventWeight()
{
    return evt_scale1fb * 137;
}

//______________________________________________________________________________________________
bool Analysis::FindZCandLeptons()
{
    if (lep_ZCand_idx1 == -999)
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::FindTwoOSNominalLeptons()
{
    if (lep_Nom_idx1 == -999)
        return false;
    if (lep_Nom_idx2 == -999)
        return false;
    if (nNominalLeptons != 2)
        return false;
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(lep_Nom_idx2) > 0)
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepLowMll()
{
    if (nNominalLeptons != 2 or lep_ZCand_idx1 == -999)
        return false;

    std::vector<int> indices;
    indices.push_back(lep_Nom_idx1);
    indices.push_back(lep_Nom_idx2);
    indices.push_back(lep_ZCand_idx1);
    indices.push_back(lep_ZCand_idx2);

    bool pass = true;
    for (unsigned int ii = 0; ii < indices.size() - 1; ++ii)
    {
        for (unsigned int jj = ii + 1; jj < indices.size(); ++jj)
        {
            int id1 = lep_id->at(indices.at(ii));
            int id2 = lep_id->at(indices.at(jj));

            if (id1 * id2 < 0)
            {
                if ((leptons[indices.at(ii)] + leptons[indices.at(jj)]).M() < 12.)
                    pass = false;
            }
        }
    }

    return pass;

}

//______________________________________________________________________________________________
bool Analysis::Cut4LepBVeto()
{
    return nb == 0;
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepLeptonPt()
{
    if (nNominalLeptons != 2 or lep_ZCand_idx1 == -999)
        return false;

    float lepZpt1 = leptons[lep_ZCand_idx1].Pt();
    float lepZpt2 = leptons[lep_ZCand_idx2].Pt();
    float lepNpt1 = leptons[lep_Nom_idx1].Pt();
    float lepNpt2 = leptons[lep_Nom_idx2].Pt();

    if (lepZpt1 > 25. and lepZpt2 > 10. and lepNpt1 > 25. and lepNpt2 > 10.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::Is4LeptonEvent()
{
    return nVetoLeptons == 4;
}

//______________________________________________________________________________________________
bool Analysis::Is5LeptonEvent()
{
    return nVetoLeptons == 5;
}

//______________________________________________________________________________________________
bool Analysis::IsTwoOSLeptonEvent()
{
    if (not (nVetoLeptons == 2)) return false;
    if (not (leptons[lep_veto_idxs[0]].Pt() > 25.)) return false;
    if (not (leptons[lep_veto_idxs[1]].Pt() > 25.)) return false;
    if (not (lep_id->at(lep_veto_idxs[0]) * lep_id->at(lep_veto_idxs[1]) < 0)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelEMu()
{
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(lep_Nom_idx2) == -143)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOnZ()
{
    if (fabs(dilepNominal.M() - 91.1876) < 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(lep_Nom_idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOffZ()
{
    if (fabs(dilepNominal.M() - 91.1876) >= 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(lep_Nom_idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::Is2ndOnZFiveLepton()
{
    if (fabs((leptons[lep_2ndZCand_idx1] + leptons[lep_2ndZCand_idx2]).M() - 91.1876) < 10. and leptons[lep_2ndZCand_idx1].Pt() > 25.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::ChannelEMuHighMll()
{
    if (dilepNominal.M() > 120.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::ChannelOffZHighMET()
{
    if (met_pt > 120.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::Is5thNominal()
{
    if (lep_relIso03EA->at(lep_WCand_idx1) < 0.1)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsNjetGeq2()
{
    return nj >= 2;
}

//______________________________________________________________________________________________
float Analysis::VarMll()
{
    return dilepNominal.M();
}

//______________________________________________________________________________________________
float Analysis::VarMll2ndZ()
{
    return (leptons[lep_2ndZCand_idx1] + leptons[lep_2ndZCand_idx2]).M();
}

//______________________________________________________________________________________________
float Analysis::VarMET()
{
    return met_pt;
}

//______________________________________________________________________________________________
float Analysis::VarMT5th()
{
    return sqrt(2 * met_pt * leptons[lep_WCand_idx1].Et() * (1.0 - cos(leptons[lep_WCand_idx1].Phi() - met_phi)));
}

//______________________________________________________________________________________________
float Analysis::VarRelIso5th()
{
    if (lep_WCand_idx1 >= 0)
        return lep_relIso03EA->at(lep_WCand_idx1);
    else
        return -999;
}

//______________________________________________________________________________________________
float Analysis::VarPt5th()
{
    if (lep_WCand_idx1 >= 0)
        return lep_pt->at(lep_WCand_idx1);
    else
        return -999;
}

//______________________________________________________________________________________________
float Analysis::VarNjet()
{
    return nj;
}

// eof
