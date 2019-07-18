#!/bin/env python

import plottery_wrapper as p
import argparse

bkgfnames = ["outputs/zz.root", "outputs/ttz.root", "outputs/twz.root", "outputs/wz.root", "outputs/other.root"]
sigfnames = ["outputs/sig.root"]
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

        extraoptions={
            "nbins":15,
            "signal_scale": 1,
            "legend_scalex":1.8,
            "legend_scaley":1.1,
            "legend_ncolumns": 3,
            "lumi_value": 137,
            "xaxis_ndivisions": 505,
            "bkg_sort_method":"unsorted",
            },
        # _plotter=p.plot_cut_scan,
        )
