#!/bin/env python

import plottery_wrapper as p
import ROOT as r

import glob
bkgfiles = [
        "outputs/ttz.root",
        "outputs/zz.root",
        "outputs/wz.root",
        "outputs/twz.root",
        "outputs/rare.root",
        "outputs/dyttbar.root",
        "outputs/higgs.root",
        ]
sigfiles = [
        "outputs/wwz.root",
        "outputs/wzz.root",
        #"outputs/www.root",
        "outputs/zzz.root",
        # "outputs/sig.root",
        ]
sigfiles_detail = [
        "outputs/nonh_wwz.root",
        "outputs/zh_wwz.root",
        "outputs/nonh_wzz.root",
        "outputs/wh_wzz.root",
        #"outputs/www.root",
        "outputs/nonh_zzz.root",
        "outputs/zh_zzz.root",
        # "outputs/sig.root",
        ]

#datafile = None
datafile = "outputs/data.root"

allfiles = glob.glob("outputs/MC_*")
allfiles = [ f for f in allfiles if "ttbar" not in f ]
allfiles = [ f for f in allfiles if "dy_m" not in f ]
allfiles = [ f for f in allfiles if "ttz" not in f ]
allfiles = [ f for f in allfiles if "_zz_" not in f ]
allfiles = [ f for f in allfiles if "_wz_" not in f ]

colors = [2005, 2001, 2003, 2007, 920, 2012, 2011]

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles,
	data_fname=datafile,
        usercolors=colors,
        legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
        signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
        dirname="plots/lin",
        filter_pattern="__",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "ymax_scale": 1.2,
            "lumi_value":58.90000
            }
        )

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles,
	data_fname=datafile,
        usercolors=colors,
        legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
        signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
        dirname="plots/lin5x",
        filter_pattern="__",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 5,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "ymax_scale": 1.2,
            "lumi_value":58.90000
            }
        )

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles,
	data_fname=datafile,
        usercolors=colors,
        legend_labels=["ttz", "zz", "wz", "twz", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
        signal_labels=["WWZ", "WZZ", "ZZZ", "VVV"],
        dirname="plots/cutflow",
        filter_pattern="_cutflow",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "ymax_scale": 1.2,
            "lumi_value":58.90000
            }
        )

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles_detail,
	data_fname=datafile,
        usercolors=colors,
        legend_labels=["ttz", "zz", "wz", "twz", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
        signal_labels=["WWZ", "ZH#rightarrowWWZ", "WZZ", "WH#rightarrowZZ", "ZZZ", "ZH#rightarrowZZ", "VVV"],
        dirname="plots/cutflow_detail",
        filter_pattern="_cutflow",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "ymax_scale": 1.2,
            "lumi_value":58.90000
            }
        )

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles_detail,
	data_fname=datafile,
        usercolors=colors,
        legend_labels=["t#bar{t}Z", "ZZ", "WZ", "tWZ", "Other", "Z/Z#gamma/t#bar{t}", "Higgs"],
        signal_labels=["WWZ", "ZH#rightarrowWWZ", "WZZ", "WH#rightarrowZZ", "ZZZ", "ZH#rightarrowZZ", "VVV"],
        dirname="plots/lin_detail",
        filter_pattern="__",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "ymax_scale": 1.2,
            "lumi_value":58.90000
            }
        )

