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

    write_datacards("{}2016_{}".format(args.ntuple_type, args.ntuple_version), "y2016_" + args.baseline_tag)
    write_datacards("{}2017_{}".format(args.ntuple_type, args.ntuple_version), "y2017_" + args.baseline_tag)
    write_datacards("{}2018_{}".format(args.ntuple_type, args.ntuple_version), "y2018_" + args.baseline_tag)

def write_datacards(ntuple_version, tag):

    # ntuple_version = args.sample_set_name
    # tag = args.tag

    fname_sig     = "outputs/{}/{}/sig.root".format(ntuple_version, tag)
    fname_ttz     = "outputs/{}/{}/ttz.root".format(ntuple_version, tag)
    fname_zz      = "outputs/{}/{}/zz.root".format(ntuple_version, tag)
    fname_wz      = "outputs/{}/{}/wz.root".format(ntuple_version, tag)
    fname_twz     = "outputs/{}/{}/twz.root".format(ntuple_version, tag)
    fname_rare    = "outputs/{}/{}/rare.root".format(ntuple_version, tag)
    fname_dyttbar = "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag)
    fname_higgs   = "outputs/{}/{}/higgs.root".format(ntuple_version, tag)
    fname_data    = "outputs/{}/{}/data.root".format(ntuple_version, tag)

    year = "2" + ntuple_version.split("_")[0].split("2")[1]
    prefix = "{}/{}".format(ntuple_version, tag)

    files = [fname_sig, fname_ttz, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonzzbkg = [fname_sig, fname_ttz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
    nonttzbkg = [fname_sig, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]

    ofile = r.TFile("stat_input.root", "recreate")

    ##########################
    # OnZ Control region gmN syst line
    ##########################

    onz_zz_h = pr.get_summed_histogram([fname_zz], "ChannelOnZCR__Yield")
    onz_data_h = pr.get_summed_histogram([fname_data], "ChannelOnZCR__Yield")
    onz_nonzz_h = pr.get_summed_histogram(nonzzbkg, "ChannelOnZCR__Yield")
    zz_sf = pr.get_sf(onz_zz_h, onz_data_h, onz_nonzz_h).GetBinContent(1)
    expected_nevt_zz = onz_data_h.GetBinContent(1)

    ##########################
    # BTag Control region gmN syst line
    ##########################

    bcr_ttz_h = pr.get_summed_histogram([fname_ttz], "ChannelBTagEMuCR__Yield")
    bcr_data_h = pr.get_summed_histogram([fname_data], "ChannelBTagEMuCR__Yield")
    bcr_nonttz_h = pr.get_summed_histogram(nonttzbkg, "ChannelBTagEMuCR__Yield")
    ttz_sf = pr.get_sf(bcr_ttz_h, bcr_data_h, bcr_nonttz_h).GetBinContent(1)
    expected_nevt_ttz = bcr_data_h.GetBinContent(1)

    ##########################
    # EMu channel
    ##########################

    tfiles = []
    procnames = []
    hists = []

    for index, s in enumerate(files):

        # ROOT file
        f = r.TFile(s)
        tfiles.append(f)

        # Name of the process
        basename = os.path.basename(s)
        procname = basename.replace(".root", "")

        # List of procnames
        procnames.append(procname)

        # EMu channel
        h = f.Get("ChannelEMuHighMT__MllNom")

        # Bin it to 5 bin
        h.Rebin(36)
        bin5 = E(h.GetBinContent(5), h.GetBinError(5))
        bin6 = E(h.GetBinContent(6), h.GetBinError(6))
        bin5plus6 = bin5 + bin6
        h.SetBinContent(6, 0)
        h.SetBinError(6, 0)
        h.SetBinContent(5, bin5plus6.val)
        h.SetBinError(5, bin5plus6.err)

        # If index == 0 then write data
        if index == 0:
            d = h.Clone("emu{}_data_obs".format(year))
            d.Reset()
            d.SetBinContent(1, 1)
            d.SetBinContent(2, 1)
            d.SetBinContent(3, 1)
            d.SetBinContent(4, 1)
            d.SetBinContent(5, 1)
            ofile.cd()
            d.Write()
            hists.append(d)

        # Aggregate histograms
        hists.append(h.Clone("emu{}_".format(year) + procname))
        hists[-1].SetTitle("emu{}_".format(year) + procname)

        if index == 1: # this is ttZ process
            hists[-1].Scale(ttz_sf)
            print year, "ttz_sf", ttz_sf

        if index == 2: # this is ZZ process
            hists[-1].Scale(zz_sf)
            print year, "zz_sf", zz_sf

        # Write to ofile
        ofile.cd()
        hists[-1].Write()

    systs = []

    # ZZ CR systematic line
    onz_cr_hist = r.TH1F("onz_cr", "", 5, 0, 5)
    onz_cr_hist.SetBinContent(1, expected_nevt_zz)
    onz_cr_hist.SetBinContent(2, expected_nevt_zz)
    onz_cr_hist.SetBinContent(3, expected_nevt_zz)
    onz_cr_hist.SetBinContent(4, expected_nevt_zz)
    onz_cr_hist.SetBinContent(5, expected_nevt_zz)
    alpha = hists[3].Clone("alpha") # The index 3 == zz (the order is data, sig, ttz, zz, ...)
    alpha.Divide(onz_cr_hist)
    systs.append( ("CRZZ{}".format(year), "gmN", [onz_cr_hist], {"emu{}_sig".format(year):0, "emu{}_ttz".format(year):0, "emu{}_zz".format(year):[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu{}_wz".format(year):0, "emu{}_twz".format(year):0, "emu{}_rare".format(year):0, "emu{}_dyttbar".format(year):0, "emu{}_higgs".format(year):0}) )

    # ttZ CR systematic line
    btag_cr_hist = r.TH1F("btag_cr", "", 5, 0, 5)
    btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
    btag_cr_hist.SetBinContent(2, expected_nevt_ttz)
    btag_cr_hist.SetBinContent(3, expected_nevt_ttz)
    btag_cr_hist.SetBinContent(4, expected_nevt_ttz)
    btag_cr_hist.SetBinContent(5, expected_nevt_ttz)
    alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
    alpha.Divide(btag_cr_hist)
    systs.append( ("CRTTZ{}".format(year), "gmN", [btag_cr_hist], {"emu{}_sig".format(year):0, "emu{}_ttz".format(year):[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu{}_zz".format(year):0, "emu{}_wz".format(year):0, "emu{}_twz".format(year):0, "emu{}_rare".format(year):0, "emu{}_dyttbar".format(year):0, "emu{}_higgs".format(year):0}) )

    # Now create data card writer
    d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["emu{}_zz".format(year), "emu{}_ttz".format(year)])

    d.set_bin(1)
    d.set_region_name("bin1")
    d.write("stats/{}/emu_datacard_bin1.txt".format(prefix))

    d.set_bin(2)
    d.set_region_name("bin2")
    d.write("stats/{}/emu_datacard_bin2.txt".format(prefix))

    d.set_bin(3)
    d.set_region_name("bin3")
    d.write("stats/{}/emu_datacard_bin3.txt".format(prefix))

    d.set_bin(4)
    d.set_region_name("bin4")
    d.write("stats/{}/emu_datacard_bin4.txt".format(prefix))

    d.set_bin(5)
    d.set_region_name("bin5")
    d.write("stats/{}/emu_datacard_bin5.txt".format(prefix))

    ##########################
    # OffZ channel
    ##########################

    tfiles = []
    procnames = []
    hists = []

    for index, s in enumerate(files):

        # ROOT file
        f = r.TFile(s)
        tfiles.append(f)

        # Name of the process
        basename = os.path.basename(s)
        procname = basename.replace(".root", "")

        # List of procnames
        procnames.append(procname)

        # EMu channel
        h = f.Get("ChannelOffZHighMET__Yield")

        # If index == 0 then write data
        if index == 0:
            d = h.Clone("offz{}_data_obs".format(year))
            d.Reset()
            d.SetBinContent(1, 1)
            ofile.cd()
            d.Write()
            hists.append(d)

        # Aggregate histograms
        hists.append(h.Clone("offz{}_".format(year) + procname))
        hists[-1].SetTitle("offz{}_".format(year) + procname)

        if index == 1: # this is ttZ process
            hists[-1].Scale(ttz_sf)

        if index == 2: # this is ZZ process
            hists[-1].Scale(zz_sf)

        # Write to ofile
        ofile.cd()
        hists[-1].Write()

    systs = []

    # If procname == zz
    onz_cr_hist = r.TH1F("onz_cr", "", 1, 0, 1)
    onz_cr_hist.SetBinContent(1, expected_nevt_zz)
    alpha = hists[3].Clone("alpha") # The index 3 == zz (the order is data, sig, ttz, zz, ...)
    alpha.Divide(onz_cr_hist)
    systs.append( ("CRZZ{}".format(year), "gmN", [onz_cr_hist], {"offz{}_sig".format(year):0, "offz{}_ttz".format(year):0, "offz{}_zz".format(year):[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz{}_wz".format(year):0, "offz{}_twz".format(year):0, "offz{}_rare".format(year):0, "offz{}_dyttbar".format(year):0, "offz{}_higgs".format(year):0}) )

    # ttZ CR systematic line
    btag_cr_hist = r.TH1F("btag_cr", "", 1, 0, 1)
    btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
    alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
    alpha.Divide(btag_cr_hist)
    systs.append( ("CRTTZ{}".format(year), "gmN", [btag_cr_hist], {"offz{}_sig".format(year):0, "offz{}_ttz".format(year):[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz{}_zz".format(year):0, "offz{}_wz".format(year):0, "offz{}_twz".format(year):0, "offz{}_rare".format(year):0, "offz{}_dyttbar".format(year):0, "offz{}_higgs".format(year):0}) )

    # Now create data card writer
    d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["offz{}_zz".format(year), "offz{}_ttz".format(year)])

    d.set_bin(1)
    d.set_region_name("bin1")
    d.write("stats/{}/offz_datacard_bin1.txt".format(prefix))

if __name__ == "__main__":

    main()

###########################
## OnZ channel
###########################

#tfiles = []
#procnames = []
#hists = []

#for index, s in enumerate(files):

#    # ROOT file
#    f = r.TFile(s)
#    tfiles.append(f)

#    # Name of the process
#    basename = os.path.basename(s)
#    procname = basename.replace(".root", "")

#    # List of procnames
#    procnames.append(procname)

#    # EMu channel
#    h = f.Get("ChannelOnZ__Yield")

#    # If index == 0 then write data
#    if index == 0:
#        d = h.Clone("onz_data_obs")
#        d.Reset()
#        d.SetBinContent(1, 4)
#        ofile.cd()
#        d.Write()
#        hists.append(d)

#    # Yields
#    print procname, h.Integral()

#    # Aggregate histograms
#    hists.append(h.Clone("onz_" + procname))
#    hists[-1].SetTitle("onz_" + procname)

#    # Write to ofile
#    ofile.cd()
#    hists[-1].Write()

## Now create data card writer
#d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0])

#d.set_bin(1)
#d.set_region_name("bin1")
#d.write("stats/{}/onz_datacard_bin1.txt".format(prefix))
