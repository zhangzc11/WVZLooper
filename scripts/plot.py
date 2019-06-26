#!/bin/env python

###########################################
#
# Main plotting script for the WVZ analysis
#
###########################################

#____________________________________________________________________________________
def main_analysis_make_plot_userfilter():

    import plottery_wrapper as p
    import argparse
    import sys
    
    parser = argparse.ArgumentParser(description="Plotter for the WVZ analysis")
    parser.add_argument('-s' , '--sample_set_name' , dest='sample_set_name' , help='Sample set name (e.g. WVZ2016_v0.0.9 or even the combined ones like WVZ2016_v0.0.9_WVZ2017_v0.0.9_WVZ2018_v0.0.9' , required=True)
    parser.add_argument('-t' , '--tag'             , dest='tag'             , help='Tag of the looper output'                                                                                         , required=True)
    parser.add_argument('-d' , '--dirname'         , dest='dirname'         , help='plots/<sample_set_name>/<tag>/<dirname> for plot output'                                                          , required=True)
    parser.add_argument('-p' , '--filter_pattern'  , dest='filter_pattern'  , help='To filter out plot'                                                                                               , required=True)
    parser.add_argument('-u' , '--unblind'         , dest='unblind'         , help='To unblind data'     , default=False, action='store_true'                                                                        )
    parser.add_argument('-f' , '--dofakes'         , dest='dofakes'         , help='Do data-driven fakes', default=False, action='store_true'                                                                        )
    parser.add_argument('-n' , '--nbins'           , dest='nbins'           , help='# of bins'           , default=15                                                                                                )
    
    args = parser.parse_args()


    # Make plot function of the main analysis
    # The main thing is that this plotting script runs over the output files
    # that was ran with "WVZ201*_v*" ntuples
    # This means that it assumes specific histogram files to exists

    ntuple_version = args.sample_set_name
    tag = args.tag
    dirname = args.dirname
    filter_pattern = args.filter_pattern
    unblind = args.unblind

    bkgfiles = [
            "outputs/{}/{}/ttz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wz.root".format(ntuple_version, tag),
            "outputs/{}/{}/twz.root".format(ntuple_version, tag),
            "outputs/{}/{}/rare.root".format(ntuple_version, tag),
            "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag),
            "outputs/{}/{}/higgs.root".format(ntuple_version, tag),
            ]
    bkgnames = ["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"]
    sigfiles = [
            "outputs/{}/{}/wwz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wzz.root".format(ntuple_version, tag),
            #"outputs/{}/{}/www.root".format(ntuple_version, tag),
            "outputs/{}/{}/zzz.root".format(ntuple_version, tag),
            # "outputs/{}/{}/sig.root".format(ntuple_version, tag),
            ]
    sigfiles_detail = [
            "outputs/{}/{}/nonh_wwz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zh_wwz.root".format(ntuple_version, tag),
            "outputs/{}/{}/nonh_wzz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wh_wzz.root".format(ntuple_version, tag),
            #"outputs/{}/{}/www.root".format(ntuple_version, tag),
            "outputs/{}/{}/nonh_zzz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zh_zzz.root".format(ntuple_version, tag),
            # "outputs/{}/{}/sig.root".format(ntuple_version, tag),
            ]
    bkgfilesfake = [
            "outputs/{}/{}/ttz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wz.root".format(ntuple_version, tag),
            "outputs/{}/{}/twz.root".format(ntuple_version, tag),
            "outputs/{}/{}/rare.root".format(ntuple_version, tag),
            "outputs/{}/{}/higgs.root".format(ntuple_version, tag),
            "outputs/{}/{}/ttbar.root".format(ntuple_version, tag),
            ]
    bkgnamesddfake = ["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Higgs", "t#bar{t}"]


    colors = [2005, 2001, 2003, 2007, 920, 2012, 2011, 2002]

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74
    if "2016" in ntuple_version and "2017" in ntuple_version and "2018" in ntuple_version: lumi = 137

    p.dump_plot(fnames=bkgfilesfake if args.dofakes else bkgfiles,
            sig_fnames=sigfiles,
            data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag) if unblind else None,
            usercolors=colors,
            legend_labels=bkgnamesddfake if args.dofakes else bkgnames,
            signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
            dirname="plots/{}/{}/{}".format(ntuple_version, tag, dirname),
            filter_pattern=filter_pattern,
            dogrep=True,
            extraoptions={
                "print_yield":True,
                "nbins":int(args.nbins),
                "signal_scale": 1,
                "legend_scalex":1.8,
                "legend_scaley":1.1,
                "legend_ncolumns": 3,
                "ymax_scale": 1.2,
                "lumi_value":lumi,
                "xaxis_ndivisions":505,
                },
            # _plotter=p.plot_cut_scan,
            )

if __name__ == "__main__":

    # Run different make plot script based on what dataset the looper ran on
    main_analysis_make_plot_userfilter()

