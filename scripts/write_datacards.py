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
parser.add_argument('-s' , '--sample_set_name' , dest='sample_set_name' , help='Sample set name (e.g. WVZ2016_v0.0.9 or even the combined ones like WVZ2016_v0.0.9_WVZ2017_v0.0.9_WVZ2018_v0.0.9' , required=True)
parser.add_argument('-t' , '--tag'             , dest='tag'             , help='Tag of the looper output'                                                                                         , required=True)

args = parser.parse_args()

ntuple_version = args.sample_set_name
tag = args.tag

fname_sig     = "outputs/{}/{}/sig.root".format(ntuple_version, tag)
fname_ttz     = "outputs/{}/{}/ttz.root".format(ntuple_version, tag)
fname_zz      = "outputs/{}/{}/zz.root".format(ntuple_version, tag)
fname_wz      = "outputs/{}/{}/wz.root".format(ntuple_version, tag)
fname_twz     = "outputs/{}/{}/twz.root".format(ntuple_version, tag)
fname_rare    = "outputs/{}/{}/rare.root".format(ntuple_version, tag)
fname_dyttbar = "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag)
fname_higgs   = "outputs/{}/{}/higgs.root".format(ntuple_version, tag)
fname_data    = "outputs/{}/{}/data.root".format(ntuple_version, tag)

