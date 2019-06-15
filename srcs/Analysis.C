#define Analysis_C
#include "Analysis.h"

//______________________________________________________________________________________________
// e.g.
// NtupleVersion = "WVZ2018_v0.0.9"
// TagName = "LoopTag1"
void Analysis::Loop(const char* NtupleVersion, const char* TagName)
{

    //==================
    // This is the main analysis looper
    // The entry point is being called in Control.C
    //==================

    // Exit if no fTTree
    if (fTTree == 0)
        return;

    // Because TString is better than const char*
    TString ntupleVersion = NtupleVersion;

    // Parsing year
    if (ntupleVersion.Contains("v0.0.5")) year = -1; // Meaning use this sets to scale it up to 137
    else if (ntupleVersion.Contains("2016")) year = 2016;
    else if (ntupleVersion.Contains("2017")) year = 2017;
    else if (ntupleVersion.Contains("2018")) year = 2018;
    else cout << "could not recognize year! defaulting to '2018'! Make sure this is OK!" << endl;

    // Creating output directory and output files where histograms will be
    TString output_path = TString::Format("outputs/%s/%s", NtupleVersion, TagName);
    gSystem->Exec(TString::Format("mkdir -p %s", output_path.Data()));
    TFile* output_file = new TFile(output_path + "/" + output_tfile_name, "RECREATE");

    // Load scale factor histograms
    loadScaleFactors();

    // The RooUtil::Cutflow object facilitates various cutflow/histogramming
    RooUtil::Cutflow cutflow(output_file);
    cutflow.addCut("Weight", [&](){ return 1; }, [&](){ return this->EventWeight(); } ); 

    // There are two types of NtupleVersion
    // 1. WVZ201*_v* which only contains events with 4 or more leptons
    // 2. Dilep201*_v* which contains dilepton events
    if (ntupleVersion.Contains("WVZ") or ntupleVersion.Contains("Trilep"))
    {
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("FourLeptons", [&](){ return this->Is4LeptonEvent(); }, UNITY ); 
        cutflow.addCutToLastActiveCut("FindZCandLeptons", [&](){ return this->FindZCandLeptons(); }, UNITY ); 
        cutflow.addCutToLastActiveCut("FindTwoOSNominalLeptons", [&](){ return this->FindTwoOSNominalLeptons(); }, UNITY ); 
        cutflow.addCutToLastActiveCut("Cut4LepLeptonPt", [&](){ return this->Cut4LepLeptonPt(); }, UNITY ); 
        cutflow.addCutToLastActiveCut("CutHLT", [&](){ return this->CutHLT(); }, UNITY ); 
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
    }
    else if (ntupleVersion.Contains("Dilep"))
    {
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("TwoOSLeptons", [&](){ return this->IsTwoOSLeptonEvent(); }, UNITY ); 
    }

    // Book Cutflow
    cutflow.bookCutflows();

    // Histogram object contains histogram definitions and the lambda to be used for histogram filling
    RooUtil::Histograms histograms;
    if (ntupleVersion.Contains("WVZ") or ntupleVersion.Contains("Trilep"))
    {
        histograms.addHistogram("Mll", 180, 0, 300, [&](){ return this->VarMll(); });
        histograms.addHistogram("MET", 180, 0, 300, [&](){ return this->VarMET(); });
        histograms.addHistogram("Mll2ndZ", 180, 0, 300, [&](){ return this->VarMll2ndZ(); });
        histograms.addHistogram("MT5th", 180, 0, 300, [&](){ return this->VarMT5th(); });
        histograms.addHistogram("RelIso5th", 180, 0, 0.4, [&](){ return this->VarRelIso5th(); });
        histograms.addHistogram("Pt5th", 180, 0, 200, [&](){ return this->VarPt5th(); });
        histograms.addHistogram("Njet", 4, 0, 4, [&](){ return this->VarNjet(); });
        histograms.addHistogram("Mll2l", 180, 0, 300, [&](){ return this->VarMll2l(); });
        histograms.addHistogram("lepZPt0", 180, 0, 200, [&](){ return wvz.lep_pt()[lep_ZCand_idx1]; });
        histograms.addHistogram("lepZPt1", 180, 0, 200, [&](){ return wvz.lep_pt()[lep_ZCand_idx2]; });
        histograms.addHistogram("lepNPt0", 180, 0, 200, [&](){ return wvz.lep_pt()[lep_Nom_idx1]; });
        histograms.addHistogram("lepNPt1", 180, 0, 200, [&](){ return wvz.lep_pt()[lep_Nom_idx2]; });
        histograms.addHistogram("MllZCand", 180, 0, 200, [&](){ return (wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).M(); });
        histograms.addHistogram("MllNom", 180, 0, 200, [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2]).M(); });
        histograms.addHistogram("M4l", 180, 0, 450, [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2] + wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).M(); });
        histograms.addHistogram("PtllZCand", 180, 0, 200, [&](){ return (wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).Pt(); });
        histograms.addHistogram("PtllNom", 180, 0, 200, [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2]).Pt(); });
        histograms.addHistogram("lepZrelIso03EA0", 180, 0, 0.4, [&](){ return wvz.lep_relIso03EA()[lep_ZCand_idx1]; });
        histograms.addHistogram("lepZrelIso03EA1", 180, 0, 0.4, [&](){ return wvz.lep_relIso03EA()[lep_ZCand_idx2]; });
        histograms.addHistogram("lepNrelIso03EA0", 180, 0, 0.4, [&](){ return wvz.lep_relIso03EA()[lep_Nom_idx1]; });
        histograms.addHistogram("lepNrelIso03EA1", 180, 0, 0.4, [&](){ return wvz.lep_relIso03EA()[lep_Nom_idx2]; });
        histograms.addHistogram("lepZrelIso04DB0", 180, 0, 0.4, [&](){ return wvz.lep_relIso04DB()[lep_ZCand_idx1]; });
        histograms.addHistogram("lepZrelIso04DB1", 180, 0, 0.4, [&](){ return wvz.lep_relIso04DB()[lep_ZCand_idx2]; });
        histograms.addHistogram("lepNrelIso04DB0", 180, 0, 0.4, [&](){ return wvz.lep_relIso04DB()[lep_Nom_idx1]; });
        histograms.addHistogram("lepNrelIso04DB1", 180, 0, 0.4, [&](){ return wvz.lep_relIso04DB()[lep_Nom_idx2]; });
        histograms.addHistogram("lepZsIP3D0", 180, 0, 10, [&](){ return fabs(wvz.lep_sip3d()[lep_ZCand_idx1]); });
        histograms.addHistogram("lepZsIP3D1", 180, 0, 10, [&](){ return fabs(wvz.lep_sip3d()[lep_ZCand_idx2]); });
        histograms.addHistogram("lepNsIP3D0", 180, 0, 10, [&](){ return fabs(wvz.lep_sip3d()[lep_Nom_idx1]); });
        histograms.addHistogram("lepNsIP3D1", 180, 0, 10, [&](){ return fabs(wvz.lep_sip3d()[lep_Nom_idx2]); });
        histograms.addHistogram("lepZdxy0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dxy()[lep_ZCand_idx1]); });
        histograms.addHistogram("lepZdxy1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dxy()[lep_ZCand_idx2]); });
        histograms.addHistogram("lepNdxy0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dxy()[lep_Nom_idx1]); });
        histograms.addHistogram("lepNdxy1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dxy()[lep_Nom_idx2]); });
        histograms.addHistogram("lepZdz0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dz()[lep_ZCand_idx1]); });
        histograms.addHistogram("lepZdz1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dz()[lep_ZCand_idx2]); });
        histograms.addHistogram("lepNdz0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dz()[lep_Nom_idx1]); });
        histograms.addHistogram("lepNdz1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_dz()[lep_Nom_idx2]); });
        histograms.addHistogram("lepZIP3D0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_ip3d()[lep_ZCand_idx1]); });
        histograms.addHistogram("lepZIP3D1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_ip3d()[lep_ZCand_idx2]); });
        histograms.addHistogram("lepNIP3D0", 180, 0, 0.2, [&](){ return fabs(wvz.lep_ip3d()[lep_Nom_idx1]); });
        histograms.addHistogram("lepNIP3D1", 180, 0, 0.2, [&](){ return fabs(wvz.lep_ip3d()[lep_Nom_idx2]); });
        histograms.addHistogram("lepZsMotherID0", 7, -4, 3, [&](){ return fabs(wvz.lep_motherIdv2()[lep_ZCand_idx1]); });
        histograms.addHistogram("lepZsMotherID1", 7, -4, 3, [&](){ return fabs(wvz.lep_motherIdv2()[lep_ZCand_idx2]); });
        histograms.addHistogram("lepNsMotherID0", 7, -4, 3, [&](){ return fabs(wvz.lep_motherIdv2()[lep_Nom_idx1]); });
        histograms.addHistogram("lepNsMotherID1", 7, -4, 3, [&](){ return fabs(wvz.lep_motherIdv2()[lep_Nom_idx2]); });
    }
    else if (ntupleVersion.Contains("Dilep"))
    {
        histograms.addHistogram("Mll2l", 180, 0, 300, [&](){ return this->VarMll2l(); });
        histograms.addHistogram("Njet", 4, 0, 4, [&](){ return this->VarNjet(); });
        histograms.addHistogram("MET", 180, 0, 300, [&](){ return this->VarMET(); });
        histograms.addHistogram("nvtx", 60, 0, 60, [&](){ return this->VarNvtx(); });
        histograms.addHistogram("lep_pt0", 180, 0, 150, [&](){ return wvz.lep_pt().size() > 0 ? wvz.lep_pt()[0] : -999; });
        histograms.addHistogram("lep_pt1", 180, 0, 125, [&](){ return wvz.lep_pt().size() > 1 ? wvz.lep_pt()[1] : -999; });
        histograms.addHistogram("lep_eta0", 180, -2.5, 2.5, [&](){ return wvz.lep_eta().size() > 0 ? wvz.lep_eta()[0] : -999; });
        histograms.addHistogram("lep_eta1", 180, -2.5, 2.5, [&](){ return wvz.lep_eta().size() > 1 ? wvz.lep_eta()[1] : -999; });
        histograms.addHistogram("jets_pt0", 180, 0, 150, [&](){ return wvz.jets_pt().size() > 0 ? wvz.jets_pt()[0] : -999; });
        histograms.addHistogram("jets_pt1", 180, 0, 125, [&](){ return wvz.jets_pt().size() > 1 ? wvz.jets_pt()[1] : -999; });
        histograms.addHistogram("jets_pt2", 180, 0, 100, [&](){ return wvz.jets_pt().size() > 2 ? wvz.jets_pt()[2] : -999; });
        histograms.addHistogram("jets_eta0", 180, -5, 5, [&](){ return wvz.jets_eta().size() > 0 ? wvz.jets_eta()[0] : -999; });
        histograms.addHistogram("jets_eta1", 180, -5, 5, [&](){ return wvz.jets_eta().size() > 1 ? wvz.jets_eta()[1] : -999; });
        histograms.addHistogram("jets_eta2", 180, -5, 5, [&](){ return wvz.jets_eta().size() > 2 ? wvz.jets_eta()[2] : -999; });
    }

    // Book histograms
    cutflow.bookHistograms(histograms);

    // // Book event list
    // cutflow.bookEventLists();

    // Looper class to facilitate various things
    TChain* ch = new TChain("t");
    ch->Add(fTTree->GetCurrentFile()->GetName());
    looper = new RooUtil::Looper<wvztree>(ch, &wvz, -1); // -1 means process all events
    while (looper->nextEvent())
    {
        // Once it enters loop it's 1, and then 2, and so on.
        // So need to subtract one and set it to 'ii' so fTTree can load event
        int ii = looper->getNEventsProcessed() - 1; 
        // Load the entry
        fTTree->GetEntry(ii, 0);
        readLeptons();
        selectVetoLeptons();
        selectZCandLeptons();
        selectNominalLeptons();
        select2ndZCandAndWCandLeptons();
        sortLeptonIndex();
        setDilepMasses();
        // cutflow.setEventID(wvz.run(), wvz.lumi(), wvz.evt());
        cutflow.fill();
        // if (cutflow.getCut("ChannelEMu").pass)
        // {
        //     std::cout << wvz.lep_motherIdv2()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_motherIdv2()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_motherIdv2()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_motherIdv2()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_id()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_id()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_id()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_id()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_sip3d()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_sip3d()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_sip3d()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_sip3d()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_pt()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_pt()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_pt()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_pt()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << wvz.lep_eta()[lep_ZCand_idx1] << " ";
        //     std::cout << wvz.lep_eta()[lep_ZCand_idx2] << " ";
        //     std::cout << wvz.lep_eta()[lep_Nom_idx1] << " ";
        //     std::cout << wvz.lep_eta()[lep_Nom_idx2] << " ";
        //     std::cout << std::endl;
        //     std::cout << "====electron from nominal====" << std::endl;
        //     if (abs(wvz.lep_id()[lep_Nom_idx1]) == 11)
        //     {
        //         std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_Nom_idx1] << " ";
        //         std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_Nom_idx1] << " ";
        //         std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_Nom_idx1] << " ";
        //         std::cout << std::endl;
        //     }
        //     else if (abs(wvz.lep_id()[lep_Nom_idx2]) == 11)
        //     {
        //         std::cout << wvz.lep_isCutBasedIsoLoosePOG()[lep_Nom_idx2] << " ";
        //         std::cout << wvz.lep_isCutBasedIsoMediumPOG()[lep_Nom_idx2] << " ";
        //         std::cout << wvz.lep_isCutBasedIsoTightPOG()[lep_Nom_idx2] << " ";
        //         std::cout << std::endl;
        //     }
        // }
    }

    // cutflow.getCut("ChannelEMu").printEventList();

    cutflow.saveOutput();


}//end of whole function

//______________________________________________________________________________________________
void Analysis::loadScaleFactors()
{
    histmap_purwegt = new RooUtil::HistMap(TString::Format("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/puWeight%d.root:pileupWeight", year));
}

//______________________________________________________________________________________________
void Analysis::readLeptons()
{
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
        leptons[jj].SetPtEtaPhiE(((*lep_pt).at(jj)), ((*lep_eta).at(jj)), ((*lep_phi).at(jj)), ((*lep_energy).at(jj)));
}

// Object selection algorithms
//______________________________________________________________________________________________
void Analysis::selectVetoLeptons()
{
    nVetoLeptons = 0;
    lep_veto_idxs.clear();
    for (unsigned int kk = 0 ; kk < lep_pt->size() ; kk++)
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
    for (unsigned int jj = 0 ; jj < (lep_pt->size() - 1) ; jj ++)
    {

        if (not passZCandLeptonID(jj))
            continue;

        for (unsigned int kk = jj + 1 ; kk < lep_pt->size() ; kk++)
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
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == (unsigned int) lep_ZCand_idx1)
            continue;

        if (jj == (unsigned int) lep_ZCand_idx2)
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
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == (unsigned int) lep_ZCand_idx1)
            continue;

        if (jj == (unsigned int) lep_ZCand_idx2)
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
    // int tmp1 = lep_ZCand_idx1;
    // int tmp2 = lep_ZCand_idx2;
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

    //-------------------------------------------------
    // What is already applied at the baby making stage
    //-------------------------------------------------

    // if (!( cms3.els_p4()[idx].pt() > 10.          )) return false;
    // if (!( isVetoElectronPOGfall17_v2(idx)        )) return false;
    // if (!( fabs(cms3.els_p4()[idx].eta()) < 2.5   )) return false;
    // if (fabs(cms3.els_etaSC()[idx]) <= 1.479)
    // {
    //     if (!( fabs(cms3.els_dzPV()[idx]) < 0.1       )) return false;
    //     if (!( fabs(cms3.els_dxyPV()[idx]) < 0.05     )) return false;
    // }
    // else
    // {
    //     if (!( fabs(cms3.els_dzPV()[idx]) < 0.2       )) return false;
    //     if (!( fabs(cms3.els_dxyPV()[idx]) < 0.1      )) return false;
    // }
    // return true;

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passVetoMuonID(int idx)
{

    //-------------------------------------------------
    // What is already applied at the baby making stage
    //-------------------------------------------------

    // if (!( cms3.mus_p4()[idx].pt() > 10.        )) return false;
    // if (!( isLooseMuonPOG(idx)                  )) return false;
    // if (!( fabs(cms3.mus_p4()[idx].eta()) < 2.4 )) return false;
    // if (!( muRelIso04DB(idx)  < 0.25            )) return false;
    // if (fabs(cms3.mus_p4()[idx].eta()) <= 1.479)
    // {
    //     if (!( fabs(cms3.mus_dzPV()[idx]) < 0.1       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.05     )) return false;
    // }
    // else
    // {
    //     if (!( fabs(cms3.mus_dzPV()[idx]) < 0.2       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV()[idx]) < 0.1      )) return false;
    // }
    // return true;

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

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

    // Must pass Veto ID
    if (not (passVetoElectronID(idx))) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passZCandMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passVetoMuonID(idx))) return false;

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

    // Cut-based IsoMedium
    if (looper->getCurrentFileName().Contains("WVZMVA"))
    {
        if (not (wvz.lep_isMVAwp80IsoPOG()[idx])) return false;
    }
    else
    {
        if (not (wvz.lep_isCutBasedIsoMediumPOG()[idx])) return false;
    }

    return true;
}

//______________________________________________________________________________________________
bool Analysis::passNominalMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passZCandMuonID(idx))) return false;

    // Tight POG muon isolation
    if (not (fabs(wvz.lep_relIso04DB()[idx]) < 0.15)) return false;

    return true;
}

//______________________________________________________________________________________________
float Analysis::EventWeight()
{
    if (isData)
    {
        return 1;
    }
    else
    {
        float fixXsec = 1;
        if (looper->getCurrentFileName().Contains("v0.1.1")
                and looper->getCurrentFileName().Contains("2017")
                and looper->getCurrentFileName().Contains("ggh_hzz4l"))
            fixXsec = 1.236e-05 / 5.617e-05; // Error from wrong scale1fb setting
        if (year == 2016)
            return fixXsec * evt_scale1fb * 35.9 * histmap_purwegt->eval(wvz.nTrueInt());
        else if (year == 2017)
            return fixXsec * evt_scale1fb * 41.3 * histmap_purwegt->eval(wvz.nTrueInt());
        else if (year == 2018)
            return fixXsec * evt_scale1fb * 59.74 * histmap_purwegt->eval(wvz.nTrueInt());
        else
            return fixXsec * evt_scale1fb * 137;
    }
}

//==============================================================================================
//
//
// Booleans for event selection cuts
//
//
//==============================================================================================

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
bool Analysis::CutHLT()
{
    if (nNominalLeptons != 2 or lep_ZCand_idx1 == -999)
        return false;
    if (wvz.isData())
        if (not wvz.pass_duplicate_mm_em_ee())
            return false;
    if (abs(wvz.lep_id()[lep_ZCand_idx1]) == 11 and abs(wvz.lep_id()[lep_Nom_idx1]) == 11)
        return wvz.HLT_DoubleEl();
    else if (abs(wvz.lep_id()[lep_ZCand_idx1]) == 13 and abs(wvz.lep_id()[lep_Nom_idx1]) == 11)
        return wvz.HLT_MuEG();
    else if (abs(wvz.lep_id()[lep_ZCand_idx1]) == 11 and abs(wvz.lep_id()[lep_Nom_idx1]) == 13)
        return wvz.HLT_MuEG();
    else if (abs(wvz.lep_id()[lep_ZCand_idx1]) == 13 and abs(wvz.lep_id()[lep_Nom_idx1]) == 13)
        return wvz.HLT_DoubleMu();
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
    if (not (abs(lep_id->at(lep_veto_idxs[0])) == 13)) return false;
    if (not (abs(lep_id->at(lep_veto_idxs[1])) == 13)) return false;
    if (not (HLT_DoubleMu)) return false;
    if (not ((leptons[lep_veto_idxs[0]] + leptons[lep_veto_idxs[1]]).M() > 50.)) return false;
    if (not (passZCandLeptonID(lep_veto_idxs[0]))) return false;
    if (not (passZCandLeptonID(lep_veto_idxs[1]))) return false;
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




//==============================================================================================
//
//
// float functions for histogram variables
//
//
//==============================================================================================

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
float Analysis::VarNvtx()
{
    return nvtx;
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

//______________________________________________________________________________________________
float Analysis::VarMll2l()
{
    if (nVetoLeptons < 2)
        return -999;
    return (leptons[lep_veto_idxs[0]] + leptons[lep_veto_idxs[1]]).M();
}

// eof
