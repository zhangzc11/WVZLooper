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
import math

syst_list_all = ["Nominal", "JES", "Pileup", "BTagHF", "BTagLF", "PDF", "Qsq", "AlphaS", "MET"]
syst_list = ["Nominal", "JES", "Pileup", "MET"]
syst_list = syst_list_all

def get_alpha_uncertainty(ntuple_version, tag, numerator, denominator, num_proc, valopt):

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
        # print E_numer_data, E_numer_zz
        # print "mc_eff:", mc_eff
        # print "data_eff:", data_eff
        if valopt == "eff":
            return mc_eff
        elif valopt == "den":
            return E_denom_zz
        elif valopt == "num":
            return E_numer_zz

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
        # print "mc_eff:", mc_eff
        # print "data_eff:", data_eff
        if valopt == "eff":
            return mc_eff
        elif valopt == "den":
            return E_denom_ttz
        elif valopt == "num":
            return E_numer_ttz


def get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, num_proc, valopt):

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
        # print E_numer_data, E_numer_zz
        # print "data_eff:", data_eff
        if valopt == "ratio":
            return eff_ratio
        elif valopt == "mc":
            return mc_eff
        elif valopt == "data":
            return data_eff
        elif valopt == "mc_num":
            return E_numer_zz
        elif valopt == "data_num":
            return (E_numer_data - E_numer_nonzzbkg)
        elif valopt == "mc_den":
            return E_denom_zz
        elif valopt == "data_den":
            return (E_denom_data - E_denom_nonzzbkg)

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
        # print "mc_eff:", mc_eff
        # print "data_eff:", data_eff
        if valopt == "ratio":
            return eff_ratio
        elif valopt == "mc":
            return mc_eff
        elif valopt == "data":
            return data_eff
        elif valopt == "mc_num":
            return E_numer_ttz
        elif valopt == "data_num":
            return (E_numer_data - E_numer_nonttzbkg)
        elif valopt == "mc_den":
            return E_denom_ttz
        elif valopt == "data_den":
            return (E_denom_data - E_denom_nonttzbkg)


def run_for_variation(variation=""):

    ntuple_version = "WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11"
    tag = "y2016_baseline_0711_METCorr_w_syst_y2017_baseline_0711_METCorr_w_syst_y2018_baseline_0711_METCorr_w_syst"

    denominator = "ChannelBTagEMu{}__Yield".format(variation)
    numerator = "ChannelBTagEMuHighMET{}__Yield".format(variation)
    # print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    print get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, "ttz")
    print ""
    print ""

    denominator = "ChannelBTagEMu{}__Yield".format(variation)
    numerator = "ChannelBTagEMuHighMT{}__Yield".format(variation)
    # print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    print get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, "ttz")
    print ""
    print ""

    denominator = "ChannelOnZ{}__Yield".format(variation)
    numerator = "ChannelOnZHighMET{}__Yield".format(variation)
    # print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    print get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, "zz")
    print ""
    print ""

    denominator = "ChannelOnZ{}__Yield".format(variation)
    numerator = "ChannelOnZHighMT{}__Yield".format(variation)
    # print "Cut denominator:", denominator
    print "Cut numerator:", numerator
    print get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, "zz")
    print ""
    print ""

def run(process, region, variable, variation="", valopt="ratio"):
    ntuple_version = "WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11"
    tag = "y2016_baseline_0711_METCorr_w_syst_y2017_baseline_0711_METCorr_w_syst_y2018_baseline_0711_METCorr_w_syst"
    denominator = "Channel{}{}__Yield".format(region, variation)
    numerator = "Channel{}High{}{}__Yield".format(region, variable, variation)
    # print denominator, numerator
    return get_extrapolation_uncertainty(ntuple_version, tag, numerator, denominator, process, valopt)

def run_alpha(process, numerator_region, denominator_region, variation="", valopt="eff"):
    ntuple_version = "WVZ2016_v0.1.11_WVZ2017_v0.1.11_WVZ2018_v0.1.11"
    tag = "y2016_baseline_0711_METCorr_w_syst_y2017_baseline_0711_METCorr_w_syst_y2018_baseline_0711_METCorr_w_syst"
    denominator = "{}{}__Yield".format(denominator_region, variation)
    numerator = "{}{}__Yield".format(numerator_region, variation)
    # print denominator, numerator
    return get_alpha_uncertainty(ntuple_version, tag, numerator, denominator, process, valopt)

def get_eff_ratios(process, region, variable, valopt="ratio"):

    systs = syst_list[1:]

    nominal = run(process, region, variable, "", valopt)

    rtn_val = {}
    rtn_val["Nominal"] = nominal

    for syst in systs:
        var = E(nominal.val, 0)
        varup = run(process, region, variable, syst+"Up", valopt)
        vardn = run(process, region, variable, syst+"Down", valopt)
        err = math.sqrt(abs(((varup - var) * (vardn - var)).val))
        var.err = err
        rtn_val[syst] = var
        # print syst, varup, vardn, nominal

    for key in syst_list:
        print "{:<10s} {:.4f} {:.4f} {:.4f}".format(key, rtn_val[key].val, rtn_val[key].err, rtn_val[key].err / rtn_val[key].val)

    hists = []

    for index, key in enumerate(syst_list):
        h = r.TH1F("{}".format(key), "", 1, 0, 1)
        h.SetBinContent(1, rtn_val[key].val)
        h.SetBinError(1, rtn_val[key].err)
        hists.append(h)

    return hists

