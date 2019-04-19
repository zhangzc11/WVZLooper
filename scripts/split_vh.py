#!/bin/env python

import ROOT as r

f = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/vh_nonbb_amcatnlo_1.root")

t = f.Get("t")

f_www = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/wh_ww_amcatnlo_1.root", "recreate")
t_www = t.CopyTree("VHchannel==24&&Higgschannel==24")
t_www.Write()

f_zww = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/zh_ww_amcatnlo_1.root", "recreate")
t_zww = t.CopyTree("VHchannel==23&&Higgschannel==24")
t_zww.Write()

f_wzz = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/wh_zz_amcatnlo_1.root", "recreate")
t_wzz = t.CopyTree("VHchannel==24&&Higgschannel==23")
t_wzz.Write()

f_zzz = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/zh_zz_amcatnlo_1.root", "recreate")
t_zzz = t.CopyTree("VHchannel==23&&Higgschannel==23")
t_zzz.Write()

f_rest = r.TFile("/nfs-7/userdata/phchang/babies/WVZ2018_v0.0.5/vh_nonbbwwzz_amcatnlo_1.root", "recreate")
t_rest = t.CopyTree("Higgschannel!=23&&Higgschannel!=24")
t_rest.Write()

