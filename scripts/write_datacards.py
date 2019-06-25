#!/bin/env python

import ROOT as r

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

# Make plot function of the main analysis
# The main thing is that this plotting script runs over the output files
# that was ran with "WVZ201*_v*" ntuples
# This means that it assumes specific histogram files to exists

ntuple_version = args.sample_set_name
tag = args.tag

files = [
        "outputs/{}/{}/sig.root".format(ntuple_version, tag),
        "outputs/{}/{}/ttz.root".format(ntuple_version, tag),
        "outputs/{}/{}/zz.root".format(ntuple_version, tag),
        "outputs/{}/{}/wz.root".format(ntuple_version, tag),
        "outputs/{}/{}/twz.root".format(ntuple_version, tag),
        "outputs/{}/{}/rare.root".format(ntuple_version, tag),
        "outputs/{}/{}/dyttbar.root".format(ntuple_version, tag),
        "outputs/{}/{}/higgs.root".format(ntuple_version, tag),
        ]

ofile = r.TFile("stat_input.root", "recreate")

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

    # Yields
    print procname, h.Integral()

    # Aggregate histograms
    hists.append(h.Clone("emu_" + procname))
    hists[-1].SetTitle("emu_" + procname)

    # Write to ofile
    ofile.cd()
    hists[-1].Write()

# Now create data card writer
# bkg2 does not need stat error as it is taken care of by CR stats
d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0])

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

    # Yields
    print procname, h.Integral()

    # Aggregate histograms
    hists.append(h.Clone("offz_" + procname))
    hists[-1].SetTitle("offz_" + procname)

    # Write to ofile
    ofile.cd()
    hists[-1].Write()

# Now create data card writer
# bkg2 does not need stat error as it is taken care of by CR stats
d = dw.DataCardWriter(sig=hists[1], bgs=hists[2:], data=hists[0])

d.set_bin(1)
d.set_region_name("bin1")
d.write("stats/offz_datacard_bin1.txt")
