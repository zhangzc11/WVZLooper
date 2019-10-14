#!/bin/env python

import plottery_wrapper as p
import sys
import pyrootutil as pr

def compare_fake(lepflav, variable):

    tag = "test"

    sample_dir = "outputs/Trilep2016_v0.1.11_Trilep2017_v0.1.11_Trilep2018_v0.1.11/y2016_{}_y2017_{}_y2018_{}/".format(tag, tag, tag)

    all_process_fnames = [ sample_dir + "ttbar.root", sample_dir + "dy.root" ]
    histogram_name = "ThreeLeptonFakeValidationEvents{}__{}".format(lepflav, variable)
    hists_threelep = pr.get_histograms(all_process_fnames, histogram_name)

    histogram_name = "FourLeptonFakeValidationEvents{}__{}".format(lepflav, variable)
    hists_fourlep = pr.get_histograms([sample_dir + "wz.root"] , histogram_name)

    for hist in hists_threelep:
        if hist.Integral() != 0:
            hist.Scale(1./hist.Integral())

    for hist in hists_fourlep:
        if hist.Integral() != 0:
            hist.Scale(1./hist.Integral())

    xaxis_label = ""
    if "MotherID" in variable and lepflav == "El":
        xaxis_label = "Flavor of misid'd electron"

    if "MotherID" in variable and lepflav == "Mu":
        xaxis_label = "Flavor of misid'd muon"

    if "relIso03EA" in variable:
        xaxis_label = "I_{rel,0.3,EA,e}"

    if "relIso04DB" in variable:
        xaxis_label = "I_{rel,0.4,DB,#mu}"

    extraoptions={
        "print_yield":True,
        "nbins":45,
        "signal_scale": 1,
        "legend_scalex":1.8,
        "legend_scaley":0.5,
        "legend_ncolumns": 3,
        "legend_smart":False,
        "yaxis_log":True,
        "ymax_scale": 1.2,
        "lumi_value":-1,
        "remove_underflow": True,
        "xaxis_ndivisions":505,
        "xaxis_label":xaxis_label,
        "ratio_xaxis_title":xaxis_label,
        "ratio_range":[0.,4.],
        "output_name":"plots_fakes/wz_v_dy_{}.pdf".format(histogram_name),
        "legend_datalabel":"WZ",
        }

    p.plot_hist(data=hists_fourlep[0], bgs=[hists_threelep[1]], options=extraoptions, colors=[2012], sig_labels=[], legend_labels=["DY"])

    extraoptions={
        "print_yield":True,
        "nbins":45,
        "signal_scale": 1,
        "legend_scalex":1.8,
        "legend_scaley":0.5,
        "legend_ncolumns": 3,
        "legend_smart":False,
        "yaxis_log":True,
        "ymax_scale": 1.2,
        "lumi_value":-1,
        "remove_underflow": True,
        "xaxis_ndivisions":505,
        "xaxis_label":xaxis_label,
        "ratio_xaxis_title":xaxis_label,
        "ratio_range":[0.,4.],
        "output_name":"plots_fakes/wz_v_ttbar_{}.pdf".format(histogram_name),
        "legend_datalabel":"WZ",
        }

    p.plot_hist(data=hists_fourlep[0], bgs=[hists_threelep[0]], options=extraoptions, colors=[2011], sig_labels=[], legend_labels=["t#bar{t}"])

if __name__ == "__main__":

    # compare_fake("El", "lepFakeMCmatchedMotherID")
    # compare_fake("El", "lepFakeMCmatchedMCID")
    # compare_fake("El", "lepFakeMCmatchedPt")
    # compare_fake("El", "lepFakeMCmatcheddz")
    # compare_fake("El", "lepFakeMCmatcheddxy")
    # compare_fake("El", "lepFakeMCmatchedsip3d")
    # compare_fake("El", "lepFakeMCmatchedip3d")
    # compare_fake("El", "lepFakeMCmatchedrelIso03EA")
    # compare_fake("El", "lepFakeMCmatchedrelIso04DB")
    # compare_fake("El", "lepFakeMCmatchedPtVarBin")
    # compare_fake("El", "lepFakeMCmatchedPtVarBinFwd")
    # compare_fake("El", "lepFakeMCmatchedPtVarBinCen")
    # compare_fake("El", "lepFakeMCmatchedPtFineVarBin")
    # compare_fake("El", "lepFakeMCmatchedPtFineVarBinFwd")
    # compare_fake("El", "lepFakeMCmatchedPtFineVarBinCen")
    # compare_fake("Mu", "lepFakeMCmatchedMotherID")
    # compare_fake("Mu", "lepFakeMCmatchedMCID")
    # compare_fake("Mu", "lepFakeMCmatchedPt")
    # compare_fake("Mu", "lepFakeMCmatcheddz")
    # compare_fake("Mu", "lepFakeMCmatcheddxy")
    # compare_fake("Mu", "lepFakeMCmatchedsip3d")
    # compare_fake("Mu", "lepFakeMCmatchedip3d")
    # compare_fake("Mu", "lepFakeMCmatchedrelIso03EA")
    # compare_fake("Mu", "lepFakeMCmatchedrelIso04DB")
    # compare_fake("Mu", "lepFakeMCmatchedPtVarBin")
    # compare_fake("Mu", "lepFakeMCmatchedPtVarBinFwd")
    # compare_fake("Mu", "lepFakeMCmatchedPtVarBinCen")
    # compare_fake("Mu", "lepFakeMCmatchedPtFineVarBin")
    # compare_fake("Mu", "lepFakeMCmatchedPtFineVarBinFwd")
    # compare_fake("Mu", "lepFakeMCmatchedPtFineVarBinCen")

    compare_fake("El", "lepFakeMCmatchedMotherID")
    compare_fake("El", "lepFakeMCmatchedPt")
    compare_fake("El", "lepFakeMCmatchedrelIso03EA")
    compare_fake("Mu", "lepFakeMCmatchedMotherID")
    compare_fake("Mu", "lepFakeMCmatchedPt")
    compare_fake("Mu", "lepFakeMCmatchedrelIso04DB")
