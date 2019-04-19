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
        "outputs/vh.root",
        "outputs/dyttbar.root",
        "outputs/MC_ttw_lnu_amcatnlo_1_results.root",
        ]
sigfiles = [
        "outputs/wwz.root",
        "outputs/wzz.root",
        "outputs/www.root",
        "outputs/zzz.root",
        # "outputs/sig.root",
        ]

allfiles = glob.glob("outputs/MC_*")
allfiles = [ f for f in allfiles if "ttbar" not in f ]
allfiles = [ f for f in allfiles if "dy_m" not in f ]
allfiles = [ f for f in allfiles if "ttz" not in f ]
allfiles = [ f for f in allfiles if "_zz_" not in f ]
allfiles = [ f for f in allfiles if "_wz_" not in f ]

colors = [2005, 2001, 2003, 2007, 920, 2010, 2012]

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles,
        usercolors=colors,
        legend_labels=["ttz", "zz", "wz", "twz", "rare", "vh", "Z/t#bar{t}"],
        signal_labels=["WWZ", "WZZ", "WWW", "ZZZ", "VVV"],
        dirname="plots/lin",
        filter_pattern="__",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.4,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "lumi_value":137
            }
        )

p.dump_plot(fnames=bkgfiles,
        sig_fnames=sigfiles,
        usercolors=colors,
        legend_labels=["ttz", "zz", "wz", "twz", "rare", "vh", "Z/t#bar{t}"],
        signal_labels=["WWZ", "WZZ", "WWW", "ZZZ", "VVV"],
        dirname="plots/cutflow",
        filter_pattern="_cutflow",
        dogrep=True,
        extraoptions={
            "print_yield":True,
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.4,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "lumi_value":137
            }
        )
