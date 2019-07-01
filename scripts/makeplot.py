#!/bin/env python

###########################################
#
# Main plotting script for the WVZ analysis
#
###########################################

import plottery_wrapper as p
import ROOT as r
import glob
import sys

#____________________________________________________________________________________
# Help function of this plotter script
def usage():
    print "Usage:"
    print ""
    print "   $ {} ntuple_version tag".format(sys.argv[0])
    print ""
    print ""
    sys.exit(-1)

#____________________________________________________________________________________
def dilep_analysis_make_plot():

    # Make plot function of the dilepton region
    # This is going to run over looper output from "Dilep201*_v*" ntuple sets
    # The "Dilep201*_v*" ntuple sets have different MC and data list
    # As of May 19, 2019 it had only DY and dimuon data events

    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()

    bkgfiles = [
            "outputs/{}/{}/top.root".format(ntuple_version, tag),
            "outputs/{}/{}/dy.root".format(ntuple_version, tag),
            "outputs/{}/{}/wj.root".format(ntuple_version, tag),
            "outputs/{}/{}/ww.root".format(ntuple_version, tag),
            ]

    sigfiles = [
            ]

    sigfiles_detail = [
            ]

    data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag)

    colors = [2005, 2001, 2003, 2007, 920, 2012, 2011]

    p.dump_plot(fnames=bkgfiles,
            sig_fnames=sigfiles,
            data_fname=data_fname,
            usercolors=colors,
            legend_labels=["t#bar{t}", "DY", "W", "WW"],
            signal_labels=[],
            dirname="plots/{}/{}/lin".format(ntuple_version, tag),
            filter_pattern="__",
            dogrep=True,
            extraoptions={
                "print_yield":True,
                "nbins":180,
                "signal_scale": 1,
                "legend_scalex":1.8,
                "legend_scaley":1.1,
                "legend_ncolumns": 3,
                "ymax_scale": 1.2,
                "lumi_value":59.74,
                "remove_underflow": True,
                }
            )

    p.dump_plot(fnames=bkgfiles,
            sig_fnames=sigfiles,
            data_fname=data_fname,
            usercolors=colors,
            legend_labels=["t#bar{t}", "DY", "W", "WW"],
            signal_labels=[],
            dirname="plots/{}/{}/log".format(ntuple_version, tag),
            filter_pattern="__",
            dogrep=True,
            extraoptions={
                "print_yield":True,
                "nbins":180,
                "signal_scale": 1,
                "legend_scalex":1.8,
                "legend_scaley":1.1,
                "legend_ncolumns": 3,
                "ymax_scale": 1.2,
                "lumi_value":59.74,
                "yaxis_log":True,
                "legend_smart":False,
                "remove_underflow": True,
                }
            )

#____________________________________________________________________________________
def main_analysis_make_plot_userfilter():

    # Make plot function of the main analysis
    # The main thing is that this plotting script runs over the output files
    # that was ran with "WVZ201*_v*" ntuples
    # This means that it assumes specific histogram files to exists

    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
        dirname = sys.argv[3]
        filterpattern = sys.argv[4]
    except:
        usage()

    bkgfiles = [
            "outputs/{}/{}/ttz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wz.root".format(ntuple_version, tag),
            "outputs/{}/{}/twz.root".format(ntuple_version, tag),
            "outputs/{}/{}/rare.root".format(ntuple_version, tag),
            "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag),
            "outputs/{}/{}/higgs.root".format(ntuple_version, tag),
            ]
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
            "outputs/{}/{}/dy.root".format(ntuple_version, tag),
            "outputs/{}/{}/ttbar.root".format(ntuple_version, tag),
            ]


    colors = [2005, 2001, 2003, 2007, 920, 2012, 2011]

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74

    p.dump_plot(fnames=bkgfiles,
            sig_fnames=sigfiles,
            data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
            usercolors=colors,
            legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
            signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
            dirname="plots/{}/{}/{}".format(ntuple_version, tag, dirname),
            filter_pattern=filter_pattern,
            dogrep=True,
            extraoptions={
                "print_yield":True,
                "nbins":15,
                "signal_scale": 1,
                "legend_scalex":1.8,
                "legend_scaley":1.1,
                "legend_ncolumns": 3,
                "ymax_scale": 1.2,
                "lumi_value":lumi,
                }
            )

