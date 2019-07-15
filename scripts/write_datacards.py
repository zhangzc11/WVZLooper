#!/bin/env python

import ROOT as r

import pyrootutil as pr
import plottery_wrapper as p
import argparse
import sys
import os
from errors import E
import datacard_writer as dw

parser = argparse.ArgumentParser(description="Plotter for the WVZ analysis")
parser.add_argument('-b' , '--baseline_tag'    , dest='baseline_tag'    , help='baseline tag (e.g. test, test1. test2, etc.)' , required=True)
parser.add_argument('-t' , '--ntuple_type'     , dest='ntuple_type'     , help='WVZ, Trilep, etc.'                            , required=True)
parser.add_argument('-v' , '--ntuple_version'  , dest='ntuple_version'  , help='v0.1.6, v0.1.7, etc.'                         , required=True)

args = parser.parse_args()

def main():

    sample2016 = "{}2016_{}".format(args.ntuple_type, args.ntuple_version)
    sample2017 = "{}2017_{}".format(args.ntuple_type, args.ntuple_version)
    sample2018 = "{}2018_{}".format(args.ntuple_type, args.ntuple_version)
    tag2016 = "y2016_" + args.baseline_tag
    tag2017 = "y2017_" + args.baseline_tag
    tag2018 = "y2018_" + args.baseline_tag
    write_datacards(sample2016, tag2016)
    write_datacards(sample2017, tag2017)
    write_datacards(sample2018, tag2018)
    write_datacards("_".join([ sample2016, sample2017, sample2018 ]), "_".join([ tag2016, tag2017, tag2018 ]))

