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
    // Output file
    TFile* outputFile = TFile::Open("BDT.root", "RECREATE");

    // Initialize TMVA
    TMVA::Tools::Instance();
    TMVA::Factory *factory = new TMVA::Factory("TMVA", outputFile, "V:DrawProgressBar=True:Transformations=I;P;G:AnalysisType=Classification");

    // sample parent directory
    TString version = "v0.1.12.7";
    TString dirpath = "/nfs-7/userdata/phchang/babies/BDT_" + version;

    // Get signal sample TChain
    TString SigSampleGlobber = "";
    SigSampleGlobber += dirpath + "/wwz_20172018.root";
    TChain* SigChain = RooUtil::FileUtil::createTChain("t", SigSampleGlobber);

    // Get background sample TChain
    vector<TString> BkgSampleGlobber;

    BkgSampleGlobber.push_back(dirpath + "/zz.root");
    BkgSampleGlobber.push_back(dirpath + "/ttz.root");
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

    TCut cut = "1";
    dataloader->PrepareTrainingAndTestTree(cut, cut, "SplitMode=random:!V");

    // TString option = "!H:V:DoBoostMonitor:CreateMVAPdfs:NTrees=100:BoostType=Grad:Shrinkage=0.1:MinNodeSize=15.%:MaxDepth=3";
    TString option = "!H:V:DoBoostMonitor:CreateMVAPdfs";
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT", option);
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    return 0;
}
