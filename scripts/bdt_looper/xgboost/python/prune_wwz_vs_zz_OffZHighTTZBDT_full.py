#!/usr/bin/env python
# coding: utf-8
import matplotlib.pyplot as plt
import xgboost as xgb
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, roc_curve, roc_auc_score
import numpy as np
import pandas as pd
import math
import pickle as pickle
import ROOT as root
import os
import shlex
import uproot

####################option A:##################
##train a single BDT to discriminate signal with zz backgrounds
##use OffZ events for training
##with zz backgrounds included
##############################################
doAppend = False

root.gROOT.SetBatch(True)
test_name = 'xgb_wwz_vs_zz_OffZHighTTZBDT'

lumi_sf_sig = 1.0 # scale lumi from 2018 sample to full run2
lumi_sf_bkg = 1.0

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
pwd = os.getcwd()
dataDir = pwd.replace("python", "data/All/OffZHighTTZBDT/")

os.system("mkdir -p "+plotDir)
os.system("mkdir -p "+plotDir+"training")
os.system("mkdir -p "+plotDir+"results")
os.system("mkdir -p "+plotDir+"scores")
os.system("mkdir -p "+plotDir+"variables")
os.system("mkdir -p "+plotDir+"variables_compare")
os.system("mkdir -p "+plotDir+"cuts_scan")

#signal
signalFileName =  dataDir + 'wwz.root'
signalFile = root.TFile(signalFileName)
signalTree = signalFile.Get('t')
signalTree.Draw('met_pt_corr>>tmp1', 'eventweight')
signalHisto = root.gDirectory.Get('tmp1')
signalEvents = lumi_sf_sig*signalHisto.Integral()

#bkg
bkgFileName    = dataDir + 'zz.root'
bkgFile = root.TFile(bkgFileName)
bkgTree = bkgFile.Get('t')
bkgTree.Draw('met_pt_corr>>tmp2', 'eventweight')
bkgHisto = root.gDirectory.Get('tmp2')
bkgEvents = lumi_sf_bkg*bkgHisto.Integral()


print('[INFO]: S =' + str(signalEvents) + '; B =' + str(bkgEvents) +"; S/sqrt(B) = " + str(signalEvents/math.sqrt(bkgEvents)))

##Define variables to be used
variables = ['met_pt_corr','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', 'M4l', 'Pt4l']
variables_names = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','Mll34', 'pt_zeta', 'pt_zeta_vis', 'M4l', 'Pt4l']
print(len(variables))


print(len(variables))

##Getting ROOT files into pandas
df_signal = uproot.open(signalFileName)['t'].pandas.df(variables, flatten=False)
df_bkg = uproot.open(bkgFileName)['t'].pandas.df(variables, flatten=False)

#getting a numpy array from two pandas data frames
x = np.concatenate([df_bkg.values,df_signal.values])
#creating numpy array for target variables
y = np.concatenate([np.zeros(len(df_bkg)),
                        np.ones(len(df_signal))])

# split data into train and test sets
seed = 7
test_size = 0.4
sample_size = 1.0
x_train, x_test, y_train, y_test = train_test_split(x, y, train_size = sample_size*(1-test_size), test_size=sample_size*test_size, random_state=seed)


model = xgb.XGBClassifier(max_depth=4, learning_rate=0.1, n_estimators=400, verbosity=2, n_jobs=4, reg_lambda=1.0)
model.fit(x_train, y_train)
y_pred = model.predict_proba(x_test)[:, 1]
predictions = [round(value) for value in y_pred]
accuracy = accuracy_score(y_test, predictions)
print("Accuracy: %.2f%%" % (accuracy * 100.0))
AUC=roc_auc_score(y_test, y_pred)
print("AUC: "+str(AUC))
fpr, tpr, thr = roc_curve(y_test, y_pred)


index_remove = []
AUC_after_remove = []
fpr_remove = []
tpr_remove = []
variables_left = variables.copy()
x_train_left = np.copy(x_train)
x_test_left = np.copy(x_test)

