#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TChain.h"
#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/DataLoader.h"
#include "TMVA/TMVAGui.h"

#include "rooutil.cc"

using namespace std;

int main(int argc, char** argv)
{

    enum BkgToTarget { kAll, kZZ, kTTZ, kCombine };

    BkgToTarget targettedBkg;

    if (argc >= 2)
    {
        if (atoi(argv[1]) == 0)
            targettedBkg = kZZ;
        else if (atoi(argv[1]) == 1)
            targettedBkg = kTTZ;
        else if (atoi(argv[1]) == 2)
            targettedBkg = kCombine;
    }
    else
    {
        targettedBkg = kAll;
    }

    TString targettedBkgName;
    if (targettedBkg == kZZ)
    {
        targettedBkgName = "ZZ";
    }
    else if (targettedBkg == kTTZ)
    {
        targettedBkgName = "TTZ";
    }
    else if (targettedBkg == kAll)
    {
        targettedBkgName = "All";
    }
    else if (targettedBkg == kCombine)
    {
        targettedBkgName = "Combine";
    }

    std::cout <<  " targettedBkgName: " << targettedBkgName <<  std::endl;

    // Output file
    TFile* outputFile = TFile::Open(TString::Format("BDT_%s.root", targettedBkgName.Data()), "RECREATE");

    // Initialize TMVA
    TMVA::Tools::Instance();
    TMVA::Factory *factory = new TMVA::Factory("TMVA", outputFile, "V:DrawProgressBar=True:Transformations=I;P;G:AnalysisType=Classification");

    // sample parent directory
    TString version = "v0.1.12.7";
    TString dirpath = "/nfs-7/userdata/phchang/babies/BDT_" + version;

    // Get signal sample TChain
    TString SigSampleGlobber = "";
    if (targettedBkg == kCombine)
    {
        SigSampleGlobber += dirpath + "/wwz_20172018_skim.root";
    }
    else
    {
        SigSampleGlobber += dirpath + "/wwz_20172018.root";
    }
    TChain* SigChain = RooUtil::FileUtil::createTChain("t", SigSampleGlobber);

    // Get background sample TChain
    vector<TString> BkgSampleGlobber;

    if (targettedBkg == kZZ)
    {
        BkgSampleGlobber.push_back(dirpath + "/zz.root");
    }
    else if (targettedBkg == kTTZ)
    {
        BkgSampleGlobber.push_back(dirpath + "/ttz.root");
    }
    else if (targettedBkg == kAll)
    {
        BkgSampleGlobber.push_back(dirpath + "/zz.root");
        BkgSampleGlobber.push_back(dirpath + "/ttz.root");
    }
    else if (targettedBkg == kCombine)
    {
        BkgSampleGlobber.push_back(dirpath + "/zz_skim.root");
        BkgSampleGlobber.push_back(dirpath + "/ttz_skim.root");
    }
    // BkgSampleGlobber.push_back(dirpath + "/twz.root");
    // BkgSampleGlobber.push_back(dirpath + "/wz.root");
    // BkgSampleGlobber.push_back(dirpath + "/other.root");
    TChain* BkgChain = RooUtil::FileUtil::createTChain("t", RooUtil::StringUtil::join(BkgSampleGlobber));

    // Data Loader
    TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset");
    dataloader->AddSignalTree(SigChain, 1.0);
    dataloader->AddBackgroundTree(BkgChain, 1.0);
    dataloader->SetSignalWeightExpression("eventweight*lepsf*weight_btagsf");
    dataloader->SetBackgroundWeightExpression("eventweight*lepsf*weight_btagsf");

    // Add Variables
    if (targettedBkg == kZZ)
    {
        dataloader->AddVariable("phi0");
        dataloader->AddVariable("phi");
        dataloader->AddVariable("theta0");
        dataloader->AddVariable("theta1");
        dataloader->AddVariable("theta2");
        dataloader->AddVariable("MllN");
        dataloader->AddVariable("lep3MT");
        dataloader->AddVariable("lep4MT");
        dataloader->AddVariable("lep34MT");
        dataloader->AddVariable("pt_zeta_vis");
        dataloader->AddVariable("pt_zeta");
        dataloader->AddVariable("ZPt");
    }
    else if (targettedBkg == kTTZ)
    {
        dataloader->AddVariable("MllN");
        dataloader->AddVariable("lep3MT");
        dataloader->AddVariable("lep4MT");
        dataloader->AddVariable("lep34MT");
        dataloader->AddVariable("ZPt");
        dataloader->AddVariable("nj");
        dataloader->AddVariable("ht");
        dataloader->AddVariable("minDRJetToLep3");
        dataloader->AddVariable("minDRJetToLep4");
    }
    else if (targettedBkg == kAll)
    {
        dataloader->AddVariable("phi0");
        dataloader->AddVariable("phi");
        dataloader->AddVariable("theta0");
        dataloader->AddVariable("theta1");
        dataloader->AddVariable("theta2");
        dataloader->AddVariable("MllN");
        dataloader->AddVariable("lep3MT");
        dataloader->AddVariable("lep4MT");
        dataloader->AddVariable("lep34MT");
        dataloader->AddVariable("pt_zeta_vis");
        dataloader->AddVariable("pt_zeta");
        dataloader->AddVariable("ZPt");
        dataloader->AddVariable("nj");
        dataloader->AddVariable("ht");
        dataloader->AddVariable("minDRJetToLep3");
        dataloader->AddVariable("minDRJetToLep4");
    }
    else if (targettedBkg == kCombine)
    {
        dataloader->AddVariable("bdt_zz");
        dataloader->AddVariable("bdt_ttz");
    }

    TCut cut = "1";
    dataloader->PrepareTrainingAndTestTree(cut, cut, "SplitMode=random:!V");

    TString option;
    if (targettedBkg == kZZ)
        option = "!H:V:DoBoostMonitor:CreateMVAPdfs:NTrees=400:MaxDepth=3";
    else if (targettedBkg == kTTZ)
        option = "!H:V:DoBoostMonitor:CreateMVAPdfs:NTrees=200:MaxDepth=3";
    else if (targettedBkg == kAll)
        option = "!H:V:DoBoostMonitor:CreateMVAPdfs:NTrees=400:MaxDepth=3:IgnoreNegWeightsInTraining";
    else if (targettedBkg == kCombine)
        option = "!H:V:DoBoostMonitor:CreateMVAPdfs:NTrees=200:MaxDepth=2:IgnoreNegWeightsInTraining";
    factory->BookMethod(dataloader, TMVA::Types::kBDT, TString::Format("BDT_%s", targettedBkgName.Data()), option);
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    return 0;
}
