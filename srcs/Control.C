#include "Analysis.h"
#include "makeHists.h"
#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char** argv)
{
    //usage output
    if (argc != 3)
    {
        std::cout << "usage : " << argv[0] << " ntuple_file_name type_of_samples  " << std::endl;
        return 0;
    }

    //
    // read in filelist
    // filelist will contain a list of file names to run over without .root at the end
    //
    // e.g.  If we have,
    //
    // /nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/
    //      dy_m1050_madgraph_1.root
    //      dy_m50_madgraph_1.root
    //      ggh_hzz4l_powheg_1.root
    //      sts_4f_leptonic_madgraph_1.root
    //      ttbar_amcatnlo_1.root
    //      ...
    //
    // The input file list should have
    //
    // $ cat  configs/Filelist_bkg.list
    // dy_m1050_madgraph_1
    // dy_m50_madgraph_1
    // ggh_hzz4l_powheg_1
    // sts_4f_leptonic_madgraph_1
    // ttbar_amcatnlo_1
    //
    // N.B. without .root
    //

    ifstream infile;
    infile.open(argv[1], ios::in);

    int count = 0; // To keep track of how many input files were ran over

    cout << "*************************" << endl;
    cout << "*** Analysis Begin ****" << endl;
    cout << "*************************" << endl;
    cout << endl;

    TString InputRoot;
    while (infile >> InputRoot)
    {

        // Form full path to the input root file
        TString RootName = InputRoot;
        TString RootAdd = "/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/" + (TString)InputRoot + ".root";

        // Increase # of input files ran over
        count++;

        // Create the analysis instance
        Analysis Run(argv[1], argv[2], InputRoot);

        cout << " Initial Begin: " << endl;

        // Set the input root file and the "index" of the input root file
        Run.Initial(RootAdd, count);

        cout << " Loop Begin:" << endl;

        //
        // Loop!
        //
        Run.Loop(argv[2]);

        // Done
        Run.End(count);

        Run.Output();

        Run.Finish(count);
    }

    cout << endl;
    cout << "**********************" << endl;
    cout << "**** Analysis End ****" << endl;
    cout << "**********************" << endl;


    return 1;
}
