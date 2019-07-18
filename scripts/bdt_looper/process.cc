#include "bdttree.h"
#include "rooutil.h"
#include "cxxopts.h"

class AnalysisConfig {

public:

    // TString that holds the input file list (comma separated)
    TString input_file_list_tstring;

    // TString that holds the name of the TTree to open for each input files
    TString input_tree_name;

    // Output TFile
    TFile* output_tfile;

    // Number of events to loop over
    int n_events;

    // Jobs to split (if this number is positive, then we will skip certain number of events)
    // If there are N events, and was asked to split 2 ways, then depending on job_index, it will run over first half or latter half
    int nsplit_jobs;

    // Job index (assuming nsplit_jobs is set, the job_index determine where to loop over)
    int job_index;

    // Debug boolean
    bool debug;

    // TChain that holds the input TTree's
    TChain* events_tchain;

    // Custom Looper object to facilitate looping over many files
    RooUtil::Looper<bdttree> looper;

    // Custom Cutflow framework
    RooUtil::Cutflow cutflow;

    // Custom Histograms object compatible with RooUtil::Cutflow framework
    RooUtil::Histograms histograms;

};

AnalysisConfig ana;

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

//********************************************************************************
//
// 1. Parsing options
//
//********************************************************************************

    // cxxopts is just a tool to parse argc, and argv easily

    // Grand option setting
    cxxopts::Options options("\n  $ doAnalysis",  "\n         **********************\n         *                    *\n         *       Looper       *\n         *                    *\n         **********************\n");

    // Read the options
    options.add_options()
        ("i,input"       , "Comma separated input file list OR if just a directory is provided it will glob all in the directory BUT must end with '/' for the path", cxxopts::value<std::string>())
        ("t,tree"        , "Name of the tree in the root file to open and loop over"                                             , cxxopts::value<std::string>())
        ("o,output"      , "Output file name"                                                                                    , cxxopts::value<std::string>())
        ("n,nevents"     , "N events to loop over"                                                                               , cxxopts::value<int>()->default_value("-1"))
        ("j,nsplit_jobs" , "Enable splitting jobs by N blocks (--job_index must be set)"                                         , cxxopts::value<int>())
        ("I,job_index"   , "job_index of split jobs (--nsplit_jobs must be set. index starts from 0. i.e. 0, 1, 2, 3, etc...)"   , cxxopts::value<int>())
        ("d,debug"       , "Run debug job. i.e. overrides output option to 'debug.root' and 'recreate's the file.")
        ("h,help"        , "Print help")
        ;

    auto result = options.parse(argc, argv);

    // NOTE: When an option was provided (e.g. -i or --input), then the result.count("<option name>") is more than 0
    // Therefore, the option can be parsed easily by asking the condition if (result.count("<option name>");
    // That's how the several options are parsed below

    //_______________________________________________________________________________
    // --help
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --input
    if (result.count("input"))
    {
        ana.input_file_list_tstring = result["input"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input list is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --tree
    if (result.count("tree"))
    {
        ana.input_tree_name = result["tree"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input tree name is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --debug
    if (result.count("debug"))
    {
        ana.output_tfile = new TFile("debug.root", "recreate");
    }
    else
    {
        //_______________________________________________________________________________
        // --output
        if (result.count("output"))
        {
            ana.output_tfile = new TFile(result["output"].as<std::string>().c_str(), "create");
            if (not ana.output_tfile->IsOpen())
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: output already exists! provide new output name or delete old file. OUTPUTFILE=" << result["output"].as<std::string>() << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: Output file name is not provided! Check your arguments" << std::endl;
            exit(1);
        }
    }

    //_______________________________________________________________________________
    // --nevents
    ana.n_events = result["nevents"].as<int>();

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("nsplit_jobs"))
    {
        ana.nsplit_jobs = result["nsplit_jobs"].as<int>();
        if (ana.nsplit_jobs <= 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --nsplit_jobs" << ana.nsplit_jobs << " has zero or negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.nsplit_jobs = -1;
    }

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("job_index"))
    {
        ana.job_index = result["job_index"].as<int>();
        if (ana.job_index < 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index" << ana.job_index << " has negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.job_index = -1;
    }


    // Sanity check for split jobs (if one is set the other must be set too)
    if (result.count("job_index") or result.count("nsplit_jobs"))
    {
        // If one is not provided then throw error
        if ( not (result.count("job_index") and result.count("nsplit_jobs")))
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index and --nsplit_jobs must be set at the same time!" << std::endl;
            exit(1);
        }
        // If it is set then check for sanity
        else
        {
            if (ana.job_index >= ana.nsplit_jobs)
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: --job_index >= --nsplit_jobs ! This does not make sense..." << std::endl;
                exit(1);
            }
        }
    }

    //
    // Printing out the option settings overview
    //
    std::cout <<  "=========================================================" << std::endl;
    std::cout <<  " Setting of the analysis job based on provided arguments " << std::endl;
    std::cout <<  "---------------------------------------------------------" << std::endl;
    std::cout <<  " ana.input_file_list_tstring: " << ana.input_file_list_tstring <<  std::endl;
    std::cout <<  " ana.output_tfile: " << ana.output_tfile->GetName() <<  std::endl;
    std::cout <<  " ana.n_events: " << ana.n_events <<  std::endl;
    std::cout <<  " ana.nsplit_jobs: " << ana.nsplit_jobs <<  std::endl;
    std::cout <<  " ana.job_index: " << ana.job_index <<  std::endl;
    std::cout <<  "=========================================================" << std::endl;

//********************************************************************************
//
// 2. Opening input baby files
//
//********************************************************************************

    // Create the TChain that holds the TTree's of the baby ntuples
    ana.events_tchain = RooUtil::FileUtil::createTChain(ana.input_tree_name, ana.input_file_list_tstring);

    // Create a Looper object to loop over input files
    // the "www" object is defined in the wwwtree.h/cc
    // This is an instance which helps read variables in the WWW baby TTree
    // It is a giant wrapper that facilitates reading TBranch values.
    // e.g. if there is a TBranch named "lep_pt" which is a std::vector<float> then, one can access the branch via
    //
    //    std::vector<float> lep_pt = www.lep_pt();
    //
    // and no need for "SetBranchAddress" and declaring variable shenanigans necessary
    // This is a standard thing SNT does pretty much every looper we use
    ana.looper.init(ana.events_tchain, &bdt, ana.n_events);

//********************************************************************************
//
// Interlude... notes on RooUtil framework
//
//********************************************************************************

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Cutflow object cut tree formation
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Cutflow object facilitates creating a tree structure of cuts
    //
    // To add cuts to each node of the tree with cuts defined, use "addCut" or "addCutToLastActiveCut"
    // The "addCut" or "addCutToLastActiveCut" accepts three argument, <name>, and two lambda's that define the cut selection, and the weight to apply to that cut stage
    //
    // e.g. To create following cut-tree structure one does
    //
    //             (Root) <--- Always exists as soon as RooUtil::Cutflow object is created. But this is basically hidden underneath and users do not have to care
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    //
    //   code:
    //
    //      // Create the object (Root node is created as soon as the instance is created)
    //      RooUtil::Cutflow cutflow;
    //
    //      cutflow.addCut("CutWeight"                 , <lambda> , <lambda>); // CutWeight is added below "Root"-node
    //      cutflow.addCutToLastActiveCut("CutPresel1" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I just added that. So "CutPresel1" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel1"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel1" since I just added that. So "CutSel1" is added below "CutPresel1"
    //
    //      cutflow.getCut("CutWeight"); // By "getting" the cut object, this makes the "CutWeight" the last "active" cut.
    //      cutflow.addCutToLastActiveCut("CutPresel2" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I "getCut" on it. So "CutPresel2" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel2"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel2" since I just added that. So "CutSel2" is added below "CutPresel1"
    //
    // (Side note: "UNITY" lambda is defined in the framework to just return 1. This so that use don't have to type [&]() {return 1;} so many times.)
    //
    // Once the cutflow is formed, create cutflow histograms can be created by calling RooUtil::Cutflow::bookCutflows())
    // This function looks through the terminating nodes of the tree structured cut tree. and creates a histogram that will fill the yields
    // For the example above, there are two terminationg nodes, "CutSel1", and "CutSel2"
    // So in this case Root::Cutflow::bookCutflows() will create two histograms. (Actually four histograms.)
    //
    //  - TH1F* type object :  CutSel1_cutflow (4 bins, with first bin labeled "Root", second bin labeled "CutWeight", third bin labeled "CutPreSel1", fourth bin labeled "CutSel1")
    //  - TH1F* type object :  CutSel2_cutflow (...)
    //  - TH1F* type object :  CutSel1_rawcutflow (...)
    //  - TH1F* type object :  CutSel2_rawcutflow (...)
    //                                ^
    //                                |
    // NOTE: There is only one underscore "_" between <CutName>_cutflow or <CutName>_rawcutflow
    //
    // And later in the loop when RooUtil::Cutflow::fill() function is called, the tree structure will be traversed through and the appropriate yields will be filled into the histograms
    //
    // After running the loop check for the histograms in the output root file

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Histograms object
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Histograms object facilitates book keeping histogram definitions
    // And in conjunction with RooUtil::Cutflow object, one can book same histograms across different cut stages easily without copy pasting codes many times by hand.
    //
    // The histogram addition happens in two steps.
    // 1. Defining histograms
    // 2. Booking histograms to cuts
    //
    // Histograms are defined via following functions
    //
    //      RooUtil::Histograms::addHistogram       : Typical 1D histogram (TH1F*) "Fill()" called once per event
    //      RooUtil::Histograms::addVecHistogram    : Typical 1D histogram (TH1F*) "Fill()" called multiple times per event
    //      RooUtil::Histograms::add2DHistogram     : Typical 2D histogram (TH2F*) "Fill()" called once per event
    //      RooUtil::Histograms::add2DVecHistogram  : Typical 2D histogram (TH2F*) "Fill()" called multiple times per event
    // e.g.
    //
    //    RooUtil::Histograms histograms;
    //    histograms.addHistogram   ("MllSS"    , 180 , 0. , 300. , [&]() { return www.MllSS()  ; }); // The lambda returns float
    //    histograms.addVecHistogram("AllLepPt" , 180 , 0. , 300. , [&]() { return www.lep_pt() ; }); // The lambda returns vector<float>
    //
    // The addVecHistogram will have lambda to return vector<float> and it will loop over the values and call TH1F::Fill() for each item
    //
    // To book histograms to cuts one uses
    //
    //      RooUtil::Cutflow::bookHistogramsForCut()
    //      RooUtil::Cutflow::bookHistogramsForCutAndBelow()
    //      RooUtil::Cutflow::bookHistogramsForCutAndAbove()
    //      RooUtil::Cutflow::bookHistogramsForEndCuts()
    //
    // e.g. Given a tree like the following, we can book histograms to various cuts as we want
    //
    //              Root
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    // For example,
    //
    //    1. book a set of histograms to one cut:
    //
    //       cutflow.bookHistogramsForCut(histograms, "CutPreSel2")
    //
    //    2. book a set of histograms to a cut and below
    //
    //       cutflow.bookHistogramsForCutAndBelow(histograms, "CutWeight") // will book a set of histograms to CutWeight, CutPreSel1, CutPreSel2, CutSel1, and CutSel2
    //
    //    3. book a set of histograms to a cut and above (... useless...?)
    //
    //       cutflow.bookHistogramsForCutAndAbove(histograms, "CutPreSel2") // will book a set of histograms to CutPreSel2, CutWeight (nothing happens to Root node)
    //
    //    4. book a set of histograms to a terminating nodes
    //
    //       cutflow.bookHistogramsForEndCuts(histograms) // will book a set of histograms to CutSel1 and CutSel2
    //
    // The naming convention of the booked histograms are as follows
    //
    //   cutflow.bookHistogramsForCut(histograms, "CutSel1");
    //
    //  - TH1F* type object : CutSel1__MllSS;
    //  - TH1F* type object : CutSel1__AllLepPt;
    //                               ^^
    //                               ||
    // NOTE: There are two underscores "__" between <CutName>__<HistogramName>
    //
    // And later in the loop when RooUtil::CutName::fill() function is called, the tree structure will be traversed through and the appropriate histograms will be filled with appropriate variables
    // After running the loop check for the histograms in the output root file

    // TMVA
    RooUtil::TMVAUtil::ReaderX readerx_ZZ("BDT_ZZ", "tmvabdt/dataset/weights/TMVA_BDT_ZZ.weights.xml");
    RooUtil::TTreeX tx_ZZ("BDTInput", "Temp tree existing only in run time to read BDT inputs");
    tx_ZZ.createBranch<float>("phi0");
    tx_ZZ.createBranch<float>("phi");
    tx_ZZ.createBranch<float>("theta0");
    tx_ZZ.createBranch<float>("theta1");
    tx_ZZ.createBranch<float>("theta2");
    tx_ZZ.createBranch<float>("MllN");
    tx_ZZ.createBranch<float>("lep3MT");
    tx_ZZ.createBranch<float>("lep4MT");
    tx_ZZ.createBranch<float>("lep34MT");
    tx_ZZ.createBranch<float>("pt_zeta_vis");
    tx_ZZ.createBranch<float>("pt_zeta");
    tx_ZZ.createBranch<float>("ZPt");
    float BDT_score_ZZ;

    RooUtil::TMVAUtil::ReaderX readerx_TTZ("BDT_TTZ", "tmvabdt/dataset/weights/TMVA_BDT_TTZ.weights.xml");
    RooUtil::TTreeX tx_TTZ("BDTInput", "Temp tree existing only in run time to read BDT inputs");
    tx_TTZ.createBranch<float>("MllN");
    tx_TTZ.createBranch<float>("lep3MT");
    tx_TTZ.createBranch<float>("lep4MT");
    tx_TTZ.createBranch<float>("lep34MT");
    tx_TTZ.createBranch<float>("ZPt");
    tx_TTZ.createBranch<float>("nj");
    tx_TTZ.createBranch<float>("ht");
    tx_TTZ.createBranch<float>("minDRJetToLep3");
    tx_TTZ.createBranch<float>("minDRJetToLep4");
    float BDT_score_TTZ;

    RooUtil::TMVAUtil::ReaderX readerx_All("BDT_All", "tmvabdt/dataset/weights/TMVA_BDT_All.weights.xml");
    RooUtil::TTreeX tx_All("BDTInput", "Temp tree existing only in run time to read BDT inputs");
    tx_All.createBranch<float>("phi0");
    tx_All.createBranch<float>("phi");
    tx_All.createBranch<float>("theta0");
    tx_All.createBranch<float>("theta1");
    tx_All.createBranch<float>("theta2");
    tx_All.createBranch<float>("MllN");
    tx_All.createBranch<float>("lep3MT");
    tx_All.createBranch<float>("lep4MT");
    tx_All.createBranch<float>("lep34MT");
    tx_All.createBranch<float>("pt_zeta_vis");
    tx_All.createBranch<float>("pt_zeta");
    tx_All.createBranch<float>("ZPt");
    tx_All.createBranch<float>("nj");
    tx_All.createBranch<float>("ht");
    tx_All.createBranch<float>("minDRJetToLep3");
    tx_All.createBranch<float>("minDRJetToLep4");
    float BDT_score_All;

    RooUtil::TMVAUtil::ReaderX readerx_Combine("BDT_Combine", "tmvabdt/dataset/weights/TMVA_BDT_Combine.weights.xml");
    RooUtil::TTreeX tx_Combine("BDTInput", "Temp tree existing only in run time to read BDT inputs");
    tx_Combine.createBranch<float>("bdt_zz");
    tx_Combine.createBranch<float>("bdt_ttz");
    float BDT_score_Combine;

    // Set the cutflow object output file
    ana.cutflow.setTFile(ana.output_tfile);

    ana.cutflow.addCut("Weight", [&]() { return 1; }, [&]() { return bdt.eventweight() * bdt.lepsf() * bdt.weight_btagsf(); } );
    ana.cutflow.addCutToLastActiveCut("HighMT", [&]() { return bdt.lep3MT() > 40. and bdt.lep4MT() > 20.; }, UNITY);
    ana.cutflow.getCut("Weight");
    ana.cutflow.addCutToLastActiveCut("HighBDTTTZ", [&]() { return BDT_score_TTZ > -0.1; }, UNITY);
    ana.cutflow.getCut("Weight");
    ana.cutflow.addCutToLastActiveCut("HighBDTZZ", [&]() { return BDT_score_ZZ > 0.0; }, UNITY);
    ana.cutflow.getCut("Weight");
    ana.cutflow.addCutToLastActiveCut("HighBDTAll", [&]() { return BDT_score_All > 0.15; }, UNITY);

    // Print cut structure
    ana.cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
    ana.histograms.addHistogram("MllNom", 180, 0, 200, [&]() { return bdt.MllN(); } );
    ana.histograms.addHistogram("lep3MT", 180, 0, 200, [&]() { return bdt.lep3MT(); } );
    ana.histograms.addHistogram("lep4MT", 180, 0, 200, [&]() { return bdt.lep4MT(); } );
    ana.histograms.addHistogram("nj", 6, 0, 6, [&]() { return bdt.nj(); } );
    ana.histograms.addHistogram("ht", 180, 0, 500, [&]() { return bdt.ht(); } );
    ana.histograms.addHistogram("ZPt", 180, 0, 200, [&]() { return bdt.ZPt(); } );
    ana.histograms.addHistogram("BDTZZ", 180, -0.35, 0.6, [&]() { return BDT_score_ZZ; } );
    ana.histograms.addHistogram("BDTTTZ", 180, -0.35, 0.6, [&]() { return BDT_score_TTZ; } );
    ana.histograms.addHistogram("BDTAll", 180, -0.35, 0.6, [&]() { return BDT_score_All; } );
    ana.histograms.addHistogram("BDTCombine", 180, -0.35, 0.6, [&]() { return BDT_score_Combine; } );
    ana.histograms.addHistogram("lepNsumip3d", 180, 0., 0.02, [&]() { return fabs(bdt.lep_ip3d()[bdt.lep_N_idx0()]) + fabs(bdt.lep_ip3d()[bdt.lep_N_idx1()]); } );
    ana.histograms.addHistogram("lepNsumdxy", 180, 0., 0.02, [&]() { return fabs(bdt.lep_dxy()[bdt.lep_N_idx0()]) + fabs(bdt.lep_dxy()[bdt.lep_N_idx1()]); } );
    ana.histograms.addHistogram("lepNsumdz", 180, 0., 0.02, [&]() { return fabs(bdt.lep_dz()[bdt.lep_N_idx0()]) + fabs(bdt.lep_dz()[bdt.lep_N_idx1()]); } );

    // 2d histograms
    // ana.histograms.add2DHistogram("pt_zeta_vis", 50 , -200, 550 , "pt_zeta_sum", 50, -200, 550, [&](){ return bdt.pt_zeta_vis(); }, [&](){ return bdt.pt_zeta(); });
    // ana.histograms.add2DHistogram("lep3ip3d" , 50 , 0 , 0.02 , "lep4ip3d" , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_ip3d()[bdt.lep_N_idx0()]); } , [&](){ return fabs(bdt.lep_ip3d()[bdt.lep_N_idx1()]); });
    // ana.histograms.add2DHistogram("lep3dxy"  , 50 , 0 , 0.02 , "lep4dxy"  , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_dxy()[bdt.lep_N_idx0()]); }  , [&](){ return fabs(bdt.lep_dxy()[bdt.lep_N_idx1()]); });
    // ana.histograms.add2DHistogram("lep3dz"   , 50 , 0 , 0.02 , "lep4dz"   , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_dz()[bdt.lep_N_idx0()]); }   , [&](){ return fabs(bdt.lep_dz()[bdt.lep_N_idx1()]); });
    // ana.histograms.add2DHistogram("lep1ip3d" , 50 , 0 , 0.02 , "lep2ip3d" , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_ip3d()[bdt.lep_Z_idx0()]); } , [&](){ return fabs(bdt.lep_ip3d()[bdt.lep_Z_idx1()]); });
    // ana.histograms.add2DHistogram("lep1dxy"  , 50 , 0 , 0.02 , "lep2dxy"  , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_dxy()[bdt.lep_Z_idx0()]); }  , [&](){ return fabs(bdt.lep_dxy()[bdt.lep_Z_idx1()]); });
    // ana.histograms.add2DHistogram("lep1dz"   , 50 , 0 , 0.02 , "lep2dz"   , 50 , 0 , 0.02 , [&](){ return fabs(bdt.lep_dz()[bdt.lep_Z_idx0()]); }   , [&](){ return fabs(bdt.lep_dz()[bdt.lep_Z_idx1()]); });

    // Book cutflows
    ana.cutflow.bookCutflows();

    // Book Histograms
    ana.cutflow.bookHistograms(ana.histograms); // if just want to book everywhere

    // Skim file
    bool doSkim = false;
    if (not ana.input_file_list_tstring.Contains("_skim"))
        doSkim = true;
    TString skimfile_name = ana.input_file_list_tstring;
    skimfile_name.ReplaceAll(".root", "_skim.root");
    if (doSkim)
        ana.looper.setSkim(skimfile_name);
    RooUtil::TTreeX* tx_skim;

    // Looping input file
    while (ana.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        if (ana.looper.isNewFileInChain())
        {
            if (doSkim)
            {
                tx_skim = new RooUtil::TTreeX(ana.looper.getSkimTree());
                tx_skim->createBranch<float>("bdt_zz");
                tx_skim->createBranch<float>("bdt_ttz");
            }
        }

        tx_ZZ.setBranch<float>("phi0", bdt.phi0());
        tx_ZZ.setBranch<float>("phi", bdt.phi());
        tx_ZZ.setBranch<float>("theta0", bdt.theta0());
        tx_ZZ.setBranch<float>("theta1", bdt.theta1());
        tx_ZZ.setBranch<float>("theta2", bdt.theta2());
        tx_ZZ.setBranch<float>("MllN", bdt.MllN());
        tx_ZZ.setBranch<float>("lep3MT", bdt.lep3MT());
        tx_ZZ.setBranch<float>("lep4MT", bdt.lep4MT());
        tx_ZZ.setBranch<float>("lep34MT", bdt.lep34MT());
        tx_ZZ.setBranch<float>("pt_zeta_vis", bdt.pt_zeta_vis());
        tx_ZZ.setBranch<float>("pt_zeta", bdt.pt_zeta());
        tx_ZZ.setBranch<float>("ZPt", bdt.ZPt());
        BDT_score_ZZ = readerx_ZZ.eval(tx_ZZ);

        tx_TTZ.setBranch<float>("MllN", bdt.MllN());
        tx_TTZ.setBranch<float>("lep3MT", bdt.lep3MT());
        tx_TTZ.setBranch<float>("lep4MT", bdt.lep4MT());
        tx_TTZ.setBranch<float>("lep34MT", bdt.lep34MT());
        tx_TTZ.setBranch<float>("ZPt", bdt.ZPt());
        tx_TTZ.setBranch<float>("nj", bdt.nj());
        tx_TTZ.setBranch<float>("ht", bdt.ht());
        tx_TTZ.setBranch<float>("minDRJetToLep3", bdt.minDRJetToLep3());
        tx_TTZ.setBranch<float>("minDRJetToLep4", bdt.minDRJetToLep4());
        BDT_score_TTZ = readerx_TTZ.eval(tx_TTZ);

        tx_All.setBranch<float>("phi0", bdt.phi0());
        tx_All.setBranch<float>("phi", bdt.phi());
        tx_All.setBranch<float>("theta0", bdt.theta0());
        tx_All.setBranch<float>("theta1", bdt.theta1());
        tx_All.setBranch<float>("theta2", bdt.theta2());
        tx_All.setBranch<float>("MllN", bdt.MllN());
        tx_All.setBranch<float>("lep3MT", bdt.lep3MT());
        tx_All.setBranch<float>("lep4MT", bdt.lep4MT());
        tx_All.setBranch<float>("lep34MT", bdt.lep34MT());
        tx_All.setBranch<float>("pt_zeta_vis", bdt.pt_zeta_vis());
        tx_All.setBranch<float>("pt_zeta", bdt.pt_zeta());
        tx_All.setBranch<float>("ZPt", bdt.ZPt());
        tx_All.setBranch<float>("nj", bdt.nj());
        tx_All.setBranch<float>("ht", bdt.ht());
        tx_All.setBranch<float>("minDRJetToLep3", bdt.minDRJetToLep3());
        tx_All.setBranch<float>("minDRJetToLep4", bdt.minDRJetToLep4());
        BDT_score_All = readerx_All.eval(tx_All);

        tx_Combine.setBranch<float>("bdt_zz", bdt.bdt_zz());
        tx_Combine.setBranch<float>("bdt_ttz", bdt.bdt_ttz());
        BDT_score_Combine = readerx_Combine.eval(tx_Combine);

        if (doSkim)
        {
            tx_skim->setBranch<float>("bdt_zz", BDT_score_ZZ);
            tx_skim->setBranch<float>("bdt_ttz", BDT_score_TTZ);
            ana.looper.fillSkim();
        }

        //Do what you need to do in for each event here
        //To save use the following function
        ana.cutflow.fill();
    }

    // Writing output file
    ana.cutflow.saveOutput();

    if (doSkim)
        ana.looper.saveSkim();

    // The below can be sometimes crucial
    delete ana.output_tfile;
}
