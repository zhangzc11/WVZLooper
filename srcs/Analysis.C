#define Analysis_C
#include "Analysis.h"
#include "puw2016.h"
#include "puw2017.h"
#include "puw2018.h"

TheoryWeight theoryweight;

//______________________________________________________________________________________________
// e.g.
// NtupleVersion = "WVZ2018_v0.0.9"
// TagName = "LoopTag1"
void Analysis::Loop(const char* NtupleVersion, const char* TagName, bool dofake)
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

    doFakeEst = dofake;
    std::cout <<  " doFakeEst: " << doFakeEst <<  std::endl;

    // Parsing year
    if (ntupleVersion.Contains("v0.0.5")) year = -1; // Meaning use this sets to scale it up to 137
    else if (ntupleVersion.Contains("2016")) year = 2016;
    else if (ntupleVersion.Contains("2017")) year = 2017;
    else if (ntupleVersion.Contains("2018")) year = 2018;
    else cout << "could not recognize year! defaulting to '2018'! Make sure this is OK!" << endl;

    // Creating output directory and output files where histograms will be
    TString output_path = TString::Format("outputs/%s/%s", NtupleVersion, TagName);
    gSystem->Exec(TString::Format("mkdir -p %s", output_path.Data()));
    TString prefix = doFakeEst ? "FAKE_" : "";
    TFile* output_file = new TFile(output_path + "/" + prefix + output_tfile_name, "RECREATE");

    // Load scale factor histograms
    loadScaleFactors();

    // The RooUtil::Cutflow object facilitates various cutflow/histogramming
    RooUtil::Cutflow cutflow(output_file);
    cutflow.addCut("Weight", [&](){ return 1; }, [&](){ return this->EventWeight(); } );

    // There are two types of NtupleVersion
    // 1. WVZ201*_v* which only contains events with 4 or more leptons
    // 2. Trilep201*_v* which also contains trilepton events in addition to 4 or more  lepton events

    // List of common four lepton related selections
    cutflow.getCut("Weight");
    cutflow.addCutToLastActiveCut("FourLeptons"             , [&](){ return this->Is4LeptonEvent();          } , [&](){ return this->LeptonScaleFactor(); } );
    cutflow.addCutToLastActiveCut("FindZCandLeptons"        , [&](){ return this->FindZCandLeptons();        } , UNITY );
    cutflow.addCutToLastActiveCut("FindTwoOSNominalLeptons" , [&](){ return this->FindTwoOSNominalLeptons(); } , [&]() { return this->FakeFactor();       } );
    cutflow.addCutToLastActiveCut("Cut4LepLeptonPt"         , [&](){ return this->Cut4LepLeptonPt();         } , UNITY );
    cutflow.addCutToLastActiveCut("CutHLT"                  , [&](){ return this->CutHLT();                  } , UNITY );
    cutflow.addCutToLastActiveCut("Cut4LepLowMll"           , [&](){ return this->Cut4LepLowMll();           } , UNITY );
    cutflow.addCutToLastActiveCut("Cut4LepBVeto"            , [&](){ return this->Cut4LepBVeto();            } , [&](){ return this->BTagSF();            } );

    // emu channel
    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelEMu"              , [&](){ return this->IsChannelEMu();            } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelEMuHighMT"        , [&](){ return this->CutHighMT();               } , UNITY );

    // OnZ channel
    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelOnZ"              , [&](){ return this->IsChannelOnZ();            } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelOnZNjet"          , [&](){ return this->IsNjetGeq2();              } , UNITY );

    // OffZ channel
    cutflow.getCut("Cut4LepBVeto");
    cutflow.addCutToLastActiveCut("ChannelOffZ"             , [&](){ return this->IsChannelOffZ();           } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelOffZHighMET"      , [&](){ return this->CutHighMET();              } , UNITY );

    // OnZ Control regions
    cutflow.getCut("ChannelOnZ");
    cutflow.addCutToLastActiveCut("ChannelOnZHighMT"        , [&](){ return this->CutHighMT();               } , UNITY );
    cutflow.getCut("ChannelOnZ");
    cutflow.addCutToLastActiveCut("ChannelOnZHighMET"       , [&](){ return this->CutHighMET();              } , UNITY );
    cutflow.getCut("ChannelOnZ");
    cutflow.addCutToLastActiveCut("ChannelOnZCR"            , [&](){ return 1;                               } , UNITY );

    // Common B-tagged preselection
    cutflow.getCut("Cut4LepLowMll");
    cutflow.addCutToLastActiveCut("Cut4LepBTag"             , [&](){ return this->Cut4LepBTag();             } , [&](){ return this->BTagSF();            } );

    // b-tagged emu channel
    cutflow.getCut("Cut4LepBTag");
    cutflow.addCutToLastActiveCut("ChannelBTagEMu"          , [&](){ return this->IsChannelEMu();            } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelBTagEMuHighMT"    , [&](){ return this->CutHighMT();               } , UNITY );

    cutflow.getCut("ChannelBTagEMu");
    cutflow.addCutToLastActiveCut("ChannelBTagEMuCR"        , [&](){ return 1;                               } , UNITY );

    // b-tagged onz channel
    cutflow.getCut("Cut4LepBTag");
    cutflow.addCutToLastActiveCut("ChannelBTagOnZ"          , [&](){ return this->IsChannelOnZ();            } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelBTagOnZNjet"      , [&](){ return this->IsNjetGeq2();              } , UNITY );

    // b-tagged offz channel
    cutflow.getCut("Cut4LepBTag");
    cutflow.addCutToLastActiveCut("ChannelBTagOffZ"         , [&](){ return this->IsChannelOffZ();           } , UNITY );
    cutflow.addCutToLastActiveCut("ChannelBTagOffZHighMET"  , [&](){ return this->CutHighMET();              } , UNITY );

    // Common five lepton selections
    cutflow.getCut("Weight");
    cutflow.addCutToLastActiveCut("FiveLeptons"             , [&](){ return this->Is5LeptonEvent();          } , UNITY );
    cutflow.addCutToLastActiveCut("FiveLeptonsMllZ"         , [&](){ return this->Is2ndOnZFiveLepton();      } , UNITY );
    cutflow.addCutToLastActiveCut("FiveLeptonsRelIso5th"    , [&](){ return this->Is5thNominal();            } , UNITY );
    cutflow.addCutToLastActiveCut("FiveLeptonsMT5th"        , [&](){ return this->VarMT5th() > 30.;          } , UNITY );

    // For fake rate related studies
    if (ntupleVersion.Contains("Trilep"))
    {
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("ThreeLeptons"              , [&]() { return this->Is3LeptonEvent(); }                       , [&](){ return this->LeptonScaleFactor(); } );
        cutflow.addCutToLastActiveCut("EMuPlusX"                  , [&]() { return this->IsEMuPlusX(); }                           , UNITY );
        cutflow.getCut("EMuPlusX");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMu"            , [&]() { return abs(wvz.lep_id()[lep_FakeCand_idx2]) == 13; }   , UNITY );
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTight"       , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.getCut("EMuPlusXFakeMu");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuPrompt"      , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeMuTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTightPrompt" , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeMu");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuFake"        , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] < 1; }  , UNITY );
        cutflow.getCut("EMuPlusXFakeMuTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTightFake"   , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] < 1; }  , UNITY );
        cutflow.getCut("EMuPlusX");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeEl"            , [&]() { return abs(wvz.lep_id()[lep_FakeCand_idx2]) == 11; }   , UNITY );
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTight"       , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.getCut("EMuPlusXFakeEl");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElPrompt"      , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeElTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTightPrompt" , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeEl");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElFake"        , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] < 1; }  , UNITY );
        cutflow.getCut("EMuPlusXFakeElTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTightFake"   , [&]() { return wvz.lep_motherIdv2()[lep_FakeCand_idx2] < 1; }  , UNITY );
        cutflow.getCut("ThreeLeptons");
        cutflow.addCutToLastActiveCut("Channel1SFOS"              , [&]() { return this->VarNSFOS() == 1; }                        , UNITY );
        cutflow.getCut("ThreeLeptons");
        cutflow.addCutToLastActiveCut("Channel2SFOS"              , [&]() { return this->VarNSFOS() == 2; }                        , UNITY );
    }

    // Histogram object contains histogram definitions and the lambda to be used for histogram filling
    RooUtil::Histograms histograms;
    histograms.addHistogram("Mll"                        , 180 , 0       , 300    , [&](){ return this->VarMll(); });
    histograms.addHistogram("MET"                        , 180 , 0       , 300    , [&](){ return this->VarMET(); });
    histograms.addHistogram("Mll2ndZ"                    , 180 , 0       , 300    , [&](){ return this->VarMll2ndZ(); });
    histograms.addHistogram("MT5th"                      , 180 , 0       , 300    , [&](){ return this->VarMT5th(); });
    histograms.addHistogram("RelIso5th"                  , 180 , 0       , 0.4    , [&](){ return this->VarRelIso5th(); });
    histograms.addHistogram("Pt5th"                      , 180 , 0       , 200    , [&](){ return this->VarPt5th(); });
    histograms.addHistogram("Njet"                       , 4   , 0       , 4      , [&](){ return this->VarNjet(); });
    histograms.addHistogram("Nbjet"                      , 4   , 0       , 4      , [&](){ return wvz.nb(); });
    histograms.addHistogram("Mll2l"                      , 180 , 0       , 300    , [&](){ return this->VarMll2l(); });
    histograms.addHistogram("lepZPt0"                    , 180 , 0       , 200    , [&](){ return wvz.lep_pt()[lep_ZCand_idx1]; });
    histograms.addHistogram("lepZPt1"                    , 180 , 0       , 200    , [&](){ return wvz.lep_pt()[lep_ZCand_idx2]; });
    histograms.addHistogram("lepNPt0"                    , 180 , 0       , 200    , [&](){ return wvz.lep_pt()[lep_Nom_idx1]; });
    histograms.addHistogram("lepNPt1"                    , 180 , 0       , 200    , [&](){ return wvz.lep_pt()[lep_Nom_idx2]; });
    histograms.addHistogram("MllZCand"                   , 180 , 0       , 200    , [&](){ return (wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).M(); });
    histograms.addHistogram("MllNom"                     , 180 , 0       , 200    , [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2]).M(); });
    histograms.addHistogram("M4l"                        , 180 , 0       , 450    , [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2] + wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).M(); });
    histograms.addHistogram("M4lZoom"                    , 180 , 100     , 150    , [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2] + wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).M(); });
    histograms.addHistogram("PtllZCand"                  , 180 , 0       , 200    , [&](){ return (wvz.lep_p4()[lep_ZCand_idx1] + wvz.lep_p4()[lep_ZCand_idx2]).Pt(); });
    histograms.addHistogram("PtllNom"                    , 180 , 0       , 200    , [&](){ return (wvz.lep_p4()[lep_Nom_idx1] + wvz.lep_p4()[lep_Nom_idx2]).Pt(); });
    // histograms.addHistogram("lepZrelIso03EA0"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso03EA()[lep_ZCand_idx1]; });
    // histograms.addHistogram("lepZrelIso03EA1"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso03EA()[lep_ZCand_idx2]; });
    // histograms.addHistogram("lepNrelIso03EA0"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso03EA()[lep_Nom_idx1]; });
    // histograms.addHistogram("lepNrelIso03EA1"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso03EA()[lep_Nom_idx2]; });
    // histograms.addHistogram("lepZrelIso04DB0"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso04DB()[lep_ZCand_idx1]; });
    // histograms.addHistogram("lepZrelIso04DB1"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso04DB()[lep_ZCand_idx2]; });
    // histograms.addHistogram("lepNrelIso04DB0"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso04DB()[lep_Nom_idx1]; });
    // histograms.addHistogram("lepNrelIso04DB1"            , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso04DB()[lep_Nom_idx2]; });
    // histograms.addHistogram("lepZsIP3D0"                 , 180 , 0       , 10     , [&](){ return fabs(wvz.lep_sip3d()[lep_ZCand_idx1]); });
    // histograms.addHistogram("lepZsIP3D1"                 , 180 , 0       , 10     , [&](){ return fabs(wvz.lep_sip3d()[lep_ZCand_idx2]); });
    // histograms.addHistogram("lepNsIP3D0"                 , 180 , 0       , 10     , [&](){ return fabs(wvz.lep_sip3d()[lep_Nom_idx1]); });
    // histograms.addHistogram("lepNsIP3D1"                 , 180 , 0       , 10     , [&](){ return fabs(wvz.lep_sip3d()[lep_Nom_idx2]); });
    // histograms.addHistogram("lepZdxy0"                   , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dxy()[lep_ZCand_idx1]); });
    // histograms.addHistogram("lepZdxy1"                   , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dxy()[lep_ZCand_idx2]); });
    // histograms.addHistogram("lepNdxy0"                   , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dxy()[lep_Nom_idx1]); });
    // histograms.addHistogram("lepNdxy1"                   , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dxy()[lep_Nom_idx2]); });
    // histograms.addHistogram("lepZdz0"                    , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dz()[lep_ZCand_idx1]); });
    // histograms.addHistogram("lepZdz1"                    , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dz()[lep_ZCand_idx2]); });
    // histograms.addHistogram("lepNdz0"                    , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dz()[lep_Nom_idx1]); });
    // histograms.addHistogram("lepNdz1"                    , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_dz()[lep_Nom_idx2]); });
    // histograms.addHistogram("lepZIP3D0"                  , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_ip3d()[lep_ZCand_idx1]); });
    // histograms.addHistogram("lepZIP3D1"                  , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_ip3d()[lep_ZCand_idx2]); });
    // histograms.addHistogram("lepNIP3D0"                  , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_ip3d()[lep_Nom_idx1]); });
    // histograms.addHistogram("lepNIP3D1"                  , 180 , 0       , 0.2    , [&](){ return fabs(wvz.lep_ip3d()[lep_Nom_idx2]); });
    // histograms.addHistogram("lepZsMotherID0"             , 7   , -4      , 3      , [&](){ return wvz.lep_motherIdv2()[lep_ZCand_idx1]; });
    // histograms.addHistogram("lepZsMotherID1"             , 7   , -4      , 3      , [&](){ return wvz.lep_motherIdv2()[lep_ZCand_idx2]; });
    // histograms.addHistogram("lepNsMotherID0"             , 7   , -4      , 3      , [&](){ return wvz.lep_motherIdv2()[lep_Nom_idx1]; });
    // histograms.addHistogram("lepNsMotherID1"             , 7   , -4      , 3      , [&](){ return wvz.lep_motherIdv2()[lep_Nom_idx2]; });
    // histograms.addHistogram("lepZid0"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_ZCand_idx1]); });
    // histograms.addHistogram("lepZid1"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_ZCand_idx2]); });
    // histograms.addHistogram("lepNid0"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Nom_idx1]); });
    // histograms.addHistogram("lepNid1"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Nom_idx2]); });
    // histograms.addHistogram("lepVid0"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Veto_idx1]); });
    // histograms.addHistogram("lepVid1"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Veto_idx2]); });
    // histograms.addHistogram("lepVid2"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Veto_idx3]); });
    // histograms.addHistogram("lepVid3"                    , 5   , 10      , 15     , [&](){ return fabs(wvz.lep_id()[lep_Veto_idx4]); });
    // histograms.addHistogram("lepVpt0"                    , 180 , 0       , 200    , [&](){ return fabs(wvz.lep_pt()[lep_Veto_idx1]); });
    // histograms.addHistogram("lepVpt1"                    , 180 , 0       , 200    , [&](){ return fabs(wvz.lep_pt()[lep_Veto_idx2]); });
    // histograms.addHistogram("lepVpt2"                    , 180 , 0       , 200    , [&](){ return fabs(wvz.lep_pt()[lep_Veto_idx3]); });
    // histograms.addHistogram("lepVpt3"                    , 180 , 0       , 200    , [&](){ return fabs(wvz.lep_pt()[lep_Veto_idx4]); });
    // histograms.addHistogram("lepVrelIso03EA0"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso03EA()[lep_Veto_idx1]); });
    // histograms.addHistogram("lepVrelIso03EA1"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso03EA()[lep_Veto_idx2]); });
    // histograms.addHistogram("lepVrelIso03EA2"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso03EA()[lep_Veto_idx3]); });
    // histograms.addHistogram("lepVrelIso03EA3"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso03EA()[lep_Veto_idx4]); });
    // histograms.addHistogram("lepVrelIso04DB0"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso04DB()[lep_Veto_idx1]); });
    // histograms.addHistogram("lepVrelIso04DB1"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso04DB()[lep_Veto_idx2]); });
    // histograms.addHistogram("lepVrelIso04DB2"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso04DB()[lep_Veto_idx3]); });
    // histograms.addHistogram("lepVrelIso04DB3"            , 180 , 0       , 0.4    , [&](){ return fabs(wvz.lep_relIso04DB()[lep_Veto_idx4]); });
    // // histograms.addHistogram("lepVmotherid0"           , 30  , 0       , 30     , [&](){ return fabs(wvz.lep_mc_motherid()[lep_Veto_idx1]); });
    // // histograms.addHistogram("lepVmotherid1"           , 30  , 0       , 30     , [&](){ return fabs(wvz.lep_mc_motherid()[lep_Veto_idx2]); });
    // // histograms.addHistogram("lepVmotherid2"           , 30  , 0       , 30     , [&](){ return fabs(wvz.lep_mc_motherid()[lep_Veto_idx3]); });
    // // histograms.addHistogram("lepVmotherid3"           , 30  , 0       , 30     , [&](){ return fabs(wvz.lep_mc_motherid()[lep_Veto_idx4]); });
    // // histograms.addHistogram("lepNTauLep"              , 5   , 0       , 5      , [&](){ return (abs(wvz.lep_mc_motherid()[lep_Veto_idx1])==15)+(abs(wvz.lep_mc_motherid()[lep_Veto_idx2])==15)+(abs(wvz.lep_mc_motherid()[lep_Veto_idx3])==15)+(abs(wvz.lep_mc_motherid()[lep_Veto_idx4])==15); });
    // histograms.addHistogram("lepVetoButNotNomMotherID"   , 7   , -4      , 3      , [&](){ return wvz.lep_motherIdv2()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNomMCID"       , 7   , -4      , 3      , [&](){ return wvz.lep_mc_id()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNomrelIso03EA" , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso03EA()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNomrelIso04DB" , 180 , 0       , 0.4    , [&](){ return wvz.lep_relIso04DB()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNompt"         , 180 , 0       , 70     , [&](){ return wvz.lep_pt()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNomid"         , 5   , 10      , 15     , [&](){ return abs(wvz.lep_id()[lep_VetoButNotNom_idx]); });
    // histograms.addHistogram("lepVetoButNotNometa"        , 180 , -2.5    , 2.5    , [&](){ return wvz.lep_eta()[lep_VetoButNotNom_idx]; });
    // histograms.addHistogram("lepVetoButNotNomphi"        , 180 , -3.1416 , 3.1416 , [&](){ return wvz.lep_phi()[lep_VetoButNotNom_idx]; });
    histograms.addHistogram("MTNom0"                     , 180 , 0       , 180    , [&](){ return this->VarMTNom0(); });
    histograms.addHistogram("MTNom1"                     , 180 , 0       , 150    , [&](){ return this->VarMTNom1(); });
    histograms.addHistogram("MTNomMax"                   , 180 , 0       , 180    , [&](){ return this->VarMTMax(); });
    histograms.addHistogram("MTNomMin"                   , 180 , 0       , 180    , [&](){ return this->VarMTMin(); });
    histograms.addHistogram("MllNomWindow"               , 180 , 0       , 50     , [&](){ return fabs((wvz.lep_p4()[lep_Nom_idx1]+wvz.lep_p4()[lep_Nom_idx2]).M()-91.1876); });
    histograms.addHistogram("Yield"                      , 1   , 0       , 1      , [&](){ return 0; });

    if (ntupleVersion.Contains("Trilep"))
    {
        histograms.addHistogram("lepFakeCand2MotherID"    , 7   , -4  , 3    , [&](){ return wvz.lep_motherIdv2()[lep_FakeCand_idx2]; });
        histograms.addHistogram("lepFakeCand2MCID"        , 7   , -4  , 3    , [&](){ return wvz.lep_mc_id()[lep_FakeCand_idx2]; });
        histograms.addHistogram("lepFakeCand2Pt"          , 180 , 0.  , 150. , [&](){ return fabs(wvz.lep_pt()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2dz"          , 180 , 0.  , 0.2  , [&](){ return fabs(wvz.lep_dz()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2dxy"         , 180 , 0.  , 0.2  , [&](){ return fabs(wvz.lep_dxy()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2sip3d"       , 180 , 0.  , 20   , [&](){ return fabs(wvz.lep_sip3d()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2ip3d"        , 180 , 0.  , 0.2  , [&](){ return fabs(wvz.lep_ip3d()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2relIso03EA"  , 180 , 0.  , 0.4  , [&](){ return fabs(wvz.lep_relIso03EA()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2relIso04DB"  , 180 , 0.  , 0.4  , [&](){ return fabs(wvz.lep_relIso04DB()[lep_FakeCand_idx2]); });
        histograms.addHistogram("lepFakeCand2PtVarBin"    , {0. , 10. , 20.  , 70.}, [&](){ float rtn = std::min((double) wvz.lep_pt()[lep_FakeCand_idx2]                                                 , 149.9); return rtn; });
        histograms.addHistogram("lepFakeCand2PtVarBinFwd" , {0. , 10. , 20.  , 70.}, [&](){ float rtn = fabs(wvz.lep_eta()[lep_FakeCand_idx2]) >  1.6 ? std::min((double) wvz.lep_pt()[lep_FakeCand_idx2] , 149.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeCand2PtVarBinCen" , {0. , 10. , 20.  , 70.}, [&](){ float rtn = fabs(wvz.lep_eta()[lep_FakeCand_idx2]) <= 1.6 ? std::min((double) wvz.lep_pt()[lep_FakeCand_idx2] , 149.9) : -999; return rtn; });
        }

    // Book event list
    // cutflow.bookEventLists();

    // Systematic variations
    cutflow.addWgtSyst("BTagHFUp"  , [&]() { return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_heavy_UP() / wvz.weight_btagsf() : 1; } );
    cutflow.addWgtSyst("BTagHFDown", [&]() { return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_heavy_DN() / wvz.weight_btagsf() : 1; } );
    cutflow.addWgtSyst("BTagLFUp"  , [&]() { return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_light_UP() / wvz.weight_btagsf() : 1; } );
    cutflow.addWgtSyst("BTagLFDown", [&]() { return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_light_DN() / wvz.weight_btagsf() : 1; } );
    cutflow.addWgtSyst("PDFUp"     , [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.pdfup() == 0 ? 0 : wvz.weight_pdf_up()   / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfup(); } );
    cutflow.addWgtSyst("PDFDown"   , [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.pdfdn() == 0 ? 0 : wvz.weight_pdf_down() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfdn(); } );
    cutflow.addWgtSyst("QsqUp"     , [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.qsqup() == 0 ? 0 : wvz.weight_fr_r2_f2()     / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqup(); } );
    cutflow.addWgtSyst("QsqDown"   , [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.qsqdn() == 0 ? 0 : wvz.weight_fr_r0p5_f0p5() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqdn(); } );
    cutflow.addWgtSyst("AlphaSUp"  , [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.alsup() == 0 ? 0 : wvz.weight_alphas_up()   / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsup(); } );
    cutflow.addWgtSyst("AlphaSDown", [&]() { return wvz.weight_fr_r1_f1() == 0 or theoryweight.alsdn() == 0 ? 0 : wvz.weight_alphas_down() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsdn(); } );
    // TODO on weight systematics
    // 2. LepSF
    // 3. TrigSF
    cutflow.addWgtSyst("PileupUp"  , [&]()
            {
                if (year == 2016) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2016(wvz.nTrueInt()) / getTruePUw2016(wvz.nTrueInt()) : 0;
                else if (year == 2017) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2017(wvz.nTrueInt()) / getTruePUw2017(wvz.nTrueInt()) : 0;
                else if (year == 2018) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2018(wvz.nTrueInt()) / getTruePUw2018(wvz.nTrueInt()) : 0;
                return 0.f;
            } );
    cutflow.addWgtSyst("PileupDown"  , [&]()
            {
                if (year == 2016) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2016(wvz.nTrueInt()) / getTruePUw2016(wvz.nTrueInt()) : 0;
                else if (year == 2017) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2017(wvz.nTrueInt()) / getTruePUw2017(wvz.nTrueInt()) : 0;
                else if (year == 2018) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2018(wvz.nTrueInt()) / getTruePUw2018(wvz.nTrueInt()) : 0;
                return 0.f;
            } );

    // The strings in {} are the patterns that should declare the systematic variatiosn for
    // i.e. *MT*, *MET*, and *Cut4LepB* are the pattern used to search the cut names in the cutflow object to declare a systematic varations
    // e.g. ChannelBTagEMuHighMT contains "MT" and therefore a JES variations will be declared for these cuts
    // Then later below with "setCutSyst" function the variational cut defn will be defined
    cutflow.addCutSyst("JESUp", {"MT", "MET", "Cut4LepB"});
    cutflow.addCutSyst("JESDown", {"MT", "MET", "Cut4LepB"});

    // 1 represents up variation
    //-1 represents down variation
    // 1. MT cuts
    cutflow.setCutSyst("ChannelEMuHighMT"    , "JESUp", [&]() { return this->CutHighMT(1); } , UNITY );
    cutflow.setCutSyst("ChannelOnZHighMT"    , "JESUp", [&]() { return this->CutHighMT(1); } , UNITY );
    cutflow.setCutSyst("ChannelBTagEMuHighMT", "JESUp", [&]() { return this->CutHighMT(1); } , UNITY );
    cutflow.setCutSyst("FiveLeptonsMT5th"    , "JESUp", [&]() { return this->CutHighMT(1); } , UNITY );
    cutflow.setCutSyst("ChannelEMuHighMT"    , "JESDown", [&]() { return this->CutHighMT(-1); } , UNITY );
    cutflow.setCutSyst("ChannelOnZHighMT"    , "JESDown", [&]() { return this->CutHighMT(-1); } , UNITY );
    cutflow.setCutSyst("ChannelBTagEMuHighMT", "JESDown", [&]() { return this->CutHighMT(-1); } , UNITY );
    cutflow.setCutSyst("FiveLeptonsMT5th"    , "JESDown", [&]() { return this->CutHighMT(-1); } , UNITY );
    // 2. MET cuts
    cutflow.setCutSyst("ChannelOffZHighMET"    , "JESUp", [&]() { return this->CutHighMET(1); } , UNITY );
    cutflow.setCutSyst("ChannelOnZHighMET"     , "JESUp", [&]() { return this->CutHighMET(1); } , UNITY );
    cutflow.setCutSyst("ChannelBTagOffZHighMET", "JESUp", [&]() { return this->CutHighMET(1); } , UNITY );
    cutflow.setCutSyst("ChannelOffZHighMET"    , "JESDown", [&]() { return this->CutHighMET(-1); } , UNITY );
    cutflow.setCutSyst("ChannelOnZHighMET"     , "JESDown", [&]() { return this->CutHighMET(-1); } , UNITY );
    cutflow.setCutSyst("ChannelBTagOffZHighMET", "JESDown", [&]() { return this->CutHighMET(-1); } , UNITY );
    // 3. Cut4LepB cuts (the btag and bveto)
    cutflow.setCutSyst("Cut4LepBVeto" , "JESUp"   , [&]() { return this->Cut4LepBVeto(1); }  , UNITY );
    cutflow.setCutSyst("Cut4LepBTag"  , "JESUp"   , [&]() { return this->Cut4LepBTag(1); }   , UNITY );
    cutflow.setCutSyst("Cut4LepBVeto" , "JESDown" , [&]() { return this->Cut4LepBVeto(-1); } , UNITY );
    cutflow.setCutSyst("Cut4LepBTag"  , "JESDown" , [&]() { return this->Cut4LepBTag(-1); }  , UNITY );

    // Book Cutflow
    // cutflow.bookCutflows();

    // Book histograms
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMu");
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOnZ");
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOffZ");
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagEMu");
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagOnZ");
    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagOffZ");
    cutflow.bookHistogramsForCutAndBelow(histograms, "FiveLeptonsRelIso5th");

    // Looper class to facilitate various things
    TChain* ch = new TChain("t");
    ch->Add(fTTree->GetCurrentFile()->GetName());
    looper = new RooUtil::Looper<wvztree>(ch, &wvz, -1); // -1 means process all events
    while (looper->nextEvent())
    {

        // If new file in chain set some things may need to be set
        if (looper->isNewFileInChain())
        {
            theoryweight.setFile(looper->getCurrentFileName());
        }

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
        selectVetoButNotNomLeptons();
        sortLeptonIndex();
        selectFakeStudyLeptons();
        setDilepMasses();
        // cutflow.setEventID(wvz.run(), wvz.lumi(), wvz.evt());
        cutflow.fill();
        // if (cutflow.getCut("ChannelEMuHighMT").pass)
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

    // cutflow.getCut("ChannelOnZ").printEventList();

    cutflow.saveOutput();


}//end of whole function

//______________________________________________________________________________________________
void Analysis::loadScaleFactors()
{
    histmap_2016_elec_reco_highpt_sf    = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2016_elec_reco_lowpt_sf     = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EGM2D_BtoH_low_RecoSF_Legacy2016.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2016_elec_medium_sf         = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2016LegacyReReco_ElectronMedium_Fall17V2.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2016_elec_veto_sf           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2016_ElectronWPVeto_Fall17V2.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2017_elec_reco_highpt_sf    = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2017_elec_reco_lowpt_sf     = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2017_elec_medium_sf         = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2017_ElectronMedium.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2017_elec_veto_sf           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2017_ElectronWPVeto_Fall17V2.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2018_elec_reco_sf           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/egammaEffi.txt_EGM2D_updatedAll.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2018_elec_medium_sf         = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2018_ElectronMedium.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2018_elec_veto_sf           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/2018_ElectronWPVeto_Fall17V2.root:EGamma_SF2D"); // x=eta, y=pt
    histmap_2016_muon_BCDEF_id_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_eta_pt"); // x=eta, y=pt
    histmap_2016_muon_BCDEF_id_lowpt_sf = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_lowpt_RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta"); // x=pt, y=abseta
    histmap_2016_muon_BCDEF_tightiso_sf = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunBCDEF_SF_ISO.root:NUM_TightRelIso_DEN_MediumID_eta_pt"); // x=eta, y=pt
    histmap_2016_muon_BCDEF_looseiso_sf = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunBCDEF_SF_ISO.root:NUM_LooseRelIso_DEN_MediumID_eta_pt"); // x=eta, y=pt
    histmap_2016_muon_GH_id_sf          = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunGH_SF_ID.root:NUM_MediumID_DEN_genTracks_eta_pt"); // x=eta, y=pt 45% of 2016 data
    histmap_2016_muon_GH_id_lowpt_sf    = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_lowpt_RunGH_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta"); // x=pt, y=abseta 45% of 2016 data
    histmap_2016_muon_GH_tightiso_sf    = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunGH_SF_ISO.root:NUM_TightRelIso_DEN_MediumID_eta_pt"); // x=eta, y=pt 45% of 2016 data
    histmap_2016_muon_GH_looseiso_sf    = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2016_rootfiles_RunGH_SF_ISO.root:NUM_LooseRelIso_DEN_MediumID_eta_pt"); // x=eta, y=pt 45% of 2016 data
    histmap_2017_muon_id_sf             = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2017_rootfiles_RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta"); // x=pt, y=abseta
    histmap_2017_muon_id_lowpt_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2017_rootfiles_lowpt_RunBCDEF_SF_ID_JPsi.root:NUM_MediumID_DEN_genTracks_pt_abseta"); // x=pt, y=abseta
    histmap_2017_muon_tightiso_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2017_rootfiles_RunBCDEF_SF_ISO.root:NUM_TightRelIso_DEN_MediumID_pt_abseta"); // x=pt, y=abseta
    histmap_2017_muon_looseiso_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2017_rootfiles_RunBCDEF_SF_ISO.root:NUM_LooseRelIso_DEN_MediumID_pt_abseta"); // x=pt, y=abseta
    histmap_2018_muon_id_sf             = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2018_rootfiles_RunABCD_SF_ID.root:NUM_MediumID_DEN_TrackerMuons_pt_abseta"); // x=pt, y=abseta
    histmap_2018_muon_id_lowpt_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2018_rootfiles_lowpt_RunABCD_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta"); // x=pt, y=abseta
    histmap_2018_muon_tightiso_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2018_rootfiles_RunABCD_SF_ISO.root:NUM_TightRelIso_DEN_MediumID_pt_abseta"); // x=pt, y=abseta
    histmap_2018_muon_looseiso_sf       = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/EfficiencyStudies_2018_rootfiles_RunABCD_SF_ISO.root:NUM_LooseRelIso_DEN_MediumID_pt_abseta"); // x=pt, y=abseta
    histmap_2016_fake_rate_el           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_el_2016.root:fake_rate_el_data");
    histmap_2016_fake_rate_mu           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_mu_2016.root:fake_rate_mu_data");
    histmap_2017_fake_rate_el           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_el_2017.root:fake_rate_el_data");
    histmap_2017_fake_rate_mu           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_mu_2017.root:fake_rate_mu_data");
    histmap_2018_fake_rate_el           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_el_2018.root:fake_rate_el_data");
    histmap_2018_fake_rate_mu           = new RooUtil::HistMap("/nfs-7/userdata/phchang/analysis_data/scalefactors/wvz/v1/fake_rate_mu_2018.root:fake_rate_mu_data");
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

    if (doFakeEst) // if do fake overwrite the lep_Nom_idx for fake
    {

        std::vector<int> lbnt_idx; // index of good nominal leptons

        // Loop over the leptons
        for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
        {

            if (jj == (unsigned int) lep_ZCand_idx1)
                continue;

            if (jj == (unsigned int) lep_ZCand_idx2)
                continue;

            if (not passVetoLeptonID(jj))
                continue;

            if (passNominalLeptonID(jj))
                continue;

            lbnt_idx.push_back(jj);

        }

        if (lbnt_idx.size() == 1)
        {
            if (nNominalLeptons == 1)
            {
                lep_Nom_idx2 = lbnt_idx[0];
            }
            else if (nNominalLeptons == 2)
            {
                lep_Nom_idx3 = lbnt_idx[0];
            }
        }

    }

}

//______________________________________________________________________________________________
void Analysis::selectFakeStudyLeptons()
{

    nDFOS = 0;
    lep_FakeCand_idx1 = -999;
    lep_FakeCand_idx2 = -999;
    lep_NonFakeCand_idx = -999;
    lep_FakeCand_MaxIso_idx = -999;

    if (not (nVetoLeptons == 3)) return;
    if (not (nNominalLeptons >= 2)) return;

    // Check that the sum of the sign of the lepton is +/- 1
    int charge1 = (wvz.lep_id()[lep_Veto_idx1] > 0) - (wvz.lep_id()[lep_Veto_idx1] < 0);
    int charge2 = (wvz.lep_id()[lep_Veto_idx2] > 0) - (wvz.lep_id()[lep_Veto_idx2] < 0);
    int charge3 = (wvz.lep_id()[lep_Veto_idx3] > 0) - (wvz.lep_id()[lep_Veto_idx3] < 0);

    if (not (abs(charge1 + charge2 + charge3) == 1))
        return;

    // Check the flavor
    // The accepted flavors are
    // nom   veto/nom
    // e+mu- e+
    // e-mu+ e-
    // mu+e- mu+
    // mu-e+ mu-
    // Essentially two DFOS pair (or 0SFOS)
    if (wvz.lep_id()[lep_Veto_idx1] * wvz.lep_id()[lep_Veto_idx2] == -143) nDFOS++;
    if (wvz.lep_id()[lep_Veto_idx1] * wvz.lep_id()[lep_Veto_idx3] == -143) nDFOS++;
    if (wvz.lep_id()[lep_Veto_idx2] * wvz.lep_id()[lep_Veto_idx3] == -143) nDFOS++;

    if (not (nDFOS == 2))
        return;

    // Select the same-sign pair idx
    if (charge1 == charge2)
    {
        lep_FakeCand_idx1 = lep_Veto_idx1;
        lep_FakeCand_idx2 = lep_Veto_idx2;
        lep_NonFakeCand_idx = lep_Veto_idx3;
    }
    else if (charge1 == charge3)
    {
        lep_FakeCand_idx1 = lep_Veto_idx1;
        lep_FakeCand_idx2 = lep_Veto_idx3;
        lep_NonFakeCand_idx = lep_Veto_idx2;
    }
    else if (charge2 == charge3)
    {
        lep_FakeCand_idx1 = lep_Veto_idx2;
        lep_FakeCand_idx2 = lep_Veto_idx3;
        lep_NonFakeCand_idx = lep_Veto_idx1;
    }

    // the fake candidate have same flavor so checking one is fine
    if (abs(wvz.lep_id()[lep_FakeCand_idx1]) == 11)
    {
        lep_FakeCand_MaxIso_idx = wvz.lep_relIso03EA()[lep_FakeCand_idx1] > wvz.lep_relIso03EA()[lep_FakeCand_idx2] ? lep_FakeCand_idx1 : lep_FakeCand_idx2;
    }
    else
    {
        lep_FakeCand_MaxIso_idx = wvz.lep_relIso04DB()[lep_FakeCand_idx1] > wvz.lep_relIso04DB()[lep_FakeCand_idx2] ? lep_FakeCand_idx1 : lep_FakeCand_idx2;
    }

    // lep_E_idx = abs(wvz.lep_id()[lep_Veto_idx1]) == 11 ? lep_Veto_idx1 : lep_Veto_idx2;
    // lep_Mu_idx = abs(wvz.lep_id()[lep_Veto_idx1]) == 11 ? lep_Veto_idx2 : lep_Veto_idx1;
    // lep_FakeCand_idx = lep_Veto_idx3;

    // std::cout <<  " lep_E_idx: " << lep_E_idx <<  " lep_Mu_idx: " << lep_Mu_idx <<  std::endl;
    // std::cout <<  " lep_Veto_idx1: " << lep_Veto_idx1 <<  " lep_Veto_idx2: " << lep_Veto_idx2 <<  " lep_Veto_idx3: " << lep_Veto_idx3 <<  " lep_Veto_idx4: " << lep_Veto_idx4 <<  std::endl;

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
void Analysis::selectVetoButNotNomLeptons()
{
    lep_VetoButNotNom_idx = -999;
    if (nVetoLeptons != 4 and nVetoLeptons != 5) // Only select for 4 or 5 lepton regions
        return;

    // When this runs the nNominalLeptons should already been filled
    // Then check whether the z candidate is found
    if (lep_ZCand_idx1 == -999)
        return;

    // and check whether the number of nominal found is appropriate
    if (nVetoLeptons == 4 and nNominalLeptons != 1)
        return;

    // and check whether the number of nominal found is appropriate
    if (nVetoLeptons == 5 and nNominalLeptons != 2)
        return;

    // Loop over the leptons and find that one lepton that failed nominal but passed veto
    std::vector<int> good_idx;
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == (unsigned int) lep_ZCand_idx1)
            continue;

        if (jj == (unsigned int) lep_ZCand_idx2)
            continue;

        if (not passVetoLeptonID(jj))
            continue;

        if (passNominalLeptonID(jj))
            continue;

        good_idx.push_back(jj);
    }

    // Sanity check that I found only one loose but not tight
    if (not (good_idx.size() == 1))
    {
        std::cout << "ERROR! I should never be here!" << std::endl;
        std::cout << "I specifically requested that there be a presence of only one Veto-but-not-Nominal lepton but found something other than 1" << std::endl;
        std::cout << " good_idx.size(): " << good_idx.size() <<  std::endl;
        std::cout << " wvz.run(): " << wvz.run() <<  " wvz.lumi(): " << wvz.lumi() <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
        abort();
        return;
    }

    lep_VetoButNotNom_idx = good_idx[0];
    return;

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

    lep_Veto_idx1 = -999;
    lep_Veto_idx2 = -999;
    lep_Veto_idx3 = -999;
    lep_Veto_idx4 = -999;

    std::vector<MyLepton> lepvetoidx;
    for (auto& idx : lep_veto_idxs)
    {
        lepvetoidx.push_back(MyLepton(idx, leptons[idx].Pt()));
    }
    std::sort(lepvetoidx.begin(), lepvetoidx.end(), less_than_key());

    if (nVetoLeptons > 0) lep_Veto_idx1 = lepvetoidx[0].idx;
    if (nVetoLeptons > 1) lep_Veto_idx2 = lepvetoidx[1].idx;
    if (nVetoLeptons > 2) lep_Veto_idx3 = lepvetoidx[2].idx;
    if (nVetoLeptons > 3) lep_Veto_idx4 = lepvetoidx[3].idx;

    lep_veto_idxs.clear();
    lep_veto_idxs.push_back(lep_Veto_idx1);
    lep_veto_idxs.push_back(lep_Veto_idx2);
    lep_veto_idxs.push_back(lep_Veto_idx3);
    lep_veto_idxs.push_back(lep_Veto_idx4);

    // Sort Z lepton indices
    int tmp1 = lep_ZCand_idx1;
    int tmp2 = lep_ZCand_idx2;
    lep_ZCand_idx1 = leptons[lep_ZCand_idx1].Pt() > leptons[lep_ZCand_idx2].Pt() ? tmp1 : tmp2;
    lep_ZCand_idx2 = leptons[lep_ZCand_idx1].Pt() > leptons[lep_ZCand_idx2].Pt() ? tmp1 : tmp2;

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

    if (!( wvz.lep_p4()[idx].pt() > 10.           )) return false;
    if (!( fabs(wvz.lep_p4()[idx].eta()) < 2.5    )) return false;
    if (!( wvz.lep_isCutBasedIsoVetoPOG()[idx]    )) return false;
    // following is already applied in the baby making stage
    // if (fabs(wvz.els_etaSC()[idx]) <= 1.479)
    // {
    //     if (!( fabs(wvz.lep_dz()[idx]) < 0.1      )) return false;
    //     if (!( fabs(wvz.lep_dxy()[idx]) < 0.05    )) return false;
    // }
    // else
    // {
    //     if (!( fabs(wvz.lep_dz()[idx]) < 0.2      )) return false;
    //     if (!( fabs(wvz.lep_dxy()[idx]) < 0.1     )) return false;
    // }

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passVetoMuonID(int idx)
{

    if (!( wvz.lep_p4()[idx].pt() > 10.        )) return false;
    if (!( fabs(wvz.lep_p4()[idx].eta()) < 2.4 )) return false;
    if (!( wvz.lep_relIso04DB()[idx] < 0.25    )) return false;
    // following is already applied in the baby making stage
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

    // Try Medium POG ID
    if (not (wvz.lep_isMediumPOG()[idx])) return false;

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

    if (!( wvz.lep_isCutBasedIsoVetoPOG()[idx]    )) return false;
    // Must pass Veto ID
    if (not (passVetoElectronID(idx))) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passZCandMuonID(int idx)
{
    if (!( wvz.lep_relIso04DB()[idx] < 0.25    )) return false;
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
            return fixXsec * evt_scale1fb * 35.9 * getTruePUw2016(wvz.nTrueInt());
        else if (year == 2017)
            return fixXsec * evt_scale1fb * 41.3 * getTruePUw2017(wvz.nTrueInt());
        else if (year == 2018)
            return fixXsec * evt_scale1fb * 59.74 * getTruePUw2018(wvz.nTrueInt());
        else
            return fixXsec * evt_scale1fb * 137;
    }
}

//______________________________________________________________________________________________
float Analysis::LeptonScaleFactorv1()
{
    if (wvz.isData())
        return 1.;
    // Based on lep_Veto indices
    float scalefactor = 1;
    for (auto& lep_idx : lep_veto_idxs)
    {
        int absid = abs(wvz.lep_id()[lep_idx]);
        float pt = absid == 11 ? std::min((double) wvz.lep_pt()[lep_idx], 499.9) : std::min((double) wvz.lep_pt()[lep_idx], 119.9);
        float eta = wvz.lep_eta()[lep_idx];
        float abseta = fabs(eta);
        if (absid == 11)
        {
            if (year == 2016)
            {
                if (pt > 20)
                {
                    scalefactor *= histmap_2016_elec_reco_highpt_sf    ->eval(eta, pt); // x=eta, y=pt
                }
                else
                {
                    scalefactor *= histmap_2016_elec_reco_lowpt_sf     ->eval(eta, pt); // x=eta, y=pt
                }
                if (passNominalElectronID(lep_idx))
                {
                    scalefactor *= histmap_2016_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
                }
                else
                {
                    scalefactor *= histmap_2016_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
                }
            }
            else if (year == 2017)
            {
                if (pt > 20)
                {
                    scalefactor *= histmap_2017_elec_reco_highpt_sf    ->eval(eta, pt); // x=eta, y=pt
                }
                else
                {
                    scalefactor *= histmap_2017_elec_reco_lowpt_sf     ->eval(eta, pt); // x=eta, y=pt
                }
                if (passNominalElectronID(lep_idx))
                {
                    scalefactor *= histmap_2017_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
                }
                else
                {
                    scalefactor *= histmap_2017_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
                }
            }
            else if (year == 2018)
            {
                scalefactor *= histmap_2018_elec_reco_sf           ->eval(eta, pt); // x=eta, y=pt
                if (passNominalElectronID(lep_idx))
                {
                    scalefactor *= histmap_2018_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
                }
                else
                {
                    scalefactor *= histmap_2018_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
                }
            }
        }
        else if (absid == 13)
        {
            if (year == 2016)
            {
                if (pt > 20)
                {
                    scalefactor *= 0.550 * histmap_2016_muon_BCDEF_id_sf       ->eval(eta, pt)  // x=eta, y=pt
                                  +0.450 * histmap_2016_muon_GH_id_sf          ->eval(eta, pt); // x=eta, y=pt 0.450 of 2016 data
                }
                else
                {
                    scalefactor *= 0.550 * histmap_2016_muon_BCDEF_id_lowpt_sf ->eval(pt, abseta)  // x=pt, y=abseta
                                  +0.450 * histmap_2016_muon_GH_id_lowpt_sf    ->eval(pt, abseta); // x=pt, y=abseta 0.450 of 2016 data
                }
                if (passNominalMuonID(lep_idx))
                {
                    scalefactor *= 0.550 * histmap_2016_muon_BCDEF_tightiso_sf ->eval(eta, std::max((double) pt, 20.1))  // x=eta, y=pt
                                  +0.450 * histmap_2016_muon_GH_tightiso_sf    ->eval(eta, std::max((double) pt, 20.1)); // x=eta, y=pt 0.450 of 2016 data
                }
                else
                {
                    scalefactor *= 0.550 * histmap_2016_muon_BCDEF_looseiso_sf ->eval(eta, std::max((double) pt, 20.1))  // x=eta, y=pt
                                  +0.450 * histmap_2016_muon_GH_looseiso_sf    ->eval(eta, std::max((double) pt, 20.1)); // x=eta, y=pt 0.450 of 2016 data
                }
            }
            else if (year == 2017)
            {
                if (pt > 20)
                {
                    scalefactor *= histmap_2017_muon_id_sf             ->eval(pt, abseta); // x=pt, y=abseta
                }
                else
                {
                    scalefactor *= histmap_2017_muon_id_lowpt_sf       ->eval(pt, abseta); // x=pt, y=abseta
                }
                if (passNominalMuonID(lep_idx))
                {
                    scalefactor *= histmap_2017_muon_tightiso_sf       ->eval(std::max((double) pt, 20.1), abseta); // x=pt, y=abseta
                }
                else
                {
                    scalefactor *= histmap_2017_muon_looseiso_sf       ->eval(std::max((double) pt, 20.1), abseta); // x=pt, y=abseta
                }
            }
            else if (year == 2018)
            {
                if (pt > 20)
                {
                    scalefactor *= histmap_2018_muon_id_sf             ->eval(pt, abseta); // x=pt, y=abseta
                }
                else
                {
                    scalefactor *= histmap_2018_muon_id_lowpt_sf       ->eval(pt, abseta); // x=pt, y=abseta
                }
                if (passNominalMuonID(lep_idx))
                {
                    scalefactor *= histmap_2018_muon_tightiso_sf       ->eval(std::max((double) pt, 15.1), abseta); // x=pt, y=abseta
                }
                else
                {
                    scalefactor *= histmap_2018_muon_looseiso_sf       ->eval(std::max((double) pt, 15.1), abseta); // x=pt, y=abseta
                }
            }
        }
    }
    return scalefactor;
}

//______________________________________________________________________________________________
float Analysis::LeptonScaleFactor()
{
    if (wvz.isData())
        return 1.;
    // Based on lep_Veto indices
    float scalefactor = 1;
    scalefactor *= IndividualLeptonScaleFactor(lep_ZCand_idx1, false);
    scalefactor *= IndividualLeptonScaleFactor(lep_ZCand_idx2, false);
    scalefactor *= IndividualLeptonScaleFactor(lep_Nom_idx1, true);
    scalefactor *= IndividualLeptonScaleFactor(lep_Nom_idx2, true);
    return scalefactor;
}

//______________________________________________________________________________________________
float Analysis::IndividualLeptonScaleFactor(int lep_idx, bool isNominal)
{
    if (wvz.isData())
        return 1.;
    // Based on lep_Veto indices
    float scalefactor = 1;
    if (lep_idx < 0)
        return 1;
    int absid = abs(wvz.lep_id()[lep_idx]);
    float pt = absid == 11 ? std::min((double) wvz.lep_pt()[lep_idx], 499.9) : std::min((double) wvz.lep_pt()[lep_idx], 119.9);
    float eta = wvz.lep_eta()[lep_idx];
    float abseta = fabs(eta);
    if (absid == 11)
    {
        if (year == 2016)
        {
            if (pt > 20)
            {
                scalefactor *= histmap_2016_elec_reco_highpt_sf    ->eval(eta, pt); // x=eta, y=pt
            }
            else
            {
                scalefactor *= histmap_2016_elec_reco_lowpt_sf     ->eval(eta, pt); // x=eta, y=pt
            }
            if (isNominal)
            {
                scalefactor *= histmap_2016_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
            }
            else
            {
                scalefactor *= histmap_2016_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
            }
        }
        else if (year == 2017)
        {
            if (pt > 20)
            {
                scalefactor *= histmap_2017_elec_reco_highpt_sf    ->eval(eta, pt); // x=eta, y=pt
            }
            else
            {
                scalefactor *= histmap_2017_elec_reco_lowpt_sf     ->eval(eta, pt); // x=eta, y=pt
            }
            if (isNominal)
            {
                scalefactor *= histmap_2017_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
            }
            else
            {
                scalefactor *= histmap_2017_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
            }
        }
        else if (year == 2018)
        {
            scalefactor *= histmap_2018_elec_reco_sf           ->eval(eta, pt); // x=eta, y=pt
            if (isNominal)
            {
                scalefactor *= histmap_2018_elec_medium_sf         ->eval(eta, pt); // x=eta, y=pt
            }
            else
            {
                scalefactor *= histmap_2018_elec_veto_sf           ->eval(eta, pt); // x=eta, y=pt
            }
        }
    }
    else if (absid == 13)
    {
        if (year == 2016)
        {
            if (pt > 20)
            {
                scalefactor *= 0.550 * histmap_2016_muon_BCDEF_id_sf       ->eval(eta, pt)  // x=eta, y=pt
                    +0.450 * histmap_2016_muon_GH_id_sf          ->eval(eta, pt); // x=eta, y=pt 0.450 of 2016 data
            }
            else
            {
                scalefactor *= 0.550 * histmap_2016_muon_BCDEF_id_lowpt_sf ->eval(pt, abseta)  // x=pt, y=abseta
                    +0.450 * histmap_2016_muon_GH_id_lowpt_sf    ->eval(pt, abseta); // x=pt, y=abseta 0.450 of 2016 data
            }
            if (isNominal)
            {
                scalefactor *= 0.550 * histmap_2016_muon_BCDEF_tightiso_sf ->eval(eta, std::max((double) pt, 20.1))  // x=eta, y=pt
                    +0.450 * histmap_2016_muon_GH_tightiso_sf    ->eval(eta, std::max((double) pt, 20.1)); // x=eta, y=pt 0.450 of 2016 data
            }
            else
            {
                scalefactor *= 0.550 * histmap_2016_muon_BCDEF_looseiso_sf ->eval(eta, std::max((double) pt, 20.1))  // x=eta, y=pt
                    +0.450 * histmap_2016_muon_GH_looseiso_sf    ->eval(eta, std::max((double) pt, 20.1)); // x=eta, y=pt 0.450 of 2016 data
            }
        }
        else if (year == 2017)
        {
            if (pt > 20)
            {
                scalefactor *= histmap_2017_muon_id_sf             ->eval(pt, abseta); // x=pt, y=abseta
            }
            else
            {
                scalefactor *= histmap_2017_muon_id_lowpt_sf       ->eval(pt, abseta); // x=pt, y=abseta
            }
            if (isNominal)
            {
                scalefactor *= histmap_2017_muon_tightiso_sf       ->eval(std::max((double) pt, 20.1), abseta); // x=pt, y=abseta
            }
            else
            {
                scalefactor *= histmap_2017_muon_looseiso_sf       ->eval(std::max((double) pt, 20.1), abseta); // x=pt, y=abseta
            }
        }
        else if (year == 2018)
        {
            if (pt > 20)
            {
                scalefactor *= histmap_2018_muon_id_sf             ->eval(pt, abseta); // x=pt, y=abseta
            }
            else
            {
                scalefactor *= histmap_2018_muon_id_lowpt_sf       ->eval(pt, abseta); // x=pt, y=abseta
            }
            if (isNominal)
            {
                scalefactor *= histmap_2018_muon_tightiso_sf       ->eval(std::max((double) pt, 15.1), abseta); // x=pt, y=abseta
            }
            else
            {
                scalefactor *= histmap_2018_muon_looseiso_sf       ->eval(std::max((double) pt, 15.1), abseta); // x=pt, y=abseta
            }
        }
    }
    return scalefactor;
}

//______________________________________________________________________________________________
float Analysis::BTagSF()
{
    if (wvz.isData())
        return 1.;
    return wvz.weight_btagsf();
}

//______________________________________________________________________________________________
float Analysis::FakeFactor()
{
    if (doFakeEst)
    {
        float fr = 0;
        if (year == 2016)
        {
            if (abs(wvz.lep_id()[lep_VetoButNotNom_idx]) == 11)
                fr = histmap_2016_fake_rate_el->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
            else
                fr = histmap_2016_fake_rate_mu->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
        }
        else if (year == 2017)
        {
            if (abs(wvz.lep_id()[lep_VetoButNotNom_idx]) == 11)
                fr = histmap_2017_fake_rate_el->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
            else
                fr = histmap_2017_fake_rate_mu->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
        }
        else if (year == 2018)
        {
            if (abs(wvz.lep_id()[lep_VetoButNotNom_idx]) == 11)
                fr = histmap_2018_fake_rate_el->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
            else
                fr = histmap_2018_fake_rate_mu->eval(std::min((double) leptons[lep_VetoButNotNom_idx].Pt(), 69.999), fabs(leptons[lep_VetoButNotNom_idx].Eta()));
        }
        if (wvz.isData())
        {
            return fr / (1. - fr);
        }
        else
        {
            if (wvz.lep_motherIdv2()[lep_VetoButNotNom_idx] >= 1)
                return - fr / (1. - fr);
            else
                return 0;
        }
    }
    else
    {
        return 1.;
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
    if (doFakeEst)
    {
        if (not (nNominalLeptons == 1))
            return false;
    }
    else
    {
        if (not (nNominalLeptons == 2))
            return false;
    }
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(lep_Nom_idx2) > 0)
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::FindOSOneNomOneVbntLeptons()
{
    if (lep_Nom_idx1 == -999)
        return false;
    if (lep_VetoButNotNom_idx == -999)
        return false;
    if (nNominalLeptons != 1)
        return false;
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(lep_VetoButNotNom_idx) > 0)
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepLowMll(bool isAR)
{
    if (nVetoLeptons < 4)
        return false;

    std::vector<int> indices;
    indices.push_back(lep_Nom_idx1);
    if (isAR)
        indices.push_back(lep_VetoButNotNom_idx);
    else
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
bool Analysis::Cut4LepBVeto(int var)
{
    if (var == 0)
    {
        return wvz.nb() == 0;
    }
    else if (var == 1)
    {
        return wvz.nb_up() == 0;
    }
    else if (var ==-1)
    {
        return wvz.nb_dn() == 0;
    }
    else
    {
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "Cut4LepBVeto");
        return false;
    }
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepBTag(int var)
{
    if (var == 0)
    {
        return wvz.nb() >= 1;
    }
    else if (var == 1)
    {
        return wvz.nb_up() >= 1;
    }
    else if (var ==-1)
    {
        return wvz.nb_dn() >= 1;
    }
    else
    {
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "Cut4LepBTag");
        return false;
    }
}

//______________________________________________________________________________________________
bool Analysis::CutHighMT(int var)
{
    if (not (VarMTNom0(var) > 40.)) return false;
    if (not (VarMTNom1(var) > 20.)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepLeptonPt(bool isAR)
{
    if (nVetoLeptons < 4)
        return false;

    float lepZpt1 = leptons[lep_ZCand_idx1].Pt();
    float lepZpt2 = leptons[lep_ZCand_idx2].Pt();
    float lepNpt1 = leptons[lep_Nom_idx1].Pt();
    float lepNpt2 = isAR ? leptons[lep_VetoButNotNom_idx].Pt() : leptons[lep_Nom_idx2].Pt();
    float tmp1 = lepNpt1;
    float tmp2 = lepNpt2;
    lepNpt1 = lepNpt1 > lepNpt2 ? tmp1 : tmp2;
    lepNpt2 = lepNpt1 > lepNpt2 ? tmp2 : tmp1;

    if (lepZpt1 > 25. and lepZpt2 > 10. and lepNpt1 > 25. and lepNpt2 > 10.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::CutHLT()
{
    if (nVetoLeptons < 2)
        return false;
    if (wvz.isData())
        if (not wvz.pass_duplicate_mm_em_ee())
            return false;
    bool passTrigger = false;
    for (auto& lep_idx : lep_veto_idxs)
    {
        for (auto& lep_jdx : lep_veto_idxs)
        {
            if (lep_idx == lep_jdx)
                continue;
            // Check if any of the combination of leptons pass the trigger thresholds
            // Ele 23 12
            // El23 Mu8
            // Mu23 El12
            // Mu 17 8
            // The thresholds are rounded up to 25, 15, or 10
            if (abs(wvz.lep_id()[lep_idx]) == 11 and abs(wvz.lep_id()[lep_jdx]) == 11)
                passTrigger |= (wvz.HLT_DoubleEl() and wvz.lep_pt()[lep_idx] > 25 and wvz.lep_pt()[lep_jdx] > 15);
            else if (abs(wvz.lep_id()[lep_idx]) == 13 and abs(wvz.lep_id()[lep_jdx]) == 11)
                passTrigger |= (wvz.HLT_MuEG() and wvz.lep_pt()[lep_idx] > 25 and wvz.lep_pt()[lep_jdx] > 15);
            else if (abs(wvz.lep_id()[lep_idx]) == 11 and abs(wvz.lep_id()[lep_jdx]) == 13)
                passTrigger |= (wvz.HLT_MuEG() and wvz.lep_pt()[lep_idx] > 25 and wvz.lep_pt()[lep_jdx] > 10);
            else if (abs(wvz.lep_id()[lep_idx]) == 13 and abs(wvz.lep_id()[lep_jdx]) == 13)
                passTrigger |= (wvz.HLT_DoubleMu() and wvz.lep_pt()[lep_idx] > 20 and wvz.lep_pt()[lep_jdx] > 10);
        }
    }
    return passTrigger;
}

//______________________________________________________________________________________________
bool Analysis::Is3LeptonEvent()
{
    if (not (nVetoLeptons == 3)) return false;
    if (not (CutHLT())) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Is4LeptonEvent()
{
    return nVetoLeptons == 4;
}

//______________________________________________________________________________________________
bool Analysis::Is5LeptonEvent()
{
    if (not (nVetoLeptons == 5)) return false;
    if (not (CutHLT())) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::IsEMuPlusX()
{
    if (not (nVetoLeptons == 3)) return false;
    if (not (nNominalLeptons >= 2)) return false;

    // Check that the sum of the sign of the lepton is +/- 1
    int charge1 = (wvz.lep_id()[lep_Veto_idx1] > 0) - (wvz.lep_id()[lep_Veto_idx1] < 0);
    int charge2 = (wvz.lep_id()[lep_Veto_idx2] > 0) - (wvz.lep_id()[lep_Veto_idx2] < 0);
    int charge3 = (wvz.lep_id()[lep_Veto_idx3] > 0) - (wvz.lep_id()[lep_Veto_idx3] < 0);

    if (not (abs(charge1 + charge2 + charge3) == 1))
        return false;

    if (not (nDFOS == 2))
        return false;

    // Ask for the different flavor lepton to be tight
    if (not (passNominalLeptonID(lep_NonFakeCand_idx)))
        return false;

    // Ask for the leading leg of the same-sign pair is tight
    if (not (passNominalLeptonID(lep_FakeCand_idx1)))
        return false;

    if (not (wvz.nb() == 1))
        return false;

    return true;
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
bool Analysis::IsChannelEMu(bool isAR)
{
    int idx2 = isAR ? lep_VetoButNotNom_idx : lep_Nom_idx2;
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(idx2) == -143)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOnZ(bool isAR)
{
    int idx2 = isAR ? lep_VetoButNotNom_idx : lep_Nom_idx2;
    float mll = isAR ? (leptons[lep_VetoButNotNom_idx]+leptons[lep_Nom_idx1]).M() : dilepNominal.M();
    if (fabs(mll - 91.1876) < 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOffZ(bool isAR)
{
    int idx2 = isAR ? lep_VetoButNotNom_idx : lep_Nom_idx2;
    if (fabs(dilepNominal.M() - 91.1876) >= 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
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
bool Analysis::ChannelEMuHighMll(bool isAR)
{
    float mll = isAR ? (leptons[lep_Nom_idx1]+leptons[lep_VetoButNotNom_idx]).M() : dilepNominal.M();
    if (mll > 120.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::CutHighMET(int var)
{
    if (var == 0)
    {
        if (wvz.met_pt() > 100.)
            return true;
        else
            return false;
    }
    else if (var == 1)
    {
        if (wvz.met_up_pt() > 100.)
            return true;
        else
            return false;
    }
    else if (var ==-1)
    {
        if (wvz.met_dn_pt() > 100.)
            return true;
        else
            return false;
    }
    else
    {
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "CutHighMET");
        return false;
    }
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
    return wvz.met_pt();
}

//______________________________________________________________________________________________
float Analysis::VarNvtx()
{
    return wvz.nvtx();
}

//______________________________________________________________________________________________
float Analysis::VarMT(int idx, int var)
{
    if (var == 0)
    {
        return sqrt(2 * wvz.met_pt() * leptons[idx].Et() * (1.0 - cos(leptons[idx].Phi() - wvz.met_phi())));
    }
    else if (var == 1)
    {
        return sqrt(2 * wvz.met_up_pt() * leptons[idx].Et() * (1.0 - cos(leptons[idx].Phi() - wvz.met_up_phi())));
    }
    else if (var ==-1)
    {
        return sqrt(2 * wvz.met_dn_pt() * leptons[idx].Et() * (1.0 - cos(leptons[idx].Phi() - wvz.met_dn_phi())));
    }
    else
    {
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "VarMT");
        return false;
    }
}

//______________________________________________________________________________________________
float Analysis::VarMT5th(int var) { return VarMT(lep_WCand_idx1, var); }
float Analysis::VarMTNom0(int var) { return VarMT(lep_Nom_idx1, var); }
float Analysis::VarMTNom1(int var) { return VarMT(lep_Nom_idx2, var); }
float Analysis::VarMTMax(int var) { return std::max((double) VarMT(lep_Nom_idx1, var), (double) VarMT(lep_Nom_idx2, var)); }
float Analysis::VarMTMin(int var) { return std::min((double) VarMT(lep_Nom_idx1, var), (double) VarMT(lep_Nom_idx2, var)); }

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

//______________________________________________________________________________________________
float Analysis::VarNSFOS()
{
    if (nVetoLeptons != 3)
        return -999;
    int nSFOS = 0;
    if (wvz.lep_id()[lep_Veto_idx1] == -wvz.lep_id()[lep_Veto_idx2]) nSFOS++;
    if (wvz.lep_id()[lep_Veto_idx1] == -wvz.lep_id()[lep_Veto_idx3]) nSFOS++;
    if (wvz.lep_id()[lep_Veto_idx2] == -wvz.lep_id()[lep_Veto_idx3]) nSFOS++;
    return nSFOS;
}

// eof