def write_datacards(ntuple_version, tag):

    # ntuple_version = args.sample_set_name
    # tag = args.tag

    fname_sig     = "outputs/{}/{}/sig.root".format(ntuple_version, tag)
    # fname_sig     = "outputs/{}/{}/wwz.root".format(ntuple_version, tag)
    # fname_sig     = "outputs/{}/{}/wzz.root".format(ntuple_version, tag)
    # fname_sig     = "outputs/{}/{}/zzz.root".format(ntuple_version, tag)
    fname_ttz     = "outputs/{}/{}/ttz.root".format(ntuple_version, tag)
    fname_zz      = "outputs/{}/{}/zz.root".format(ntuple_version, tag)
    fname_wz      = "outputs/{}/{}/wz.root".format(ntuple_version, tag)
    fname_twz     = "outputs/{}/{}/twz.root".format(ntuple_version, tag)
    fname_rare    = "outputs/{}/{}/rare.root".format(ntuple_version, tag)
    # fname_rare    = "outputs/{}/{}/rarevvv.root".format(ntuple_version, tag)
    fname_dyttbar = "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag)
    fname_higgs   = "outputs/{}/{}/higgs.root".format(ntuple_version, tag)
    fname_other   = "outputs/{}/{}/other.root".format(ntuple_version, tag)
    # fname_other   = "outputs/{}/{}/othervvv.root".format(ntuple_version, tag)
    fname_data    = "outputs/{}/{}/data.root".format(ntuple_version, tag)

    year = "2" + ntuple_version.split("_")[0].split("2")[1]
    prefix = "{}/{}".format(ntuple_version, tag)

    procs = ["data_obs", "sig", "ttz", "zz", "wz", "twz", "rare", "dyttbar", "higgs"]
    mcprocs = procs[1:]
    bkgprocs = procs[2:]
    fnames = [fname_data, fname_sig, fname_ttz, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonzzbkg = [fname_sig, fname_ttz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonttzbkg = [fname_sig, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]

    procs = ["data_obs", "sig", "ttz", "zz", "wz", "twz", "other"]
    mcprocs = procs[1:]
    bkgprocs = procs[2:]
    fnames = [fname_data, fname_sig, fname_ttz, fname_zz, fname_wz, fname_twz, fname_other]
    nonzzbkg = [fname_sig, fname_ttz, fname_wz, fname_twz, fname_other]
    nonttzbkg = [fname_sig, fname_zz, fname_wz, fname_twz, fname_other]

    systcategs = ["BTagHF", "BTagLF", "JES", "Pileup", "Qsq", "PDF", "AlphaS", "MET"] # Null string is the nominal variation
    # Form systnames (i.e. ["Nominal", "BTagHFUp", "BTagHFDown", "BTagLFUp", "BTagLFDown"])
    systnames = ["Nominal"] # Nominal always exist
    for systcateg in systcategs:
        systnames.append(systcateg+"Up")
        systnames.append(systcateg+"Down")

    #############
    # Open TFiles
    #############
    tfiles = {}
    for proc, fname in zip(procs, fnames):
        tfiles[proc] = r.TFile(fname)

    ###########################
    # OnZ Control region yields
    ###########################

    onz_zz_h = pr.get_summed_histogram([fname_zz], "ChannelOnZ__Yield")
    onz_data_h = pr.get_summed_histogram([fname_data], "ChannelOnZ__Yield")
    onz_nonzz_h = pr.get_summed_histogram(nonzzbkg, "ChannelOnZ__Yield")
    zz_sf = pr.get_sf(onz_zz_h, onz_data_h, onz_nonzz_h).GetBinContent(1)
    zz_sferr = pr.get_sf(onz_zz_h, onz_data_h, onz_nonzz_h).GetBinError(1)
    expected_nevt_zz = onz_data_h.GetBinContent(1)

    ############################
    # BTag Control region yields
    ############################

    bcr_ttz_h = pr.get_summed_histogram([fname_ttz], "ChannelBTagEMu__Yield")
    bcr_data_h = pr.get_summed_histogram([fname_data], "ChannelBTagEMu__Yield")
    bcr_nonttz_h = pr.get_summed_histogram(nonttzbkg, "ChannelBTagEMu__Yield")
    ttz_sf = pr.get_sf(bcr_ttz_h, bcr_data_h, bcr_nonttz_h).GetBinContent(1)
    ttz_sferr = pr.get_sf(bcr_ttz_h, bcr_data_h, bcr_nonttz_h).GetBinError(1)
    expected_nevt_ttz = bcr_data_h.GetBinContent(1)

    print year, "ttz_sf", "{:.2f} +/- {:.2f}".format(ttz_sf, ttz_sferr), expected_nevt_ttz
    print year, "zz_sf", "{:.2f} +/- {:.2f}".format(zz_sf, zz_sferr), expected_nevt_zz

    ###############################
    # EMu channel data card writing
    ###############################

    # number of bins
    # nbins = 1
    # fitvar = "Yield"
    nbins = 5
    fitvar = "MllNom"

    # Main data base to hold all the histograms
    hists_db = {}

    # Loop over the processes
    for proc in procs:

        # Retrieve the tfile
        tfile = tfiles[proc]

        # For each processes create another map to hold various histograms
        hists_db[proc] = {}

        # Loop over the systematic variations
        for syst in systnames:

            if syst == "Nominal":
                if fitvar == "MllNom":
                    h = rebin36(tfile.Get("ChannelEMuHighMT__{}".format(fitvar)).Clone())
                else:
                    h = tfile.Get("ChannelEMuHighMT__{}".format(fitvar)).Clone()
            else:
                if fitvar == "MllNom":
                    h = rebin36(tfile.Get("ChannelEMuHighMT{}__{}".format(syst, fitvar)).Clone())
                else:
                    h = tfile.Get("ChannelEMuHighMT{}__{}".format(syst, fitvar)).Clone()

            h.SetTitle("emu{}_{}".format(year, proc))

            if proc == "ttz": h.Scale(ttz_sf)
            if proc == "zz": h.Scale(zz_sf)
            # if proc == "wz": h.Scale(2)

            hists_db[proc][syst] = h

    systs = []

    # ZZ CR systematic line
    onz_cr_hist = r.TH1F("onz_cr", "", nbins, 0, nbins)
    for i in xrange(1, nbins+1):
        onz_cr_hist.SetBinContent(i, expected_nevt_zz)
    alpha = hists_db["zz"]["Nominal"].Clone("alpha")
    alpha.Divide(onz_cr_hist)
    thissyst = {}
    for proc in mcprocs:
       if proc == "zz":
           thissyst["emu{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,nbins+1) ]
       else:
           thissyst["emu{}_".format(year) + proc] = 0
    systs.append(("CRZZ{}".format(year), "gmN", [onz_cr_hist], thissyst))

    # ttZ CR systematic line
    btag_cr_hist = r.TH1F("btag_cr", "", nbins, 0, nbins)
    for i in xrange(1, nbins+1):
        btag_cr_hist.SetBinContent(i, expected_nevt_ttz)
    alpha = hists_db["ttz"]["Nominal"].Clone("alpha")
    alpha.Divide(btag_cr_hist)
    thissyst = {}
    for proc in mcprocs:
       if proc == "ttz":
           thissyst["emu{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,nbins+1) ]
       else:
           thissyst["emu{}_".format(year) + proc] = 0
    systs.append(("CRTTZ{}".format(year), "gmN", [btag_cr_hist], thissyst))

    # Experimental systematics
    for systcateg in systcategs:
        thissyst = {}
        for proc in mcprocs:
            if proc not in ["zz", "ttz"]:
                thissyst["emu{}_".format(year) + proc] = [hists_db[proc][systcateg+"Up"], hists_db[proc][systcateg+"Down"]]
            else:
                thissyst["emu{}_".format(year) + proc] = 0
        systs.append( (systcateg+year, "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "ttz": thissyst["emu{}_".format(year) + proc] = "1.10"
        else: thissyst["emu{}_".format(year) + proc] = 0
    systs.append( ("FlatSystTFNbTTZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "ttz": thissyst["emu{}_".format(year) + proc] = "1.02"
        else: thissyst["emu{}_".format(year) + proc] = 0
    systs.append( ("FlatSystMTexpTTZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "zz": thissyst["emu{}_".format(year) + proc] = "1.04"
        else: thissyst["emu{}_".format(year) + proc] = 0
    systs.append( ("FlatSystTFEMuZZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "zz": thissyst["emu{}_".format(year) + proc] = "1.05"
        else: thissyst["emu{}_".format(year) + proc] = 0
    systs.append( ("FlatSystMTexpZZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "wz": thissyst["emu{}_".format(year) + proc] = "1.6" # Fake Syst
        else: thissyst["emu{}_".format(year) + proc] = 0
    systs.append( ("FlatSystWZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        thissyst["emu{}_".format(year) + proc] = "1.025"
    systs.append( ("FlatSystLumi{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        thissyst["emu{}_".format(year) + proc] = "1.03"
    systs.append( ("FlatSystsIP3D{}".format(year), "lnN", [], thissyst) )

    # Now create data card writer
    sig = hists_db["sig"]["Nominal"]
    bgs = [ hists_db[proc]["Nominal"] for proc in bkgprocs ]
    data = hists_db["data_obs"]["Nominal"]
    d = dw.DataCardWriter(sig=sig, bgs=bgs, data=data, systs=systs, no_stat_procs=["emu{}_zz".format(year), "emu{}_ttz".format(year)])

    if nbins == 5:
        for i in xrange(1, nbins+1):
            d.set_bin(i)
            d.set_region_name("bin{}".format(i))
            d.write("stats/{}/emu_datacard_bin{}.txt".format(prefix, i))
    elif nbins == 1:
        d.set_bin(1)
        d.set_region_name("bin{}".format(1))
        d.write("stats/{}/emu_datacard_singlebin{}.txt".format(prefix, 1))
        d.print_yields()

    # colors = [2005, 2001, 2003, 2007, 920, 2012, 2011, 2002]
    # p.plot_hist(data=None, bgs=bgs, sigs=[sig], options={"bkg_sort_method":"ascending", "yaxis_range":[0.,2.5]}, colors=colors, sig_labels=["sig"], legend_labels=bkgprocs)

    ################################
    # OffZ channel data card writing
    ################################

    # number of bins
    nbins = 1

    # Main data base to hold all the histograms
    hists_db = {}

    # Loop over the processes
    for proc in procs:

        # Retrieve the tfile
        tfile = tfiles[proc]

        # For each processes create another map to hold various histograms
        hists_db[proc] = {}

        # Loop over the systematic variations
        for syst in systnames:

            if syst == "Nominal":
                h = tfile.Get("ChannelOffZHighMET__Yield").Clone()
            else:
                h = tfile.Get("ChannelOffZHighMET{}__Yield".format(syst)).Clone()

            h.SetTitle("offz{}_{}".format(year, proc))

            if proc == "ttz": h.Scale(ttz_sf)
            if proc == "zz": h.Scale(zz_sf)
            # if proc == "wz": h.Scale(2)

            hists_db[proc][syst] = h

    systs = []

    # ZZ CR systematic line
    onz_cr_hist = r.TH1F("onz_cr", "", nbins, 0, nbins)
    for i in xrange(1, nbins+1):
        onz_cr_hist.SetBinContent(i, expected_nevt_zz)
    alpha = hists_db["zz"]["Nominal"].Clone("alpha")
    alpha.Divide(onz_cr_hist)
    thissyst = {}
    for proc in mcprocs:
       if proc == "zz":
           thissyst["offz{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(1)) ]
       else:
           thissyst["offz{}_".format(year) + proc] = 0
    systs.append(("CRZZ{}".format(year), "gmN", [onz_cr_hist], thissyst))

    # ttZ CR systematic line
    btag_cr_hist = r.TH1F("btag_cr", "", nbins, 0, nbins)
    for i in xrange(1, nbins+1):
        btag_cr_hist.SetBinContent(i, expected_nevt_ttz)
    alpha = hists_db["ttz"]["Nominal"].Clone("alpha")
    alpha.Divide(btag_cr_hist)
    thissyst = {}
    for proc in mcprocs:
       if proc == "ttz":
           thissyst["offz{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(1)) ]
       else:
           thissyst["offz{}_".format(year) + proc] = 0
    systs.append(("CRTTZ{}".format(year), "gmN", [btag_cr_hist], thissyst))

    # Experimental systematics
    for systcateg in systcategs:
        thissyst = {}
        for proc in mcprocs:
            if proc not in ["zz", "ttz"]:
                thissyst["offz{}_".format(year) + proc] = [hists_db[proc][systcateg+"Up"], hists_db[proc][systcateg+"Down"]]
            else:
                thissyst["offz{}_".format(year) + proc] = 0
        systs.append( (systcateg+year, "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "ttz": thissyst["offz{}_".format(year) + proc] = "1.10"
        else: thissyst["offz{}_".format(year) + proc] = 0
    systs.append( ("FlatSystTFeemmTTZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "ttz": thissyst["offz{}_".format(year) + proc] = "1.03"
        else: thissyst["offz{}_".format(year) + proc] = 0
    systs.append( ("FlatSystMETexpTTZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "zz": thissyst["offz{}_".format(year) + proc] = "1.03"
        else: thissyst["offz{}_".format(year) + proc] = 0
    systs.append( ("FlatSystTFMllZZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "zz": thissyst["offz{}_".format(year) + proc] = "1.23"
        else: thissyst["offz{}_".format(year) + proc] = 0
    systs.append( ("FlatSystMETexpZZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        if proc == "wz": thissyst["offz{}_".format(year) + proc] = "1.6" # Fake Syst
        else: thissyst["offz{}_".format(year) + proc] = 0
    systs.append( ("FlatSystWZ{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        thissyst["offz{}_".format(year) + proc] = "1.025"
    systs.append( ("FlatSystLumi{}".format(year), "lnN", [], thissyst) )

    # Flat additional systematics
    thissyst = {}
    for proc in mcprocs:
        thissyst["offz{}_".format(year) + proc] = "1.03"
    systs.append( ("FlatSystsIP3D{}".format(year), "lnN", [], thissyst) )

    # Now create data card writer
    sig = hists_db["sig"]["Nominal"]
    bgs = [ hists_db[proc]["Nominal"] for proc in bkgprocs ]
    data = hists_db["data_obs"]["Nominal"]
    d = dw.DataCardWriter(sig=sig, bgs=bgs, data=data, systs=systs, no_stat_procs=["offz{}_zz".format(year), "offz{}_ttz".format(year)])

    for i in xrange(1, nbins+1):
        d.set_bin(i)
        d.set_region_name("bin{}".format(i))
        d.write("stats/{}/offz_datacard_bin{}.txt".format(prefix, i))
        d.print_yields()

#========

    ##################################
    ## 5 lep channel data card writing
    ##################################

    ## number of bins
    #nbins = 1

    ## Main data base to hold all the histograms
    #hists_db = {}

    ## Loop over the processes
    #for proc in procs:

    #    # Retrieve the tfile
    #    tfile = tfiles[proc]

    #    # For each processes create another map to hold various histograms
    #    hists_db[proc] = {}

    #    # Loop over the systematic variations
    #    for syst in systnames:

    #        if syst == "Nominal":
    #            h = tfile.Get("FiveLeptonsRelIso5th__Yield").Clone()
    #        else:
    #            h = tfile.Get("FiveLeptonsRelIso5th{}__Yield".format(syst)).Clone()

    #        h.SetTitle("five{}_{}".format(year, proc))

    #        # if proc == "ttz": h.Scale(ttz_sf)
    #        # if proc == "zz": h.Scale(zz_sf)

    #        hists_db[proc][syst] = h

    #systs = []

    ## # ZZ CR systematic line
    ## onz_cr_hist = r.TH1F("onz_cr", "", nbins, 0, nbins)
    ## for i in xrange(1, nbins+1):
    ##     onz_cr_hist.SetBinContent(i, expected_nevt_zz)
    ## alpha = hists_db["zz"]["Nominal"].Clone("alpha")
    ## alpha.Divide(onz_cr_hist)
    ## thissyst = {}
    ## for proc in mcprocs:
    ##    if proc == "zz":
    ##        thissyst["five{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(1)) ]
    ##    else:
    ##        thissyst["five{}_".format(year) + proc] = 0
    ## systs.append(("CRZZ{}".format(year), "gmN", [onz_cr_hist], thissyst))

    ## # ttZ CR systematic line
    ## btag_cr_hist = r.TH1F("btag_cr", "", nbins, 0, nbins)
    ## for i in xrange(1, nbins+1):
    ##     btag_cr_hist.SetBinContent(i, expected_nevt_ttz)
    ## alpha = hists_db["ttz"]["Nominal"].Clone("alpha")
    ## alpha.Divide(btag_cr_hist)
    ## thissyst = {}
    ## for proc in mcprocs:
    ##    if proc == "ttz":
    ##        thissyst["five{}_".format(year) + proc] = [ "{:4f}".format(alpha.GetBinContent(1)) ]
    ##    else:
    ##        thissyst["five{}_".format(year) + proc] = 0
    ## systs.append(("CRTTZ{}".format(year), "gmN", [btag_cr_hist], thissyst))

    ## Experimental systematics
    #for systcateg in systcategs:
    #    thissyst = {}
    #    for proc in mcprocs:
    #        # if proc not in ["zz", "ttz"]:
    #            thissyst["five{}_".format(year) + proc] = [hists_db[proc][systcateg+"Up"], hists_db[proc][systcateg+"Down"]]
    #        # else:
    #            # thissyst["five{}_".format(year) + proc] = 0
    #    systs.append( (systcateg+year, "lnN", [], thissyst) )

    ## # Flat additional systematics
    ## thissyst = {}
    ## for proc in mcprocs:
    ##     if proc == "ttz": thissyst["five{}_".format(year) + proc] = "1.11"
    ##     else: thissyst["five{}_".format(year) + proc] = 0
    ## systs.append( ("FlatSystTTZ{}".format(year), "lnN", [], thissyst) )

    ## # Flat additional systematics
    ## thissyst = {}
    ## for proc in mcprocs:
    ##     if proc == "zz": thissyst["five{}_".format(year) + proc] = "1.9"
    ##     else: thissyst["five{}_".format(year) + proc] = 0
    ## systs.append( ("FlatSystZZ{}".format(year), "lnN", [], thissyst) )

    ## # Flat additional systematics
    ## thissyst = {}
    ## for proc in mcprocs:
    ##     if proc == "wz": thissyst["five{}_".format(year) + proc] = "1.6"
    ##     else: thissyst["five{}_".format(year) + proc] = 0
    ## systs.append( ("FlatSystWZ{}".format(year), "lnN", [], thissyst) )

    ## Flat additional systematics
    #thissyst = {}
    #for proc in mcprocs:
    #    thissyst["five{}_".format(year) + proc] = "1.025"
    #systs.append( ("FlatSystLumi{}".format(year), "lnN", [], thissyst) )

    ## Flat additional systematics
    #thissyst = {}
    #for proc in mcprocs:
    #    thissyst["five{}_".format(year) + proc] = "1.03"
    #systs.append( ("FlatSystsIP3D{}".format(year), "lnN", [], thissyst) )

    ## Now create data card writer
    #sig = hists_db["sig"]["Nominal"]
    #bgs = [ hists_db[proc]["Nominal"] for proc in bkgprocs ]
    #data = hists_db["data_obs"]["Nominal"]
    #d = dw.DataCardWriter(sig=sig, bgs=bgs, data=data, systs=systs,
    #        # no_stat_procs=["five{}_zz".format(year), "five{}_ttz".format(year)]
    #        )

    #for i in xrange(1, nbins+1):
    #    d.set_bin(i)
    #    d.set_region_name("bin{}".format(i))
    #    d.write("stats/{}/five_datacard_bin{}.txt".format(prefix, i))

def rebin36(h):

    # To rebin emu channels 180 bins to 5 bin

    h.Rebin(36)
    bin5 = E(h.GetBinContent(5), h.GetBinError(5))
    bin6 = E(h.GetBinContent(6), h.GetBinError(6))
    bin5plus6 = bin5 + bin6
    h.SetBinContent(6, 0)
    h.SetBinError(6, 0)
    h.SetBinContent(5, bin5plus6.val)
    h.SetBinError(5, bin5plus6.err)

    return h



##-----------------------------------------------------------------------------------------------------------

#    tfiles = []
#    procnames = []
#    hists = []

#    for index, s in enumerate(files):

#        # ROOT file
#        f = r.TFile(s)
#        tfiles.append(f)

#        # Name of the process
#        basename = os.path.basename(s)
#        procname = basename.replace(".root", "")

#        # List of procnames
#        procnames.append(procname)

#        # EMu channel
#        h = f.Get("ChannelEMuHighMT__MllNom")

#        # Bin it to 5 bin
#        h.Rebin(36)
#        bin5 = E(h.GetBinContent(5), h.GetBinError(5))
#        bin6 = E(h.GetBinContent(6), h.GetBinError(6))
#        bin5plus6 = bin5 + bin6
#        h.SetBinContent(6, 0)
#        h.SetBinError(6, 0)
#        h.SetBinContent(5, bin5plus6.val)
#        h.SetBinError(5, bin5plus6.err)

#        # If index == 0 then write data
#        if index == 0:
#            d = h.Clone("emu{}_data_obs".format(year))
#            d.Reset()
#            d.SetBinContent(1, 1)
#            d.SetBinContent(2, 1)
#            d.SetBinContent(3, 1)
#            d.SetBinContent(4, 1)
#            d.SetBinContent(5, 1)
#            hists.append(d)

#        # Aggregate histograms
#        hists.append(h.Clone("emu{}_".format(year) + procname))
#        hists[-1].SetTitle("emu{}_".format(year) + procname)

#        if index == 1: # this is ttZ process
#            hists[-1].Scale(ttz_sf)
#            print year, "ttz_sf", ttz_sf

#        if index == 2: # this is ZZ process
#            hists[-1].Scale(zz_sf)
#            print year, "zz_sf", zz_sf

#    systs = []

#    # ZZ CR systematic line
#    onz_cr_hist = r.TH1F("onz_cr", "", 5, 0, 5)
#    onz_cr_hist.SetBinContent(1, expected_nevt_zz)
#    onz_cr_hist.SetBinContent(2, expected_nevt_zz)
#    onz_cr_hist.SetBinContent(3, expected_nevt_zz)
#    onz_cr_hist.SetBinContent(4, expected_nevt_zz)
#    onz_cr_hist.SetBinContent(5, expected_nevt_zz)
#    alpha = hists[3].Clone("alpha") # The index 3 == zz (the order is data, sig, ttz, zz, ...)
#    alpha.Divide(onz_cr_hist)
#    systs.append( ("CRZZ{}".format(year), "gmN", [onz_cr_hist], {"emu{}_sig".format(year):0, "emu{}_ttz".format(year):0, "emu{}_zz".format(year):[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu{}_wz".format(year):0, "emu{}_twz".format(year):0, "emu{}_rare".format(year):0, "emu{}_dyttbar".format(year):0, "emu{}_higgs".format(year):0}) )

#    # ttZ CR systematic line
#    btag_cr_hist = r.TH1F("btag_cr", "", 5, 0, 5)
#    btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
#    btag_cr_hist.SetBinContent(2, expected_nevt_ttz)
#    btag_cr_hist.SetBinContent(3, expected_nevt_ttz)
#    btag_cr_hist.SetBinContent(4, expected_nevt_ttz)
#    btag_cr_hist.SetBinContent(5, expected_nevt_ttz)
#    alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
#    alpha.Divide(btag_cr_hist)
#    systs.append( ("CRTTZ{}".format(year), "gmN", [btag_cr_hist], {"emu{}_sig".format(year):0, "emu{}_ttz".format(year):[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu{}_zz".format(year):0, "emu{}_wz".format(year):0, "emu{}_twz".format(year):0, "emu{}_rare".format(year):0, "emu{}_dyttbar".format(year):0, "emu{}_higgs".format(year):0}) )

#    # Now create data card writer
#    d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["emu{}_zz".format(year), "emu{}_ttz".format(year)])

#    d.set_bin(1)
#    d.set_region_name("bin1")
#    d.write("stats/{}/emu_datacard_bin1.txt".format(prefix))

#    d.set_bin(2)
#    d.set_region_name("bin2")
#    d.write("stats/{}/emu_datacard_bin2.txt".format(prefix))

#    d.set_bin(3)
#    d.set_region_name("bin3")
#    d.write("stats/{}/emu_datacard_bin3.txt".format(prefix))

#    d.set_bin(4)
#    d.set_region_name("bin4")
#    d.write("stats/{}/emu_datacard_bin4.txt".format(prefix))

#    d.set_bin(5)
#    d.set_region_name("bin5")
#    d.write("stats/{}/emu_datacard_bin5.txt".format(prefix))

#    ##########################
#    # OffZ channel
#    ##########################

#    tfiles = []
#    procnames = []
#    hists = []

#    for index, s in enumerate(files):

#        # ROOT file
#        f = r.TFile(s)
#        tfiles.append(f)

#        # Name of the process
#        basename = os.path.basename(s)
#        procname = basename.replace(".root", "")

#        # List of procnames
#        procnames.append(procname)

#        # EMu channel
#        h = f.Get("ChannelOffZHighMET__Yield")

#        # If index == 0 then write data
#        if index == 0:
#            d = h.Clone("offz{}_data_obs".format(year))
#            d.Reset()
#            d.SetBinContent(1, 1)
#            hists.append(d)

#        # Aggregate histograms
#        hists.append(h.Clone("offz{}_".format(year) + procname))
#        hists[-1].SetTitle("offz{}_".format(year) + procname)

#        if index == 1: # this is ttZ process
#            hists[-1].Scale(ttz_sf)

#        if index == 2: # this is ZZ process
#            hists[-1].Scale(zz_sf)

#    systs = []

#    # If procname == zz
#    onz_cr_hist = r.TH1F("onz_cr", "", 1, 0, 1)
#    onz_cr_hist.SetBinContent(1, expected_nevt_zz)
#    alpha = hists[3].Clone("alpha") # The index 3 == zz (the order is data, sig, ttz, zz, ...)
#    alpha.Divide(onz_cr_hist)
#    systs.append( ("CRZZ{}".format(year), "gmN", [onz_cr_hist], {"offz{}_sig".format(year):0, "offz{}_ttz".format(year):0, "offz{}_zz".format(year):[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz{}_wz".format(year):0, "offz{}_twz".format(year):0, "offz{}_rare".format(year):0, "offz{}_dyttbar".format(year):0, "offz{}_higgs".format(year):0}) )

#    # ttZ CR systematic line
#    btag_cr_hist = r.TH1F("btag_cr", "", 1, 0, 1)
#    btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
#    alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
#    alpha.Divide(btag_cr_hist)
#    systs.append( ("CRTTZ{}".format(year), "gmN", [btag_cr_hist], {"offz{}_sig".format(year):0, "offz{}_ttz".format(year):[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz{}_zz".format(year):0, "offz{}_wz".format(year):0, "offz{}_twz".format(year):0, "offz{}_rare".format(year):0, "offz{}_dyttbar".format(year):0, "offz{}_higgs".format(year):0}) )

#    # Now create data card writer
#    d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["offz{}_zz".format(year), "offz{}_ttz".format(year)])

#    d.set_bin(1)
#    d.set_region_name("bin1")
#    d.write("stats/{}/offz_datacard_bin1.txt".format(prefix))

if __name__ == "__main__":

    main()

