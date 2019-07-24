#!/bin/env python

import plottery_wrapper as p

p.dump_plot(fnames=["4l2v.root"], data_fname="incl.root",
    dirname="plots/lin",
    # donorm=True,
    extraoptions={"nbins":20, "ratio_range":[0.,2.], "print_yield":True, "legend_datalabel":"incl", "lumi_value":1},
    )

p.dump_plot(fnames=["4l2v.root"], data_fname="incl.root",
    dirname="plots/log",
    # donorm=True,
    extraoptions={"nbins":20, "ratio_range":[0.,2.], "print_yield":True, "yield_prec":5, "yaxis_log":True, "legend_smart":False, "legend_datalabel":"incl", "lumi_value":1},
    )