files = [fname_sig, fname_ttz, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
nonzzbkg = [fname_sig, fname_ttz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]
nonttzbkg = [fname_sig, fname_zz, fname_wz, fname_twz, fname_rare, fname_dyttbar, fname_higgs]

ofile = r.TFile("stat_input.root", "recreate")

##########################
# OnZ Control region gmN syst line
##########################

onz_zz_h = pr.get_summed_histogram([fname_zz], "ChannelOnZ__Yield")
onz_data_h = pr.get_summed_histogram([fname_data], "ChannelOnZ__Yield")
onz_nonzz_h = pr.get_summed_histogram(nonzzbkg, "ChannelOnZ__Yield")
zz_sf = pr.get_sf(onz_zz_h, onz_data_h, onz_nonzz_h).GetBinContent(1)
expected_nevt_zz = onz_data_h.GetBinContent(1)

##########################
# BTag Control region gmN syst line
##########################

bcr_ttz_h = pr.get_summed_histogram([fname_ttz], "ChannelBTagEMu__Yield")
bcr_data_h = pr.get_summed_histogram([fname_data], "ChannelBTagEMu__Yield")
bcr_nonttz_h = pr.get_summed_histogram(nonttzbkg, "ChannelBTagEMu__Yield")
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
        d = h.Clone("emu_data_obs")
        d.Reset()
        d.SetBinContent(1, 4)
        d.SetBinContent(2, 8)
        d.SetBinContent(3, 3)
        d.SetBinContent(4, 2)
        d.SetBinContent(5, 4)
        ofile.cd()
        d.Write()
        hists.append(d)

    if index == 2: # this is ttZ process
        hists[2].Scale(ttz_sf)

    if index == 3: # this is ZZ process
        hists[3].Scale(zz_sf)

    # Yields
    print procname, h.Integral()

    # Aggregate histograms
    hists.append(h.Clone("emu_" + procname))
    hists[-1].SetTitle("emu_" + procname)

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
systs.append( ("CRZZ", "gmN", [onz_cr_hist], {"emu_sig":0, "emu_ttz":0, "emu_zz":[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu_wz":0, "emu_twz":0, "emu_rare":0, "emu_dyttbar":0, "emu_higgs":0}) )

# ttZ CR systematic line
btag_cr_hist = r.TH1F("btag_cr", "", 5, 0, 5)
btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
btag_cr_hist.SetBinContent(2, expected_nevt_ttz)
btag_cr_hist.SetBinContent(3, expected_nevt_ttz)
btag_cr_hist.SetBinContent(4, expected_nevt_ttz)
btag_cr_hist.SetBinContent(5, expected_nevt_ttz)
alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
alpha.Divide(btag_cr_hist)
systs.append( ("CRTTZ", "gmN", [btag_cr_hist], {"emu_sig":0, "emu_ttz":[ "{:4f}".format(alpha.GetBinContent(i)) for i in range(1,6) ], "emu_zz":0, "emu_wz":0, "emu_twz":0, "emu_rare":0, "emu_dyttbar":0, "emu_higgs":0}) )

# Now create data card writer
d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["emu_zz", "emu_ttz"])

d.set_bin(1)
d.set_region_name("bin1")
d.write("stats/emu_datacard_bin1.txt")

d.set_bin(2)
d.set_region_name("bin2")
d.write("stats/emu_datacard_bin2.txt")

d.set_bin(3)
d.set_region_name("bin3")
d.write("stats/emu_datacard_bin3.txt")

d.set_bin(4)
d.set_region_name("bin4")
d.write("stats/emu_datacard_bin4.txt")

d.set_bin(5)
d.set_region_name("bin5")
d.write("stats/emu_datacard_bin5.txt")

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
        d = h.Clone("offz_data_obs")
        d.Reset()
        d.SetBinContent(1, 4)
        ofile.cd()
        d.Write()
        hists.append(d)

    if index == 2: # this is ttZ process
        hists[2].Scale(ttz_sf)

    if index == 3: # this is ZZ process
        hists[3].Scale(zz_sf)

    # Yields
    print procname, h.Integral()

    # Aggregate histograms
    hists.append(h.Clone("offz_" + procname))
    hists[-1].SetTitle("offz_" + procname)

    # Write to ofile
    ofile.cd()
    hists[-1].Write()

systs = []

# If procname == zz
onz_cr_hist = r.TH1F("onz_cr", "", 1, 0, 1)
onz_cr_hist.SetBinContent(1, expected_nevt_zz)
alpha = hists[3].Clone("alpha") # The index 3 == zz (the order is data, sig, ttz, zz, ...)
alpha.Divide(onz_cr_hist)
systs.append( ("CRZZ", "gmN", [onz_cr_hist], {"offz_sig":0, "offz_ttz":0, "offz_zz":[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz_wz":0, "offz_twz":0, "offz_rare":0, "offz_dyttbar":0, "offz_higgs":0}) )

# ttZ CR systematic line
btag_cr_hist = r.TH1F("btag_cr", "", 1, 0, 1)
btag_cr_hist.SetBinContent(1, expected_nevt_ttz)
alpha = hists[2].Clone("alpha") # The index 2 == ttz (the order is data, sig, ttz, zz, ...)
alpha.Divide(btag_cr_hist)
systs.append( ("CRTTZ", "gmN", [btag_cr_hist], {"offz_sig":0, "offz_ttz":[ "{:4f}".format(alpha.GetBinContent(1)) ], "offz_zz":0, "offz_wz":0, "offz_twz":0, "offz_rare":0, "offz_dyttbar":0, "offz_higgs":0}) )

# Now create data card writer
d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0], systs=systs, no_stat_procs=["offz_zz", "offz_ttz"])

d.set_bin(1)
d.set_region_name("bin1")
d.write("stats/offz_datacard_bin1.txt")

##########################
# OnZ channel
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
    h = f.Get("ChannelOnZ__Yield")

    # If index == 0 then write data
    if index == 0:
        d = h.Clone("onz_data_obs")
        d.Reset()
        d.SetBinContent(1, 4)
        ofile.cd()
        d.Write()
        hists.append(d)

    # Yields
    print procname, h.Integral()

    # Aggregate histograms
    hists.append(h.Clone("onz_" + procname))
    hists[-1].SetTitle("onz_" + procname)

    # Write to ofile
    ofile.cd()
    hists[-1].Write()

# Now create data card writer
d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0])

d.set_bin(1)
d.set_region_name("bin1")
d.write("stats/onz_datacard_bin1.txt")