for num_remove in range(len(variables)-1):
	plt.figure()
	plt.plot(fpr, tpr, lw=2, label='all variables, AUC = %.4f'%AUC)
	print("trying to remove "+str(num_remove+1)+" variable...")

	AUC_all = np.zeros(len(variables))
	AUC_maximum = 0.0
	fpr_remove_this = None
	tpr_remove_this = None
	for idx in range(len(variables)):
		if idx in index_remove:
			continue
		idx_in_left =  variables_left.index(variables[idx])
		print("removing input variable: "+variables[idx])

		x_train_prune = np.delete(x_train_left, idx_in_left, 1)
		x_test_prune = np.delete(x_test_left, idx_in_left, 1)

		model = xgb.XGBClassifier(max_depth=4, learning_rate=0.1, n_estimators=400, verbosity=2, n_jobs=4, reg_lambda=1.0)
		model.fit(x_train_prune, y_train)
		y_pred = model.predict_proba(x_test_prune)[:, 1]
		predictions = [round(value) for value in y_pred]
		accuracy = accuracy_score(y_test, predictions)
		print("Accuracy: %.2f%%" % (accuracy * 100.0))
		AUC_this=roc_auc_score(y_test, y_pred)
		print("AUC: "+str(AUC_this))
		fpr_this, tpr_this, thr_this = roc_curve(y_test, y_pred)
		plt.plot(fpr_this, tpr_this, lw=2, label='w/o '+variables_names[idx]+", AUC = %.4f"%AUC_this)
		AUC_all[idx] = AUC_this
		if AUC_this > AUC_maximum:
			AUC_maximum = AUC_this
			fpr_remove_this = np.copy(fpr_this)
			tpr_remove_this = np.copy(tpr_this)
	fpr_remove.append(fpr_remove_this)
	tpr_remove.append(tpr_remove_this)
	print("least important variable:")
	idx_remove = np.argmax(AUC_all)
	print("var"+str(idx_remove)+", "+variables[idx_remove]+", AUC = %.4f"%AUC_all[idx_remove])
	#plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
	plt.xlim([0.0, 0.4])
	plt.ylim([0.6, 1.05])
	plt.ylabel('Signal Efficiency')
	plt.xlabel('Background Efficiency')
	#plt.axhline(y=0.9, color="black", linestyle='--')
	#plt.title('Receiver operating characteristic example')
	plt.legend(loc="lower right")
	#plt.show()
	plt.savefig(plotDir+'training/myroc_prune_' + test_name + '_remove'+str(num_remove+1)+'.png')
	os.system("chmod 755 "+plotDir+"training/*")
		
	idx_remove_in_left = variables_left.index(variables[idx_remove])
	variables_left.remove(variables[idx_remove])
	x_train_left = np.delete(x_train_left, idx_remove_in_left, 1)
	x_test_left = np.delete(x_test_left, idx_remove_in_left, 1)
	index_remove.append(idx_remove)
	AUC_after_remove.append(AUC_all[idx_remove])

print("order of variables to be removed, and AUC after removing it")
names_removal = []
AUCs_removal = []
for idx in range(len(index_remove)):
	print(variables[index_remove[idx]]+" %.4f"%AUC_after_remove[idx])
	names_removal.append(variables_names[index_remove[idx]])
	AUCs_removal.append(AUC_after_remove[idx])


##plot AUC vs feature names plot:

plt.figure()
plt.plot(names_removal, AUCs_removal, lw=2)
plt.xticks(rotation=90)
plt.xlabel('cumulative removed features (left to right)')
plt.ylabel('AUC after removal')
plt.savefig(plotDir+'training/AUC_vs_removed_features_'+test_name+'.png', bbox_inches='tight')
os.system("chmod 755 "+plotDir+"training/*")

##plot roc curve in steps of removal

plt.figure()
plt.plot(fpr, tpr, lw=2, label='all variables, AUC = %.4f'%AUC)
for idx in range(len(index_remove)):
	plt.plot(fpr_remove[idx], tpr_remove[idx], lw=2, label='-'+variables_names[index_remove[idx]]+", AUC = %.4f"%AUC_after_remove[idx])

plt.xlim([0.0, 0.4])
plt.ylim([0.6, 1.05])
plt.ylabel('Signal Efficiency')
plt.xlabel('Background Efficiency')
#plt.axhline(y=0.9, color="black", linestyle='--')
#plt.title('Receiver operating characteristic example')
plt.legend(loc="lower right")
#plt.show()
plt.savefig(plotDir+'training/myroc_prune_' + test_name + '_cumulative.png')
plt.yscale("log")
#plt.xscale("log")
plt.savefig(plotDir+'training/myroc_prune_' + test_name + '_cumulative_logY.png',bbox_inches='tight')
plt.yscale("log")
os.system("chmod 755 "+plotDir+"training/*")
