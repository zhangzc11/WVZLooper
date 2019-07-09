#!/bin/env python

from plottery import plottery as ply
import plottery_wrapper as p
import ROOT as r
import glob
import sys
import read_table as rt
from errors import E
from array import array
import pyrootutil as pr

def get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, num_proc):

    if "2016" in ntuple_version: lumi = 35.9
    if "2017" in ntuple_version: lumi = 41.3
    if "2018" in ntuple_version: lumi = 59.74
    
    plots_basedir = "plots/{}/{}/exp/".format(ntuple_version, tag)

    fname_sig     = "outputs/{}/{}/sig.root".format(ntuple_version, tag)
    # fname_sig     = "outputs/{}/{}/wwz.root".format(ntuple_version, tag)
    fname_ttz     = "outputs/{}/{}/ttz.root".format(ntuple_version, tag)
    fname_zz      = "outputs/{}/{}/zz.root".format(ntuple_version, tag)
    fname_wz      = "outputs/{}/{}/wz.root".format(ntuple_version, tag)
    fname_twz     = "outputs/{}/{}/twz.root".format(ntuple_version, tag)
    fname_rare    = "outputs/{}/{}/rare.root".format(ntuple_version, tag)
    # fname_rare    = "outputs/{}/{}/rarevvv.root".format(ntuple_version, tag)
    fname_dyttbar = "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag)
    fname_higgs   = "outputs/{}/{}/higgs.root".format(ntuple_version, tag)
    fname_data    = "outputs/{}/{}/data.root".format(ntuple_version, tag)

    year = "2" + ntuple_version.split("_")[0].split("2")[1]
    prefix = "{}/{}".format(ntuple_version, tag)

    procs = ["data_obs", "sig", "ttz", "zz", "wz", "twz", "rare", "dyttbar", "higgs"]
    mcprocs = procs[1:]
    bkgprocs = procs[2:]
    fnames = [fname_data, fname_sig, fname_ttz, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonzzbkg = [fname_sig, fname_ttz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonttzbkg = [fname_sig, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]

    if num_proc == "zz":

        h_denom_nonzzbkg = pr.get_summed_histogram(nonzzbkg, denominator)
        E_denom_nonzzbkg = pr.get_integral_as_E(h_denom_nonzzbkg)
        h_denom_data = pr.get_summed_histogram([fname_data], denominator)
        E_denom_data = pr.get_integral_as_E(h_denom_data)
        h_denom_zz = pr.get_summed_histogram([fname_zz], denominator)
        E_denom_zz = pr.get_integral_as_E(h_denom_zz)
        # print (E_denom_data - E_denom_nonzzbkg)
        # print E_denom_zz

        h_numer_nonzzbkg = pr.get_summed_histogram(nonzzbkg, numerator)
        E_numer_nonzzbkg = pr.get_integral_as_E(h_numer_nonzzbkg)
        h_numer_data = pr.get_summed_histogram([fname_data], numerator)
        E_numer_data = pr.get_integral_as_E(h_numer_data)
        h_numer_zz = pr.get_summed_histogram([fname_zz], numerator)
        E_numer_zz = pr.get_integral_as_E(h_numer_zz)
        # print (E_numer_data - E_numer_nonzzbkg)
        # print E_numer_zz

        data_eff =  (E_numer_data - E_numer_nonzzbkg) / (E_denom_data - E_denom_nonzzbkg)
        mc_eff =  E_numer_zz / E_denom_zz
        eff_ratio = data_eff / mc_eff
        print "mc_eff:", mc_eff
        print "data_eff:", data_eff
        print "eff_ratio:", eff_ratio

    else:


        h_denom_nonttzbkg = pr.get_summed_histogram(nonttzbkg, denominator)
        E_denom_nonttzbkg = pr.get_integral_as_E(h_denom_nonttzbkg)
        h_denom_data = pr.get_summed_histogram([fname_data], denominator)
        E_denom_data = pr.get_integral_as_E(h_denom_data)
        h_denom_ttz = pr.get_summed_histogram([fname_ttz], denominator)
        E_denom_ttz = pr.get_integral_as_E(h_denom_ttz)
        # print (E_denom_data - E_denom_nonttzbkg)
        # print E_denom_ttz

        h_numer_nonttzbkg = pr.get_summed_histogram(nonttzbkg, numerator)
        E_numer_nonttzbkg = pr.get_integral_as_E(h_numer_nonttzbkg)
        h_numer_data = pr.get_summed_histogram([fname_data], numerator)
        E_numer_data = pr.get_integral_as_E(h_numer_data)
        h_numer_ttz = pr.get_summed_histogram([fname_ttz], numerator)
        E_numer_ttz = pr.get_integral_as_E(h_numer_ttz)
        # print (E_numer_data - E_numer_nonttzbkg)
        # print E_numer_ttz

        data_eff = (E_numer_data - E_numer_nonttzbkg) / (E_denom_data - E_denom_nonttzbkg)
        mc_eff = E_numer_ttz / E_denom_ttz
        eff_ratio = data_eff / mc_eff
        print "mc_eff:", mc_eff
        print "data_eff:", data_eff
        print "eff_ratio:", eff_ratio

if __name__ == "__main__":

    denominator = "ChannelBTagEMu__Yield"
    numerator = "ChannelBTagEMuHighMET__Yield"
    print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    get_extrapolation_uncertainty("WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11", "y2016_baseline_0709_y2017_baseline_0709_y2018_baseline_0709", numerator, denominator, "ttz")
    print ""
    print ""

    denominator = "ChannelBTagEMu__Yield"
    numerator = "ChannelBTagEMuHighMT__Yield"
    print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    get_extrapolation_uncertainty("WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11", "y2016_baseline_0709_y2017_baseline_0709_y2018_baseline_0709", numerator, denominator, "ttz")
    print ""
    print ""

    denominator = "ChannelOnZ__Yield"
    numerator = "ChannelOnZHighMET__Yield"
    print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    get_extrapolation_uncertainty("WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11", "y2016_baseline_0709_y2017_baseline_0709_y2018_baseline_0709", numerator, denominator, "zz")
    print ""
    print ""

    denominator = "ChannelOnZ__Yield"
    numerator = "ChannelOnZHighMT__Yield"
    print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    get_extrapolation_uncertainty("WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11", "y2016_baseline_0709_y2017_baseline_0709_y2018_baseline_0709", numerator, denominator, "zz")
    print ""
    print ""