#____________________________________________________________________________________
def main_analysis_make_plot():

    # Make plot function of the main analysis
    # The main thing is that this plotting script runs over the output files
    # that was ran with "WVZ201*_v*" ntuples
    # This means that it assumes specific histogram files to exists

    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()

    bkgfiles = [
            "outputs/{}/{}/ttz.root".format(ntuple_version, tag),
            "outputs/{}/{}/zz.root".format(ntuple_version, tag),
            "outputs/{}/{}/wz.root".format(ntuple_version, tag),
            "outputs/{}/{}/twz.root".format(ntuple_version, tag),
            "outputs/{}/{}/rare.root".format(ntuple_version, tag),
            "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag),
            "outputs/{}/{}/higgs.root".format(ntuple_version, tag),
            ]
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
            "outputs/{}/{}/dy.root".format(ntuple_version, tag),
            "outputs/{}/{}/ttbar.root".format(ntuple_version, tag),
            ]


    colors = [2005, 2001, 2003, 2007, 920, 2012, 2011]

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cr".format(ntuple_version, tag),
    #         filter_pattern="ARChannelEMu__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cr".format(ntuple_version, tag),
    #         filter_pattern="ARChannelOffZHighMET__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cr".format(ntuple_version, tag),
    #         filter_pattern="ARChannelOnZ__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cr".format(ntuple_version, tag),
    #         filter_pattern="OnZ__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cr_log".format(ntuple_version, tag),
    #         filter_pattern="OnZ__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             "yaxis_log":True,
    #             "legend_smart":False,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/lin".format(ntuple_version, tag),
    #         filter_pattern="VetoButNotNomid",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfilesfake,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma", "t#bar{t}"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/fake".format(ntuple_version, tag),
    #         filter_pattern="EMuPlusXFakeMu*__*FakeCand2PtVarBin",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfilesfake,
    #         sig_fnames=sigfiles,
    #         data_fname="outputs/{}/{}/data.root".format(ntuple_version, tag),
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/fake".format(ntuple_version, tag),
    #         filter_pattern="EMuPlusXFakeEl*__*FakeCand2PtVarBin",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/lin5x".format(ntuple_version, tag),
    #         filter_pattern="__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 5,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles,
    #         usercolors=colors,
    #         legend_labels=["ttz", "zz", "wz", "twz", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
    #         dirname="plots/{}/{}/cutflow".format(ntuple_version, tag),
    #         filter_pattern="_cutflow",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles_detail,
    #         usercolors=colors,
    #         legend_labels=["ttz", "zz", "wz", "twz", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "ZH#rightarrowWWZ", "WZZ", "WH#rightarrowZZ", "ZZZ", "ZH#rightarrowZZ", "VVV"],
    #         dirname="plots/{}/{}/cutflow_detail".format(ntuple_version, tag),
    #         filter_pattern="_cutflow",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )

    # p.dump_plot(fnames=bkgfiles,
    #         sig_fnames=sigfiles_detail,
    #         usercolors=colors,
    #         legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
    #         signal_labels=["WWZ", "ZH#rightarrowWWZ", "WZZ", "WH#rightarrowZZ", "ZZZ", "ZH#rightarrowZZ", "VVV"],
    #         dirname="plots/{}/{}/lin_detail".format(ntuple_version, tag),
    #         filter_pattern="__",
    #         dogrep=True,
    #         extraoptions={
    #             "print_yield":True,
    #             "nbins":15,
    #             "signal_scale": 1,
    #             "legend_scalex":1.8,
    #             "legend_scaley":1.1,
    #             "legend_ncolumns": 3,
    #             "ymax_scale": 1.2,
    #             "lumi_value":lumi,
    #             }
    #         )


if __name__ == "__main__":


    # Parse the input arguments
    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()

    # Run different make plot script based on what dataset the looper ran on
    if "WVZ" in ntuple_version:
        main_analysis_make_plot()
    elif "Trilep" in ntuple_version:
        main_analysis_make_plot()
    elif "Dilep" in ntuple_version:
        dilep_analysis_make_plot()

