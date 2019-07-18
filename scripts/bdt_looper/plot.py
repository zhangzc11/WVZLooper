#!/bin/env python

import plottery_wrapper as p
import argparse

# bkgfnames = ["outputs/zz.root", "outputs/ttz.root", "outputs/twz.root", "outputs/wz.root", "outputs/other.root"]
# sigfnames = ["outputs/sig.root"]
bkgfnames = ["outputs/zz_skim.root", "outputs/ttz_skim.root", "outputs/twz_skim.root", "outputs/wz_skim.root", "outputs/other_skim.root"]
sigfnames = ["outputs/sig_skim.root"]
colors = [2001, 2005, 2007, 2003, 920]
bkgfnames.reverse()
colors.reverse()

p.dump_plot(
        fnames=bkgfnames,
        sig_fnames=sigfnames,
        dirname="plots/",
        usercolors=colors,
        signal_scale=1,

        # If one wants to plot just a few
        # filter_pattern="HighBDT__lep3MT,HighBDT__lep4MT",
        # dogrep=True,
        filter_pattern="Weight__BDTCombine",
        dogrep=True,

        extraoptions={
            "nbins":5,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "lumi_value": 137,
            "xaxis_ndivisions": 505,
            "bkg_sort_method":"unsorted",
            "print_yield":True,
            },
        # _plotter=p.plot_cut_scan,
        )