def get_alpha(process, numerator_region, denominator_region, valopt="eff"):

    systs = syst_list_all[1:]

    nominal = run_alpha(process, numerator_region, denominator_region, "", valopt)

    rtn_val = {}
    rtn_val["Nominal"] = nominal

    for syst in systs:
        var = E(nominal.val, 0)
        varup = run_alpha(process, numerator_region, denominator_region, syst+"Up", valopt)
        vardn = run_alpha(process, numerator_region, denominator_region, syst+"Down", valopt)
        err = math.sqrt(abs(((varup - var) * (vardn - var)).val))
        var.err = err
        rtn_val[syst] = var
        # print syst, varup, vardn, nominal

    for key in syst_list_all:
        print "{:<10s} {:.3f} {:.3f} {:.3f}".format(key, rtn_val[key].val, rtn_val[key].err, rtn_val[key].err / rtn_val[key].val)

    hists = []

    for index, key in enumerate(syst_list_all):
        h = r.TH1F("{}".format(key), "", 1, 0, 1)
        h.SetBinContent(1, rtn_val[key].val)
        h.SetBinError(1, rtn_val[key].err)
        hists.append(h)

    return hists

def main_onz_ttz_only():
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "mc") , options={"output_name":"exp/mc_eff_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "data") , options={"output_name":"exp/data_eff_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "ratio") , options={"output_name":"exp/eff_ratio_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "mc_num") , options={"output_name":"exp/eff_ratio_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "mc_den") , options={"output_name":"exp/eff_ratio_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "data_num") , options={"output_name":"exp/eff_ratio_ttz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MET" , "data_den") , options={"output_name":"exp/eff_ratio_ttz_met.pdf"  , "print_yield":True} ) 

    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "mc") , options={"output_name":"exp/mc_eff_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "data") , options={"output_name":"exp/data_eff_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "ratio") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "mc_num") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "mc_den") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "data_num") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz"  , "BTagEMu"     , "MT" , "data_den") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 

def main_onz_zz_met_only():
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc") , options={"output_name":"exp/mc_eff_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data") , options={"output_name":"exp/data_eff_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "ratio") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc_num") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc_den") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data_num") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data_den") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 

def main():

    # Get TTZ MET Modeling Uncertainty
    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MET" , "mc") , options={"output_name":"exp/mc_eff_ttz_met.pdf" , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MET" , "data") , options={"output_name":"exp/data_eff_ttz_met.pdf" , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MET" , "ratio") , options={"output_name":"exp/eff_ratio_ttz_met.pdf" , "print_yield":True} ) 

    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MT"  , "mc") , options={"output_name":"exp/mc_eff_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MT"  , "data") , options={"output_name":"exp/data_eff_ttz_mt.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("ttz" , "BTagEMu" , "MT"  , "ratio") , options={"output_name":"exp/eff_ratio_ttz_mt.pdf"  , "print_yield":True} ) 

    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc") , options={"output_name":"exp/mc_eff_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data") , options={"output_name":"exp/data_eff_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "ratio") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc_num") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "mc_den") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data_num") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MET" , "data_den") , options={"output_name":"exp/eff_ratio_zz_met.pdf"  , "print_yield":True} ) 

    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MT"  , "mc") , options={"output_name":"exp/mc_eff_zz_mt.pdf"   , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MT"  , "data") , options={"output_name":"exp/data_eff_zz_mt.pdf"   , "print_yield":True} ) 
    p.plot_hist(bgs=get_eff_ratios("zz"  , "OnZ"     , "MT"  , "ratio") , options={"output_name":"exp/eff_ratio_zz_mt.pdf"   , "print_yield":True} ) 

    p.plot_hist(bgs=get_alpha("ttz", "ChannelEMu", "ChannelBTagEMu", "num"), options={"output_name":"exp/ttz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("ttz", "ChannelEMu", "ChannelBTagEMu", "den"), options={"output_name":"exp/ttz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("ttz", "ChannelEMu", "ChannelBTagEMu", "eff"), options={"output_name":"exp/ttz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("ttz", "ChannelOffZ", "ChannelBTagEMu", "num"), options={"output_name":"exp/ttz_offz_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("ttz", "ChannelOffZ", "ChannelBTagEMu", "den"), options={"output_name":"exp/ttz_offz_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("ttz", "ChannelOffZ", "ChannelBTagEMu", "eff"), options={"output_name":"exp/ttz_offz_alpha.pdf", "print_yield":True})

    p.plot_hist(bgs=get_alpha("zz", "ChannelEMu", "ChannelOnZ", "num"), options={"output_name":"exp/zz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("zz", "ChannelEMu", "ChannelOnZ", "den"), options={"output_name":"exp/zz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("zz", "ChannelEMu", "ChannelOnZ", "eff"), options={"output_name":"exp/zz_emu_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("zz", "ChannelOffZ", "ChannelOnZ", "num"), options={"output_name":"exp/zz_offz_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("zz", "ChannelOffZ", "ChannelOnZ", "den"), options={"output_name":"exp/zz_offz_alpha.pdf", "print_yield":True})
    p.plot_hist(bgs=get_alpha("zz", "ChannelOffZ", "ChannelOnZ", "eff"), options={"output_name":"exp/zz_offz_alpha.pdf", "print_yield":True})

if __name__ == "__main__":

    main_onz_ttz_only()

