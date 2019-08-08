#!/usr/bin/env python
# coding: utf-8
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
import ROOT as root
import os
import shlex
import uproot

####################option A:##################
##train a single BDT to discriminate signal with zz backgrounds
##use emu events for training
##with zz backgrounds included
##############################################

root.gROOT.SetBatch(True)

#for channel in ["emu", "OffZ"]:
for channel in ["emu"]:

	test_name = channel+'_wwz_vs_zz'

	plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
	pwd = os.getcwd()
	dataDir = pwd.replace("python", "data/All/"+channel+"HighTTZBDT/")

	os.system("mkdir -p "+plotDir)
	os.system("mkdir -p "+plotDir+"variables_compare")

	##Define variables to be used
	variables = ['eventweight', 'met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis']
	variables_names = ['eventweight', 'met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','Mll34', 'pt_zeta', 'pt_zeta_vis']

	print(len(variables))

	##Getting ROOT files into pandas
	df_wwz_HighZZBDT = uproot.open(dataDir+"wwz_HighZZBDT.root")['t'].pandas.df(variables, flatten=False)
	df_wwz_LowZZBDT = uproot.open(dataDir+"wwz_LowZZBDT.root")['t'].pandas.df(variables, flatten=False)
	df_zz_HighZZBDT = uproot.open(dataDir+"zz_HighZZBDT.root")['t'].pandas.df(variables, flatten=False)
	df_zz_LowZZBDT = uproot.open(dataDir+"zz_LowZZBDT.root")['t'].pandas.df(variables, flatten=False)

	####Plot input variables######
	for idx in range(1, len(variables)):
		plt.figure()
		y_wwz_HighZZBDT, x_wwz_HighZZBDT, _ = plt.hist(df_wwz_HighZZBDT[df_wwz_HighZZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - ZZ BDT > 0.9", color='b')
		y_wwz_LowZZBDT, x_wwz_LowZZBDT, _ = plt.hist(df_wwz_LowZZBDT[df_wwz_LowZZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - ZZ BDT < 0.9", color='y')

		y_zz_HighZZBDT, x_zz_HighZZBDT, _ = plt.hist(df_zz_HighZZBDT[df_zz_HighZZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="zz "+channel+" - ZZ BDT > 0.9", color='g')
		y_zz_LowZZBDT, x_zz_LowZZBDT, _ = plt.hist(df_zz_LowZZBDT[df_zz_LowZZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="zz "+channel+" - ZZ BDT < 0.9", color='r')

		y_max1 = max([max(y_wwz_HighZZBDT), max(y_wwz_LowZZBDT), max(y_zz_HighZZBDT), max(y_zz_LowZZBDT)])
		y_max2 = max([max(y_zz_HighZZBDT), max(y_zz_LowZZBDT)])

		plt.legend(loc="upper center", fontsize = 30)
		plt.xlabel(variables[idx], fontsize=50)
		plt.ylabel('Events', fontsize=30)
		plt.xticks(fontsize=35)
		plt.yticks(fontsize=30)
		if "MT" in variables[idx]:
			plt.xlim([0.0, 500.0])
		if "Pt" in variables[idx]:
			plt.xlim([0.0, 500.0])
		if "met_pt" in variables[idx]:
			plt.xlim([0.0, 500.0])
		if "MllN" in variables[idx]:
			plt.xlim([0.0, 500.0])
		if "pt_zeta" in variables[idx]:
			plt.xlim([-100.0, 500.0])
		if "dZ" in variables[idx]:
			plt.xlim([-0.05, 0.05])
		plt.ylim([0.0*y_max1, 2.0*y_max1])
		#plt.ylim([0.00001*y_max, 200.0*y_max])
		#plt.yscale("log")
		fig = plt.gcf()
		fig.set_size_inches(16, 12)
		plt.draw()
		plt.savefig(plotDir+'variables_compare/'+test_name + '_' + variables[idx]+'.png')
	
		plt.figure()


	os.system("chmod 755 "+plotDir+"variables_compare/*")

