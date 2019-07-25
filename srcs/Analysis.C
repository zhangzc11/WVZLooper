#define Analysis_C
#include "Analysis.h"
#include "puw2016.h"
#include "puw2017.h"
#include "puw2018.h"
#include "METCorrectionHandler.cc"
#include "METObject.cc"

#include "xgboost/wwz_vs_ttz_bVeto.h"
#include "xgboost/wwz_vs_ttz_emu.h"
#include "xgboost/wwz_vs_ttz_nbAll.h"
#include "xgboost/wwz_vs_ttz_OffZ.h"
#include "xgboost/wwz_vs_ttzzz_bVeto.h"
#include "xgboost/wwz_vs_ttzzz_emu.h"
#include "xgboost/wwz_vs_ttzzz_OffZ.h"
#include "xgboost/wwz_vs_zz_emu.h"
#include "xgboost/wwz_vs_zz_emuHighTTZBDT.h"
#include "xgboost/wwz_vs_zz_OffZ.h"
#include "xgboost/wwz_vs_zz_OffZHighTTZBDT.h"

TheoryWeight theoryweight;

//______________________________________________________________________________________________
// e.g.
// NtupleVersion = "WVZ2018_v0.0.9"
// TagName = "LoopTag1"
void Analysis::Loop(const char* NtupleVersion, const char* TagName, bool dosyst, bool doskim)
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

    // Whether to run systematic variations as well or not
    doSyst = dosyst;
    // Whether to run skimming or not
    doSkim = doskim;

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

    // Load scale factor histograms (year needs to be set prior)
    loadScaleFactors();

    // The RooUtil::Cutflow object facilitates various cutflow/histogramming
    RooUtil::Cutflow cutflow(output_file);
    cutflow.addCut("EventWeight", [&](){ return 1; }, [&](){ return this->EventWeight(); } );
    cutflow.addCutToLastActiveCut("GenFilter", [&](){ return this->CutGenFilter(); }, UNITY );
    cutflow.addCutToLastActiveCut("Weight", [&](){ return 1; }, UNITY );

    // There are two types of NtupleVersion
    // 1. WVZ201*_v* which only contains events with 4 or more leptons
    // 2. Trilep201*_v* which also contains trilepton events in addition to 4 or more  lepton events

    //========================
    //
    // Defining cuts
    //
    //========================

    if (ntupleVersion.Contains("WVZ"))
    {
        // List of common four lepton related selections
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("FourLeptons"             , [&](){ return this->Is4LeptonEvent();          } , [&](){ return this->LeptonScaleFactor(); } );
        cutflow.addCutToLastActiveCut("CutHLT"                  , [&](){ return this->CutHLT();                  } , UNITY );
        cutflow.addCutToLastActiveCut("FindTwoOSNominalLeptons" , [&](){ return this->FindTwoOSNominalLeptons(); } , UNITY );
        cutflow.addCutToLastActiveCut("Cut4LepLowMll"           , [&](){ return this->Cut4LepLowMll();           } , UNITY );
        cutflow.addCutToLastActiveCut("Cut4LepLeptonPt"         , [&](){ return this->Cut4LepLeptonPt();         } , UNITY );
        cutflow.addCutToLastActiveCut("FindZCandLeptons"        , [&](){ return this->FindZCandLeptons();        } , UNITY );
        cutflow.addCutToLastActiveCut("Cut4LepBVeto"            , [&](){ return this->Cut4LepBVeto();            } , [&](){ return this->BTagSF();            } );

        // emu channel
        cutflow.getCut("Cut4LepBVeto");
        cutflow.addCutToLastActiveCut("ChannelEMu"              , [&](){ return this->IsChannelEMu();            } , UNITY );
        cutflow.addCutToLastActiveCut("ChannelEMuHighMT"        , [&](){ return this->CutHighMT();               } , UNITY );
        cutflow.getCut("ChannelEMu");
        cutflow.addCutToLastActiveCut("ChannelEMuLowPtZeta"     , [&](){ return this->CutLowPtZeta();            } , UNITY );

        // OnZ channel
        cutflow.getCut("Cut4LepBVeto");
        cutflow.addCutToLastActiveCut("ChannelOnZ"              , [&](){ return this->IsChannelOnZ();            } , UNITY );
        cutflow.addCutToLastActiveCut("ChannelOnZNjet"          , [&](){ return this->IsNjetGeq2();              } , UNITY );

        // OffZ channel
        cutflow.getCut("Cut4LepBVeto");
        cutflow.addCutToLastActiveCut("ChannelOffZ"             , [&](){ return this->IsChannelOffZ();           } , UNITY );
        cutflow.addCutToLastActiveCut("ChannelOffZHighMET"      , [&](){ return this->CutHighMET();              } , UNITY );
        cutflow.getCut("ChannelOffZ");
        cutflow.addCutToLastActiveCut("ChannelOffZLowMET"       , [&](){ return not this->CutHighMET();          } , UNITY );
        cutflow.getCut("ChannelOffZ");
        cutflow.addCutToLastActiveCut("ChannelOffZHighMT"       , [&](){ return this->CutHighMT();               } , UNITY );
        cutflow.getCut("ChannelOffZ");
        cutflow.addCutToLastActiveCut("ChannelOffZLowMT"        , [&](){ return not this->CutHighMT();           } , UNITY );

        // OffZ Low/High Mll
        cutflow.getCut("ChannelOffZ");
        cutflow.addCutToLastActiveCut("ChannelOffZLowMll"        , [&](){ return this->IsChannelOffZLowMll();     } , UNITY );
        cutflow.getCut("ChannelOffZLowMll");
        cutflow.addCutToLastActiveCut("ChannelOffZLowMllHighMET" , [&](){ return this->CutHighMET();              } , UNITY );
        cutflow.getCut("ChannelOffZLowMll");
        cutflow.addCutToLastActiveCut("ChannelOffZLowMllLowMET"  , [&](){ return not this->CutHighMET();          } , UNITY );
        cutflow.getCut("ChannelOffZ");
        cutflow.addCutToLastActiveCut("ChannelOffZHighMll"       , [&](){ return this->IsChannelOffZHighMll();    } , UNITY );
        cutflow.getCut("ChannelOffZHighMll");
        cutflow.addCutToLastActiveCut("ChannelOffZHighMllHighMET", [&](){ return this->CutHighMET();              } , UNITY );
        cutflow.getCut("ChannelOffZHighMll");
        cutflow.addCutToLastActiveCut("ChannelOffZHighMllLowMET" , [&](){ return not this->CutHighMET();          } , UNITY );

        // OnZ Control regions
        cutflow.getCut("ChannelOnZ");
        cutflow.addCutToLastActiveCut("ChannelOnZHighMET"       , [&](){ return this->CutHighMET();              } , UNITY );
        cutflow.getCut("ChannelOnZ");
        cutflow.addCutToLastActiveCut("ChannelOnZLowMET"        , [&](){ return not this->CutHighMET();          } , UNITY );
        cutflow.getCut("ChannelOnZ");
        cutflow.addCutToLastActiveCut("ChannelOnZHighMT"        , [&](){ return this->CutHighMT();               } , UNITY );
        cutflow.getCut("ChannelOnZ");
        cutflow.addCutToLastActiveCut("ChannelOnZLowMT"         , [&](){ return not this->CutHighMT();           } , UNITY );

        // Common B-tagged preselection
        cutflow.getCut("Cut4LepLowMll");
        cutflow.addCutToLastActiveCut("Cut4LepBTag"             , [&](){ return this->Cut4LepBTag();             } , [&](){ return this->BTagSF();            } );

        //HZZ4l Control regions
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("FourLeptonsZZ4l"         , [&](){ return this->Is4LeptonEvent();          } , [&](){ return this->LeptonScaleFactorZZ4l(); } );
        cutflow.addCutToLastActiveCut("FindZCandLeptonsZZ4l"    , [&](){ return this->FindZCandLeptons();        } , UNITY );
        cutflow.addCutToLastActiveCut("FindTwoOSZ2Leptons"      , [&](){ return this->FindTwoOSZ2Leptons();      } , UNITY );
        cutflow.addCutToLastActiveCut("CutZZ4LepLeptonPt"       , [&](){ return this->CutZZ4LepLeptonPt();       } , UNITY );
        cutflow.addCutToLastActiveCut("CutHLTZZ4l"              , [&](){ return this->CutHLT();                  } , UNITY );
        cutflow.addCutToLastActiveCut("ChannelHZZ4l"            , [&](){ return this->IsChannelHZZ4l();          } , UNITY );

        // b-tagged emu channel
        cutflow.getCut("Cut4LepBTag");
        cutflow.addCutToLastActiveCut("ChannelBTagEMu"          , [&](){ return this->IsChannelEMu();            } , UNITY );
        cutflow.addCutToLastActiveCut("ChannelBTagEMuHighMT"    , [&](){ return this->CutHighMT();               } , UNITY );
        cutflow.getCut("ChannelBTagEMu");
        cutflow.addCutToLastActiveCut("ChannelBTagEMuHighMET"   , [&](){ return this->CutHighMET();              } , UNITY );
        cutflow.getCut("ChannelBTagEMu");
        cutflow.addCutToLastActiveCut("ChannelBTagEMuOneNb"     , [&](){ return wvz.nb() == 1;                   } , UNITY );

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
        cutflow.addCutToLastActiveCut("FiveLeptons"             , [&](){ return this->Is5LeptonEvent();          } , [&](){ return this->LeptonScaleFactor(); } );
        cutflow.addCutToLastActiveCut("FiveLeptonsMllZ"         , [&](){ return this->Is2ndOnZFiveLepton();      } , UNITY );
        cutflow.addCutToLastActiveCut("FiveLeptonsRelIso5th"    , [&](){ return this->Is5thNominal();            } , UNITY );
        cutflow.addCutToLastActiveCut("FiveLeptonsMT5th"        , [&](){ return this->VarMT5th() > 30.;          } , UNITY );

        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("ARFindZCandLeptons"            , [&](){ return this->FindZCandLeptons();        } , UNITY );
        cutflow.addCutToLastActiveCut("ARFindOSOneNomOneNotNomLeptons", [&](){ return this->FindOSOneNomOneNotNomLeptons(); }, UNITY );
        cutflow.addCutToLastActiveCut("ARCutHLT", [&](){ return this->CutHLT(); }, UNITY );
        cutflow.addCutToLastActiveCut("ARCut4LepLowMll", [&](){ return this->Cut4LepLowMll(true); }, UNITY );
        cutflow.addCutToLastActiveCut("ARCut4LepBVeto", [&](){ return this->Cut4LepBVeto(); }, [&](){ return this->BTagSF(); } );
        cutflow.getCut("ARCut4LepBVeto");
        cutflow.addCutToLastActiveCut("ChannelAREMu", [&](){ return this->IsChannelEMu(true); }, UNITY );
        cutflow.addCutToLastActiveCut("ChannelAREMuHighMT", [&](){ return this->CutHighMTAR(true); }, UNITY );
        cutflow.getCut("ARCut4LepBVeto");
        cutflow.addCutToLastActiveCut("ChannelAROffZ", [&](){ return this->IsChannelOffZ(true); }, UNITY );
        cutflow.addCutToLastActiveCut("ChannelAROffZHighMET", [&](){ return this->CutHighMET(); }, UNITY );

	if (ntupleVersion.Contains("BDT"))
        {
                //Option A: emu channel: cut on disc_ttz_emu, fit on disc_zz_emu
                cutflow.getCut("ChannelEMu");
                cutflow.addCutToLastActiveCut("ChannelEMuHighBDTttZemu"       		, [&](){ return this->VarXGBBDT(1) > 0.987;             	} , UNITY );
                cutflow.addCutToLastActiveCut("ChannelEMuHighMTHighBDTttZemu"      	, [&](){ return this->CutHighMT();                   	} , UNITY );

                //Option B: replace bVeto with disc_ttz_nbAll cut; and do fit analysis on disc_zz_emuHighTTZBDT disc_zz_OffZHighTTZBDT
                //For option B: make new EMu channel and OffZ channel
                cutflow.getCut("FindZCandLeptons");
                cutflow.addCutToLastActiveCut("Cut4LepHighBDTttZnbAll"          	, [&](){ return this->VarXGBBDT(2) > 0.960;           } , UNITY );
                cutflow.addCutToLastActiveCut("ChannelEMuHighBDTttZnbAll"               , [&](){ return this->IsChannelEMu();                   } , UNITY );
                cutflow.addCutToLastActiveCut("ChannelEMuHighMTHighBDTttZnbAll"      	, [&](){ return this->CutHighMT();                 	} , UNITY );

                cutflow.getCut("Cut4LepHighBDTttZnbAll");
                cutflow.addCutToLastActiveCut("ChannelOffZHighBDTttZnbAll"           	, [&](){ return this->IsChannelOffZ();             	} , UNITY );
                cutflow.addCutToLastActiveCut("ChannelOffZHighMETHighBDTttZnbAll"    	, [&](){ return this->CutHighMET();                	} , UNITY );

        }
	
    }
    // For fake rate related studies
    else if (ntupleVersion.Contains("Trilep"))
    {

        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("ThreeLeptons"              , [&]() { return this->Is3LeptonEvent(); }                       , [&](){ return this->LeptonScaleFactor(); } );
        cutflow.addCutToLastActiveCut("EMuPlusX"                  , [&]() { return this->IsEMuPlusX(); }                           , UNITY );
        cutflow.getCut("EMuPlusX");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMu"            , [&]() { return abs(wvz.lep_id().at(lep_FakeCand_idx2)) == 13; }   , UNITY );
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTight"       , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.getCut("EMuPlusXFakeMu");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuPrompt"      , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeMuTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTightPrompt" , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeMu");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuFake"        , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) < 1; }  , UNITY );
        cutflow.getCut("EMuPlusXFakeMuTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeMuTightFake"   , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) < 1; }  , UNITY );
        cutflow.getCut("EMuPlusX");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeEl"            , [&]() { return abs(wvz.lep_id().at(lep_FakeCand_idx2)) == 11; }   , UNITY );
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTight"       , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.getCut("EMuPlusXFakeEl");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElPrompt"      , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeElTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTightPrompt" , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("EMuPlusXFakeEl");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElFake"        , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) < 1; }  , UNITY );
        cutflow.getCut("EMuPlusXFakeElTight");
        cutflow.addCutToLastActiveCut("EMuPlusXFakeElTightFake"   , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) < 1; }  , UNITY );

        cutflow.getCut("ThreeLeptons");
        cutflow.addCutToLastActiveCut("DYPlusX"                   , [&]() { return this->IsDYPlusX(); }                           , UNITY );

        cutflow.getCut("DYPlusX");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMu"             , [&]() { return abs(wvz.lep_id().at(lep_FakeCand_idx2)) == 13; }   , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuTight"        , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuTightHighMT"  , [&]() { return this->VarMT(lep_FakeCand_idx2) > 70.; }         , UNITY );
        cutflow.getCut("DYPlusXFakeMu");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMR"           , [&]() { return this->VarMT(lep_FakeCand_idx2) < 35.; } , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMRTight"      , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); } , UNITY );
        cutflow.getCut("DYPlusXFakeMuMR");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMRPrompt"     , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("DYPlusXFakeMuMRTight");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMRTightPrompt", [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("DYPlusXFakeMuMR");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMRFake"       , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) <  1; } , UNITY );
        cutflow.getCut("DYPlusXFakeMuMRTight");
        cutflow.addCutToLastActiveCut("DYPlusXFakeMuMRTightFake"  , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) <  1; } , UNITY );

        cutflow.getCut("DYPlusX");
        cutflow.addCutToLastActiveCut("DYPlusXFakeEl"             , [&]() { return abs(wvz.lep_id().at(lep_FakeCand_idx2)) == 11; }   , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeElTight"        , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); }       , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeElTightHighMT"  , [&]() { return (this->VarMT(lep_FakeCand_idx2) > 80.) and this->VarMET() > 80.; }         , UNITY );
        cutflow.getCut("DYPlusXFakeEl");
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMR"           , [&]() { return this->VarMT(lep_FakeCand_idx2) < 45.; } , UNITY );
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMRTight"      , [&]() { return passNominalLeptonID(lep_FakeCand_idx2); } , UNITY );
        cutflow.getCut("DYPlusXFakeElMR");
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMRPrompt"     , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("DYPlusXFakeElMRTight");
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMRTightPrompt", [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) >= 1; } , UNITY );
        cutflow.getCut("DYPlusXFakeElMR");
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMRFake"       , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) <  1; } , UNITY );
        cutflow.getCut("DYPlusXFakeElMRTight");
        cutflow.addCutToLastActiveCut("DYPlusXFakeElMRTightFake"  , [&]() { return wvz.lep_motherIdv2().at(lep_FakeCand_idx2) <  1; } , UNITY );

        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("FourLeptonFakeValidationEvents"  , [&]() { return this->Is4LeptonFakeValidationEvents(); } , UNITY );
        cutflow.getCut("FourLeptonFakeValidationEvents");
        cutflow.addCutToLastActiveCut("FourLeptonFakeValidationEventsEl"  , [&]() { return lep_Fakeable_MCmatched_idx >= 0 ? abs(wvz.lep_id()[lep_Fakeable_MCmatched_idx]) == 11 : false; } , UNITY );
        cutflow.getCut("FourLeptonFakeValidationEvents");
        cutflow.addCutToLastActiveCut("FourLeptonFakeValidationEventsMu"  , [&]() { return lep_Fakeable_MCmatched_idx >= 0 ? abs(wvz.lep_id()[lep_Fakeable_MCmatched_idx]) == 13 : false; } , UNITY );
        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("ThreeLeptonFakeValidationEvents"  , [&]() { return this->Is3LeptonFakeValidationEvents(); } , UNITY );
        cutflow.getCut("ThreeLeptonFakeValidationEvents");
        cutflow.addCutToLastActiveCut("ThreeLeptonFakeValidationEventsEl"  , [&]() { return lep_Fakeable_MCmatched_idx >= 0 ? abs(wvz.lep_id()[lep_Fakeable_MCmatched_idx]) == 11 : false; } , UNITY );
        cutflow.getCut("ThreeLeptonFakeValidationEvents");
        cutflow.addCutToLastActiveCut("ThreeLeptonFakeValidationEventsMu"  , [&]() { return lep_Fakeable_MCmatched_idx >= 0 ? abs(wvz.lep_id()[lep_Fakeable_MCmatched_idx]) == 13 : false; } , UNITY );

    }
    else if (ntupleVersion.Contains("TTZ"))
    {

        cutflow.getCut("Weight");
        cutflow.addCutToLastActiveCut("ThreeLeptonsTTZ", [&]() { return this->Is3LeptonTTZEvent(); } , [&](){ return this->LeptonScaleFactor(); } );
        cutflow.addCutToLastActiveCut("ThreeLeptonsTTZNjet", [&]() { return wvz.nj() >= 3; } , UNITY );

    }

    //========================
    //
    // Defining histograms
    //
    //========================

    // Histogram object contains histogram definitions and the lambda to be used for histogram filling
    RooUtil::Histograms histograms;
    RooUtil::Histograms histograms_Z_peak;

    if (not ntupleVersion.Contains("Trilep"))
    {
        histograms.addHistogram("Mll"            , 180 , 0       , 300    , [&](){ return this->VarMll(); });
        histograms.addHistogram("MET"            , 180 , 0       , 300    , [&](){ return this->VarMET(); });
        histograms.addHistogram("OrigMET"        , 180 , 0       , 300    , [&](){ return wvz.met_pt(); });
        // histograms.addHistogram("VarBinMET"      , {0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., 200., 500.}    , [&](){ return this->VarMET(); });
        // histograms.addHistogram("VarBinMETCoarse", {0., 100., 500.}    , [&](){ return this->VarMET(); });
        // histograms.addHistogram("Mll2ndZ"        , 180 , 0       , 300    , [&](){ return this->VarMll2ndZ(); });
        // histograms.addHistogram("MT5th"          , 180 , 0       , 300    , [&](){ return this->VarMT5th(); });
        // histograms.addHistogram("RelIso5th"      , 180 , 0       , 0.4    , [&](){ return this->VarRelIso5th(); });
        // histograms.addHistogram("Pt5th"          , 180 , 0       , 200    , [&](){ return this->VarPt5th(); });
        histograms.addHistogram("Njet"           , 6   , 0       , 6      , [&](){ return this->VarNjet(); });
        // histograms.addHistogram("Nfwdjet"        , 4   , 0       , 4      , [&](){ return wvz.nj() - wvz.nj_cen(); });
        // histograms.addHistogram("Neenoisejet"    , 4   , 0       , 4      , [&](){ return this->VarNEENoiseJet(); });
        histograms.addHistogram("Nbjet"          , 6   , 0       , 6      , [&](){ return this->VarNb(); });
        // histograms.addHistogram("Mll2l"          , 180 , 0       , 300    , [&](){ return this->VarMll2l(); });
        // histograms.addHistogram("lepZ2isNom0"    , 2   , 0       , 2      , [&](){ return lep_2ndZCand_idx1 >= 0 ? passNominalLeptonID(lep_2ndZCand_idx1) : -999; });
        // histograms.addHistogram("lepZ2isNom1"    , 2   , 0       , 2      , [&](){ return lep_2ndZCand_idx2 >= 0 ? passNominalLeptonID(lep_2ndZCand_idx2) : -999; });
        // histograms.addHistogram("lepZisNom0"     , 2   , 0       , 2      , [&](){ return lep_ZCand_idx1 >= 0 ? passNominalLeptonID(lep_ZCand_idx1) : -999; });
        // histograms.addHistogram("lepZisNom1"     , 2   , 0       , 2      , [&](){ return lep_ZCand_idx2 >= 0 ? passNominalLeptonID(lep_ZCand_idx2) : -999; });
        // histograms.addHistogram("lep5isNom"      , 2   , 0       , 2      , [&](){ return lep_WCand_idx1 >= 0 ? passNominalLeptonID(lep_WCand_idx1) : -999; });
        // histograms.addHistogram("lepZ2Pt0"       , 180 , 0       , 200    , [&](){ return lep_2ndZCand_idx1 >= 0 ? this->VarLepPt(lep_2ndZCand_idx1) : -999; });
        // histograms.addHistogram("lepZ2Pt1"       , 180 , 0       , 200    , [&](){ return lep_2ndZCand_idx2 >= 0 ? this->VarLepPt(lep_2ndZCand_idx2) : -999; });
        // histograms.addHistogram("lep5Pt"         , 180 , 0       , 200    , [&](){ return lep_WCand_idx1 >= 0 ? this->VarLepPt(lep_WCand_idx1) : -999; });
        // histograms.addHistogram("lep5Eta"        , 180 , -5      , 5      , [&](){ return lep_WCand_idx1 >= 0 ? wvz.lep_eta()[lep_WCand_idx1] : -999; });
        // histograms.addHistogram("lepZ2Eta0"      , 180 , -5      , 5      , [&](){ return lep_2ndZCand_idx1 >= 0 ? wvz.lep_eta()[lep_2ndZCand_idx1] : -999; });
        // histograms.addHistogram("lepZ2Eta1"      , 180 , -5      , 5      , [&](){ return lep_2ndZCand_idx2 >= 0 ? wvz.lep_eta()[lep_2ndZCand_idx2] : -999; });
        histograms.addHistogram("lepZPt0"        , 180 , 0       , 200    , [&](){ return this->VarLepPt(lep_ZCand_idx1); });
        histograms.addHistogram("lepZPt1"        , 180 , 0       , 200    , [&](){ return this->VarLepPt(lep_ZCand_idx2); });
        histograms.addHistogram("lepNPt0"        , 180 , 0       , 200    , [&](){ return this->VarLepPt(lep_Nom_idx1); });
        histograms.addHistogram("lepNPt1"        , 180 , 0       , 200    , [&](){ return this->VarLepPt(lep_Nom_idx2); });
        histograms.addHistogram("lepZEta0"       , 180 , -5      , 5      , [&](){ return lep_ZCand_idx1 >= 0 ? wvz.lep_eta()[lep_ZCand_idx1] : -999; });
        histograms.addHistogram("lepZEta1"       , 180 , -5      , 5      , [&](){ return lep_ZCand_idx2 >= 0 ? wvz.lep_eta()[lep_ZCand_idx2] : -999; });
        histograms.addHistogram("lepNEta0"       , 180 , -5      , 5      , [&](){ return lep_Nom_idx1 >= 0 ? wvz.lep_eta()[lep_Nom_idx1] : -999; });
        histograms.addHistogram("lepNEta1"       , 180 , -5      , 5      , [&](){ return lep_Nom_idx2 >= 0 ? wvz.lep_eta()[lep_Nom_idx2] : -999; });
        histograms.addHistogram("lepNip3d0"      , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx1 >= 0 ? fabs(wvz.lep_ip3d()[lep_Nom_idx1]) : -999; });
        histograms.addHistogram("lepNip3d1"      , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx2 >= 0 ? fabs(wvz.lep_ip3d()[lep_Nom_idx2]) : -999; });
        histograms.addHistogram("lepNdxy0"       , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx1 >= 0 ? fabs(wvz.lep_dxy()[lep_Nom_idx1]) : -999; });
        histograms.addHistogram("lepNdxy1"       , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx2 >= 0 ? fabs(wvz.lep_dxy()[lep_Nom_idx2]) : -999; });
        histograms.addHistogram("lepNdz0"        , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx1 >= 0 ? fabs(wvz.lep_dz()[lep_Nom_idx1]) : -999; });
        histograms.addHistogram("lepNdz1"        , 180 , 0       , 0.1    , [&](){ return lep_Nom_idx2 >= 0 ? fabs(wvz.lep_dz()[lep_Nom_idx2]) : -999; });
        histograms.addHistogram("MllZCand"       , 180 , 0       , 200    , [&](){ return this->VarMll(lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("MllNom"         , 180 , 0       , 200    , [&](){ return this->VarMll(lep_Nom_idx1, lep_Nom_idx2); });
        histograms.addHistogram("MllZ2Cand"       , 180 , 0       , 200    , [&](){ return this->VarMll(lep_Z2Cand_idx1, lep_Z2Cand_idx2); });
        //histograms.addHistogram("M4l"            , 180 , 0       , 450    , [&](){ return this->VarM4l(lep_Nom_idx1, lep_Nom_idx2, lep_ZCand_idx1, lep_ZCand_idx2); });
        //histograms.addHistogram("M4lZoom"        , 180 , 115     , 135    , [&](){ return this->VarM4l(lep_Nom_idx1, lep_Nom_idx2, lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("MZZ4l"            , 180 , 0       , 450    , [&](){ return this->VarM4l(lep_Z2Cand_idx1, lep_Z2Cand_idx2, lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("MZZ4lZoom"        , 180 , 115     , 135    , [&](){ return this->VarM4l(lep_Z2Cand_idx1, lep_Z2Cand_idx2, lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("PtllZCand"      , 180 , 0       , 200    , [&](){ return this->VarPtll(lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("PtllNom"        , 180 , 0       , 200    , [&](){ return this->VarPtll(lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("HTHad"          , 180 , 0       , 200    , [&](){ return wvz.ht(); });
        histograms.addHistogram("HTLep"          , 180 , 0       , 200    , [&](){ return this->VarHTLep(lep_Nom_idx1, lep_Nom_idx2, lep_ZCand_idx1, lep_ZCand_idx2); });
        histograms.addHistogram("HTLep5"         , 180 , 0       , 500    , [&](){ return this->VarHTLep5(); });
        // histograms.addHistogram("lepVmotherid0"  , 7  , -4       , 3      , [&](){ return lep_Veto_idx1 >= 0 ? wvz.lep_motherIdv2().at(lep_Veto_idx1) : -999; });
        // histograms.addHistogram("lepVmotherid1"  , 7  , -4       , 3      , [&](){ return lep_Veto_idx2 >= 0 ? wvz.lep_motherIdv2().at(lep_Veto_idx2) : -999; });
        // histograms.addHistogram("lepVmotherid2"  , 7  , -4       , 3      , [&](){ return lep_Veto_idx3 >= 0 ? wvz.lep_motherIdv2().at(lep_Veto_idx3) : -999; });
        // histograms.addHistogram("lepVmotherid3"  , 7  , -4       , 3      , [&](){ return lep_Veto_idx4 >= 0 ? wvz.lep_motherIdv2().at(lep_Veto_idx4) : -999; });
        // histograms.addHistogram("lepVmcid0"      , 8  , -4       , 4      , [&](){ return lep_Veto_idx1 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx1) : -999; });
        // histograms.addHistogram("lepVmcid1"      , 8  , -4       , 4      , [&](){ return lep_Veto_idx2 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx2) : -999; });
        // histograms.addHistogram("lepVmcid2"      , 8  , -4       , 4      , [&](){ return lep_Veto_idx3 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx3) : -999; });
        // histograms.addHistogram("lepVmcid3"      , 8  , -4       , 4      , [&](){ return lep_Veto_idx4 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx4) : -999; });
        // histograms.addHistogram("lepVisprompt0"  , 2  , 0        , 2      , [&](){ return lep_Veto_idx1 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx1) > 0 : -999; });
        // histograms.addHistogram("lepVisprompt1"  , 2  , 0        , 2      , [&](){ return lep_Veto_idx2 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx2) > 0 : -999; });
        // histograms.addHistogram("lepVisprompt2"  , 2  , 0        , 2      , [&](){ return lep_Veto_idx3 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx3) > 0 : -999; });
        // histograms.addHistogram("lepVisprompt3"  , 2  , 0        , 2      , [&](){ return lep_Veto_idx4 >= 0 ? wvz.lep_mc_id().at(lep_Veto_idx4) > 0 : -999; });
        // histograms.addHistogram("lepnVprompt"    , 5  , 0        , 5      , [&](){ return lep_Veto_idx4 >= 0 ? (wvz.lep_mc_id().at(lep_Veto_idx1)>0)+(wvz.lep_mc_id().at(lep_Veto_idx2)>0)+(wvz.lep_mc_id().at(lep_Veto_idx3)>0)+(wvz.lep_mc_id().at(lep_Veto_idx4)>0) : -999; });
        // histograms.addHistogram("lepVfakeid"     , 20 , 0        , 20     , [&](){ return lep_Veto_idx4 >= 0 ? (((wvz.lep_mc_id().at(lep_Veto_idx1)>0)+(wvz.lep_mc_id().at(lep_Veto_idx2)>0)+(wvz.lep_mc_id().at(lep_Veto_idx3)>0)+(wvz.lep_mc_id().at(lep_Veto_idx4)>0)) == 3 ? fabs((wvz.lep_mc_id().at(lep_Veto_idx1)<=0)*(wvz.lep_id().at(lep_Veto_idx1))+(wvz.lep_mc_id().at(lep_Veto_idx2)<=0)*(wvz.lep_id().at(lep_Veto_idx2))+(wvz.lep_mc_id().at(lep_Veto_idx3)<=0)*(wvz.lep_id().at(lep_Veto_idx3))+(wvz.lep_mc_id().at(lep_Veto_idx4)<=0)*(wvz.lep_id().at(lep_Veto_idx4))) : -999) : -999; });
        histograms.addHistogram("lepNTauLep"     , 5   , 0       , 5      , [&](){ return lep_Veto_idx4 >= 0 ? (abs(wvz.lep_mc_motherid().at(lep_Veto_idx1))==15)+(abs(wvz.lep_mc_motherid().at(lep_Veto_idx2))==15)+(abs(wvz.lep_mc_motherid().at(lep_Veto_idx3))==15)+(abs(wvz.lep_mc_motherid().at(lep_Veto_idx4))==15) : -999; });
        histograms.addHistogram("nlepNotNom"     , 5   , 0       , 5      , [&](){ return lep_notnom_idxs.size(); });
        histograms.addHistogram("MTNom0"         , 180 , 0       , 180    , [&](){ return this->VarMTNom0(); });
        histograms.addHistogram("MTNom1"         , 180 , 0       , 150    , [&](){ return this->VarMTNom1(); });
        histograms.addHistogram("LargeMTNom0"    , 180 , 0       , 300    , [&](){ return this->VarMTNom0(); });
        histograms.addHistogram("LargeMTNom1"    , 180 , 0       , 300    , [&](){ return this->VarMTNom1(); });
        histograms.addHistogram("MTFakeable"     , 180 , 0       , 150    , [&](){ return this->VarMTFakeable(); });
        histograms.addHistogram("MTNomMax"       , 180 , 0       , 180    , [&](){ return this->VarMTMax(); });
        histograms.addHistogram("MTNomMin"       , 180 , 0       , 180    , [&](){ return this->VarMTMin(); });
        // histograms.addHistogram("WindowMllNom"   , 180 , 0       , 50     , [&](){ return fabs(this->VarMll(lep_Nom_idx1, lep_Nom_idx2)-91.1876); });
        histograms.addHistogram("Yield"          , 1   , 0       , 1      , [&](){ return 0; });

	if(ntupleVersion.Contains("BDT"))
        {
                histograms.addHistogram("disc_ttz_bVeto"    	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(0); });
                histograms.addHistogram("disc_ttz_emu"      	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(1); });
                histograms.addHistogram("disc_ttz_nbAll"    	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(2); });
                histograms.addHistogram("disc_ttz_OffZ"     	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(3); });
                histograms.addHistogram("disc_ttzzz_bVeto"  	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(4); });
                histograms.addHistogram("disc_ttzzz_emu"    	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(5); });
                histograms.addHistogram("disc_ttzzz_OffZ"   	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(6); });
                histograms.addHistogram("disc_zz_emu"       	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(7); });
                histograms.addHistogram("disc_zz_emuHighTTZBDT" , 180 , 0       , 1      , [&](){ return this->VarXGBBDT(8); });
                histograms.addHistogram("disc_zz_OffZ"    	, 180 , 0       , 1      , [&](){ return this->VarXGBBDT(9); });
                histograms.addHistogram("disc_zz_OffZHighTTZBDT", 180 , 0       , 1      , [&](){ return this->VarXGBBDT(10); });
        }

        // histograms.addHistogram("lepFrelIso03EA" , 180 , 0       , 6.0    , [&](){ return lep_Fakeable_idx >= 0 ? wvz.lep_relIso03EA()[lep_Fakeable_idx] : -999; });
        // histograms.addHistogram("lepFrelIso04DB" , 180 , 0       , 6.0    , [&](){ return lep_Fakeable_idx >= 0 ? wvz.lep_relIso04DB()[lep_Fakeable_idx] : -999; });
        // histograms.addHistogram("jetPt0"         , 180 , 0.      , 200    , [&](){ return wvz.jets_p4().size() > 0 ? wvz.jets_p4()[0].pt() : -999; });
        // histograms.addHistogram("jetPt1"         , 180 , 0.      , 200    , [&](){ return wvz.jets_p4().size() > 1 ? wvz.jets_p4()[1].pt() : -999; });
        // histograms.addHistogram("jetPt2"         , 180 , 0.      , 200    , [&](){ return wvz.jets_p4().size() > 2 ? wvz.jets_p4()[2].pt() : -999; });
        // histograms.addHistogram("jetPt3"         , 180 , 0.      , 200    , [&](){ return wvz.jets_p4().size() > 3 ? wvz.jets_p4()[3].pt() : -999; });
        // histograms.addHistogram("jetEta0"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 0 ? wvz.jets_p4()[0].eta() : -999; });
        // histograms.addHistogram("jetEta1"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 1 ? wvz.jets_p4()[1].eta() : -999; });
        // histograms.addHistogram("jetEta2"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 2 ? wvz.jets_p4()[2].eta() : -999; });
        // histograms.addHistogram("jetEta3"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 3 ? wvz.jets_p4()[3].eta() : -999; });
        // histograms.addHistogram("jetPhi0"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 0 ? wvz.jets_p4()[0].phi() : -999; });
        // histograms.addHistogram("jetPhi1"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 1 ? wvz.jets_p4()[1].phi() : -999; });
        // histograms.addHistogram("jetPhi2"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 2 ? wvz.jets_p4()[2].phi() : -999; });
        // histograms.addHistogram("jetPhi3"        , 180 ,-5.      ,  5.    , [&](){ return wvz.jets_p4().size() > 3 ? wvz.jets_p4()[3].phi() : -999; });
        histograms.addHistogram("pt_zeta"        , 180 , -200    , 200       , [&](){ return this->VarPtZetaDiff(); });
        histograms.addHistogram("pt_zeta_vis"    , 180 , -200    , 550       , [&](){ return this->VarPtZetaVis(); });
        histograms.addHistogram("pt_zeta_sum"    , 180 , -200    , 550       , [&](){ return this->VarPtZeta(); });
        histograms.add2DHistogram("pt_zeta_vis", 50 , -200, 550 , "pt_zeta_sum", 50, -200, 550, [&](){ return this->VarPtZetaVis(); }, [&](){ return this->VarPtZeta(); });
    }
    else if (ntupleVersion.Contains("Trilep"))
    {
        histograms.addHistogram("lepFakeCand2MotherID"    , 7   , -4  , 3    , [&](){ return lep_FakeCand_idx2 >= 0 ? wvz.lep_motherIdv2().at(lep_FakeCand_idx2) : -999; });
        histograms.addHistogram("lepFakeCand2MCID"        , 7   , -4  , 3    , [&](){ return lep_FakeCand_idx2 >= 0 ? wvz.lep_mc_id().at(lep_FakeCand_idx2) : -999; });
        histograms.addHistogram("lepFakeCand2Pt"          , 180 , 0.  , 150. , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_pt().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2dz"          , 180 , 0.  , 0.2  , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_dz().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2dxy"         , 180 , 0.  , 0.2  , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_dxy().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2sip3d"       , 180 , 0.  , 20   , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_sip3d().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2ip3d"        , 180 , 0.  , 0.2  , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_ip3d().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2relIso03EA"  , 180 , 0.  , 0.4  , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_relIso03EA().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2relIso04DB"  , 180 , 0.  , 0.4  , [&](){ return lep_FakeCand_idx2 >= 0 ? fabs(wvz.lep_relIso04DB().at(lep_FakeCand_idx2)) : -999; });
        histograms.addHistogram("lepFakeCand2PtVarBin"    , {0. , 10. , 20.  , 70.},                [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2)                                                 , 69.9); return rtn; });
        histograms.addHistogram("lepFakeCand2PtVarBinFwd" , {0. , 10. , 20.  , 70.},                [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = fabs(wvz.lep_eta().at(lep_FakeCand_idx2)) >  1.6 ? std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2) , 69.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeCand2PtVarBinCen" , {0. , 10. , 20.  , 70.},                [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = fabs(wvz.lep_eta().at(lep_FakeCand_idx2)) <= 1.6 ? std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2) , 69.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeCand2PtFineVarBin"    , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2)                                                 , 69.9); return rtn; });
        histograms.addHistogram("lepFakeCand2PtFineVarBinFwd" , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = fabs(wvz.lep_eta().at(lep_FakeCand_idx2)) >  1.6 ? std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2) , 69.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeCand2PtFineVarBinCen" , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ if (lep_FakeCand_idx2 < 0) return -999.f; float rtn = fabs(wvz.lep_eta().at(lep_FakeCand_idx2)) <= 1.6 ? std::min((double) wvz.lep_pt().at(lep_FakeCand_idx2) , 69.9) : -999; return rtn; });

        histograms.addHistogram("MT"  , 180, 0., 180., [&](){ return this->VarMT(lep_FakeCand_idx2); });
        histograms.addHistogram("MET" , 180, 0., 180., [&](){ return this->VarMET(); });
        histograms.addHistogram("Mll" , 180, 0., 180., [&](){ return (lep_DYX_DYCand_idx1 >= 0 and lep_DYX_DYCand_idx2 >= 0) ? (wvz.lep_p4().at(lep_DYX_DYCand_idx1) + wvz.lep_p4().at(lep_DYX_DYCand_idx2)).M() : -999; });
        histograms.addHistogram("M3l" , 180, 0., 180., [&](){ return (lep_DYX_DYCand_idx1 >= 0 and lep_DYX_DYCand_idx2 >= 0) ? (wvz.lep_p4().at(lep_DYX_DYCand_idx1) + wvz.lep_p4().at(lep_DYX_DYCand_idx2) + wvz.lep_p4().at(lep_DYX_FakeCand_idx)).M() : -999; });

        histograms.addHistogram("nFakeable"  , 7, 0., 7., [&](){ return nFakeableLeptons; });

        histograms.addHistogram("lepFakeMCmatchedMotherID"    , 7   , -4  , 3    , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? wvz.lep_motherIdv2().at(lep_Fakeable_MCmatched_idx) : -999; });
        histograms.addHistogram("lepFakeMCmatchedMCID"        , 7   , -4  , 3    , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? wvz.lep_mc_id().at(lep_Fakeable_MCmatched_idx) : -999; });
        histograms.addHistogram("lepFakeMCmatchedPt"          , 180 , 0.  , 150. , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_pt().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatcheddz"          , 180 , 0.  , 0.2  , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_dz().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatcheddxy"         , 180 , 0.  , 0.2  , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_dxy().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatchedsip3d"       , 180 , 0.  , 20   , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_sip3d().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatchedip3d"        , 180 , 0.  , 0.2  , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_ip3d().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatchedrelIso03EA"  , 180 , 0.  , 0.4  , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_relIso03EA().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatchedrelIso04DB"  , 180 , 0.  , 0.4  , [&](){ return lep_Fakeable_MCmatched_idx >= 0 ? fabs(wvz.lep_relIso04DB().at(lep_Fakeable_MCmatched_idx)) : -999; });
        histograms.addHistogram("lepFakeMCmatchedPtVarBin"        , {0. , 10. , 20.  , 70.},            [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx)                                                             , 69.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeMCmatchedPtVarBinFwd"     , {0. , 10. , 20.  , 70.},            [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? (fabs(wvz.lep_eta().at(lep_Fakeable_MCmatched_idx)) >  1.6 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx), 69.9) : -999) : -999; return rtn; });
        histograms.addHistogram("lepFakeMCmatchedPtVarBinCen"     , {0. , 10. , 20.  , 70.},            [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? (fabs(wvz.lep_eta().at(lep_Fakeable_MCmatched_idx)) <= 1.6 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx), 69.9) : -999) : -999; return rtn; });
        histograms.addHistogram("lepFakeMCmatchedPtFineVarBin"    , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx)                                                             , 69.9) : -999; return rtn; });
        histograms.addHistogram("lepFakeMCmatchedPtFineVarBinFwd" , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? (fabs(wvz.lep_eta().at(lep_Fakeable_MCmatched_idx)) >  1.6 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx), 69.9) : -999) : -999; return rtn; });
        histograms.addHistogram("lepFakeMCmatchedPtFineVarBinCen" , {0. , 10. , 20.  , 30.,  50., 70.}, [&](){ float rtn = lep_Fakeable_MCmatched_idx >= 0 ? (fabs(wvz.lep_eta().at(lep_Fakeable_MCmatched_idx)) <= 1.6 ? std::min((double) wvz.lep_pt().at(lep_Fakeable_MCmatched_idx), 69.9) : -999) : -999; return rtn; });
    }


    if (doSyst)
    {

        // Systematic variations
        cutflow.addWgtSyst("BTagHFUp"  , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_heavy_UP() / wvz.weight_btagsf() : 1; } );
        cutflow.addWgtSyst("BTagHFDown", [&]() { if (wvz.isData()) return 1.f; return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_heavy_DN() / wvz.weight_btagsf() : 1; } );
        cutflow.addWgtSyst("BTagLFUp"  , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_light_UP() / wvz.weight_btagsf() : 1; } );
        cutflow.addWgtSyst("BTagLFDown", [&]() { if (wvz.isData()) return 1.f; return wvz.weight_btagsf() != 0 ? wvz.weight_btagsf_light_DN() / wvz.weight_btagsf() : 1; } );
        cutflow.addWgtSyst("PDFUp"     , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.pdfup() == 0 ? 0 : wvz.weight_pdf_up()   / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfup(); } );
        cutflow.addWgtSyst("PDFDown"   , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.pdfdn() == 0 ? 0 : wvz.weight_pdf_down() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfdn(); } );
        cutflow.addWgtSyst("QsqUp"     , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.qsqup() == 0 ? 0 : wvz.weight_fr_r2_f2()     / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqup(); } );
        cutflow.addWgtSyst("QsqDown"   , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.qsqdn() == 0 ? 0 : wvz.weight_fr_r0p5_f0p5() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqdn(); } );
        cutflow.addWgtSyst("AlphaSUp"  , [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.alsup() == 0 ? 0 : wvz.weight_alphas_up()   / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsup(); } );
        cutflow.addWgtSyst("AlphaSDown", [&]() { if (wvz.isData()) return 1.f; return wvz.weight_fr_r1_f1() == 0 or theoryweight.alsdn() == 0 ? 0 : wvz.weight_alphas_down() / wvz.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsdn(); } );
        // TODO on weight systematics
        // 2. LepSF
        // 3. TrigSF
        cutflow.addWgtSyst("PileupUp"  , [&]()
                {
                if (wvz.isData()) return 1.f;
                if (year == 2016) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2016(wvz.nTrueInt()) / getTruePUw2016(wvz.nTrueInt()) : 0;
                else if (year == 2017) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2017(wvz.nTrueInt()) / getTruePUw2017(wvz.nTrueInt()) : 0;
                else if (year == 2018) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwUp2018(wvz.nTrueInt()) / getTruePUw2018(wvz.nTrueInt()) : 0;
                return 0.f;
                } );
        cutflow.addWgtSyst("PileupDown"  , [&]()
                {
                if (wvz.isData()) return 1.f;
                if (year == 2016) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2016(wvz.nTrueInt()) / getTruePUw2016(wvz.nTrueInt()) : 0;
                else if (year == 2017) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2017(wvz.nTrueInt()) / getTruePUw2017(wvz.nTrueInt()) : 0;
                else if (year == 2018) return getTruePUw2016(wvz.nTrueInt()) != 0 ? getTruePUwDn2018(wvz.nTrueInt()) / getTruePUw2018(wvz.nTrueInt()) : 0;
                return 0.f;
                } );

        // The strings in {} are the patterns that should declare the systematic variatiosn for
        // i.e. *MT*, *MET*, and *Cut4LepB* are the pattern used to search the cut names in the cutflow object to declare a systematic varations
        // e.g. ChannelBTagEMuHighMT contains "MT" and therefore a JES variations will be declared for these cuts
        // Then later below with "setCutSyst" function the variational cut defn will be defined
        cutflow.addCutSyst("JESUp"         , {"PtZeta", "MT" , "MET" , "Cut4LepB"});
        cutflow.addCutSyst("JESDown"       , {"PtZeta", "MT" , "MET" , "Cut4LepB"});
        cutflow.addCutSyst("JERUp"         , {"PtZeta", "MT" , "MET"});
        cutflow.addCutSyst("JERDown"       , {"PtZeta", "MT" , "MET"});
        cutflow.addCutSyst("METUp"         , {"PtZeta", "MT" , "MET"});
        cutflow.addCutSyst("METDown"       , {"PtZeta", "MT" , "MET"});
        cutflow.addCutSyst("METPileupUp"   , {"PtZeta", "MT" , "MET"});
        cutflow.addCutSyst("METPileupDown" , {"PtZeta", "MT" , "MET"});

        // 1 represents JESup variation
        //-1 represents JESdown variation
        // 2 represents JERup variation
        //-2 represents JERdown variation
        // 3 represents METup variation (statistical error on the correction)
        //-3 represents METdown variation (statistical error on the correction)
        // 4 represents PUup variation (PU corretion on MET and MT)
        //-4 represents PUdown variation (PU corretion on MET and MT)

        // 1. MT cuts
        cutflow.setCutSyst("ChannelEMuHighMT"     , "JESUp"   , [&]() { return     this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "JESUp"   , [&]() { return     this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "JESUp"   , [&]() { return     this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "JESUp"   , [&]() { return not this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "JESUp"   , [&]() { return not this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "JESUp"   , [&]() { return     this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "JESUp"   , [&]() { return     this->CutHighMT(1); }    , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "JESUp"   , [&]() { return     this->CutHighMTAR(1); }  , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "JESDown" , [&]() { return     this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "JESDown" , [&]() { return     this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "JESDown" , [&]() { return     this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "JESDown" , [&]() { return not this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "JESDown" , [&]() { return not this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "JESDown" , [&]() { return     this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "JESDown" , [&]() { return     this->CutHighMT(-1); }   , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "JESDown" , [&]() { return     this->CutHighMTAR(-1); } , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "JERUp"   , [&]() { return     this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "JERUp"   , [&]() { return     this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "JERUp"   , [&]() { return     this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "JERUp"   , [&]() { return not this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "JERUp"   , [&]() { return not this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "JERUp"   , [&]() { return     this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "JERUp"   , [&]() { return     this->CutHighMT(2); }    , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "JERUp"   , [&]() { return     this->CutHighMTAR(2); }  , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "JERDown" , [&]() { return     this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "JERDown" , [&]() { return     this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "JERDown" , [&]() { return     this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "JERDown" , [&]() { return not this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "JERDown" , [&]() { return not this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "JERDown" , [&]() { return     this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "JERDown" , [&]() { return     this->CutHighMT(-2); }   , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "JERDown" , [&]() { return     this->CutHighMTAR(-2); } , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "METUp"   , [&]() { return     this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "METUp"   , [&]() { return     this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "METUp"   , [&]() { return     this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "METUp"   , [&]() { return not this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "METUp"   , [&]() { return not this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "METUp"   , [&]() { return     this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "METUp"   , [&]() { return     this->CutHighMT(3); }    , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "METUp"   , [&]() { return     this->CutHighMTAR(3); }  , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "METDown" , [&]() { return     this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "METDown" , [&]() { return     this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "METDown" , [&]() { return     this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "METDown" , [&]() { return not this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "METDown" , [&]() { return not this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "METDown" , [&]() { return     this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "METDown" , [&]() { return     this->CutHighMT(-3); }   , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "METDown" , [&]() { return     this->CutHighMTAR(-3); } , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "METPileupUp"   , [&]() { return     this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "METPileupUp"   , [&]() { return     this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "METPileupUp"   , [&]() { return     this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "METPileupUp"   , [&]() { return not this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "METPileupUp"   , [&]() { return not this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "METPileupUp"   , [&]() { return     this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "METPileupUp"   , [&]() { return     this->CutHighMT(4); }    , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "METPileupUp"   , [&]() { return     this->CutHighMTAR(4); }  , UNITY );

        cutflow.setCutSyst("ChannelEMuHighMT"     , "METPileupDown" , [&]() { return     this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMT"     , "METPileupDown" , [&]() { return     this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMT"    , "METPileupDown" , [&]() { return     this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMT"      , "METPileupDown" , [&]() { return not this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMT"     , "METPileupDown" , [&]() { return not this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMT" , "METPileupDown" , [&]() { return     this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("FiveLeptonsMT5th"     , "METPileupDown" , [&]() { return     this->CutHighMT(-4); }   , UNITY );
        cutflow.setCutSyst("ChannelAREMuHighMT"   , "METPileupDown" , [&]() { return     this->CutHighMTAR(-4); } , UNITY );

        // 2. MET cuts
        cutflow.setCutSyst("ChannelOffZHighMET"        , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "JESUp"   , [&]() { return not this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "JESUp"   , [&]() { return not this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "JESUp"   , [&]() { return not this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "JESUp"   , [&]() { return not this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "JESUp"   , [&]() { return     this->CutHighMET(1); }  , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "JESDown" , [&]() { return not this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "JESDown" , [&]() { return not this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "JESDown" , [&]() { return not this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "JESDown" , [&]() { return not this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "JESDown" , [&]() { return     this->CutHighMET(-1); } , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "JERUp"   , [&]() { return not this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "JERUp"   , [&]() { return not this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "JERUp"   , [&]() { return not this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "JERUp"   , [&]() { return not this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "JERUp"   , [&]() { return     this->CutHighMET(2); }  , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "JERDown" , [&]() { return not this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "JERDown" , [&]() { return not this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "JERDown" , [&]() { return not this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "JERDown" , [&]() { return not this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "JERDown" , [&]() { return     this->CutHighMET(-2); } , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "METUp"   , [&]() { return not this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "METUp"   , [&]() { return not this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "METUp"   , [&]() { return not this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "METUp"   , [&]() { return not this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "METUp"   , [&]() { return     this->CutHighMET(3); }  , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "METDown" , [&]() { return not this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "METDown" , [&]() { return not this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "METDown" , [&]() { return not this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "METDown" , [&]() { return not this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "METDown" , [&]() { return     this->CutHighMET(-3); } , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "METPileupUp"   , [&]() { return not this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "METPileupUp"   , [&]() { return not this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "METPileupUp"   , [&]() { return not this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "METPileupUp"   , [&]() { return not this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "METPileupUp"   , [&]() { return     this->CutHighMET(4); }  , UNITY );

        cutflow.setCutSyst("ChannelOffZHighMET"        , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOnZHighMET"         , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMET"         , "METPileupDown" , [&]() { return not this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOnZLowMET"          , "METPileupDown" , [&]() { return not this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllHighMET" , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllHighMET"  , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOffZHighMllLowMET"  , "METPileupDown" , [&]() { return not this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelOffZLowMllLowMET"   , "METPileupDown" , [&]() { return not this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelBTagOffZHighMET"    , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelBTagEMuHighMET"     , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );
        cutflow.setCutSyst("ChannelAROffZHighMET"      , "METPileupDown" , [&]() { return     this->CutHighMET(-4); } , UNITY );

        // 3. Cut4LepB cuts (the btag and bveto)
        cutflow.setCutSyst("Cut4LepBVeto"  , "JESUp"  , [&]() { return this->Cut4LepBVeto(1); }  , [&]() { return this->BTagSF(); } );
        cutflow.setCutSyst("Cut4LepBTag"   , "JESUp"  , [&]() { return this->Cut4LepBTag(1); }   , [&]() { return this->BTagSF(); } );
        cutflow.setCutSyst("ARCut4LepBVeto", "JESUp"  , [&]() { return this->Cut4LepBVeto(1); }  , [&]() { return this->BTagSF(); } );
        cutflow.setCutSyst("Cut4LepBVeto"  , "JESDown" , [&]() { return this->Cut4LepBVeto(-1); }, [&]() { return this->BTagSF(); } );
        cutflow.setCutSyst("Cut4LepBTag"   , "JESDown" , [&]() { return this->Cut4LepBTag(-1); } , [&]() { return this->BTagSF(); } );
        cutflow.setCutSyst("ARCut4LepBVeto", "JESDown" , [&]() { return this->Cut4LepBVeto(-1); }, [&]() { return this->BTagSF(); } );

        // 4. PtZeta
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "JESUp" , [&]() { return this->CutLowPtZeta(1); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "JESDown" , [&]() { return this->CutLowPtZeta(-1); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "JERUp" , [&]() { return this->CutLowPtZeta(2); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "JERDown" , [&]() { return this->CutLowPtZeta(-2); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "METUp" , [&]() { return this->CutLowPtZeta(3); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "METDown" , [&]() { return this->CutLowPtZeta(-3); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "METPileupUp" , [&]() { return this->CutLowPtZeta(4); } , UNITY);
        cutflow.setCutSyst("ChannelEMuLowPtZeta", "METPileupDown" , [&]() { return this->CutLowPtZeta(-4); } , UNITY);

    }

    //==========================
    //
    // Book Analysis to perform
    //
    //==========================

    // // Book Cutflow
    // cutflow.bookCutflows();

    // Book histograms
    if (ntupleVersion.Contains("WVZ"))
    {
        if (not doSyst)
        {
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMu");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOnZ");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOffZ");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelHZZ4l");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagEMu");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagOnZ");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelBTagOffZ");
            cutflow.bookHistogramsForCutAndBelow(histograms, "FiveLeptonsRelIso5th");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelAREMu");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelAROffZ");
	    if (ntupleVersion.Contains("BDT"))
            {
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuHighBDTttZemu");
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuHighMTHighBDTttZemu");
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuHighBDTttZnbAll");
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuHighMTHighBDTttZnbAll");
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOffZHighBDTttZnbAll");
                    cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOffZHighMETHighBDTttZnbAll");
            }
        }
        else
        {
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuHighMT");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelEMuLowPtZeta");
            cutflow.bookHistogramsForCutAndBelow(histograms, "ChannelOffZHighMET");
            cutflow.bookHistogramsForCut(histograms, "ChannelEMu");
            cutflow.bookHistogramsForCut(histograms, "ChannelOffZ");
            cutflow.bookHistogramsForCut(histograms, "ChannelHZZ4l");
            cutflow.bookHistogramsForCut(histograms, "ChannelOnZ");
            cutflow.bookHistogramsForCut(histograms, "ChannelOnZHighMT");
            cutflow.bookHistogramsForCut(histograms, "ChannelOnZHighMET");
            cutflow.bookHistogramsForCut(histograms, "ChannelBTagEMu");
            cutflow.bookHistogramsForCut(histograms, "ChannelBTagEMuHighMET");
            cutflow.bookHistogramsForCut(histograms, "ChannelBTagEMuHighMT");
            cutflow.bookHistogramsForCut(histograms, "ChannelBTagOffZ");
            cutflow.bookHistogramsForCut(histograms, "ChannelBTagOffZHighMET");
            histograms_Z_peak.addHistogram("MllZCandZoom" , 180 , 60 , 120 , [&](){ return this->VarMll(lep_ZCand_idx1, lep_ZCand_idx2); });
            histograms_Z_peak.addHistogram("MllNomZoom"   , 180 , 60 , 120 , [&](){ return this->VarMll(lep_Nom_idx1, lep_Nom_idx2); });
            histograms_Z_peak.addHistogram("MllZCand" , 180 , 0 , 200 , [&](){ return this->VarMll(lep_ZCand_idx1, lep_ZCand_idx2); });
            histograms_Z_peak.addHistogram("MllNom"   , 180 , 0 , 200 , [&](){ return this->VarMll(lep_Nom_idx1, lep_Nom_idx2); });
            histograms_Z_peak.addHistogram("lepZPt0"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_ZCand_idx1); });
            histograms_Z_peak.addHistogram("lepZPt1"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_ZCand_idx2); });
            histograms_Z_peak.addHistogram("lepNPt0"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Nom_idx1); });
            histograms_Z_peak.addHistogram("lepNPt1"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Nom_idx2); });
            histograms_Z_peak.addHistogram("lepVPt0"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Veto_idx1); });
            histograms_Z_peak.addHistogram("lepVPt1"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Veto_idx2); });
            histograms_Z_peak.addHistogram("lepVPt2"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Veto_idx3); });
            histograms_Z_peak.addHistogram("lepVPt3"  , 180 , 0 , 200 , [&](){ return this->VarLepPt(lep_Veto_idx4); });
            cutflow.bookHistogramsForCut(histograms_Z_peak, "Cut4LepLeptonPt");
	    if (ntupleVersion.Contains("BDT"))
            {
                    cutflow.bookHistogramsForCut(histograms, "ChannelEMuHighBDTttZemu");
                    cutflow.bookHistogramsForCut(histograms, "ChannelEMuHighMTHighBDTttZemu");
                    cutflow.bookHistogramsForCut(histograms, "ChannelEMuHighBDTttZnbAll");
                    cutflow.bookHistogramsForCut(histograms, "ChannelEMuHighMTHighBDTttZnbAll");
                    cutflow.bookHistogramsForCut(histograms, "ChannelOffZHighBDTttZnbAll");
                    cutflow.bookHistogramsForCut(histograms, "ChannelOffZHighMETHighBDTttZnbAll");
            }
        }
    }
    else if (ntupleVersion.Contains("Trilep"))
    {
        cutflow.bookHistogramsForCutAndBelow(histograms, "EMuPlusX");
        cutflow.bookHistogramsForCutAndBelow(histograms, "DYPlusX");
        cutflow.bookHistogramsForCutAndBelow(histograms, "ThreeLeptonFakeValidationEvents");
        cutflow.bookHistogramsForCutAndBelow(histograms, "FourLeptonFakeValidationEvents");
    }
    else if (ntupleVersion.Contains("TTZ"))
    {
        cutflow.bookHistogramsForCutAndBelow(histograms, "ThreeLeptonsTTZ");
    }

    // Looper class to facilitate various things
    TChain* ch = new TChain("t");
    ch->Add(fTTree->GetCurrentFile()->GetName());
    looper = new RooUtil::Looper<wvztree>(ch, &wvz, -1); // -1 means process all events

    // if doSkim 
    if (ntupleVersion.Contains("WVZ") and doSkim)
    {
        TString tree_output_tfile = output_path + "/BDTinputTree_" + output_tfile_name;
        looper->setSkim(tree_output_tfile);
    }

    // Perform event loop!
    while (looper->nextEvent())
    {

        // If new file in chain set some things may need to be set
        if (looper->isNewFileInChain())
        {
            theoryweight.setFile(looper->getCurrentFileName());
            if (ntupleVersion.Contains("WVZ") and doSkim)
            {
                createNewBranches();
            }
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
        select2ndZCandLeptons();
        select2ndZCandAndWCandLeptons();
        selectFakeableLeptons();
        /* probably not necessary anymore */ sortLeptonIndex();
        selectFakeStudyLeptons();
        correctMET();
        cutflow.fill();


        if (ntupleVersion.Contains("WVZ"))
        {
            if (cutflow.getCut("CutHLT").pass)
            {
                if (doSkim)
                    fillSkimTree();
            }
        }
    }

    cutflow.saveOutput();
    if (ntupleVersion.Contains("WVZ") and doSkim)
    {
        looper->saveSkim();
        looper->getSkimFile()->cd();
        theoryweight.histmap_neventsinfile->hist->Write();
    }


}//end of whole function

//______________________________________________________________________________________________
void Analysis::setDoSkim(bool setDoSkim)
{
    doSkim = setDoSkim;
}

//______________________________________________________________________________________________
void Analysis::createNewBranches()
{
    tx = new RooUtil::TTreeX(looper->getSkimTree());
    tx->createBranch<int>("lep_Z_idx0");
    tx->createBranch<int>("lep_Z_idx1");
    tx->createBranch<int>("lep_N_idx0");
    tx->createBranch<int>("lep_N_idx1");
    tx->createBranch<float>("eventweight");
    tx->createBranch<float>("lepsf");
    //needed for wwz vs ZZ BDT
    //lep1: Z cand lepton, id > 0
    //lep2: Z cand lepton, id < 0
    //lep3: W cand lepton, leading pt
    //lep4: W cand lepton, subleading pt
    tx->createBranch<float>("MllN");
    tx->createBranch<float>("MllZ");
    tx->createBranch<float>("ZPt");
    tx->createBranch<float>("lep1Pt");
    tx->createBranch<float>("lep2Pt");
    tx->createBranch<float>("lep3Pt");
    tx->createBranch<float>("lep4Pt");
    tx->createBranch<float>("lep3Id");
    tx->createBranch<float>("lep4Id");
    tx->createBranch<float>("lep3MT");
    tx->createBranch<float>("lep4MT");
    tx->createBranch<float>("lep34MT");
    tx->createBranch<float>("lep1dZ");
    tx->createBranch<float>("lep2dZ");
    tx->createBranch<float>("lep3dZ");
    tx->createBranch<float>("lep4dZ");
    tx->createBranch<float>("pt_zeta");
    tx->createBranch<float>("pt_zeta_vis");
    tx->createBranch<float>("phi0");
    tx->createBranch<float>("phi");
    tx->createBranch<float>("phiH");
    tx->createBranch<float>("theta0");
    tx->createBranch<float>("theta1");
    tx->createBranch<float>("theta2");
    //needed for wwz vs ttZ BDT
    //for jet1/2/3/4, if not enough jets, fill with -999
    tx->createBranch<float>("minDRJetToLep3");
    tx->createBranch<float>("minDRJetToLep4");
    tx->createBranch<float>("jet1Pt");
    tx->createBranch<float>("jet2Pt");
    tx->createBranch<float>("jet3Pt");
    tx->createBranch<float>("jet4Pt");
    tx->createBranch<float>("jet1BtagScore");
    tx->createBranch<float>("jet2BtagScore");
    tx->createBranch<float>("jet3BtagScore");
    tx->createBranch<float>("jet4BtagScore");
    
}

//______________________________________________________________________________________________
void Analysis::fillSkimTree()
{
    int lep1_idx = lep_ZCand_idx1, lep2_idx = lep_ZCand_idx2, lep3_idx = lep_Nom_idx1, lep4_idx = lep_Nom_idx2;
    //Z: lep1 is id>0 lepton
    if(wvz.lep_id().at(lep_ZCand_idx1) < 0) 
    {
        lep1_idx = lep_ZCand_idx2;
        lep2_idx = lep_ZCand_idx1;
    } 
    //W: lep3 is higher pt lepton
    if(wvz.lep_pt().at(lep_Nom_idx1) < wvz.lep_pt().at(lep_Nom_idx2)) 
    {
        lep3_idx = lep_Nom_idx2;
        lep4_idx = lep_Nom_idx1;
    } 

    LeptonVectors vLeptons = GetLeptonVectors();
    HZZ4lEventParameters eventParameters = ConvertVectorsToAngles( vLeptons );
    //start filling...
    tx->setBranch<int>("lep_Z_idx0", lep1_idx);
    tx->setBranch<int>("lep_Z_idx1", lep2_idx);
    tx->setBranch<int>("lep_N_idx0", lep3_idx);
    tx->setBranch<int>("lep_N_idx1", lep4_idx);
    tx->setBranch<float>("eventweight", this->EventWeight());
    tx->setBranch<float>("lepsf", this->LeptonScaleFactor());
    tx->setBranch<float>("MllN", this->VarMll(lep_Nom_idx1, lep_Nom_idx2));
    tx->setBranch<float>("MllZ", this->VarMll(lep_ZCand_idx1, lep_ZCand_idx2));
    tx->setBranch<float>("ZPt", this->VarPtll(lep_ZCand_idx1, lep_ZCand_idx2));
    tx->setBranch<float>("lep1Pt", this->VarLepPt(lep1_idx));
    tx->setBranch<float>("lep2Pt", this->VarLepPt(lep2_idx));
    tx->setBranch<float>("lep3Pt", this->VarLepPt(lep3_idx));
    tx->setBranch<float>("lep4Pt", this->VarLepPt(lep4_idx));
    tx->setBranch<float>("lep3Id", wvz.lep_id().at(lep3_idx));
    tx->setBranch<float>("lep4Id", wvz.lep_id().at(lep4_idx));
    tx->setBranch<float>("lep3MT", this->VarMT(lep3_idx));
    tx->setBranch<float>("lep4MT", this->VarMT(lep4_idx));
    tx->setBranch<float>("lep34MT", this->VarMTll(lep3_idx, lep4_idx));
    tx->setBranch<float>("lep1dZ", wvz.lep_dz().at(lep1_idx));
    tx->setBranch<float>("lep2dZ", wvz.lep_dz().at(lep2_idx));
    tx->setBranch<float>("lep3dZ", wvz.lep_dz().at(lep3_idx));
    tx->setBranch<float>("lep4dZ", wvz.lep_dz().at(lep4_idx));
    tx->setBranch<float>("pt_zeta", this->VarPtZeta());
    tx->setBranch<float>("pt_zeta_vis", this->VarPtZetaVis());
    tx->setBranch<float>("phi0", eventParameters.Phi0);
    tx->setBranch<float>("phi", eventParameters.Phi);
    tx->setBranch<float>("phiH", eventParameters.PhiH);
    tx->setBranch<float>("theta0", eventParameters.Theta0);
    tx->setBranch<float>("theta1", eventParameters.Theta1);
    tx->setBranch<float>("theta2", eventParameters.Theta2);
    tx->setBranch<float>("minDRJetToLep3", this->VarMinDRJetsToLep(lep3_idx));
    tx->setBranch<float>("minDRJetToLep4", this->VarMinDRJetsToLep(lep4_idx));
    tx->setBranch<float>("jet1Pt", wvz.jets_p4().size() > 0 ? wvz.jets_p4()[0].pt() : -999);
    tx->setBranch<float>("jet2Pt", wvz.jets_p4().size() > 1 ? wvz.jets_p4()[1].pt() : -999);
    tx->setBranch<float>("jet3Pt", wvz.jets_p4().size() > 2 ? wvz.jets_p4()[2].pt() : -999);
    tx->setBranch<float>("jet4Pt", wvz.jets_p4().size() > 3 ? wvz.jets_p4()[3].pt() : -999);
    tx->setBranch<float>("jet1BtagScore", wvz.jets_btag_score().size() > 0 ? wvz.jets_btag_score()[0] : -999);
    tx->setBranch<float>("jet2BtagScore", wvz.jets_btag_score().size() > 1 ? wvz.jets_btag_score()[1] : -999);
    tx->setBranch<float>("jet3BtagScore", wvz.jets_btag_score().size() > 2 ? wvz.jets_btag_score()[2] : -999);
    tx->setBranch<float>("jet4BtagScore", wvz.jets_btag_score().size() > 3 ? wvz.jets_btag_score()[3] : -999);

    looper->fillSkim();
}

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

    // MET MC Correction (scale factors)
    metcorrector.setup(year, TString::Format("%d", year), "StopAnalysis/StopCORE/METCorr/METSFs/");
}

//______________________________________________________________________________________________
void Analysis::readLeptons()
{
    leptons.clear();
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
        leptons.push_back(wvz.lep_p4().at(jj));
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

    if (lep_pt->size() == 0)
        return;

    bool ifpass; // For tagging Z boson only
    double compare = 9999; // Min value of |Mll - MZ|

    // Loop over the leptons and find the Z boson pair
    for (unsigned int jj = 0 ; jj < (lep_pt->size() - 1) ; jj ++)
    {

        if (not passZCandLeptonID(jj))
            continue;

        for (unsigned int kk = jj + 1 ; kk < lep_pt->size() ; kk++)
        {

            if (not passZCandLeptonID(kk))
                continue;

            dilepZCand = leptons.at(jj) + leptons.at(kk);
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
void Analysis::selectTightLeptons()
{
    nTightLeptons = 0;
    lep_tight_idxs.clear();

    // Loop over the leptons
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (not passNominalLeptonID(jj))
            continue;

        lep_tight_idxs.push_back(jj);

        nTightLeptons++;

    }

    return;
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
    lep_nom_idxs.clear();

    // Loop over the leptons
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == (unsigned int) lep_ZCand_idx1)
            continue;

        if (jj == (unsigned int) lep_ZCand_idx2)
            continue;

        if (not passNominalLeptonID(jj))
            continue;

        lep_nom_idxs.push_back(jj);

    }

    nNominalLeptons = lep_nom_idxs.size(); // # of good nominal leptons
    if (nNominalLeptons > 0) lep_Nom_idx1 = lep_nom_idxs.at(0);
    if (nNominalLeptons > 1) lep_Nom_idx2 = lep_nom_idxs.at(1);
    if (nNominalLeptons > 2) lep_Nom_idx3 = lep_nom_idxs.at(2);

}

//______________________________________________________________________________________________
void Analysis::select2ndZCandLeptons()
{

    // The nominal leptons are the leptons that are not tagged as Z

    lep_Z2Cand_idx1 = -999;
    lep_Z2Cand_idx2 = -999;

    std::vector<int> good_idx;
    // Loop over the leptons
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (jj == (unsigned int) lep_ZCand_idx1)
            continue;

        if (jj == (unsigned int) lep_ZCand_idx2)
            continue;

        if (not passZCandLeptonID(jj))
            continue;

        good_idx.push_back(jj);

    }

    int nZ2Leptons = good_idx.size(); // # of good z cand leptons
    if (nZ2Leptons > 0) lep_Z2Cand_idx1 = good_idx[0];
    if (nZ2Leptons > 1) lep_Z2Cand_idx2 = good_idx[1];
}


//______________________________________________________________________________________________
void Analysis::selectFakeStudyLeptons()
{

    nDFOS = 0;
    lep_FakeCand_idx1 = -999;
    lep_FakeCand_idx2 = -999;
    lep_NonFakeCand_idx = -999;
    lep_DYX_FakeCand_idx = -999;
    lep_DYX_DYCand_idx1 = -999;
    lep_DYX_DYCand_idx2 = -999;

    if (not (nFakeableLeptons >= 3)) return;

    // Check that the sum of the sign of the lepton is +/- 1
    int charge1 = (wvz.lep_id().at(lep_fakeable_idxs[0]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[0]) < 0);
    int charge2 = (wvz.lep_id().at(lep_fakeable_idxs[1]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[1]) < 0);
    int charge3 = (wvz.lep_id().at(lep_fakeable_idxs[2]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[2]) < 0);

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
    if (wvz.lep_id().at(lep_fakeable_idxs[0]) * wvz.lep_id().at(lep_fakeable_idxs[1]) == -143) nDFOS++;
    if (wvz.lep_id().at(lep_fakeable_idxs[0]) * wvz.lep_id().at(lep_fakeable_idxs[2]) == -143) nDFOS++;
    if (wvz.lep_id().at(lep_fakeable_idxs[1]) * wvz.lep_id().at(lep_fakeable_idxs[2]) == -143) nDFOS++;

    if (nDFOS == 2) // ttbar->2l + X selection
    {

        // Select the same-sign pair idx
        if (charge1 == charge2)
        {
            lep_FakeCand_idx1 = lep_fakeable_idxs[0];
            lep_FakeCand_idx2 = lep_fakeable_idxs[1];
            lep_NonFakeCand_idx = lep_fakeable_idxs[2];
        }
        else if (charge1 == charge3)
        {
            lep_FakeCand_idx1 = lep_fakeable_idxs[0];
            lep_FakeCand_idx2 = lep_fakeable_idxs[2];
            lep_NonFakeCand_idx = lep_fakeable_idxs[1];
        }
        else if (charge2 == charge3)
        {
            lep_FakeCand_idx1 = lep_fakeable_idxs[1];
            lep_FakeCand_idx2 = lep_fakeable_idxs[2];
            lep_NonFakeCand_idx = lep_fakeable_idxs[0];
        }

        // lep_E_idx = abs(wvz.lep_id()[lep_Veto_idx1]) == 11 ? lep_Veto_idx1 : lep_Veto_idx2;
        // lep_Mu_idx = abs(wvz.lep_id()[lep_Veto_idx1]) == 11 ? lep_Veto_idx2 : lep_Veto_idx1;
        // lep_FakeCand_idx = lep_Veto_idx3;

        // std::cout <<  " lep_E_idx: " << lep_E_idx <<  " lep_Mu_idx: " << lep_Mu_idx <<  std::endl;
        // std::cout <<  " lep_Veto_idx1: " << lep_Veto_idx1 <<  " lep_Veto_idx2: " << lep_Veto_idx2 <<  " lep_Veto_idx3: " << lep_Veto_idx3 <<  " lep_Veto_idx4: " << lep_Veto_idx4 <<  std::endl;
    }

    else if (nDFOS == 1) // DY + X selection
    {

        // s+ s- d+/-

        int absid1 = abs(wvz.lep_id().at(lep_fakeable_idxs[0]));
        int absid2 = abs(wvz.lep_id().at(lep_fakeable_idxs[1]));
        int absid3 = abs(wvz.lep_id().at(lep_fakeable_idxs[2]));

        // Select the same-sign pair idx
        if (absid1 == absid2)
        {
            lep_DYX_DYCand_idx1 = lep_fakeable_idxs[0];
            lep_DYX_DYCand_idx2 = lep_fakeable_idxs[1];
            lep_DYX_FakeCand_idx = lep_fakeable_idxs[2];
        }
        else if (absid1 == absid3)
        {
            lep_DYX_DYCand_idx1 = lep_fakeable_idxs[0];
            lep_DYX_DYCand_idx2 = lep_fakeable_idxs[2];
            lep_DYX_FakeCand_idx = lep_fakeable_idxs[1];
        }
        else if (absid2 == absid3)
        {
            lep_DYX_DYCand_idx1 = lep_fakeable_idxs[1];
            lep_DYX_DYCand_idx2 = lep_fakeable_idxs[2];
            lep_DYX_FakeCand_idx = lep_fakeable_idxs[0];
        }

        // The following always be the fake candidate idx TODO CLEAN UP!!!
        lep_FakeCand_idx2 = lep_DYX_FakeCand_idx;

    }

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

    int lep_veto_idx1 = good_veto_idx.at(0);
    int lep_veto_idx2 = good_veto_idx.at(1);
    int lep_veto_idx3 = good_veto_idx.at(2);

    // Assumes lep_Nom_idx1,2,3 are set
    double dM12 = fabs((leptons.at(lep_veto_idx1) + leptons.at(lep_veto_idx2)).M() - 91.1876);
    double dM13 = fabs((leptons.at(lep_veto_idx1) + leptons.at(lep_veto_idx3)).M() - 91.1876);
    double dM23 = fabs((leptons.at(lep_veto_idx2) + leptons.at(lep_veto_idx3)).M() - 91.1876);
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
void Analysis::selectFakeableLeptons()
{
    // Loop over the leptons and find leptons that fail nominal but pass fakeable id
    lep_notnom_idxs.clear();
    lep_fakeable_idxs.clear();
    lep_Fakeable_idx = -999;
    nFakeableLeptons = 0;
    nNotNomLeptons = 0;
    for (unsigned int jj = 0 ; jj < lep_pt->size(); jj++)
    {

        if (not (passFakeableLeptonID(jj)))
            continue;

        lep_fakeable_idxs.push_back(jj);

        nFakeableLeptons++;

        if (passNominalLeptonID(jj))
            continue;

        lep_notnom_idxs.push_back(jj);

        nNotNomLeptons++;
    }

    if (lep_notnom_idxs.size() > 0)
        lep_Fakeable_idx = lep_notnom_idxs.at(0);

    lep_Fakeable_MCmatched_idx = -999;
    for (auto& idx : lep_fakeable_idxs)
    {
        if (wvz.lep_motherIdv2()[idx] < 1)
        {
            lep_Fakeable_MCmatched_idx = idx;
            break;
        }
    }

    return;

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

    lep_VetoButNotNom_idx = good_idx.at(0);
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
        lep_Nom_idx1 = leptons.at(lep_Nom_idx1).Pt() > leptons.at(lep_Nom_idx2).Pt() ? tmp1 : tmp2;
        lep_Nom_idx2 = leptons.at(lep_Nom_idx1).Pt() > leptons.at(lep_Nom_idx2).Pt() ? tmp2 : tmp1;
    }
    else if (nNominalLeptons == 3)
    {
        std::vector<MyLepton> lepidx;
        lepidx.push_back(MyLepton(lep_Nom_idx1, leptons.at(lep_Nom_idx1).Pt()));
        lepidx.push_back(MyLepton(lep_Nom_idx2, leptons.at(lep_Nom_idx2).Pt()));
        lepidx.push_back(MyLepton(lep_Nom_idx3, leptons.at(lep_Nom_idx3).Pt()));
        std::sort(lepidx.begin(), lepidx.end(), less_than_key());

        lep_Nom_idx1 = lepidx.at(0).idx;
        lep_Nom_idx2 = lepidx.at(1).idx;
        lep_Nom_idx3 = lepidx.at(2).idx;

    }

    lep_Veto_idx1 = -999;
    lep_Veto_idx2 = -999;
    lep_Veto_idx3 = -999;
    lep_Veto_idx4 = -999;

    std::vector<MyLepton> lepvetoidx;
    for (auto& idx : lep_veto_idxs)
    {
        lepvetoidx.push_back(MyLepton(idx, leptons.at(idx).Pt()));
    }
    std::sort(lepvetoidx.begin(), lepvetoidx.end(), less_than_key());

    if (nVetoLeptons > 0) lep_Veto_idx1 = lepvetoidx.at(0).idx;
    if (nVetoLeptons > 1) lep_Veto_idx2 = lepvetoidx.at(1).idx;
    if (nVetoLeptons > 2) lep_Veto_idx3 = lepvetoidx.at(2).idx;
    if (nVetoLeptons > 3) lep_Veto_idx4 = lepvetoidx.at(3).idx;

    lep_veto_idxs.clear();
    if (nVetoLeptons > 0) lep_veto_idxs.push_back(lep_Veto_idx1);
    if (nVetoLeptons > 1) lep_veto_idxs.push_back(lep_Veto_idx2);
    if (nVetoLeptons > 2) lep_veto_idxs.push_back(lep_Veto_idx3);
    if (nVetoLeptons > 3) lep_veto_idxs.push_back(lep_Veto_idx4);

    // Sort Z lepton indices
    if (lep_ZCand_idx1 >= 0 and lep_ZCand_idx2 >= 0)
    {
        int tmp1 = lep_ZCand_idx1;
        int tmp2 = lep_ZCand_idx2;
        lep_ZCand_idx1 = leptons.at(lep_ZCand_idx1).Pt() > leptons.at(lep_ZCand_idx2).Pt() ? tmp1 : tmp2;
        lep_ZCand_idx2 = leptons.at(lep_ZCand_idx1).Pt() > leptons.at(lep_ZCand_idx2).Pt() ? tmp2 : tmp1;
    }

}

//______________________________________________________________________________________________
void Analysis::setDilepMasses()
{
    dilepZCand = leptons.at(lep_ZCand_idx1) + leptons.at(lep_ZCand_idx2);
    dilepNominal = leptons.at(lep_Nom_idx1) + leptons.at(lep_Nom_idx2);
}

//______________________________________________________________________________________________
void Analysis::correctMET()
{
    metobj.extras.met = metobj.extras.met_original = metobj.extras.met_raw
    = metobj.extras.met_METup = metobj.extras.met_METdn
    = metobj.extras.met_JERup = metobj.extras.met_JERdn
    = metobj.extras.met_PUup = metobj.extras.met_PUdn
    = metobj_corrected.extras.met = metobj_corrected.extras.met_original = metobj_corrected.extras.met_raw
    = metobj_corrected.extras.met_METup = metobj_corrected.extras.met_METdn
    = metobj_corrected.extras.met_JERup = metobj_corrected.extras.met_JERdn
    = metobj_corrected.extras.met_PUup = metobj_corrected.extras.met_PUdn
    = wvz.met_pt(); // This is ptmiss from the MET recipe alone (nominal).

    metobj.extras.phi = metobj.extras.phi_original = metobj.extras.phi_raw
    = metobj.extras.phi_METup = metobj.extras.phi_METdn
    = metobj.extras.phi_JECup = metobj.extras.phi_JECdn
    = metobj.extras.phi_JERup = metobj.extras.phi_JERdn
    = metobj.extras.phi_PUup = metobj.extras.phi_PUdn
    = metobj_corrected.extras.phi = metobj_corrected.extras.phi_original = metobj_corrected.extras.phi_raw
    = metobj_corrected.extras.phi_METup = metobj_corrected.extras.phi_METdn
    = metobj_corrected.extras.phi_JECup = metobj_corrected.extras.phi_JECdn
    = metobj_corrected.extras.phi_JERup = metobj_corrected.extras.phi_JERdn
    = metobj_corrected.extras.phi_PUup = metobj_corrected.extras.phi_PUdn
    = wvz.met_phi(); // Nominal MET phi from MET recipe alone

    metobj.extras.met_JECup = metobj_corrected.extras.met_JECup = wvz.met_up_pt(); // MET JES up
    metobj.extras.met_JECdn = metobj_corrected.extras.met_JECdn = wvz.met_dn_pt(); // MET JES dn
    metobj.extras.phi_JECup = metobj_corrected.extras.phi_JECup = wvz.met_up_phi(); // MET phi JES up
    metobj.extras.phi_JECdn = metobj_corrected.extras.phi_JECdn = wvz.met_dn_phi(); // MET phi JES dn

    metcorrector.correctMET(wvz.met_gen_pt(), wvz.met_gen_phi(), &metobj_corrected, false);

}

//
//
// Lepton ID functions
//
//

//______________________________________________________________________________________________
bool Analysis::passFakeableLeptonID(int idx)
{
    if (abs(lep_id->at(idx)) == 11)
        return passFakeableElectronID(idx);
    else /*if (abs(lep_id->at(idx)) == 13)*/
        return passFakeableMuonID(idx);
}

//______________________________________________________________________________________________
bool Analysis::passFakeableElectronID(int idx)
{
    // if (not (passVetoElectronID(idx))) return false;
    if (!( wvz.lep_p4().at(idx).pt() > 10.         )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.5  )) return false;
    if (!( wvz.lep_isCutBasedNoIsoVetoPOG().at(idx)    )) return false;
    if (not (fabs(lep_sip3d->at(idx)) < 4       )) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::passFakeableMuonID(int idx)
{
    // if (not (passVetoMuonID(idx))) return false;
    if (!( wvz.lep_p4().at(idx).pt() > 10.        )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.4 )) return false;
    if (not (wvz.lep_isMediumPOG().at(idx)        )) return false;
    if (not (fabs(lep_sip3d->at(idx)) < 4      )) return false;
    return true;
}

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

    // // HEM Veto
    // if (
    //         (wvz.isData() and wvz.run() >= 319077)
    //      // or ((not wvz.isData()) and ((wvz.evt() % 1961) < 1286))
    //    )
    // {
    //     if (    wvz.lep_p4().at(idx).eta() > -4.7
    //         and wvz.lep_p4().at(idx).eta() < -1.4
    //         and wvz.lep_p4().at(idx).phi() > -1.6
    //         and wvz.lep_p4().at(idx).phi() < -0.8) return false;
    // }

    if (!( wvz.lep_p4().at(idx).pt() > 10.           )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.5    )) return false;
    if (!( wvz.lep_isCutBasedIsoVetoPOG().at(idx)    )) return false;
    // following is already applied in the baby making stage
    // if (fabs(wvz.els_etaSC().at(idx)) <= 1.479)
    // {
    //     if (!( fabs(wvz.lep_dz().at(idx)) < 0.1      )) return false;
    //     if (!( fabs(wvz.lep_dxy().at(idx)) < 0.05    )) return false;
    // }
    // else
    // {
    //     if (!( fabs(wvz.lep_dz().at(idx)) < 0.2      )) return false;
    //     if (!( fabs(wvz.lep_dxy().at(idx)) < 0.1     )) return false;
    // }

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passVetoMuonID(int idx)
{

    if (!( wvz.lep_p4().at(idx).pt() > 10.        )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.4 )) return false;
    if (!( wvz.lep_relIso04DB().at(idx) < 0.25    )) return false;
    // following is already applied in the baby making stage
    // if (fabs(cms3.mus_p4().at(idx).eta()) <= 1.479)
    // {
    //     if (!( fabs(cms3.mus_dzPV().at(idx)) < 0.1       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV().at(idx)) < 0.05     )) return false;
    // }
    // else
    // {
    //     if (!( fabs(cms3.mus_dzPV().at(idx)) < 0.2       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV().at(idx)) < 0.1      )) return false;
    // }

    // Try Medium POG ID
    if (not (wvz.lep_isMediumPOG().at(idx))) return false;

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

    if (!( wvz.lep_p4().at(idx).pt() > 10.           )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.5    )) return false;
    if (!( wvz.lep_isCutBasedIsoVetoPOG().at(idx)    )) return false;
    // following is already applied in the baby making stage
    // if (fabs(wvz.els_etaSC().at(idx)) <= 1.479)
    // {
    //     if (!( fabs(wvz.lep_dz().at(idx)) < 0.1      )) return false;
    //     if (!( fabs(wvz.lep_dxy().at(idx)) < 0.05    )) return false;
    // }
    // else
    // {
    //     if (!( fabs(wvz.lep_dz().at(idx)) < 0.2      )) return false;
    //     if (!( fabs(wvz.lep_dxy().at(idx)) < 0.1     )) return false;
    // }

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::passZCandMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passVetoMuonID(idx))) return false;

    if (!( wvz.lep_p4().at(idx).pt() > 10.        )) return false;
    if (!( fabs(wvz.lep_p4().at(idx).eta()) < 2.4 )) return false;
    if (!( wvz.lep_relIso04DB().at(idx) < 0.25    )) return false;
    // following is already applied in the baby making stage
    // if (fabs(cms3.mus_p4().at(idx).eta()) <= 1.479)
    // {
    //     if (!( fabs(cms3.mus_dzPV().at(idx)) < 0.1       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV().at(idx)) < 0.05     )) return false;
    // }
    // else
    // {
    //     if (!( fabs(cms3.mus_dzPV().at(idx)) < 0.2       )) return false;
    //     if (!( fabs(cms3.mus_dxyPV().at(idx)) < 0.1      )) return false;
    // }

    // Try Medium POG ID
    if (not (wvz.lep_isMediumPOG().at(idx))) return false;

    // One addition on top of veto ID
    if (not (fabs(lep_sip3d->at(idx)) < 4)) return false;

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
        if (not (wvz.lep_isMVAwp80IsoPOG().at(idx))) return false;
    }
    else
    {
        if (not (wvz.lep_isCutBasedIsoMediumPOG().at(idx))) return false;
    }

    return true;
}

//______________________________________________________________________________________________
bool Analysis::passNominalMuonID(int idx)
{
    // Must pass ZCand ID
    if (not (passZCandMuonID(idx))) return false;

    // Tight POG muon isolation
    if (not (fabs(wvz.lep_relIso04DB().at(idx)) < 0.15)) return false;

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
        if (looper->getCurrentFileName().Contains("v0.1.12.1")
                and looper->getCurrentFileName().Contains("wwz_4l2v_amcatnlo"))
            fixXsec = 0.9913678619; // GenXsecAnalyser vs. McM for 4l2v wwz sample
        if ((looper->getCurrentFileName().Contains("v0.1.12.1")
                or looper->getCurrentFileName().Contains("v0.1.12.2")
                or looper->getCurrentFileName().Contains("v0.1.12.6"))
                and year == 2017
                and looper->getCurrentFileName().Contains("wwz_4l2v_amcatnlo"))
            fixXsec = 1.1376131152; // Error from wrong scale1fb
        if (looper->getCurrentFileName().Contains("v0.1.12.7")
                and looper->getCurrentFileName().Contains("ggh_hzz4l_powheg_1"))
            fixXsec = 1.1287633316; // Difference between scale1fb and HXSWG twiki
        if (looper->getCurrentFileName().Contains("v0.1.12.7")
                and year == 2017
                and looper->getCurrentFileName().Contains("ggh_hzz4l_powheg_1"))
            fixXsec = 1.1287633316 * 1.236e-05 / 5.617e-05; // Difference between scale1fb and HXSWG twiki
        if (looper->getCurrentFileName().Contains("v0.1.12.7")
                and looper->getCurrentFileName().Contains("zz_4l_powheg_1"))
            fixXsec = 1.1; // Missing K-factor (scale1fb set to 1.256 which is without kfactor)
        if (looper->getCurrentFileName().Contains("v0.1.12.7")
                and looper->getCurrentFileName().Contains("ttz_llvv_mll10"))
            fixXsec = 0.2728/0.2529; // TTZ AN2018-025 has 0.2728 while we used 0.2529
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
        int absid = abs(wvz.lep_id().at(lep_idx));
        float pt = absid == 11 ? std::min((double) wvz.lep_pt().at(lep_idx), 499.9) : std::min((double) wvz.lep_pt().at(lep_idx), 119.9);
        float eta = wvz.lep_eta().at(lep_idx);
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
float Analysis::LeptonScaleFactorZZ4l()
{
    if (wvz.isData())
        return 1.;
    // Based on lep_Veto indices
    float scalefactor = 1;
    scalefactor *= IndividualLeptonScaleFactor(lep_ZCand_idx1, false);
    scalefactor *= IndividualLeptonScaleFactor(lep_ZCand_idx2, false);
    scalefactor *= IndividualLeptonScaleFactor(lep_Z2Cand_idx1, false);
    scalefactor *= IndividualLeptonScaleFactor(lep_Z2Cand_idx2, false);
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
    int absid = abs(wvz.lep_id().at(lep_idx));
    float pt = absid == 11 ? std::min((double) wvz.lep_pt().at(lep_idx), 499.9) : std::min((double) wvz.lep_pt().at(lep_idx), 119.9);
    float eta = wvz.lep_eta().at(lep_idx);
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
            if (abs(wvz.lep_id().at(lep_VetoButNotNom_idx)) == 11)
                fr = histmap_2016_fake_rate_el->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
            else
                fr = histmap_2016_fake_rate_mu->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
        }
        else if (year == 2017)
        {
            if (abs(wvz.lep_id().at(lep_VetoButNotNom_idx)) == 11)
                fr = histmap_2017_fake_rate_el->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
            else
                fr = histmap_2017_fake_rate_mu->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
        }
        else if (year == 2018)
        {
            if (abs(wvz.lep_id().at(lep_VetoButNotNom_idx)) == 11)
                fr = histmap_2018_fake_rate_el->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
            else
                fr = histmap_2018_fake_rate_mu->eval(std::min((double) leptons.at(lep_VetoButNotNom_idx).Pt(), 69.999), fabs(leptons.at(lep_VetoButNotNom_idx).Eta()));
        }
        if (wvz.isData())
        {
            return fr / (1. - fr);
        }
        else
        {
            if (wvz.lep_motherIdv2().at(lep_VetoButNotNom_idx) >= 1)
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
    if (lep_ZCand_idx2 == -999)
        return false;
    float mll = (leptons.at(lep_ZCand_idx2) + leptons.at(lep_ZCand_idx1)).M();
    if (not (fabs(mll - 91.1876) < 10.))
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::FindTwoOSNominalLeptons()
{
    // Found first nominal lepton
    if (not (lep_Nom_idx1 != -999                                   )) return false;
    // Found second nominal lepton
    if (not (lep_Nom_idx2 != -999                                   )) return false;
    // The pair makes OS
    if (not (lep_id->at(lep_Nom_idx1) * lep_id->at(lep_Nom_idx2) < 0)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::FindTwoOSZ2Leptons()
{
    // Found first nominal lepton
    if (not (lep_Z2Cand_idx1 != -999                                   )) return false;
    // Found second nominal lepton
    if (not (lep_Z2Cand_idx2 != -999                                   )) return false;
    // The pair makes OS
    if (not (lep_id->at(lep_Z2Cand_idx1) * lep_id->at(lep_Z2Cand_idx2) < 0)) return false;
    return true;
}


//______________________________________________________________________________________________
bool Analysis::FindOSOneNomOneNotNomLeptons()
{
    if (lep_Nom_idx1 == -999)
        return false;
    if (nNominalLeptons != 1)
        return false;
    if (lep_notnom_idxs.size() == 0)
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
    if (lep_ZCand_idx1 < 0)
        return false;
    if (lep_ZCand_idx2 < 0)
        return false;
    if (lep_Nom_idx1 < 0)
        return false;
    if (isAR)
    {
        if (lep_Fakeable_idx < 0)
            return false;
    }
    else
    {
        if (lep_Nom_idx2 < 0)
            return false;
    }
    std::vector<int> indices;
    indices.push_back(lep_Nom_idx1);
    indices.push_back(isAR ? lep_Fakeable_idx : lep_Nom_idx2);
    indices.push_back(lep_ZCand_idx1);
    indices.push_back(lep_ZCand_idx2);

    // of the 4 leptons we have if any of the pair has < 12 GeV then return false
    bool pass = true;
    for (unsigned int ii = 0; ii < indices.size() - 1; ++ii)
    {
        for (unsigned int jj = ii + 1; jj < indices.size(); ++jj)
        {
            int id1 = lep_id->at(indices.at(ii));
            int id2 = lep_id->at(indices.at(jj));

            if (id1 * id2 < 0)
            {
                if ((leptons.at(indices.at(ii)) + leptons.at(indices.at(jj))).M() < 12.)
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
bool Analysis::CutLowPtZeta(int var)
{
    if (not (VarPtZetaDiff(var) < -40.)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::CutHighMT(int var)
{
    if (not (VarMTNom0(var) > 40.)) return false;
    if (not (VarMTNom1(var) > 20.)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::CutHighMTAR(int var)
{
    if (not (VarMTNom0(var) > 40.)) return false;
    if (not (VarMTFakeable(var) > 20.)) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Cut4LepLeptonPt(bool isAR)
{
    if (lep_ZCand_idx1 < 0)
        return false;
    if (lep_ZCand_idx2 < 0)
        return false;
    if (lep_Nom_idx1 < 0)
        return false;
    if (isAR)
    {
        if (lep_Fakeable_idx < 0)
            return false;
    }
    else
    {
        if (lep_Nom_idx2 < 0)
            return false;
    }
    float lepZpt1 = wvz.lep_pt().at(lep_ZCand_idx1);
    float lepZpt2 = wvz.lep_pt().at(lep_ZCand_idx2);
    float lepNpt1 = wvz.lep_pt().at(lep_Nom_idx1);
    float lepNpt2 = isAR ? wvz.lep_pt().at(lep_Fakeable_idx) : wvz.lep_pt().at(lep_Nom_idx2);

    // Nominal needs sorting in case it is isAR
    float tmp1 = lepNpt1;
    float tmp2 = lepNpt2;
    lepNpt1 = tmp1 > tmp2 ? tmp1 : tmp2;
    lepNpt2 = tmp1 > tmp2 ? tmp2 : tmp1;

    if (lepZpt1 > 25. and lepZpt2 > 10. and lepNpt1 > 25. and lepNpt2 > 10.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::CutZZ4LepLeptonPt()
{
    if(lep_Z2Cand_idx1<0 || lep_Z2Cand_idx2<0 || lep_ZCand_idx1<0 || lep_ZCand_idx2<0)
        return false;

    float lepZpt1 = wvz.lep_pt().at(lep_ZCand_idx1);
    float lepZpt2 = wvz.lep_pt().at(lep_ZCand_idx2);
    float lepNpt1 = wvz.lep_pt().at(lep_Z2Cand_idx1);
    float lepNpt2 = wvz.lep_pt().at(lep_Z2Cand_idx2);

    if (!(lepZpt1 > 10. and lepZpt2 > 10. and lepNpt1 > 10. and lepNpt2 > 10.)) return false;
    vector<float> leptonPtVector;
    leptonPtVector.push_back(lepZpt1);
    leptonPtVector.push_back(lepZpt2);
    leptonPtVector.push_back(lepNpt1);
    leptonPtVector.push_back(lepNpt2);
    std::sort(leptonPtVector.begin(), leptonPtVector.end());
    float leadLeptonPt = leptonPtVector[3];
    float subleadLeptonPt = leptonPtVector[2];
    if(!(leadLeptonPt > 25 && subleadLeptonPt > 15)) return false;

    return true;

}

//______________________________________________________________________________________________
bool Analysis::CutHLT()
{
    if (nVetoLeptons < 2)
        return false;
    if (not wvz.passesMETfiltersRun2())
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
            if (abs(wvz.lep_id().at(lep_idx)) == 11 and abs(wvz.lep_id().at(lep_jdx)) == 11)
                passTrigger |= (wvz.HLT_DoubleEl() and wvz.lep_pt().at(lep_idx) > 25 and wvz.lep_pt().at(lep_jdx) > 15);
            else if (abs(wvz.lep_id().at(lep_idx)) == 13 and abs(wvz.lep_id().at(lep_jdx)) == 11)
                passTrigger |= (wvz.HLT_MuEG() and wvz.lep_pt().at(lep_idx) > 25 and wvz.lep_pt().at(lep_jdx) > 15);
            else if (abs(wvz.lep_id().at(lep_idx)) == 11 and abs(wvz.lep_id().at(lep_jdx)) == 13)
                passTrigger |= (wvz.HLT_MuEG() and wvz.lep_pt().at(lep_idx) > 25 and wvz.lep_pt().at(lep_jdx) > 10);
            else if (abs(wvz.lep_id().at(lep_idx)) == 13 and abs(wvz.lep_id().at(lep_jdx)) == 13)
                passTrigger |= (wvz.HLT_DoubleMu() and wvz.lep_pt().at(lep_idx) > 20 and wvz.lep_pt().at(lep_jdx) > 10);
        }
    }
    return passTrigger;
}

//______________________________________________________________________________________________
bool Analysis::Is3LeptonEvent()
{
    if (not (nVetoLeptons >= 2 and nVetoLeptons <= 3)) return false;
    if (not (nTightLeptons >= 2)) return false;
    if (not (nFakeableLeptons >= 3)) return false;
    if (not (CutHLT())) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Is3LeptonTTZEvent()
{
    if (not (lep_ZCand_idx1 >= 0 or lep_ZCand_idx2 >= 0)) return false;
    if (not (nNominalLeptons == 1)) return false;
    if (not (nVetoLeptons == 3)) return false;
    if (not (CutHLT())) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::CutGenFilter()
{
    if (looper->getCurrentFileName().Contains("wwz_amcatnlo"))
    {
        if (year == 2016)
        {
            return true;
        }
        else
        {
            // if (not wvz.hasTau())
            // if (wvz.nGenTau() == 0)
            if (wvz.nLightLep() == 4)
                return false;
            else
                return true;
        }
    }
    // else if (looper->getCurrentFileName().Contains("wwz_4l2v_amcatnlo"))
    // {
    //     if (year == 2016)
    //     {
    //         return true;
    //     }
    //     else
    //     {
    //         if (wvz.nLightLep() == 4 and abs(wvz.gen_V_p4()[2].mass() - 91.1876) < 10.)
    //             return true;
    //         else
    //             return false;
    //     }
    // }
    else
    {
        return true;
    }
}

//______________________________________________________________________________________________
bool Analysis::Is4LeptonEvent()
{
    if (not (nVetoLeptons == 4))
        return false;
    int nSFOS = 0;
    int totalsign = 1;
    for (auto& idx : lep_veto_idxs)
    {
        totalsign *= wvz.lep_id()[idx];
        for (auto& jdx : lep_veto_idxs)
        {
            if (idx == jdx)
                continue;
            if (wvz.lep_id()[idx] == -wvz.lep_id()[jdx])
                nSFOS++;
        }
    }
    if (not (nSFOS >= 1))
        return false;
    if (not (totalsign > 0))
        return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::Is4LeptonFakeValidationEvents()
{
    return nFakeableLeptons == 4;
}

//______________________________________________________________________________________________
bool Analysis::Is3LeptonFakeValidationEvents()
{
    return nFakeableLeptons == 3;
}

//______________________________________________________________________________________________
bool Analysis::Is5LeptonEvent()
{
    if (not (nVetoLeptons == 5)) return false;
    if (not (CutHLT())) return false;
    if (lep_ZCand_idx1 < 0) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::IsEMuPlusX()
{

    if (nFakeableLeptons < 3)
        return false;

    // Check that the sum of the sign of the lepton is +/- 1
    int charge1 = (wvz.lep_id().at(lep_fakeable_idxs[0]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[0]) < 0);
    int charge2 = (wvz.lep_id().at(lep_fakeable_idxs[1]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[1]) < 0);
    int charge3 = (wvz.lep_id().at(lep_fakeable_idxs[2]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[2]) < 0);

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
bool Analysis::IsDYPlusX()
{

    if (nFakeableLeptons < 3)
        return false;

    // Check that the sum of the sign of the lepton is +/- 1
    int charge1 = (wvz.lep_id().at(lep_fakeable_idxs[0]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[0]) < 0);
    int charge2 = (wvz.lep_id().at(lep_fakeable_idxs[1]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[1]) < 0);
    int charge3 = (wvz.lep_id().at(lep_fakeable_idxs[2]) > 0) - (wvz.lep_id().at(lep_fakeable_idxs[2]) < 0);

    if (not (abs(charge1 + charge2 + charge3) == 1))
        return false;

    if (not (nDFOS == 1))
        return false;

    if (not (wvz.nb() == 0))
        return false;

    // Ask for the DY candidate to be on Z peak
    if (not (fabs((wvz.lep_p4().at(lep_DYX_DYCand_idx1) + wvz.lep_p4().at(lep_DYX_DYCand_idx2)).M() - 91.1876) < 10.))
        return false;

    return true;
}

//______________________________________________________________________________________________
bool Analysis::IsTwoOSLeptonEvent()
{
    if (not (nVetoLeptons == 2)) return false;
    if (not (leptons.at(lep_veto_idxs.at(0)).Pt() > 25.)) return false;
    if (not (leptons.at(lep_veto_idxs.at(1)).Pt() > 25.)) return false;
    if (not (lep_id->at(lep_veto_idxs.at(0)) * lep_id->at(lep_veto_idxs.at(1)) < 0)) return false;
    if (not (abs(lep_id->at(lep_veto_idxs.at(0))) == 13)) return false;
    if (not (abs(lep_id->at(lep_veto_idxs.at(1))) == 13)) return false;
    if (not (HLT_DoubleMu)) return false;
    if (not ((leptons.at(lep_veto_idxs.at(0)) + leptons.at(lep_veto_idxs.at(1))).M() > 50.)) return false;
    if (not (passZCandLeptonID(lep_veto_idxs.at(0)))) return false;
    if (not (passZCandLeptonID(lep_veto_idxs.at(1)))) return false;
    return true;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelEMu(bool isAR)
{
    int idx2 = isAR ? lep_Fakeable_idx : lep_Nom_idx2;
    if (lep_id->at(lep_Nom_idx1) * lep_id->at(idx2) == -143)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOnZ(bool isAR)
{
    int idx2 = isAR ? lep_Fakeable_idx : lep_Nom_idx2;
    float mll = isAR ? (leptons.at(lep_Fakeable_idx)+leptons.at(lep_Nom_idx1)).M() : (leptons.at(lep_Nom_idx2)+leptons.at(lep_Nom_idx1)).M();
    if (fabs(mll - 91.1876) < 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOffZ(bool isAR)
{
    int idx2 = isAR ? lep_Fakeable_idx : lep_Nom_idx2;
    float mll = isAR ? (leptons.at(lep_Fakeable_idx)+leptons.at(lep_Nom_idx1)).M() : (leptons.at(lep_Nom_idx2)+leptons.at(lep_Nom_idx1)).M();
    if (fabs(mll - 91.1876) >= 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOffZLowMll(bool isAR)
{
    int idx2 = isAR ? lep_Fakeable_idx : lep_Nom_idx2;
    float mll = isAR ? (leptons.at(lep_Fakeable_idx)+leptons.at(lep_Nom_idx1)).M() : (leptons.at(lep_Nom_idx2)+leptons.at(lep_Nom_idx1)).M();
    if (mll - 91.1876 < -10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelOffZHighMll(bool isAR)
{
    int idx2 = isAR ? lep_Fakeable_idx : lep_Nom_idx2;
    float mll = isAR ? (leptons.at(lep_Fakeable_idx)+leptons.at(lep_Nom_idx1)).M() : (leptons.at(lep_Nom_idx2)+leptons.at(lep_Nom_idx1)).M();
    if (mll - 91.1876 > 10. and lep_id->at(lep_Nom_idx1) == -lep_id->at(idx2)) // must be SFOS
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::IsChannelHZZ4l()
{
    if(lep_Z2Cand_idx1 < 0 || lep_Z2Cand_idx2 <0)
        return false;
    float mll = (leptons.at(lep_Z2Cand_idx1) + leptons.at(lep_Z2Cand_idx2)).M();
    float m4l = (leptons.at(lep_Z2Cand_idx1) + leptons.at(lep_Z2Cand_idx2) + leptons.at(lep_ZCand_idx1) + leptons.at(lep_ZCand_idx2)).M();
    if (lep_id->at(lep_Z2Cand_idx1) != -lep_id->at(lep_Z2Cand_idx2))
        return false;
    if (fabs(mll - 28.5) > 16.5)
        return false;
    if (fabs(m4l - 125.0) > 5.0)
        return false;
    else
        return true;
}

//______________________________________________________________________________________________
bool Analysis::Is2ndOnZFiveLepton()
{
    if (lep_2ndZCand_idx1 < 0 or lep_2ndZCand_idx2 < 0)
        return false;
    // if (fabs((leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M() - 91.1876) < 30. and leptons.at(lep_2ndZCand_idx1).Pt() > 25.)
    // if ((leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M() > 76.1876 and leptons.at(lep_2ndZCand_idx1).Pt() > 25.)
    // if (fabs((leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M() - 91.1876) < 15. and leptons.at(lep_2ndZCand_idx1).Pt() > 25.)
    if ((leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M() > 76.1876 and (leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M() < (91.1876+15) and leptons.at(lep_2ndZCand_idx1).Pt() > 25.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::ChannelEMuHighMll(bool isAR)
{
    float mll = isAR ? (leptons.at(lep_Nom_idx1)+leptons.at(lep_Fakeable_idx)).M() : (leptons.at(lep_Nom_idx1)+leptons.at(lep_Nom_idx2)).M();
    if (mll > 120.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::CutHighMET(int var)
{
    if (this->VarMET(var) > 100.)
        return true;
    else
        return false;
}

//______________________________________________________________________________________________
bool Analysis::Is5thNominal()
{

    // std::vector<int> idxs;
    // idxs.push_back(lep_ZCand_idx1);
    // idxs.push_back(lep_ZCand_idx2);
    // idxs.push_back(lep_2ndZCand_idx1);
    // idxs.push_back(lep_2ndZCand_idx2);
    // idxs.push_back(lep_WCand_idx1);

    // int idx_w_max_reliso = -1;
    // float max_iso = 0;

    // for (auto& idx : idxs)
    // {
    //     float tmpiso = wvz.lep_relIso03EA()[idx];
    //     if (max_iso < tmpiso)
    //     {
    //         max_iso = tmpiso;
    //         idx_w_max_reliso = idx;
    //     }
    // }

    // if (idx_w_max_reliso < 0)
    //     return passNominalLeptonID(lep_WCand_idx1) and wvz.lep_p4()[lep_WCand_idx1].pt() > 20.;
    // else
    //     return passNominalLeptonID(idx_w_max_reliso) and wvz.lep_p4()[lep_WCand_idx1].pt() > 20.;

    return passNominalLeptonID(lep_WCand_idx1) and passNominalLeptonID(lep_2ndZCand_idx2) and wvz.lep_p4()[lep_WCand_idx1].pt() > 20.;
    // if (lep_relIso03EA->at(lep_WCand_idx1) < 0.1)
    //     return true;
    // else
    //     return false;
}

//______________________________________________________________________________________________
bool Analysis::IsNjetGeq2()
{
    return nj >= 2;
}

//______________________________________________________________________________________________
bool Analysis::CutHEMVeto()
{
    // HEM region def
    // https://github.com/cmstas/MT2Analysis/blob/d62b3eb0ce95a158e28e39a43d8c8d279c57aca6/MT2Looper/MT2Looper.cc#L144-L148
    int HEM_startRun = 319077; // affects 38.58 out of 58.83 fb-1 in 2018
    uint HEM_fracNum = 1286, HEM_fracDen = 1961; // 38.58/58.82 ~= 1286/1961. Used for figuring out if we should veto MC events
    float HEM_region[4] = {-4.7, -1.4, -1.6, -0.8}; // etalow, etahigh, philow, phihigh

    if (year == 2018)
    {
        bool isHEMVeto = false;
        if (
                (wvz.isData() and (wvz.run() >= HEM_startRun))
             or ((not wvz.isData()) and ((wvz.evt() % HEM_fracDen) < HEM_fracNum))
           )
        {

            for (unsigned int i = 0; i < wvz.rawjets_p4().size(); i++)
            {

                // if (not wvz.rawjets_passJetID()[i])
                //     continue;

                // if (wvz.rawjets_isORwLep()[i])
                //     continue;

                LV jet = wvz.rawjets_p4()[i] * wvz.rawjets_JEC()[i];

                if (jet.pt() < 30.)
                    continue;

                if (jet.eta() > HEM_region[0]
                and jet.eta() < HEM_region[1]
                and jet.phi() > HEM_region[2]
                and jet.phi() < HEM_region[3])
                    isHEMVeto = true;

            }

            for (unsigned int i = 0; i < wvz.lep_p4().size(); i++)
            {

                LV lep = wvz.lep_p4()[i];

                if (lep.eta() > HEM_region[0]
                and lep.eta() < HEM_region[1]
                and lep.phi() > HEM_region[2]
                and lep.phi() < HEM_region[3])
                    isHEMVeto = true;

            }

        }

        if (isHEMVeto)
            return false;
    }

    return true;
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
    if (lep_Nom_idx1 < 0 or lep_Nom_idx2 < 0)
        return -999;
    return (leptons.at(lep_Nom_idx1) + leptons.at(lep_Nom_idx2)).M();
}

//______________________________________________________________________________________________
float Analysis::VarMll2ndZ()
{
    if (lep_2ndZCand_idx1 < 0 or lep_2ndZCand_idx2 < 0)
        return -999;
    return (leptons.at(lep_2ndZCand_idx1) + leptons.at(lep_2ndZCand_idx2)).M();
}

//______________________________________________________________________________________________
float Analysis::VarMET(int var)
{
    if (wvz.isData())
    {
        if      (var == 0) return wvz.met_pt();
        else if (var == 1) return wvz.met_up_pt();
        else if (var == 2) return wvz.met_pt();
        else if (var == 3) return wvz.met_pt();
        else if (var == 4) return wvz.met_pt();
        else if (var ==-1) return wvz.met_pt();
        else if (var ==-2) return wvz.met_dn_pt();
        else if (var ==-3) return wvz.met_pt();
        else if (var ==-4) return wvz.met_pt();
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "VarMET");
        return -999;
    }
    else
    {
        if      (var == 0) return metobj_corrected.extras.met;
        else if (var == 1) return metobj_corrected.extras.met_JECup;
        else if (var == 2) return metobj_corrected.extras.met_JERup;
        else if (var == 3) return metobj_corrected.extras.met_METup;
        else if (var == 4) return metobj_corrected.extras.met_PUup;
        else if (var ==-1) return metobj_corrected.extras.met_JECdn;
        else if (var ==-2) return metobj_corrected.extras.met_JERdn;
        else if (var ==-3) return metobj_corrected.extras.met_METdn;
        else if (var ==-4) return metobj_corrected.extras.met_PUdn;
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "VarMET");
        return -999;
    }
}

//______________________________________________________________________________________________
float Analysis::VarMETPhi(int var)
{
    if (wvz.isData())
    {
        if      (var == 0) return wvz.met_phi();
        else if (var == 1) return wvz.met_up_phi();
        else if (var == 2) return wvz.met_phi();
        else if (var == 3) return wvz.met_phi();
        else if (var == 4) return wvz.met_phi();
        else if (var ==-1) return wvz.met_phi();
        else if (var ==-2) return wvz.met_dn_phi();
        else if (var ==-3) return wvz.met_phi();
        else if (var ==-4) return wvz.met_phi();
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "VarMETPhi");
        return -999;
    }
    else
    {
        if      (var == 0) return metobj_corrected.extras.phi;
        else if (var == 1) return metobj_corrected.extras.phi_JECup;
        else if (var == 2) return metobj_corrected.extras.phi_JERup;
        else if (var == 3) return metobj_corrected.extras.phi_METup;
        else if (var == 4) return metobj_corrected.extras.phi_PUup;
        else if (var ==-1) return metobj_corrected.extras.phi_JECdn;
        else if (var ==-2) return metobj_corrected.extras.phi_JERdn;
        else if (var ==-3) return metobj_corrected.extras.phi_METdn;
        else if (var ==-4) return metobj_corrected.extras.phi_PUdn;
        RooUtil::error(TString::Format("Unrecognized variation value var = %d", var).Data(), "VarMETPhi");
        return -999;
    }
}

//______________________________________________________________________________________________
float Analysis::VarNvtx()
{
    return wvz.nvtx();
}

//______________________________________________________________________________________________
float Analysis::VarMT(int idx, int var)
{
    if (idx < 0)
    {
        return -999;
    }
    if (idx >= (int) leptons.size())
    {
        return -999;
    }
    return sqrt(2 * this->VarMET(var) * leptons.at(idx).Et() * (1.0 - cos(leptons.at(idx).Phi() - this->VarMETPhi(var))));
}

//______________________________________________________________________________________________
float Analysis::VarMTll(int idx1, int idx2, int var)
{
    if (idx1 < 0 || idx2 < 0)
    {
        return -999;
    }
    if (idx1 >= (int) leptons.size() || idx2 >= (int) leptons.size())
    {
        return -999;
    }
    return sqrt(2 * this->VarMET(var) * (leptons.at(idx1) + leptons.at(idx2)).Et() * (1.0 - cos((leptons.at(idx1) + leptons.at(idx2)).Phi() - this->VarMETPhi(var))));
}


//______________________________________________________________________________________________
float Analysis::VarMT5th(int var)           { return VarMT(lep_WCand_idx1, var);                                   } 
float Analysis::VarMTNom0(int var)          { return VarMT(lep_Nom_idx1, var);                                     } 
float Analysis::VarMTNom1(int var)          { return VarMT(lep_Nom_idx2, var);                                     } 
float Analysis::VarMTMax(int var)           { return std::max((double) VarMTNom0(var), (double) VarMTNom1(var));   } 
float Analysis::VarMTMin(int var)           { return std::min((double) VarMTNom0(var), (double) VarMTNom1(var));   } 
float Analysis::VarMTVetoButNotNom(int var) { return VarMT(lep_VetoButNotNom_idx, var);                            } 
float Analysis::VarMTFakeable(int var)      { return VarMT(lep_Fakeable_idx, var);                                 } 

//______________________________________________________________________________________________
float Analysis::VarRelIso5th()
{
    if (lep_WCand_idx1 >= 0)
        return lep_relIso03EA->at(lep_WCand_idx1);
        // return lep_relIso03EA->at(lep_WCand_idx1);
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
    return wvz.nj();
}

//______________________________________________________________________________________________
float Analysis::VarNEENoiseJet()
{
    int n_eenoise_jet = 0;
    for (unsigned int i = 0; i < wvz.rawjets_p4().size(); i++)
    {
        if (not wvz.rawjets_passJetID()[i])
            continue;

        if (wvz.rawjets_isORwLep()[i])
            continue;

        LV jet = wvz.rawjets_p4()[i] * wvz.rawjets_JEC()[i];

        float absjeteta = fabs(jet.eta());
        if (absjeteta > 2.65 and absjeteta < 3.139 and jet.pt() < 50. and jet.pt() > 15.)
            n_eenoise_jet++;
    }
    return n_eenoise_jet;
}

//______________________________________________________________________________________________
float Analysis::VarNfwdjet()
{
    int n_fwdjet = 0;
    for (unsigned int i = 0; i < wvz.rawjets_p4().size(); i++)
    {
        if (not wvz.rawjets_passJetID()[i])
            continue;

        if (wvz.rawjets_isORwLep()[i])
            continue;

        LV jet = wvz.rawjets_p4()[i] * wvz.rawjets_JEC()[i];

        float absjeteta = fabs(jet.eta());
        if (absjeteta > 2.5 and jet.pt() > 20.)
            n_fwdjet++;
    }
    return n_fwdjet;
}

//______________________________________________________________________________________________
float Analysis::VarNb()
{
    return wvz.nb();
}

//______________________________________________________________________________________________
float Analysis::VarLepPt(int idx)
{
    if (idx < 0)
        return -999;
    return wvz.lep_p4().at(idx).pt();
}

//______________________________________________________________________________________________
float Analysis::VarLepEta(int idx)
{
    if (idx < 0)
        return -999;
    return wvz.lep_p4().at(idx).eta();
}

//______________________________________________________________________________________________
float Analysis::VarLepPhi(int idx)
{
    if (idx < 0)
        return -999;
    return wvz.lep_p4().at(idx).phi();
}

//______________________________________________________________________________________________
float Analysis::VarMll(int idx, int jdx)
{
    if (idx < 0 or jdx < 0)
        return -999;
    return (wvz.lep_p4().at(idx) + wvz.lep_p4().at(jdx)).M();
}

//______________________________________________________________________________________________
float Analysis::VarPtll(int idx, int jdx)
{
    if (idx < 0 or jdx < 0)
        return -999;
    return (wvz.lep_p4().at(idx) + wvz.lep_p4().at(jdx)).Pt();
}

//______________________________________________________________________________________________
float Analysis::VarM4l(int idx, int jdx, int kdx, int ldx)
{
    if (idx < 0 or jdx < 0 or kdx < 0 or ldx < 0)
        return -999;
    return (wvz.lep_p4().at(idx) + wvz.lep_p4().at(jdx) + wvz.lep_p4().at(kdx) + wvz.lep_p4().at(ldx)).M();
}

//______________________________________________________________________________________________
float Analysis::VarMll2l()
{
    if (nVetoLeptons < 2)
        return -999;
    return (leptons.at(lep_veto_idxs.at(0)) + leptons.at(lep_veto_idxs.at(1))).M();
}

//______________________________________________________________________________________________
float Analysis::VarNSFOS()
{
    if (nVetoLeptons != 3)
        return -999;
    int nSFOS = 0;
    if (wvz.lep_id().at(lep_Veto_idx1) == -wvz.lep_id().at(lep_Veto_idx2)) nSFOS++;
    if (wvz.lep_id().at(lep_Veto_idx1) == -wvz.lep_id().at(lep_Veto_idx3)) nSFOS++;
    if (wvz.lep_id().at(lep_Veto_idx2) == -wvz.lep_id().at(lep_Veto_idx3)) nSFOS++;
    return nSFOS;
}

//______________________________________________________________________________________________
float Analysis::VarHTLep(int idx, int jdx, int kdx, int ldx)
{
    if (idx < 0 or jdx < 0 or kdx < 0 or ldx < 0)
        return -999;
    return (wvz.lep_pt().at(idx) + wvz.lep_pt().at(jdx) + wvz.lep_pt().at(kdx) + wvz.lep_pt().at(ldx));
}

//______________________________________________________________________________________________
float Analysis::VarHTLep5()
{
    float ht = 0;
    for (auto& idx : lep_veto_idxs)
    {
        ht += wvz.lep_pt().at(idx);
    }
    return ht;
}

//______________________________________________________________________________________________
float Analysis::VarTauTauDisc(int var)
{
    return -999;
}

//______________________________________________________________________________________________
float Analysis::VarPtZetaDiff(int var)
{
    if (nVetoLeptons < 4)
        return -999;
    if (lep_ZCand_idx1 < 0)
        return -999;
    if (lep_ZCand_idx2 < 0)
        return -999;
    if (lep_Nom_idx1 < 0)
        return -999;
    if (lep_Nom_idx2 < 0)
        return -999;
    TLorentzVector metv3_, lep1_, lep2_;
    metv3_.SetPtEtaPhiM(this->VarMET(var), 0., this->VarMETPhi(var), 0);
    lep1_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx1), 0,  this->VarLepPhi(lep_Nom_idx1), 0);
    lep2_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx2), 0,  this->VarLepPhi(lep_Nom_idx2), 0);
    metv3 = RooUtil::Calc::getLV(metv3_);
    lep1 = RooUtil::Calc::getLV(lep1_);
    lep2 = RooUtil::Calc::getLV(lep2_);

    float lep1mag = sqrt(lep1.Px()*lep1.Px() + lep1.Py()*lep1.Py() + lep1.Pz()*lep1.Pz());
    float lep2mag = sqrt(lep2.Px()*lep2.Px() + lep2.Py()*lep2.Py() + lep2.Pz()*lep2.Pz());

    zeta = lep1 * lep2mag + lep2 * lep1mag; // find bisector

    float zetamag = sqrt(zeta.Px()*zeta.Px() + zeta.Py()*zeta.Py() + zeta.Pz()*zeta.Pz());

    sum_vis = lep1 + lep2;
    sum = sum_vis + metv3;

    return (sum.Px()*zeta.Px() + sum.Py()*zeta.Py() + sum.Pz()*zeta.Pz()) / zetamag - 1.85 *  (sum_vis.Px()*zeta.Px() + sum_vis.Py()*zeta.Py() + sum_vis.Pz()*zeta.Pz()) / zetamag;
}

//______________________________________________________________________________________________
float Analysis::VarPtZeta(int var)
{
    if (nVetoLeptons < 4)
        return -999;
    if (lep_ZCand_idx1 < 0)
        return -999;
    if (lep_ZCand_idx2 < 0)
        return -999;
    if (lep_Nom_idx1 < 0)
        return -999;
    if (lep_Nom_idx2 < 0)
        return -999;

    TLorentzVector metv3_, lep1_, lep2_;
    metv3_.SetPtEtaPhiM(this->VarMET(var), 0., this->VarMETPhi(var), 0);
    lep1_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx1), 0,  this->VarLepPhi(lep_Nom_idx1), 0);
    lep2_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx2), 0,  this->VarLepPhi(lep_Nom_idx2), 0);
    metv3 = RooUtil::Calc::getLV(metv3_);
    lep1 = RooUtil::Calc::getLV(lep1_);
    lep2 = RooUtil::Calc::getLV(lep2_);
    
    float lep1mag = sqrt(lep1.Px()*lep1.Px() + lep1.Py()*lep1.Py() + lep1.Pz()*lep1.Pz());
    float lep2mag = sqrt(lep2.Px()*lep2.Px() + lep2.Py()*lep2.Py() + lep2.Pz()*lep2.Pz());

    zeta = lep1 * lep2mag + lep2 * lep1mag; // find bisector

    float zetamag = sqrt(zeta.Px()*zeta.Px() + zeta.Py()*zeta.Py() + zeta.Pz()*zeta.Pz());
    sum_vis = lep1 + lep2;
    sum = sum_vis + metv3;

    return  (sum.Px()*zeta.Px() + sum.Py()*zeta.Py() + sum.Pz()*zeta.Pz()) / zetamag;
    
}

//______________________________________________________________________________________________
float Analysis::VarPtZetaVis(int var)
{
    if (nVetoLeptons < 4)
        return -999;
    if (lep_ZCand_idx1 < 0)
        return -999;
    if (lep_ZCand_idx2 < 0)
        return -999;
    if (lep_Nom_idx1 < 0)
        return -999;
    if (lep_Nom_idx2 < 0)
        return -999;
    TLorentzVector metv3_, lep1_, lep2_;
    metv3_.SetPtEtaPhiM(this->VarMET(var), 0., this->VarMETPhi(var), 0);
    lep1_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx1), 0,  this->VarLepPhi(lep_Nom_idx1), 0);
    lep2_.SetPtEtaPhiM(this->VarLepPt(lep_Nom_idx2), 0,  this->VarLepPhi(lep_Nom_idx2), 0);
    metv3 = RooUtil::Calc::getLV(metv3_);
    lep1 = RooUtil::Calc::getLV(lep1_);
    lep2 = RooUtil::Calc::getLV(lep2_);

    float lep1mag = sqrt(lep1.Px()*lep1.Px() + lep1.Py()*lep1.Py() + lep1.Pz()*lep1.Pz());
    float lep2mag = sqrt(lep2.Px()*lep2.Px() + lep2.Py()*lep2.Py() + lep2.Pz()*lep2.Pz());

    zeta = lep1 * lep2mag + lep2 * lep1mag; // find bisector
    float zetamag = sqrt(zeta.Px()*zeta.Px() + zeta.Py()*zeta.Py() + zeta.Pz()*zeta.Pz());

    sum_vis = lep1 + lep2;
    return  (sum_vis.Px()*zeta.Px() + sum_vis.Py()*zeta.Py() + sum_vis.Pz()*zeta.Pz()) / zetamag;
}

//______________________________________________________________________________________________
float Analysis::VarMinDRJetsToLep(int idx)
{
    float minDR = 9999;
    for (unsigned int jj = 0 ; jj < wvz.jets_p4().size(); jj++)
    {
        float dphi = wvz.jets_p4()[jj].phi() - lep_phi->at(idx);
        float deta = wvz.jets_p4()[jj].eta() - lep_eta->at(idx);
        float dR_this = sqrt(dphi * dphi + deta * deta);
        if (dR_this < minDR) minDR = dR_this;
    }
    if (minDR > 9998) minDR = -999; //default value: -999
    return minDR;
}

//______________________________________________________________________________________________
float Analysis::VarXGBBDT(int idx)
{
   //index:
   //0 - wwz_vs_ttz_bVeto
   //1 - wwz_vs_ttz_emu
   //2 - wwz_vs_ttz_nbAll
   //3 - wwz_vs_ttz_OffZ
   //4 - wwz_vs_ttzzz_bVeto
   //5 - wwz_vs_ttzzz_emu
   //6 - wwz_vs_ttzzz_OffZ
   //7 - wwz_vs_zz_emu
   //8 - wwz_vs_zz_emuHighTTZBDT
   //9 - wwz_vs_zz_OffZ	
   //10 - wwz_vs_zz_OffZHighTTZBDT	

    //////get all the input variables
    int lep1_idx = lep_ZCand_idx1, lep2_idx = lep_ZCand_idx2, lep3_idx = lep_Nom_idx1, lep4_idx = lep_Nom_idx2;
    if(lep1_idx <0 || lep2_idx<0 || lep3_idx < 0 || lep4_idx < 0) return 999;
    //Z: lep1 is id>0 lepton
    if(wvz.lep_id().at(lep_ZCand_idx1) < 0)
    {
        lep1_idx = lep_ZCand_idx2;
        lep2_idx = lep_ZCand_idx1;
    }
    //W: lep3 is higher pt lepton
    if(wvz.lep_pt().at(lep_Nom_idx1) < wvz.lep_pt().at(lep_Nom_idx2))
    {
        lep3_idx = lep_Nom_idx2;
        lep4_idx = lep_Nom_idx1;
    }

    LeptonVectors vLeptons = GetLeptonVectors();
    HZZ4lEventParameters eventParameters = ConvertVectorsToAngles( vLeptons );
    float evt_MllN = this->VarMll(lep_Nom_idx1, lep_Nom_idx2);
    float evt_MllZ = this->VarMll(lep_ZCand_idx1, lep_ZCand_idx2);
    float evt_ZPt = this->VarPtll(lep_ZCand_idx1, lep_ZCand_idx2);
    float evt_met_pt = wvz.met_pt();
    float evt_nj = wvz.nj();
    float evt_nb = wvz.nb();
    float evt_lep1Pt = this->VarLepPt(lep1_idx);
    float evt_lep2Pt = this->VarLepPt(lep2_idx);
    float evt_lep3Pt = this->VarLepPt(lep3_idx);
    float evt_lep4Pt = this->VarLepPt(lep4_idx);
    float evt_lep3Id = wvz.lep_id().at(lep3_idx);
    float evt_lep4Id = wvz.lep_id().at(lep4_idx);
    float evt_lep3MT = this->VarMT(lep3_idx);
    float evt_lep4MT = this->VarMT(lep4_idx);
    float evt_lep34MT = this->VarMTll(lep3_idx, lep4_idx);
    float evt_lep1dZ = wvz.lep_dz().at(lep1_idx);
    float evt_lep2dZ = wvz.lep_dz().at(lep2_idx);
    float evt_lep3dZ = wvz.lep_dz().at(lep3_idx);
    float evt_lep4dZ = wvz.lep_dz().at(lep4_idx);
    float evt_pt_zeta = this->VarPtZeta();
    float evt_pt_zeta_vis = this->VarPtZetaVis();
    float evt_phi0 = eventParameters.Phi0;
    float evt_phi = eventParameters.Phi;
    float evt_phiH = eventParameters.PhiH;
    float evt_theta0 = eventParameters.Theta0;
    float evt_theta1 = eventParameters.Theta1;
    float evt_theta2 = eventParameters.Theta2;
    float evt_minDRJetToLep3 = this->VarMinDRJetsToLep(lep3_idx);
    float evt_minDRJetToLep4 = this->VarMinDRJetsToLep(lep4_idx);
    float evt_jet1Pt = wvz.jets_p4().size() > 0 ? wvz.jets_p4()[0].pt() : -999 ;
    float evt_jet2Pt = wvz.jets_p4().size() > 1 ? wvz.jets_p4()[1].pt() : -999 ;
    float evt_jet3Pt = wvz.jets_p4().size() > 2 ? wvz.jets_p4()[2].pt() : -999 ;
    float evt_jet4Pt = wvz.jets_p4().size() > 3 ? wvz.jets_p4()[3].pt() : -999 ;
    float evt_jet1BtagScore = wvz.jets_btag_score().size() > 0 ? wvz.jets_btag_score()[0] : -999;
    float evt_jet2BtagScore = wvz.jets_btag_score().size() > 1 ? wvz.jets_btag_score()[1] : -999;
    float evt_jet3BtagScore = wvz.jets_btag_score().size() > 2 ? wvz.jets_btag_score()[2] : -999;
    float evt_jet4BtagScore = wvz.jets_btag_score().size() > 3 ? wvz.jets_btag_score()[3] : -999;
    

    std::vector<float> test_sample_ttz_bVeto{evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore,evt_MllN,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_ZPt};
    std::vector<float> test_sample_ttz_emu{evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore,evt_MllN,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_ZPt};
    std::vector<float> test_sample_ttz_nbAll{evt_nb,evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore,evt_MllN,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_ZPt};
    std::vector<float> test_sample_ttz_OffZ{evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore,evt_MllN,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_ZPt};
    std::vector<float> test_sample_ttzzz_bVeto{evt_met_pt,evt_lep3Id,evt_lep4Id,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis,evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore};
    std::vector<float> test_sample_ttzzz_emu{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis,evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore};
    std::vector<float> test_sample_ttzzz_OffZ{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis,evt_nj,evt_minDRJetToLep3,evt_minDRJetToLep4,evt_jet1Pt,evt_jet2Pt,evt_jet3Pt,evt_jet4Pt,evt_jet1BtagScore,evt_jet2BtagScore,evt_jet3BtagScore,evt_jet4BtagScore};
    std::vector<float> test_sample_zz_emu{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis};
    std::vector<float> test_sample_zz_emuHighTTZBDT{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis};
    std::vector<float> test_sample_zz_OffZ{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis};
    std::vector<float> test_sample_zz_OffZHighTTZBDT{evt_met_pt,evt_lep3Pt,evt_lep4Pt,evt_ZPt,evt_lep3dZ,evt_lep4dZ,evt_lep3MT,evt_lep4MT,evt_lep34MT,evt_phi0,evt_theta0,evt_phi,evt_theta1,evt_theta2,evt_MllN,evt_pt_zeta,evt_pt_zeta_vis};
    
    if(idx == 0) return wwz_vs_ttz_bVeto(test_sample_ttz_bVeto, true)[0];
    if(idx == 1) return wwz_vs_ttz_emu(test_sample_ttz_emu, true)[0];
    if(idx == 2) return wwz_vs_ttz_nbAll(test_sample_ttz_nbAll, true)[0];
    if(idx == 3) return wwz_vs_ttz_OffZ(test_sample_ttz_OffZ, true)[0];
    if(idx == 4) return wwz_vs_ttzzz_bVeto(test_sample_ttzzz_bVeto, true)[0];
    if(idx == 5) return wwz_vs_ttzzz_emu(test_sample_ttzzz_emu, true)[0];
    if(idx == 6) return wwz_vs_ttzzz_OffZ(test_sample_ttzzz_OffZ, true)[0];
    if(idx == 7) return wwz_vs_zz_emu(test_sample_zz_emu, true)[0];
    if(idx == 8) return wwz_vs_zz_emuHighTTZBDT(test_sample_zz_emuHighTTZBDT, true)[0];
    if(idx == 9) return wwz_vs_zz_OffZ(test_sample_zz_OffZ, true)[0];
    if(idx == 10) return wwz_vs_zz_OffZHighTTZBDT(test_sample_zz_OffZHighTTZBDT, true)[0];

    else return 999;  

}

//______________________________________________________________________________________________
LeptonVectors Analysis::GetLeptonVectors()
{
    int lep11_idx = lep_ZCand_idx1, lep12_idx = lep_ZCand_idx2, lep21_idx = lep_Nom_idx1, lep22_idx = lep_Nom_idx2;
    //Z: lep11 is id>0 lepton
    if (wvz.lep_id().at(lep_ZCand_idx1) < 0)
    {
        lep11_idx = lep_ZCand_idx2;
        lep12_idx = lep_ZCand_idx1;
    }
    //W: lep21 id>0 lepton
    if (wvz.lep_id().at(lep_Nom_idx1) < 0)
    {
        lep21_idx = lep_Nom_idx2;
        lep22_idx = lep_Nom_idx1;
    }
    double lep11Mass = 0, lep12Mass = 0, lep21Mass = 0, lep22Mass = 0;
    if (abs(wvz.lep_id().at(lep11_idx)) == 11) lep11Mass = 0.000511;
    else if (abs(wvz.lep_id().at(lep11_idx)) == 13) lep11Mass = 0.1057;
    if (abs(wvz.lep_id().at(lep12_idx)) == 11) lep12Mass = 0.000511;
    else if (abs(wvz.lep_id().at(lep12_idx)) == 13) lep12Mass = 0.1057;
    if (abs(wvz.lep_id().at(lep21_idx)) == 11) lep21Mass = 0.000511;
    else if (abs(wvz.lep_id().at(lep21_idx)) == 13) lep21Mass = 0.1057;
    if (abs(wvz.lep_id().at(lep22_idx)) == 11) lep22Mass = 0.000511;
    else if (abs(wvz.lep_id().at(lep22_idx)) == 13) lep22Mass = 0.1057;
    LeptonVectors vLeptons;
    vLeptons.Lepton11.SetPtEtaPhiMass(this->VarLepPt(lep11_idx), this->VarLepEta(lep11_idx), this->VarLepPhi(lep11_idx), lep11Mass);
    vLeptons.Lepton12.SetPtEtaPhiMass(this->VarLepPt(lep12_idx), this->VarLepEta(lep12_idx), this->VarLepPhi(lep12_idx), lep12Mass);
    vLeptons.Lepton21.SetPtEtaPhiMass(this->VarLepPt(lep21_idx), this->VarLepEta(lep21_idx), this->VarLepPhi(lep21_idx), lep21Mass);
    vLeptons.Lepton22.SetPtEtaPhiMass(this->VarLepPt(lep22_idx), this->VarLepEta(lep22_idx), this->VarLepPhi(lep22_idx), lep22Mass);
    return vLeptons;
}
// eof
