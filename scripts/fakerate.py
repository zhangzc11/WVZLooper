#!/bin/env python

###########################################
#
# Main plotting script for the WVZ analysis
#
###########################################

from plottery import plottery as ply
import plottery_wrapper as p
import ROOT as r
import glob
import sys
import read_table as rt
from errors import E
from array import array

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
def main_fake_rate_measurement(prefix, output_name, etaregion=""):

    # Parse the input arguments
    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74

    basedir = "plots/{}/{}/fake/".format(ntuple_version, tag)

    # Denominator : fake from data (i.e. data - prompt)
    yields_ddfake = rt.read_table(basedir + prefix + "Prompt__lepFakeCand2PtVarBin"+etaregion+".txt")
    yields_ddfake["ddfake"] = []
    for datacount, bkgcount in zip(yields_ddfake["data"], yields_ddfake["Total"]):
        yields_ddfake["ddfake"].append(datacount - bkgcount)
    # print yields_ddfake["ddfake"]

    # Numerator : fake from data (i.e. data - prompt)
    yields_ddfake_tight = rt.read_table(basedir + prefix + "TightPrompt__lepFakeCand2PtVarBin"+etaregion+".txt")
    yields_ddfake_tight["ddfake"] = []
    for datacount, bkgcount in zip(yields_ddfake_tight["data"], yields_ddfake_tight["Total"]):
        yields_ddfake_tight["ddfake"].append(datacount - bkgcount)
    # print yields_ddfake_tight["ddfake"]

    fr_data = []
    for den, num in zip(yields_ddfake["ddfake"], yields_ddfake_tight["ddfake"]):
        if den.val != 0:
            fr = num / den
            fr_data.append(fr)
        else:
            fr_data.append(E(0, 0))
    fr_data.pop(0) # first one is underflow bin
    fr_data.pop(0) # second one is underflow bin
    fr_data.pop(-1) # last one is overflow bin
    print fr_data

    # Denominator: Fake directly from ttbar MC
    yields_ttbar = rt.read_table(basedir + prefix + "Fake__lepFakeCand2PtVarBin"+etaregion+".txt")
    # print yields_ttbar["ttbar6"]

    # Numerator: fake from data (i.e. data - prompt)
    yields_ttbar_tight = rt.read_table(basedir + prefix + "TightFake__lepFakeCand2PtVarBin"+etaregion+".txt")
    # print yields_ttbar_tight["ttbar6"]

    fr_mc = []
    for den, num in zip(yields_ttbar["ttbar6"], yields_ttbar_tight["ttbar6"]):
        if den.val != 0:
            fr = num / den
            fr_mc.append(fr)
        else:
            fr_mc.append(E(0, 0))
    print fr_mc
    fr_mc.pop(0) # first one is underflow bin
    fr_mc.pop(0) # second one is underflow bin
    fr_mc.pop(-1) # last one is overflow bin

    # bin boundaries
    # bounds = [0., 10., 15., 20., 30., 150.]
    bounds = [0., 10., 20., 70.]

    h_fr_data = r.TH1F("FR","",len(bounds)-1,array('d',bounds))
    h_fr_mc = r.TH1F("FR","",len(bounds)-1,array('d',bounds))

    for idx, fr in enumerate(fr_data):
        h_fr_data.SetBinContent(idx+2, fr.val)
        h_fr_data.SetBinError(idx+2, fr.err)

    for idx, fr in enumerate(fr_mc):
        h_fr_mc.SetBinContent(idx+2, fr.val)
        h_fr_mc.SetBinError(idx+2, fr.err)

    # Options
    alloptions= {
               "ratio_range":[0.0,2.0],
               "nbins": 180,
               "autobin": False,
               "legend_scalex": 0.8,
               "legend_scaley": 0.8,
               "output_name": basedir + "/"+output_name+".pdf",
               "bkg_sort_method": "unsorted",
               "no_ratio": False,
               "print_yield": True,
               "yield_prec": 3,
               "draw_points": True,
               "hist_line_none": True,
               "show_bkg_errors": True,
               "lumi_value" : lumi,
               "yaxis_range": [0., 1],
               }

    p.plot_hist(
           sigs = [],
           bgs = [h_fr_mc.Clone()],
           data = h_fr_data.Clone(),
           syst = None,
           colors=[2001],
           legend_labels=["MC t#bar{t}"],
           options=alloptions)

    return h_fr_mc.Clone(), h_fr_data.Clone()

#____________________________________________________________________________________
def plot_cen_fwd_in_one_canvas(fr_mc_fwd, fr_data_fwd, fr_mc_cen, fr_data_cen, output_name):

    # Parse the input arguments
    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74

    basedir = "plots/{}/{}/fake/".format(ntuple_version, tag)

    # Options
    alloptions= {
               "ratio_range":[0.0,2.0],
               "nbins": 180,
               "autobin": False,
               "legend_scalex": 1.5,
               "legend_scaley": 0.8,
               "output_name": basedir + "/"+output_name+".pdf",
               "bkg_sort_method": "unsorted",
               "no_ratio": False,
               "print_yield": True,
               "yield_prec": 3,
               "draw_points": True,
               "hist_line_none": True,
               "show_bkg_errors": True,
               "lumi_value" : lumi,
               "yaxis_range": [0., 1.5] if "_mu" in output_name else [0., 1.0],
               "legend_datalabel": "Data fake-rate (|#eta|#geq1.6)"
               }

    p.plot_hist(
           sigs = [fr_mc_cen.Clone("MC t#bar{t} fake-rate (|#eta|<1.6)"), fr_data_cen.Clone("Data fake-rate (|#eta|<1.6)")],
           bgs = [fr_mc_fwd.Clone()],
           data = fr_data_fwd.Clone(),
           syst = None,
           colors=[2001],
           legend_labels=["MC t#bar{t} fake-rate (|#eta|#geq1.6)"],
           options=alloptions)


#____________________________________________________________________________________
def main():

    # Run different make plot script based on what dataset the looper ran on
    main_fake_rate_measurement("EMuPlusXFakeMu", "fake_rate_mu")
    main_fake_rate_measurement("EMuPlusXFakeEl", "fake_rate_el")
    mu_fr_mc_fwd, mu_fr_data_fwd = main_fake_rate_measurement("EMuPlusXFakeMu", "fake_rate_mu_fwd", "Fwd")
    mu_fr_mc_cen, mu_fr_data_cen = main_fake_rate_measurement("EMuPlusXFakeMu", "fake_rate_mu_cen", "Cen")
    plot_cen_fwd_in_one_canvas(mu_fr_mc_fwd, mu_fr_data_fwd, mu_fr_mc_cen, mu_fr_data_cen, "fake_rate_mu_both")
    el_fr_mc_fwd, el_fr_data_fwd = main_fake_rate_measurement("EMuPlusXFakeEl", "fake_rate_el_fwd", "Fwd")
    el_fr_mc_cen, el_fr_data_cen = main_fake_rate_measurement("EMuPlusXFakeEl", "fake_rate_el_cen", "Cen")
    plot_cen_fwd_in_one_canvas(el_fr_mc_fwd, el_fr_data_fwd, el_fr_mc_cen, el_fr_data_cen, "fake_rate_el_both")


if __name__ == "__main__":

    # Parse the input arguments
    try:
        ntuple_version = sys.argv[1]
        tag = sys.argv[2]
    except:
        usage()


    main()
