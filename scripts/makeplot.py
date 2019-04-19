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
        ]
sigfiles = [
        "outputs/sig.root",
        "outputs/wwz.root",
        ]

allfiles = glob.glob("outputs/MC_*")

allfiles = [ f for f in allfiles if "ttbar" not in f ]
allfiles = [ f for f in allfiles if "dy_m" not in f ]
allfiles = [ f for f in allfiles if "ttz" not in f ]
allfiles = [ f for f in allfiles if "_zz_" not in f ]
allfiles = [ f for f in allfiles if "_wz_" not in f ]
print allfiles

colors = [2005, 2001, 2003, 2007, 920, 2010, 2012]

p.dump_plot(fnames=bkgfiles, sig_fnames=sigfiles, usercolors=colors, legend_labels=["ttz", "zz", "wz", "twz", "rare", "vh", "Z/t#bar{t}"], signal_labels=["VVV", "WWZ"], dirname="plots", extraoptions={"print_yield":True,"nbins":20, "signal_scale": "auto", "remove_underflow":False, "legend_scalex":1.4, "legend_scaley":1.1, "legend_ncolumns": 3, "lumi_value":137})
