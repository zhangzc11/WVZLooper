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
test_name = 'significance_vs_ttZBDT'

lumi_sf_sig = 1.0 # scale lumi from 2018 sample to full run2
lumi_sf_bkg = 1.0

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
pwd = os.getcwd()

os.system("mkdir -p "+plotDir)
os.system("mkdir -p "+plotDir+"cuts_scan")

yield_emu_all = [13.67, 1.44, 45.39, 7.29, 1.7, 1.62, 1.49]
yield_OffZHighMET_all = [3.74, 0.73, 15.0, 2.38, 0.48, 0.46, 0.26]


for channel in ["emu", "OffZHighMET"]:
	print("optimize channel: "+channel)
	dataDir = pwd.replace("python", "data/All/"+channel+"NobVetoHighZZBDT/")
	
	yield_all = np.array(yield_emu_all)
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
		tree_this.Draw("met_pt>>temp","eventweight")
		hist_this = root.gDirectory.Get('temp')
		#N_all[idx] = tree_this.GetEntries()
		N_all[idx] = hist_this.Integral()
	#print("N_all:")
	#print(N_all)
	BDT_cuts = np.arange(0.8,0.996, 0.001)
	L_cuts = np.zeros(len(BDT_cuts))
	S_cuts = np.zeros(len(BDT_cuts))
	B_cuts = np.zeros(len(BDT_cuts))
	for idx1 in range(len(BDT_cuts)):
			cut_this = "disc_ttz_nbAll > "+str(BDT_cuts[idx1])
			#print(cut_this)
			yield_this = np.zeros(len(yield_all))
			for idx in range(len(N_all)):
				#N_this = tree_all[idx].GetEntries(cut_this)
				tree_all[idx].Draw("met_pt>>temp2","eventweight * ("+cut_this+")")
				hist_this = root.gDirectory.Get('temp2')
				N_this = hist_this.Integral() 
				yield_this[idx] = yield_all[idx]*N_this*1.0/N_all[idx]
			s = yield_this[0]
			b = 0.0
			for idx in range(1, len(N_all)):
				b = b + yield_this[idx]
			L = math.sqrt(2.0*((s+b)*math.log(1+s/b) - s))
			print(str(idx1)+" "+str(BDT_cuts[idx1])+", "+str(s) +", "+str(b)+ ", "+ str(L))
			#print(yield_this)
			L_cuts[idx1] = L
			S_cuts[idx1] = s
			B_cuts[idx1] = b
	print(L_cuts)
	print("max L: "+str(np.amax(L_cuts)))
	best_cut = BDT_cuts[np.argmax(L_cuts)]
	significanceWP90 = L_cuts[np.where(abs(BDT_cuts - 0.960) < 0.000001) [0]] [0]

	#plt.figure()
	fig, (ax1, ax2) = plt.subplots(nrows=2, sharex=True)

	ax1.plot(BDT_cuts, S_cuts, color='b', lw=2, label="sig vs threshold")	
	ax1.plot(BDT_cuts, B_cuts, color='r', lw=2, label="bkg vs threshold")	
	ax1.xaxis.grid()
	ax1.yaxis.grid()
	ax1.set_ylabel("Events")
	ax1.legend()
	ax1.set_title(channel+" - ZZ BDT > 0.9333")

	ax2.plot(BDT_cuts, L_cuts, color='darkorange',
		 lw=2, label='significance vs threshold')
	ax2.axvline(x=0.960, color="black", linestyle='--', label="WP90: th = 0.960, L = %.2f"%(significanceWP90))
	ax2.axvline(x=best_cut, color="black", label="optimal cut: th = %.3f"%best_cut+",  L = %.2f"%(np.amax(L_cuts)))
	#plt.xlim([0.0, 1.0])
	#plt.ylim([0.0, 10.0])
	ax2.set_ylabel('significance')
	ax2.set_xlabel('ttZ BDT cut')
	ax2.legend(loc="upper left")
	#plt.show()
	plt.savefig(plotDir+'cuts_scan/' + test_name + '_'+channel+'.png')
	os.system("chmod 755 "+plotDir+"cuts_scan/*")

