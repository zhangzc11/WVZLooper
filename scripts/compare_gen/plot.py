#!/bin/env python

import plottery_wrapper as p

p.dump_plot(fnames=["incl.root"], data_fname="4l2v.root",
    dirname="plots/lin",
    # donorm=True,
    extraoptions={"nbins":20, "ratio_range":[0.,2.], "print_yield":True},
    )

p.dump_plot(fnames=["incl.root"], data_fname="4l2v.root",
    dirname="plots/log",
    # donorm=True,
    extraoptions={"nbins":20, "ratio_range":[0.,2.], "print_yield":True, "yaxis_log":True, "legend_smart":False},
    )
