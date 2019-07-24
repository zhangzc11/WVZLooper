#!/usr/bin/env python

import matplotlib.pyplot as plt
import xgboost as xgb
import numpy as np
import pandas as pd
import math
import pickle as pickle
import ROOT as root
import uproot

import sys, os

#################
##Preliminaries
#################

root.gROOT.Reset()

if __name__ == "__main__":
	#plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
	plotDir = "../plots/"
	os.system("mkdir -p "+plotDir)
	
	fileDir = sys.argv[1]
	sample = sys.argv[2]
	filterBDTvariables = "yes"
	mkplot = "yes"
	if len(sys.argv) > 3:
		mkplot = sys.argv[3]
	if len(sys.argv) > 4:
		filterBDTvariables = sys.argv[4]

	FileName = fileDir + "/" + sample
	os.system("mkdir -p "+fileDir+".BDT")

	File = root.TFile(FileName)
	Tree = File.Get('t')
	if Tree.GetEntries() < 1:
		os.system("cp "+FileName+" "+fileDir+".BDT/"+sample)	
		sys.exit(0)

	Nevents = File.Get('h_neventsinfile')

	##Define variables to be used
	variables_ttzzz_bVeto = ['met_pt','lep3Id','lep4Id','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_ttzzz_emu = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_ttzzz_OffZ = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_zz_emu = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis']
	variables_zz_emuHighTTZBDT = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis']
	variables_zz_OffZ = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis']
	variables_zz_OffZHighTTZBDT = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis']
	variables_ttz_nbAll = ["nb","nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore', "MllN", "lep3MT", "lep4MT", "lep34MT", "ZPt"]
	variables_ttz_bVeto = ["nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore', "MllN", "lep3MT", "lep4MT", "lep34MT", "ZPt"]
	variables_ttz_emu = ["nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore', "MllN", "lep3MT", "lep4MT", "lep34MT", "ZPt"]
	variables_ttz_OffZ = ["nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore', "MllN", "lep3MT", "lep4MT", "lep34MT", "ZPt"]
	variables_multi_nbAll = ['met_pt','lep3Id','lep4Id','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", "nb", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_multi_emuHighTTZBDT = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_multi_emu = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_multi_OffZHighTTZBDT = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
	variables_multi_OffZ = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']

	##Getting ROOT files into pandas
	df_ttzzz_bVeto = uproot.open(FileName)['t'].pandas.df(variables_ttzzz_bVeto, flatten=False)
	df_ttzzz_emu = uproot.open(FileName)['t'].pandas.df(variables_ttzzz_emu, flatten=False)
	df_ttzzz_OffZ = uproot.open(FileName)['t'].pandas.df(variables_ttzzz_OffZ, flatten=False)
	df_zz_emu = uproot.open(FileName)['t'].pandas.df(variables_zz_emu, flatten=False)
	df_zz_emuHighTTZBDT = uproot.open(FileName)['t'].pandas.df(variables_zz_emuHighTTZBDT, flatten=False)
	df_zz_OffZ = uproot.open(FileName)['t'].pandas.df(variables_zz_OffZ, flatten=False)
	df_zz_OffZHighTTZBDT = uproot.open(FileName)['t'].pandas.df(variables_zz_OffZHighTTZBDT, flatten=False)
	df_ttz_nbAll = uproot.open(FileName)['t'].pandas.df(variables_ttz_nbAll, flatten=False)
	df_ttz_bVeto = uproot.open(FileName)['t'].pandas.df(variables_ttz_bVeto, flatten=False)
	df_ttz_emu = uproot.open(FileName)['t'].pandas.df(variables_ttz_emu, flatten=False)
	df_ttz_OffZ = uproot.open(FileName)['t'].pandas.df(variables_ttz_OffZ, flatten=False)
	df_multi_nbAll = uproot.open(FileName)['t'].pandas.df(variables_multi_nbAll, flatten=False)
	df_multi_emuHighTTZBDT = uproot.open(FileName)['t'].pandas.df(variables_multi_emuHighTTZBDT, flatten=False)
	df_multi_emu = uproot.open(FileName)['t'].pandas.df(variables_multi_emu, flatten=False)
	df_multi_OffZHighTTZBDT = uproot.open(FileName)['t'].pandas.df(variables_multi_OffZHighTTZBDT, flatten=False)
	df_multi_OffZ = uproot.open(FileName)['t'].pandas.df(variables_multi_OffZ, flatten=False)


	x_test_ttzzz_bVeto = df_ttzzz_bVeto.values
	y_test_ttzzz_bVeto = np.zeros(len(df_ttzzz_bVeto))

	x_test_ttzzz_emu = df_ttzzz_emu.values
	y_test_ttzzz_emu = np.zeros(len(df_ttzzz_emu))

	x_test_ttzzz_OffZ = df_ttzzz_OffZ.values
	y_test_ttzzz_OffZ = np.zeros(len(df_ttzzz_OffZ))

	x_test_zz_emu = df_zz_emu.values
	y_test_zz_emu = np.zeros(len(df_zz_emu))

	x_test_zz_emuHighTTZBDT = df_zz_emuHighTTZBDT.values
	y_test_zz_emuHighTTZBDT = np.zeros(len(df_zz_emuHighTTZBDT))

	x_test_zz_OffZ = df_zz_OffZ.values
	y_test_zz_OffZ = np.zeros(len(df_zz_OffZ))

	x_test_zz_OffZHighTTZBDT = df_zz_OffZHighTTZBDT.values
	y_test_zz_OffZHighTTZBDT = np.zeros(len(df_zz_OffZHighTTZBDT))

	x_test_ttz_nbAll = df_ttz_nbAll.values
	y_test_ttz_nbAll = np.zeros(len(df_ttz_nbAll))

	x_test_ttz_bVeto = df_ttz_bVeto.values
	y_test_ttz_bVeto = np.zeros(len(df_ttz_bVeto))

	x_test_ttz_emu = df_ttz_emu.values
	y_test_ttz_emu = np.zeros(len(df_ttz_emu))

	x_test_ttz_OffZ = df_ttz_OffZ.values
	y_test_ttz_OffZ = np.zeros(len(df_ttz_OffZ))

	x_test_multi_nbAll = df_multi_nbAll.values
	y_test_multi_nbAll = np.zeros(len(df_multi_nbAll))

	x_test_multi_emuHighTTZBDT = df_multi_emuHighTTZBDT.values
	y_test_multi_emuHighTTZBDT = np.zeros(len(df_multi_emuHighTTZBDT))

	x_test_multi_emu = df_multi_emu.values
	y_test_multi_emu = np.zeros(len(df_multi_emu))

	x_test_multi_OffZHighTTZBDT = df_multi_OffZHighTTZBDT.values
	y_test_multi_OffZHighTTZBDT = np.zeros(len(df_multi_OffZHighTTZBDT))

	x_test_multi_OffZ = df_multi_OffZ.values
	y_test_multi_OffZ = np.zeros(len(df_multi_OffZ))
	############################
	# get model from file
	############################
	model_ttzzz_bVeto = pickle.load(open('../models/model_xgb_wwz_vs_ttzzz_bVeto.pkl','rb'))
	model_ttzzz_emu = pickle.load(open('../models/model_xgb_wwz_vs_ttzzz_emu.pkl','rb'))
	model_ttzzz_OffZ = pickle.load(open('../models/model_xgb_wwz_vs_ttzzz_OffZ.pkl','rb'))
	model_zz_emu = pickle.load(open('../models/model_xgb_wwz_vs_zz_emu.pkl','rb'))
	model_zz_emuHighTTZBDT = pickle.load(open('../models/model_xgb_wwz_vs_zz_emuHighTTZBDT.pkl','rb'))
	model_zz_OffZ = pickle.load(open('../models/model_xgb_wwz_vs_zz_OffZ.pkl','rb'))
	model_zz_OffZHighTTZBDT = pickle.load(open('../models/model_xgb_wwz_vs_zz_OffZHighTTZBDT.pkl','rb'))
	model_ttz_nbAll = pickle.load(open('../models/model_xgb_wwz_vs_ttz_nbAll.pkl','rb'))
	model_ttz_bVeto = pickle.load(open('../models/model_xgb_wwz_vs_ttz_bVeto.pkl','rb'))
	model_ttz_emu = pickle.load(open('../models/model_xgb_wwz_vs_ttz_emu.pkl','rb'))
	model_ttz_OffZ = pickle.load(open('../models/model_xgb_wwz_vs_ttz_OffZ.pkl','rb'))
	model_multi_nbAll = pickle.load(open('../models/model_xgb_wwz_vs_multi_nbAll.pkl','rb'))
	model_multi_emuHighTTZBDT = pickle.load(open('../models/model_xgb_wwz_vs_multi_emuHighTTZBDT.pkl','rb'))
	model_multi_emu = pickle.load(open('../models/model_xgb_wwz_vs_multi_emu.pkl','rb'))
	model_multi_OffZHighTTZBDT = pickle.load(open('../models/model_xgb_wwz_vs_multi_OffZHighTTZBDT.pkl','rb'))
	model_multi_OffZ = pickle.load(open('../models/model_xgb_wwz_vs_multi_OffZ.pkl','rb'))
	
	# make predictions for test data
	y_pred_ttzzz_bVeto = model_ttzzz_bVeto.predict_proba(x_test_ttzzz_bVeto)[:, 1]
	y_pred_ttzzz_emu = model_ttzzz_emu.predict_proba(x_test_ttzzz_emu)[:, 1]
	y_pred_ttzzz_OffZ = model_ttzzz_OffZ.predict_proba(x_test_ttzzz_OffZ)[:, 1]
	y_pred_zz_emu = model_zz_emu.predict_proba(x_test_zz_emu)[:, 1]
	y_pred_zz_emuHighTTZBDT = model_zz_emuHighTTZBDT.predict_proba(x_test_zz_emuHighTTZBDT)[:, 1]
	y_pred_zz_OffZ = model_zz_OffZ.predict_proba(x_test_zz_OffZ)[:, 1]
	y_pred_zz_OffZHighTTZBDT = model_zz_OffZHighTTZBDT.predict_proba(x_test_zz_OffZHighTTZBDT)[:, 1]
	y_pred_ttz_nbAll = model_ttz_nbAll.predict_proba(x_test_ttz_nbAll)[:, 1]
	y_pred_ttz_bVeto = model_ttz_bVeto.predict_proba(x_test_ttz_bVeto)[:, 1]
	y_pred_ttz_emu = model_ttz_emu.predict_proba(x_test_ttz_emu)[:, 1]
	y_pred_ttz_OffZ = model_ttz_OffZ.predict_proba(x_test_ttz_OffZ)[:, 1]
	
	d_test_multi_nbAll = xgb.DMatrix(x_test_multi_nbAll, label=y_test_multi_nbAll)
	y_pred_multi_nbAll = model_multi_nbAll.predict(d_test_multi_nbAll)
	
	d_test_multi_emuHighTTZBDT = xgb.DMatrix(x_test_multi_emuHighTTZBDT, label=y_test_multi_emuHighTTZBDT)
	y_pred_multi_emuHighTTZBDT = model_multi_emuHighTTZBDT.predict(d_test_multi_emuHighTTZBDT)

	d_test_multi_emu = xgb.DMatrix(x_test_multi_emu, label=y_test_multi_emu)
	y_pred_multi_emu = model_multi_emu.predict(d_test_multi_emu)

	d_test_multi_OffZHighTTZBDT = xgb.DMatrix(x_test_multi_OffZHighTTZBDT, label=y_test_multi_OffZHighTTZBDT)
	y_pred_multi_OffZHighTTZBDT = model_multi_OffZHighTTZBDT.predict(d_test_multi_OffZHighTTZBDT)

	d_test_multi_OffZ = xgb.DMatrix(x_test_multi_OffZ, label=y_test_multi_OffZ)
	y_pred_multi_OffZ = model_multi_OffZ.predict(d_test_multi_OffZ)
	#print y_pred
	##########################################################
	# make histogram of discriminator value for signal and bkg
	##########################################################
	y_frame_ttzzz_bVeto = pd.DataFrame({'truth':y_test_ttzzz_bVeto, 'disc':y_pred_ttzzz_bVeto})
	y_frame_ttzzz_emu = pd.DataFrame({'truth':y_test_ttzzz_emu, 'disc':y_pred_ttzzz_emu})
	y_frame_ttzzz_OffZ = pd.DataFrame({'truth':y_test_ttzzz_OffZ, 'disc':y_pred_ttzzz_OffZ})
	y_frame_zz_emu = pd.DataFrame({'truth':y_test_zz_emu, 'disc':y_pred_zz_emu})
	y_frame_zz_emuHighTTZBDT = pd.DataFrame({'truth':y_test_zz_emuHighTTZBDT, 'disc':y_pred_zz_emuHighTTZBDT})
	y_frame_zz_OffZ = pd.DataFrame({'truth':y_test_zz_OffZ, 'disc':y_pred_zz_OffZ})
	y_frame_zz_OffZHighTTZBDT = pd.DataFrame({'truth':y_test_zz_OffZHighTTZBDT, 'disc':y_pred_zz_OffZHighTTZBDT})
	y_frame_ttz_nbAll = pd.DataFrame({'truth':y_test_ttz_nbAll, 'disc':y_pred_ttz_nbAll})
	y_frame_ttz_bVeto = pd.DataFrame({'truth':y_test_ttz_bVeto, 'disc':y_pred_ttz_bVeto})
	y_frame_ttz_emu = pd.DataFrame({'truth':y_test_ttz_emu, 'disc':y_pred_ttz_emu})
	y_frame_ttz_OffZ = pd.DataFrame({'truth':y_test_ttz_OffZ, 'disc':y_pred_ttz_OffZ})
	y_frame_multi_nbAll = pd.DataFrame({'truth':y_test_multi_nbAll, 'disc':y_pred_multi_nbAll})
	y_frame_multi_emuHighTTZBDT = pd.DataFrame({'truth':y_test_multi_emuHighTTZBDT, 'disc':y_pred_multi_emuHighTTZBDT})
	y_frame_multi_emu = pd.DataFrame({'truth':y_test_multi_emu, 'disc':y_pred_multi_emu})
	y_frame_multi_OffZHighTTZBDT = pd.DataFrame({'truth':y_test_multi_OffZHighTTZBDT, 'disc':y_pred_multi_OffZHighTTZBDT})
	y_frame_multi_OffZ = pd.DataFrame({'truth':y_test_multi_OffZ, 'disc':y_pred_multi_OffZ})
	disc_ttzzz_bVeto    = y_frame_ttzzz_bVeto[y_frame_ttzzz_bVeto['truth'] == 0]['disc'].values
	disc_ttzzz_emu    = y_frame_ttzzz_emu[y_frame_ttzzz_emu['truth'] == 0]['disc'].values
	disc_ttzzz_OffZ    = y_frame_ttzzz_OffZ[y_frame_ttzzz_OffZ['truth'] == 0]['disc'].values
	disc_zz_emu    = y_frame_zz_emu[y_frame_zz_emu['truth'] == 0]['disc'].values
	disc_zz_emuHighTTZBDT    = y_frame_zz_emuHighTTZBDT[y_frame_zz_emuHighTTZBDT['truth'] == 0]['disc'].values
	disc_zz_OffZ    = y_frame_zz_OffZ[y_frame_zz_OffZ['truth'] == 0]['disc'].values
	disc_zz_OffZHighTTZBDT    = y_frame_zz_OffZHighTTZBDT[y_frame_zz_OffZHighTTZBDT['truth'] == 0]['disc'].values
	disc_ttz_nbAll    = y_frame_ttz_nbAll[y_frame_ttz_nbAll['truth'] == 0]['disc'].values
	disc_ttz_bVeto    = y_frame_ttz_bVeto[y_frame_ttz_bVeto['truth'] == 0]['disc'].values
	disc_ttz_emu    = y_frame_ttz_emu[y_frame_ttz_emu['truth'] == 0]['disc'].values
	disc_ttz_OffZ    = y_frame_ttz_OffZ[y_frame_ttz_OffZ['truth'] == 0]['disc'].values
	disc_multi_nbAll    = y_frame_multi_nbAll[y_frame_multi_nbAll['truth'] == 0]['disc'].values
	disc_multi_emuHighTTZBDT    = y_frame_multi_emuHighTTZBDT[y_frame_multi_emuHighTTZBDT['truth'] == 0]['disc'].values
	disc_multi_emu    = y_frame_multi_emu[y_frame_multi_emu['truth'] == 0]['disc'].values
	disc_multi_OffZHighTTZBDT    = y_frame_multi_OffZHighTTZBDT[y_frame_multi_OffZHighTTZBDT['truth'] == 0]['disc'].values
	disc_multi_OffZ    = y_frame_multi_OffZ[y_frame_multi_OffZ['truth'] == 0]['disc'].values

	if mkplot == "yes":
		plt.figure()
		plt.hist(disc_ttzzz_bVeto, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttzzz_bVeto_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttzzz_emu, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttzzz_emu_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttzzz_OffZ, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttzzz_OffZ_' + sample + '.png')

		plt.figure()
		plt.hist(disc_zz_emu, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_zz_emu_' + sample + '.png')

		plt.figure()
		plt.hist(disc_zz_emuHighTTZBDT, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_zz_emuHighTTZBDT_' + sample + '.png')

		plt.figure()
		plt.hist(disc_zz_OffZ, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_zz_OffZ_' + sample + '.png')

		plt.figure()
		plt.hist(disc_zz_OffZHighTTZBDT, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_zz_OffZHighTTZBDT_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttz_nbAll, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttz_nbAll_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttz_bVeto, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttz_bVeto_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttz_emu, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttz_emu_' + sample + '.png')

		plt.figure()
		plt.hist(disc_ttz_OffZ, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_ttz_OffZ_' + sample + '.png')

		plt.figure()
		plt.figure()
		plt.hist(disc_multi_nbAll, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_multi_nbAll_' + sample + '.png')

		plt.figure()
		plt.hist(disc_multi_emuHighTTZBDT, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_multi_emuHighTTZBDT_' + sample + '.png')

		plt.figure()
		plt.hist(disc_multi_emu, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_multi_emu_' + sample + '.png')

		plt.figure()
		plt.hist(disc_multi_OffZHighTTZBDT, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_multi_OffZHighTTZBDT_' + sample + '.png')
		os.system("chmod 755 "+plotDir+"scores/*")

		plt.figure()
		plt.hist(disc_multi_OffZ, density=True, bins=50, alpha=0.3)
		plt.yscale("log")
		plt.savefig(plotDir+'scores/BDTscore_multi_OffZ_' + sample + '.png')
		os.system("chmod 755 "+plotDir+"scores/*")
	#############################################
	##Creating a new TTree with the discriminator
	#############################################

	ch = root.TChain("t")
	ch.Add(FileName)
	if filterBDTvariables == "yes":
		ch.SetBranchStatus("lep_Z_idx0",0)
		ch.SetBranchStatus("lep_Z_idx1",0)
		ch.SetBranchStatus("lep_N_idx0",0)
		ch.SetBranchStatus("lep_N_idx1",0)
		ch.SetBranchStatus("eventweight",0)
		ch.SetBranchStatus("lepsf",0)
		ch.SetBranchStatus("MllN",0)
		ch.SetBranchStatus("MllZ",0)
		ch.SetBranchStatus("ZPt",0)
		ch.SetBranchStatus("lep1Pt",0)
		ch.SetBranchStatus("lep2Pt",0)
		ch.SetBranchStatus("lep3Pt",0)
		ch.SetBranchStatus("lep4Pt",0)
		ch.SetBranchStatus("lep3Id",0)
		ch.SetBranchStatus("lep4Id",0)
		ch.SetBranchStatus("lep3MT",0)
		ch.SetBranchStatus("lep4MT",0)
		ch.SetBranchStatus("lep34MT",0)
		ch.SetBranchStatus("lep1dZ",0)
		ch.SetBranchStatus("lep2dZ",0)
		ch.SetBranchStatus("lep3dZ",0)
		ch.SetBranchStatus("lep4dZ",0)
		ch.SetBranchStatus("pt_zeta",0)
		ch.SetBranchStatus("pt_zeta_vis",0)
		ch.SetBranchStatus("phi0",0)
		ch.SetBranchStatus("phi",0)
		ch.SetBranchStatus("phiH",0)
		ch.SetBranchStatus("theta0",0)
		ch.SetBranchStatus("theta1",0)
		ch.SetBranchStatus("theta2",0)
		ch.SetBranchStatus("minDRJetToLep3",0)
		ch.SetBranchStatus("minDRJetToLep4",0)
		ch.SetBranchStatus("jet1Pt",0)
		ch.SetBranchStatus("jet2Pt",0)
		ch.SetBranchStatus("jet3Pt",0)
		ch.SetBranchStatus("jet4Pt",0)
		ch.SetBranchStatus("jet1BtagScore",0)
		ch.SetBranchStatus("jet2BtagScore",0)
		ch.SetBranchStatus("jet3BtagScore",0)
		ch.SetBranchStatus("jet4BtagScore",0)
	nEntries = ch.GetEntries()
	print("nEntries = "+str(nEntries))
	outFile = fileDir+".BDT/"+sample
	newFile = root.TFile(outFile,"RECREATE") 
	ch_new = ch.CloneTree(0)

	root.gROOT.ProcessLine("struct MyStruct_ttzzz_bVeto{float disc_ttzzz_bVeto;};")
	root.gROOT.ProcessLine("struct MyStruct_ttzzz_emu{float disc_ttzzz_emu;};")
	root.gROOT.ProcessLine("struct MyStruct_ttzzz_OffZ{float disc_ttzzz_OffZ;};")
	root.gROOT.ProcessLine("struct MyStruct_zz_emu{float disc_zz_emu;};")
	root.gROOT.ProcessLine("struct MyStruct_zz_emuHighTTZBDT{float disc_zz_emuHighTTZBDT;};")
	root.gROOT.ProcessLine("struct MyStruct_zz_OffZ{float disc_zz_OffZ;};")
	root.gROOT.ProcessLine("struct MyStruct_zz_OffZHighTTZBDT{float disc_zz_OffZHighTTZBDT;};")
	root.gROOT.ProcessLine("struct MyStruct_ttz_nbAll{float disc_ttz_nbAll;};")
	root.gROOT.ProcessLine("struct MyStruct_ttz_bVeto{float disc_ttz_bVeto;};")
	root.gROOT.ProcessLine("struct MyStruct_ttz_emu{float disc_ttz_emu;};")
	root.gROOT.ProcessLine("struct MyStruct_ttz_OffZ{float disc_ttz_OffZ;};")
	root.gROOT.ProcessLine("struct MyStruct_multi_nbAll{float disc_multi_nbAll;};")
	root.gROOT.ProcessLine("struct MyStruct_multi_emuHighTTZBDT{float disc_multi_emuHighTTZBDT;};")
	root.gROOT.ProcessLine("struct MyStruct_multi_emu{float disc_multi_emu;};")
	root.gROOT.ProcessLine("struct MyStruct_multi_OffZHighTTZBDT{float disc_multi_OffZHighTTZBDT;};")
	root.gROOT.ProcessLine("struct MyStruct_multi_OffZ{float disc_multi_OffZ;};")

	from ROOT import MyStruct_ttzzz_bVeto
	from ROOT import MyStruct_ttzzz_emu
	from ROOT import MyStruct_ttzzz_OffZ
	from ROOT import MyStruct_zz_emu
	from ROOT import MyStruct_zz_emuHighTTZBDT
	from ROOT import MyStruct_zz_OffZ
	from ROOT import MyStruct_zz_OffZHighTTZBDT
	from ROOT import MyStruct_ttz_nbAll
	from ROOT import MyStruct_ttz_bVeto
	from ROOT import MyStruct_ttz_emu
	from ROOT import MyStruct_ttz_OffZ
	from ROOT import MyStruct_multi_nbAll
	from ROOT import MyStruct_multi_emuHighTTZBDT
	from ROOT import MyStruct_multi_emu
	from ROOT import MyStruct_multi_OffZHighTTZBDT
	from ROOT import MyStruct_multi_OffZ

	# Create branches in the tree
	s_ttzzz_bVeto = MyStruct_ttzzz_bVeto()
	s_ttzzz_emu = MyStruct_ttzzz_emu()
	s_ttzzz_OffZ = MyStruct_ttzzz_OffZ()
	s_zz_emu = MyStruct_zz_emu()
	s_zz_emuHighTTZBDT = MyStruct_zz_emuHighTTZBDT()
	s_zz_OffZ = MyStruct_zz_OffZ()
	s_zz_OffZHighTTZBDT = MyStruct_zz_OffZHighTTZBDT()
	s_ttz_nbAll = MyStruct_ttz_nbAll()
	s_ttz_bVeto = MyStruct_ttz_bVeto()
	s_ttz_emu = MyStruct_ttz_emu()
	s_ttz_OffZ = MyStruct_ttz_OffZ()
	s_multi_nbAll = MyStruct_multi_nbAll()
	s_multi_emuHighTTZBDT = MyStruct_multi_emuHighTTZBDT()
	s_multi_emu = MyStruct_multi_emu()
	s_multi_OffZHighTTZBDT = MyStruct_multi_OffZHighTTZBDT()
	s_multi_OffZ = MyStruct_multi_OffZ()

	bpt_ttzzz_bVeto = ch_new.Branch('disc_ttzzz_bVeto',root.AddressOf(s_ttzzz_bVeto,'disc_ttzzz_bVeto'),'disc_ttzzz_bVeto/F');
	bpt_ttzzz_emu = ch_new.Branch('disc_ttzzz_emu',root.AddressOf(s_ttzzz_emu,'disc_ttzzz_emu'),'disc_ttzzz_emu/F');
	bpt_ttzzz_OffZ = ch_new.Branch('disc_ttzzz_OffZ',root.AddressOf(s_ttzzz_OffZ,'disc_ttzzz_OffZ'),'disc_ttzzz_OffZ/F');
	bpt_zz_emu = ch_new.Branch('disc_zz_emu',root.AddressOf(s_zz_emu,'disc_zz_emu'),'disc_zz_emu/F');
	bpt_zz_emuHighTTZBDT = ch_new.Branch('disc_zz_emuHighTTZBDT',root.AddressOf(s_zz_emuHighTTZBDT,'disc_zz_emuHighTTZBDT'),'disc_zz_emuHighTTZBDT/F');
	bpt_zz_OffZ = ch_new.Branch('disc_zz_OffZ',root.AddressOf(s_zz_OffZ,'disc_zz_OffZ'),'disc_zz_OffZ/F');
	bpt_zz_OffZHighTTZBDT = ch_new.Branch('disc_zz_OffZHighTTZBDT',root.AddressOf(s_zz_OffZHighTTZBDT,'disc_zz_OffZHighTTZBDT'),'disc_zz_OffZHighTTZBDT/F');
	bpt_ttz_nbAll = ch_new.Branch('disc_ttz_nbAll',root.AddressOf(s_ttz_nbAll,'disc_ttz_nbAll'),'disc_ttz_nbAll/F');
	bpt_ttz_bVeto = ch_new.Branch('disc_ttz_bVeto',root.AddressOf(s_ttz_bVeto,'disc_ttz_bVeto'),'disc_ttz_bVeto/F');
	bpt_ttz_emu = ch_new.Branch('disc_ttz_emu',root.AddressOf(s_ttz_emu,'disc_ttz_emu'),'disc_ttz_emu/F');
	bpt_ttz_OffZ = ch_new.Branch('disc_ttz_OffZ',root.AddressOf(s_ttz_OffZ,'disc_ttz_OffZ'),'disc_ttz_OffZ/F');
	bpt_multi_nbAll = ch_new.Branch('disc_multi_nbAll',root.AddressOf(s_multi_nbAll,'disc_multi_nbAll'),'disc_multi_nbAll/F');
	bpt_multi_emuHighTTZBDT = ch_new.Branch('disc_multi_emuHighTTZBDT',root.AddressOf(s_multi_emuHighTTZBDT,'disc_multi_emuHighTTZBDT'),'disc_multi_emuHighTTZBDT/F');
	bpt_multi_emu = ch_new.Branch('disc_multi_emu',root.AddressOf(s_multi_emu,'disc_multi_emu'),'disc_multi_emu/F');
	bpt_multi_OffZHighTTZBDT = ch_new.Branch('disc_multi_OffZHighTTZBDT',root.AddressOf(s_multi_OffZHighTTZBDT,'disc_multi_OffZHighTTZBDT'),'disc_multi_OffZHighTTZBDT/F');
	bpt_multi_OffZ = ch_new.Branch('disc_multi_OffZ',root.AddressOf(s_multi_OffZ,'disc_multi_OffZ'),'disc_multi_OffZ/F');

	for i in range(nEntries):
		ch.GetEntry(i)
		if i%10000==0:
			print("Processing event nr. "+str(i)+" of " + str(nEntries))
		s_ttzzz_bVeto.disc_ttzzz_bVeto = disc_ttzzz_bVeto[i]
		s_ttzzz_emu.disc_ttzzz_emu = disc_ttzzz_emu[i]
		s_ttzzz_OffZ.disc_ttzzz_OffZ = disc_ttzzz_OffZ[i]
		s_zz_emu.disc_zz_emu = disc_zz_emu[i]
		s_zz_emuHighTTZBDT.disc_zz_emuHighTTZBDT = disc_zz_emuHighTTZBDT[i]
		s_zz_OffZHighTTZBDT.disc_zz_OffZHighTTZBDT = disc_zz_OffZHighTTZBDT[i]
		s_ttz_nbAll.disc_ttz_nbAll = disc_ttz_nbAll[i]
		s_ttz_bVeto.disc_ttz_bVeto = disc_ttz_bVeto[i]
		s_ttz_emu.disc_ttz_emu = disc_ttz_emu[i]
		s_ttz_OffZ.disc_ttz_OffZ = disc_ttz_OffZ[i]
		s_multi_nbAll.disc_multi_nbAll = disc_multi_nbAll[i]
		s_multi_emuHighTTZBDT.disc_multi_emuHighTTZBDT = disc_multi_emuHighTTZBDT[i]
		s_multi_emu.disc_multi_emu = disc_multi_emu[i]
		s_multi_OffZHighTTZBDT.disc_multi_OffZHighTTZBDT = disc_multi_OffZHighTTZBDT[i]
		s_multi_OffZ.disc_multi_OffZ = disc_multi_OffZ[i]
		ch_new.Fill()
	ch_new.GetCurrentFile().Write()
	Nevents.Write()
	ch_new.GetCurrentFile().Close()
	
	
