#!/usr/bin/env python
# coding: utf-8
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
import pickle as pickle
import ROOT as root
import os
import shlex
import uproot

####################option B:##################
##train two BDTs/: ZZ and ttZ
##use emu events for ZZ BDT training
##use events without bVeto for ttZ BDT training
##############################################

root.gROOT.SetBatch(True)
test_name = 'significance_vs_minDR'

lumi_sf_sig = 1.0 # scale lumi from 2018 sample to full run2
lumi_sf_bkg = 1.0

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
pwd = os.getcwd()

os.system("mkdir -p "+plotDir)
os.system("mkdir -p "+plotDir+"cuts_scan")

yield_emuHighMT_all = [10.27, 3.2, 2.67, 1.19, 1.14, 0.8, 0.5]
yield_OffZHighMET_all = [4.95, 4.29, 1.58, 0.58, 0.5, 0.47, 0.0]


for channel in ["emuHighMT", "OffZHighMET"]:
	print("optimize channel: "+channel)
	dataDir = pwd.replace("python", "data/All/"+channel+"/")
	
	yield_all = np.array(yield_emuHighMT_all)
	if "OffZHighMET" in channel:
		yield_all = np.array(yield_OffZHighMET_all)

	N_all = np.zeros(len(yield_all)) 
	fileName_all = ["wwz", "zz", "ttz", "twz", "wz", "higgs", "othernoh"]
	tree_all = {}
	for idx in range(len(N_all)):
		tree_all[idx] = root.TChain("t")
		tree_all[idx].AddFile(dataDir + fileName_all[idx]+".root")
		root.SetOwnership(tree_all[idx], True)
		file_this = root.TFile(dataDir + fileName_all[idx]+".root")
		tree_this = file_this.Get('t')
		#tree_all.append(tree_this)
		N_all[idx] = tree_this.GetEntries()
	#print("N_all:")
	#print(N_all)
	njets_cuts = np.arange(1,10)
	L_cuts = np.zeros(len(njets_cuts))
	for idx1 in range(len(njets_cuts)):
			cut_this = "nj<"+str(njets_cuts[idx1])
			#print(cut_this)
			yield_this = np.zeros(len(yield_all))
			for idx in range(len(N_all)):
				N_this = tree_all[idx].GetEntries(cut_this)
				yield_this[idx] = yield_all[idx]*N_this*1.0/N_all[idx]
			s = yield_this[0]
			b = 0.0
			for idx in range(1, len(N_all)):
				b = b + yield_this[idx]
			L = math.sqrt(2.0*((s+b)*math.log(1+s/b) - s))
			print(str(njets_cuts[idx1])+", "+str(s) +", "+str(b)+ ", "+ str(L))
			L_cuts[idx1] = L
	print(L_cuts)
	print("max L: "+str(np.amax(L_cuts)))
