#include "wvztree.h"
#include "rooutil.h"
#include "cxxopts.h"

struct less_than_key
{
    inline bool operator() (const LorentzVector& struct1, const LorentzVector& struct2)
    {
        return (struct1.pt() > struct2.pt());
    }
};

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
    RooUtil::Looper<wvztree> looper;

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
    ana.looper.init(ana.events_tchain, &wvz, ana.n_events);

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

    // Set the cutflow object output file
    ana.cutflow.setTFile(ana.output_tfile);

    // Objects to compute
    std::vector<LV> w_p4;
    std::vector<LV> z_p4;
    std::vector<LV> wp_child_p4;
    std::vector<LV> wm_child_p4;
    std::vector<LV> z_child_p4;
    std::vector<int> wp_child_idx;
    std::vector<int> wm_child_idx;
    std::vector<int> z_child_idx;
    std::vector<LV> light_lepton_p4;

    // 5.972e-04
    // 0.0006024
    ana.cutflow.addCut("Weight", [&]() { return 1/*set your cut here*/; }, [&]() { return wvz.evt_scale1fb() * 137. * (TString(ana.output_tfile->GetName()).Contains("incl") ? 1. : 1./1.0087073007); } );
    ana.cutflow.addCutToLastActiveCut("Zmass", [&]() { return fabs(z_p4[0].mass() - 91.1876) < 10.; }, UNITY );

    // Print cut structure
    ana.cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
    ana.histograms.addHistogram("W1_pt", 180, 0, 550, [&]() { return w_p4[0].pt(); } );
    ana.histograms.addHistogram("W1_eta", 180, -5, 5, [&]() { return w_p4[0].eta(); } );
    ana.histograms.addHistogram("W1_mass", 180, 50, 150, [&]() { return w_p4[0].mass(); } );
    ana.histograms.addHistogram("W2_pt", 180, 0, 550, [&]() { return w_p4[1].pt(); } );
    ana.histograms.addHistogram("W2_eta", 180, -5, 5, [&]() { return w_p4[1].eta(); } );
    ana.histograms.addHistogram("W2_mass", 180, 50, 150, [&]() { return w_p4[1].mass(); } );
    ana.histograms.addHistogram("DiW_pt", 180, 0, 550, [&]() { return (w_p4[0]+w_p4[1]).pt(); } );
    ana.histograms.addHistogram("DiW_eta", 180, -5, 5, [&]() { return (w_p4[0]+w_p4[1]).eta(); } );
    ana.histograms.addHistogram("DiW_mass", 180, 100, 550, [&]() { return (w_p4[0]+w_p4[1]).mass(); } );
    ana.histograms.addHistogram("Z_pt", 180, 0, 250, [&]() { return z_p4[0].pt(); } );
    ana.histograms.addHistogram("Z_eta", 180, -5, 5, [&]() { return z_p4[0].eta(); } );
    ana.histograms.addHistogram("Z_mass", 180, 0, 150, [&]() { return z_p4[0].mass(); } );

    ana.histograms.addHistogram("L1_pt", 180, 0, 400, [&]() { return light_lepton_p4[0].pt(); } );
    ana.histograms.addHistogram("L2_pt", 180, 0, 300, [&]() { return light_lepton_p4[1].pt(); } );
    ana.histograms.addHistogram("L3_pt", 180, 0, 200, [&]() { return light_lepton_p4[2].pt(); } );
    ana.histograms.addHistogram("L4_pt", 180, 0, 100, [&]() { return light_lepton_p4[3].pt(); } );

    ana.histograms.addHistogram("L1_zoom_pt", 180, 0, 100, [&]() { return light_lepton_p4[0].pt(); } );
    ana.histograms.addHistogram("L2_zoom_pt", 180, 0, 100, [&]() { return light_lepton_p4[1].pt(); } );
    ana.histograms.addHistogram("L3_zoom_pt", 180, 0, 100, [&]() { return light_lepton_p4[2].pt(); } );
    ana.histograms.addHistogram("L4_zoom_pt", 180, 0, 100, [&]() { return light_lepton_p4[3].pt(); } );

    // Book cutflows
    ana.cutflow.bookCutflows();

    // Book Histograms
    ana.cutflow.bookHistograms(ana.histograms); // if just want to book everywhere

    // Looping input file
    while (ana.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        w_p4.clear();
        z_p4.clear();
        wp_child_p4.clear();
        wm_child_p4.clear();
        z_child_p4.clear();
        wp_child_idx.clear();
        wm_child_idx.clear();
        z_child_idx.clear();
        if ((TString(ana.output_tfile->GetName()).Contains("incl")))
        {
            w_p4.push_back(wvz.gen_part_p4()[0].pt() > wvz.gen_part_p4()[1].pt() ? wvz.gen_part_p4()[0] : wvz.gen_part_p4()[1]);
            w_p4.push_back(wvz.gen_part_p4()[0].pt() > wvz.gen_part_p4()[1].pt() ? wvz.gen_part_p4()[1] : wvz.gen_part_p4()[0]);
            z_p4.push_back(wvz.gen_part_p4()[2]);
            for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
            {
                if (wvz.gen_part_mother_id()[ip] == 23)
                {
                    z_child_p4.push_back(wvz.gen_part_p4()[ip]);
                    z_child_idx.push_back(ip);
                }
                if (wvz.gen_part_mother_id()[ip] == 24)
                {
                    if (wvz.gen_part_status()[ip] == 23)
                    {
                        wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                        wp_child_idx.push_back(ip);
                    }
                }
                if (wvz.gen_part_mother_id()[ip] == -24)
                {
                    if (wvz.gen_part_status()[ip] == 23)
                    {
                        wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                        wm_child_idx.push_back(ip);
                    }
                }
            }
            if (wp_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                {
                    if (wvz.gen_part_mother_id()[ip] == 24)
                    {
                        if (wvz.gen_part_status()[ip] == 1 or (wvz.gen_part_status()[ip] == 2 and abs(wvz.gen_part_id()[ip]) == 15))
                        {
                            wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wp_child_idx.push_back(ip);
                        }
                    }
                }
            }
            if (wm_child_p4.size() != 2)
            {
                for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                {
                    if (wvz.gen_part_mother_id()[ip] == -24)
                    {
                        if (wvz.gen_part_status()[ip] == 1 or (wvz.gen_part_status()[ip] == 2 and abs(wvz.gen_part_id()[ip]) == 15))
                        {
                            wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wm_child_idx.push_back(ip);
                        }
                    }
                }
            }

            if (wm_child_p4.size() > 2)
            {
                // Pruning gamma originated leptons
                std::vector<LV> tmp;
                std::vector<int> tmpidx;
                int ii = 0;
                int jj = 0;
                for (auto& ip4 : wm_child_p4)
                {
                    bool found = false;
                    for (auto& jp4 : wm_child_p4)
                    {
                        if (ip4 == jp4)
                            continue;
                        if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                        {
                            tmp.push_back(ip4);
                            tmp.push_back(jp4);
                            found = true;
                            tmpidx.push_back(ii);
                            tmpidx.push_back(jj);
                            break;
                        }
                        else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                        {
                            tmp.push_back(ip4);
                            tmp.push_back(jp4);
                            found = true;
                            tmpidx.push_back(ii);
                            tmpidx.push_back(jj);
                            break;
                        }
                        jj++;
                    }
                    ii++;
                    if (found)
                        break;
                }
                wm_child_p4 = tmp;
                wm_child_idx = tmpidx;
            }

            if (wp_child_p4.size() > 2)
            {
                // Pruning gamma originated leptons
                std::vector<LV> tmp;
                std::vector<int> tmpidx;
                int ii = 0;
                int jj = 0;
                for (auto& ip4 : wp_child_p4)
                {
                    bool found = false;
                    for (auto& jp4 : wp_child_p4)
                    {
                        if (ip4 == jp4)
                            continue;
                        if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                        {
                            tmp.push_back(ip4);
                            tmp.push_back(jp4);
                            found = true;
                            tmpidx.push_back(ii);
                            tmpidx.push_back(jj);
                            break;
                        }
                        else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                        {
                            tmp.push_back(ip4);
                            tmp.push_back(jp4);
                            found = true;
                            tmpidx.push_back(ii);
                            tmpidx.push_back(jj);
                            break;
                        }
                        jj++;
                    }
                    ii++;
                    if (found)
                        break;
                }
                wp_child_p4 = tmp;
                wp_child_idx = tmpidx;
            }

            if (wp_child_p4.size() != 2)
            {
                std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (wm_child_p4.size() != 2)
            {
                std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
            if (z_child_p4.size() != 2)
            {
                std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                std::cout <<  " z_child_p4.size(): " << z_child_p4.size() <<  std::endl;
                RooUtil::error("different than I expected");
            }
        }
        else
        {
            if (abs(wvz.gen_part_id()[0]) == 23)
            {
                if (abs(wvz.gen_part_id()[1]) != 24)
                {
                    std::cout <<  " abs(wvz.gen_part_id()[1]): " << abs(wvz.gen_part_id()[1]) <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                if (abs(wvz.gen_part_id()[2]) != 24)
                {
                    std::cout <<  " abs(wvz.gen_part_id()[2]): " << abs(wvz.gen_part_id()[2]) <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                w_p4.push_back(wvz.gen_part_p4()[1].pt() > wvz.gen_part_p4()[2].pt() ? wvz.gen_part_p4()[1] : wvz.gen_part_p4()[2]);
                w_p4.push_back(wvz.gen_part_p4()[1].pt() > wvz.gen_part_p4()[2].pt() ? wvz.gen_part_p4()[2] : wvz.gen_part_p4()[1]);
                z_p4.push_back(wvz.gen_part_p4()[0]);
                for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                {
                    if (wvz.gen_part_mother_id()[ip] == 23)
                    {
                        z_child_p4.push_back(wvz.gen_part_p4()[ip]);
                        z_child_idx.push_back(ip);
                    }
                    if (wvz.gen_part_mother_id()[ip] == 24)
                    {
                        if (wvz.gen_part_status()[ip] == 23)
                        {
                            wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wp_child_idx.push_back(ip);
                        }
                    }
                    if (wvz.gen_part_mother_id()[ip] == -24)
                    {
                        if (wvz.gen_part_status()[ip] == 23)
                        {
                            wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wm_child_idx.push_back(ip);
                        }
                    }
                }
                if (wp_child_p4.size() != 2)
                {
                    for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                    {
                        if (wvz.gen_part_mother_id()[ip] == 24)
                        {
                            if (wvz.gen_part_status()[ip] == 1)
                            {
                                wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                                wp_child_idx.push_back(ip);
                            }
                        }
                    }
                }
                if (wm_child_p4.size() != 2)
                {
                    for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                    {
                        if (wvz.gen_part_mother_id()[ip] == -24)
                        {
                            if (wvz.gen_part_status()[ip] == 1)
                            {
                                wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                                wm_child_idx.push_back(ip);
                            }
                        }
                    }
                }

                if (wm_child_p4.size() > 2)
                {
                    // Pruning gamma originated leptons
                    std::vector<LV> tmp;
                    std::vector<int> tmpidx;
                    int ii = 0;
                    int jj = 0;
                    for (auto& ip4 : wm_child_p4)
                    {
                        bool found = false;
                        for (auto& jp4 : wm_child_p4)
                        {
                            if (ip4 == jp4)
                                continue;
                            if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            jj++;
                        }
                        ii++;
                        if (found)
                            break;
                    }
                    wm_child_p4 = tmp;
                    wm_child_idx = tmpidx;
                }

                if (wp_child_p4.size() > 2)
                {
                    // Pruning gamma originated leptons
                    std::vector<LV> tmp;
                    std::vector<int> tmpidx;
                    int ii = 0;
                    int jj = 0;
                    for (auto& ip4 : wp_child_p4)
                    {
                        bool found = false;
                        for (auto& jp4 : wp_child_p4)
                        {
                            if (ip4 == jp4)
                                continue;
                            if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            jj++;
                        }
                        ii++;
                        if (found)
                            break;
                    }
                    wp_child_p4 = tmp;
                    wp_child_idx = tmpidx;
                }

                if (wp_child_p4.size() != 2)
                {
                    std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                    std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                if (wm_child_p4.size() != 2)
                {
                    std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                    std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                if (z_child_p4.size() != 2)
                {
                    std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                    std::cout <<  " z_child_p4.size(): " << z_child_p4.size() <<  std::endl;
                    RooUtil::error("different than I expected");
                }
            }
            else if(abs(wvz.gen_part_id()[0]) == 24)
            {
                if (abs(wvz.gen_part_id()[1]) != 24)
                {
                    std::cout <<  " abs(wvz.gen_part_id()[1]): " << abs(wvz.gen_part_id()[1]) <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                if (wvz.gen_part_id()[2] != -wvz.gen_part_id()[3])
                {
                    std::cout <<  " wvz.gen_part_id()[2]: " << wvz.gen_part_id()[2] <<  std::endl;
                    std::cout <<  " wvz.gen_part_id()[3]: " << wvz.gen_part_id()[3] <<  std::endl;
                    RooUtil::error("different that I expected");
                }
                w_p4.push_back(wvz.gen_part_p4()[0].pt() > wvz.gen_part_p4()[1].pt() ? wvz.gen_part_p4()[0] : wvz.gen_part_p4()[1]);
                w_p4.push_back(wvz.gen_part_p4()[0].pt() > wvz.gen_part_p4()[1].pt() ? wvz.gen_part_p4()[1] : wvz.gen_part_p4()[0]);
                z_p4.push_back((wvz.gen_part_p4()[2] + wvz.gen_part_p4()[3]));
                z_child_p4.push_back(wvz.gen_part_p4()[2]);
                z_child_p4.push_back(wvz.gen_part_p4()[3]);
                for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                {
                    if (wvz.gen_part_mother_id()[ip] == 24)
                    {
                        if (wvz.gen_part_status()[ip] == 23)
                        {
                            wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wp_child_idx.push_back(ip);
                        }
                    }
                    if (wvz.gen_part_mother_id()[ip] == -24)
                    {
                        if (wvz.gen_part_status()[ip] == 23)
                        {
                            wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                            wm_child_idx.push_back(ip);
                        }
                    }
                }
                if (wp_child_p4.size() != 2)
                {
                    for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                    {
                        if (wvz.gen_part_mother_id()[ip] == 24)
                        {
                            if (wvz.gen_part_status()[ip] == 1)
                            {
                                wp_child_p4.push_back(wvz.gen_part_p4()[ip]);
                                wp_child_idx.push_back(ip);
                            }
                        }
                    }
                }
                if (wm_child_p4.size() != 2)
                {
                    for (unsigned int ip = 0; ip < wvz.gen_part_p4().size(); ++ip)
                    {
                        if (wvz.gen_part_mother_id()[ip] == -24)
                        {
                            if (wvz.gen_part_status()[ip] == 1)
                            {
                                wm_child_p4.push_back(wvz.gen_part_p4()[ip]);
                                wm_child_idx.push_back(ip);
                            }
                        }
                    }
                }

                if (wm_child_p4.size() > 2)
                {
                    // Pruning gamma originated leptons
                    std::vector<LV> tmp;
                    std::vector<int> tmpidx;
                    int ii = 0;
                    int jj = 0;
                    for (auto& ip4 : wm_child_p4)
                    {
                        bool found = false;
                        for (auto& jp4 : wm_child_p4)
                        {
                            if (ip4 == jp4)
                                continue;
                            if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            jj++;
                        }
                        ii++;
                        if (found)
                            break;
                    }
                    wm_child_p4 = tmp;
                    wm_child_idx = tmpidx;
                }
                if (wp_child_p4.size() > 2)
                {
                    // Pruning gamma originated leptons
                    std::vector<LV> tmp;
                    std::vector<int> tmpidx;
                    int ii = 0;
                    int jj = 0;
                    for (auto& ip4 : wp_child_p4)
                    {
                        bool found = false;
                        for (auto& jp4 : wp_child_p4)
                        {
                            if (ip4 == jp4)
                                continue;
                            if (fabs((ip4 + jp4).mass() - w_p4[0].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            else if (fabs((ip4 + jp4).mass() - w_p4[1].mass()) < 0.5)
                            {
                                tmp.push_back(ip4);
                                tmp.push_back(jp4);
                                found = true;
                                tmpidx.push_back(ii);
                                tmpidx.push_back(jj);
                                break;
                            }
                            jj++;
                        }
                        ii++;
                        if (found)
                            break;
                    }
                    wp_child_p4 = tmp;
                    wp_child_idx = tmpidx;
                }

                if (wp_child_p4.size() != 2)
                {
                    std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                    std::cout <<  " wp_child_p4.size(): " << wp_child_p4.size() <<  std::endl;
                    RooUtil::error("different than I expected");
                }
                if (wm_child_p4.size() != 2)
                {
                    std::cout <<  " wvz.evt(): " << wvz.evt() <<  std::endl;
                    std::cout <<  " wm_child_p4.size(): " << wm_child_p4.size() <<  std::endl;
                    RooUtil::error("different than I expected");
                }
            }
            else
            {
                std::cout <<  " abs(wvz.gen_part_id()[0]): " << abs(wvz.gen_part_id()[0]) <<  std::endl;
                RooUtil::error("different that I expected");
            }
        }

        if (TString(ana.output_tfile->GetName()).Contains("incl"))
        {
            int nlightlep = 0;
            for (auto& idx : wp_child_idx)
            {
                if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                    nlightlep++;
            }
            for (auto& idx : wm_child_idx)
            {
                if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                    nlightlep++;
            }
            for (auto& idx : z_child_idx)
            {
                if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                    nlightlep++;
            }
            if (nlightlep != 4)
                continue;
        }

        light_lepton_p4.clear();
        for (auto& idx : wp_child_idx)
        {
            if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                light_lepton_p4.push_back(wvz.gen_part_p4()[idx]);
        }
        for (auto& idx : wm_child_idx)
        {
            if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                light_lepton_p4.push_back(wvz.gen_part_p4()[idx]);
        }
        for (auto& idx : z_child_idx)
        {
            if (abs(wvz.gen_part_id()[idx]) == 11 or abs(wvz.gen_part_id()[idx]) == 13)
                light_lepton_p4.push_back(wvz.gen_part_p4()[idx]);
        }

        std::sort(light_lepton_p4.begin(), light_lepton_p4.end(), less_than_key());

        //Do what you need to do in for each event here
        //To save use the following function
        ana.cutflow.fill();
    }

    // Writing output file
    ana.cutflow.saveOutput();

    // The below can be sometimes crucial
    delete ana.output_tfile;
}
