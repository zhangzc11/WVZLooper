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
##train a single BDT to discriminate signal with ttz backgrounds
##use emu events for training
##with ttz backgrounds included
##############################################

root.gROOT.SetBatch(True)

for channel in ["emu", "OffZ"]:

	test_name = channel+'_wwz_vs_ttz'

	plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
	pwd = os.getcwd()
	dataDir = pwd.replace("python", "data/All/compare/")

	os.system("mkdir -p "+plotDir)
	os.system("mkdir -p "+plotDir+"variables_compare")

	##Define variables to be used
	variables = ["eventweight","nb","nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore', "MllN", "lep3MT", "lep4MT", "lep34MT", "ZPt"]
	variables_names = ["eventweight","nb","nj", 'minDRJToL3','minDRJToL4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1Btag', 'jet2Btag', 'jet3Btag', 'jet4Btag', "Mll34", "lep3MT", "lep4MT", "lep34MT", "ZPt"]

	print(len(variables))

	##Getting ROOT files into pandas
	df_wwz_PassTTZBDTAndbVeto = uproot.open(dataDir+"wwz_"+channel+"_PassTTZBDTAndbVeto.root")['t'].pandas.df(variables, flatten=False)
	df_wwz_PassTTZBDTFailbVeto = uproot.open(dataDir+"wwz_"+channel+"_PassTTZBDTFailbVeto.root")['t'].pandas.df(variables, flatten=False)
	df_wwz_PassbVetoFailTTZBDT = uproot.open(dataDir+"wwz_"+channel+"_PassbVetoFailTTZBDT.root")['t'].pandas.df(variables, flatten=False)
	df_ttz_PassTTZBDTAndbVeto = uproot.open(dataDir+"ttz_"+channel+"_PassTTZBDTAndbVeto.root")['t'].pandas.df(variables, flatten=False)
	df_ttz_PassTTZBDTFailbVeto = uproot.open(dataDir+"ttz_"+channel+"_PassTTZBDTFailbVeto.root")['t'].pandas.df(variables, flatten=False)
	df_ttz_PassbVetoFailTTZBDT = uproot.open(dataDir+"ttz_"+channel+"_PassbVetoFailTTZBDT.root")['t'].pandas.df(variables, flatten=False)

	####Plot input variables######
	for idx in range(1, len(variables)):
		plt.figure()
		#y_wwz_PassTTZBDTAndbVeto, x_wwz_PassTTZBDTAndbVeto, _ = plt.hist(df_wwz_PassTTZBDTAndbVeto[df_wwz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[idx]], weights=df_wwz_PassTTZBDTAndbVeto[df_wwz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[0]], density=False, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - pass BDT and bVeto")
		y_wwz_PassTTZBDTAndbVeto, x_wwz_PassTTZBDTAndbVeto, _ = plt.hist(df_wwz_PassTTZBDTAndbVeto[df_wwz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - pass BDT and bVeto", color='b')
		y_wwz_PassTTZBDTFailbVeto, x_wwz_PassTTZBDTFailbVeto, _ = plt.hist(df_wwz_PassTTZBDTFailbVeto[df_wwz_PassTTZBDTFailbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - pass BDT fail bVeto", color='y')
		y_wwz_PassbVetoFailTTZBDT, x_wwz_PassbVetoFailTTZBDT, _ = plt.hist(df_wwz_PassbVetoFailTTZBDT[df_wwz_PassbVetoFailTTZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - pass bVeto fail BDT", color='g')

		y_ttz_PassTTZBDTAndbVeto, x_ttz_PassTTZBDTAndbVeto, _ = plt.hist(df_ttz_PassTTZBDTAndbVeto[df_ttz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass BDT and bVeto", color='r')
		y_ttz_PassTTZBDTFailbVeto, x_ttz_PassTTZBDTFailbVeto, _ = plt.hist(df_ttz_PassTTZBDTFailbVeto[df_ttz_PassTTZBDTFailbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass BDT fail bVeto", color='m')
		y_ttz_PassbVetoFailTTZBDT, x_ttz_PassbVetoFailTTZBDT, _ = plt.hist(df_ttz_PassbVetoFailTTZBDT[df_ttz_PassbVetoFailTTZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass bVeto fail BDT", color='c')

		y_max1 = max([max(y_wwz_PassTTZBDTAndbVeto), max(y_wwz_PassTTZBDTFailbVeto), max(y_wwz_PassbVetoFailTTZBDT), max(y_ttz_PassTTZBDTAndbVeto), max(y_ttz_PassTTZBDTFailbVeto), max(y_ttz_PassbVetoFailTTZBDT)])
		y_max2 = max([max(y_wwz_PassTTZBDTAndbVeto), max(y_ttz_PassTTZBDTAndbVeto), max(y_ttz_PassTTZBDTFailbVeto), max(y_ttz_PassbVetoFailTTZBDT)])

		plt.legend(loc="upper center", fontsize = 30)
		plt.xlabel(variables[idx], fontsize=50)
		plt.ylabel('Events', fontsize=30)
		plt.xticks(fontsize=35)
		plt.yticks(fontsize=30)
		if "MT" in variables[idx]:
			plt.xlim([0.0, 500.0])
		plt.ylim([0.0*y_max1, 2.0*y_max1])
		#plt.ylim([0.00001*y_max, 200.0*y_max])
		#plt.yscale("log")
		fig = plt.gcf()
		fig.set_size_inches(16, 12)
		plt.draw()
		plt.savefig(plotDir+'variables_compare/'+test_name + '_' + variables[idx]+'_all.png')
	
		plt.figure()
		y_wwz_PassTTZBDTAndbVeto, x_wwz_PassTTZBDTAndbVeto, _ = plt.hist(df_wwz_PassTTZBDTAndbVeto[df_wwz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wwz "+channel+" - pass BDT and bVeto", color='b')

		y_ttz_PassTTZBDTAndbVeto, x_ttz_PassTTZBDTAndbVeto, _ = plt.hist(df_ttz_PassTTZBDTAndbVeto[df_ttz_PassTTZBDTAndbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass BDT and bVeto", color='r')
		y_ttz_PassTTZBDTFailbVeto, x_ttz_PassTTZBDTFailbVeto, _ = plt.hist(df_ttz_PassTTZBDTFailbVeto[df_ttz_PassTTZBDTFailbVeto[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass BDT fail bVeto", color='m')
		y_ttz_PassbVetoFailTTZBDT, x_ttz_PassbVetoFailTTZBDT, _ = plt.hist(df_ttz_PassbVetoFailTTZBDT[df_ttz_PassbVetoFailTTZBDT[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz "+channel+" - pass bVeto fail BDT", color='c')

		y_max2 = max([max(y_wwz_PassTTZBDTAndbVeto), max(y_ttz_PassTTZBDTAndbVeto), max(y_ttz_PassTTZBDTFailbVeto), max(y_ttz_PassbVetoFailTTZBDT)])

		plt.legend(loc="upper center", fontsize = 30)
		plt.xlabel(variables[idx], fontsize=50)
		plt.ylabel('Events', fontsize=30)
		plt.xticks(fontsize=35)
		plt.yticks(fontsize=30)
		if "MT" in variables[idx]:
			plt.xlim([0.0, 500.0])
		plt.ylim([0.0*y_max2, 1.5*y_max2])
		fig = plt.gcf()
		fig.set_size_inches(16, 12)
		plt.draw()
		plt.savefig(plotDir+'variables_compare/'+test_name + '_' + variables[idx]+'_onewwz.png')
	


	os.system("chmod 755 "+plotDir+"variables_compare/*")

