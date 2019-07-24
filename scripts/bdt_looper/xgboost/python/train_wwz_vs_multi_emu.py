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

####################option C:##################
##train a multi-class BDT to discriminate signal with all backgrounds
##use emu events for training
##with all backgrounds included
##############################################
doAppend = False

root.gROOT.SetBatch(True)
test_name = 'xgb_wwz_vs_multi_emu'

lumi_sf_sig = 1.0 # scale lumi from 2018 sample to full run2
lumi_sf_bkg = 1.0

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
pwd = os.getcwd()
dataDir = pwd.replace("python", "data/All/emu/")

os.system("mkdir -p "+plotDir)
os.system("mkdir -p "+plotDir+"training")
os.system("mkdir -p "+plotDir+"results")
os.system("mkdir -p "+plotDir+"scores")
os.system("mkdir -p "+plotDir+"variables")

#signal
signalFileName =  dataDir + 'wwz_skim.root'
signalFile = root.TFile(signalFileName)
signalTree = signalFile.Get('t')
signalTree.Draw('met_pt>>tmp1', 'eventweight')
signalHisto = root.gDirectory.Get('tmp1')
signalEvents = lumi_sf_sig*signalHisto.Integral()

#bkg
bkgFileName    = dataDir + 'all_bkg_skim.root'
bkgFile = root.TFile(bkgFileName)
bkgTree = bkgFile.Get('t')
bkgTree.Draw('met_pt>>tmp2', 'eventweight')
bkgHisto = root.gDirectory.Get('tmp2')
bkgEvents = lumi_sf_bkg*bkgHisto.Integral()

zzFileName  = dataDir + 'zz_skim.root'
ttzFileName  = dataDir + 'ttz_skim.root'
wzFileName  = dataDir + 'wz_skim.root'
twzFileName  = dataDir + 'twz_skim.root'
otherFileName  = dataDir + 'other_skim.root'

num_class = 6


print('[INFO]: S =' + str(signalEvents) + '; B =' + str(bkgEvents) +"; S/sqrt(B) = " + str(signalEvents/math.sqrt(bkgEvents)))

##Define variables to be used
variables = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJetToLep3','minDRJetToLep4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1BtagScore', 'jet2BtagScore', 'jet3BtagScore', 'jet4BtagScore']
variables_names = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','Mll34', 'pt_zeta', 'pt_zeta_vis', "nj", 'minDRJToL3','minDRJToL4', 'jet1Pt', 'jet2Pt', 'jet3Pt', 'jet4Pt', 'jet1Btag', 'jet2Btag', 'jet3Btag', 'jet4Btag']
print(len(variables))

##Getting ROOT files into pandas
df_signal = uproot.open(signalFileName)['t'].pandas.df(variables, flatten=False)
df_zz = uproot.open(zzFileName)['t'].pandas.df(variables, flatten=False)
df_ttz = uproot.open(ttzFileName)['t'].pandas.df(variables, flatten=False)
df_wz = uproot.open(wzFileName)['t'].pandas.df(variables, flatten=False)
df_twz = uproot.open(twzFileName)['t'].pandas.df(variables, flatten=False)
df_other = uproot.open(otherFileName)['t'].pandas.df(variables, flatten=False)

##Getting Sample weights
weights_signal = uproot.open(signalFileName)['t'].pandas.df('eventweight', flatten=False)
weights_zz = uproot.open(zzFileName)['t'].pandas.df('eventweight', flatten=False)
weights_ttz = uproot.open(ttzFileName)['t'].pandas.df('eventweight', flatten=False)
weights_wz = uproot.open(wzFileName)['t'].pandas.df('eventweight', flatten=False)
weights_twz = uproot.open(twzFileName)['t'].pandas.df('eventweight', flatten=False)
weights_other = uproot.open(otherFileName)['t'].pandas.df('eventweight', flatten=False)


##Getting a numpy array out of two pandas data frame
sample_weights = np.concatenate([weights_signal.values, weights_zz.values[:100000], weights_ttz.values, weights_wz.values, weights_twz.values, weights_other.values])
#print sample_weights

#getting a numpy array from two pandas data frames
x = np.concatenate([df_signal.values, df_zz.values[:100000], df_ttz.values, df_wz.values, df_twz.values, df_other.values])
#creating numpy array for target variables
y = np.concatenate([0*np.ones(len(df_signal.values)), 1*np.ones(len(df_zz.values[:100000])), 2*np.ones(len(df_ttz.values)), 3*np.ones(len(df_wz.values)), 4*np.ones(len(df_twz.values)), 5*np.ones(len(df_other.values))])

####Plot input variables######

for idx in range(len(variables)):
        plt.figure()
        plt.hist(df_signal[df_signal[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="WWZ")
        plt.hist(df_zz[df_zz[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="zz")
        plt.hist(df_ttz[df_ttz[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="ttz")
        plt.hist(df_wz[df_wz[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="wz")
        plt.hist(df_twz[df_twz[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="twz")
        plt.hist(df_other[df_other[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=2, label="other")
        plt.legend(loc="upper center")
        plt.xlabel(variables[idx])
        plt.ylabel('Events')
        plt.savefig(plotDir+'variables/'+test_name + '_' + variables[idx]+'.png')

os.system("chmod 755 "+plotDir+"variables/*")

# split data into train and test sets
seed = 7
test_size = 0.4
sample_size = 1.0
x_train, x_test, y_train, y_test = train_test_split(x, y, train_size = sample_size*(1-test_size), test_size=sample_size*test_size, random_state=seed)

d_train = xgb.DMatrix(x_train, label=y_train)
d_test = xgb.DMatrix(x_test, label=y_test)

# fit model no training data
watchlist = [(d_train, 'train'), (d_test, 'test')]
params = {'max_depth':7, 'learning_rate':0.1, 'n_estimators':1000, 'n_jobs':4, 'objective':'multi:softmax', 'num_class':num_class}
model = xgb.train(params, d_train)#, evals_result=watchlist)
pred_label = model.predict(d_test)
pred_label_train = model.predict(d_train)
error_rate = np.sum(pred_label != y_test) / y_test.shape[0]
print('Test error using softmax = {}'.format(error_rate))

#params['objective'] = 'multi:softprob'
#model = xgb.train(params, d_train)#, evals_result=watchlist)
#pred_prob = model.predict(d_test).reshape(y_test.shape[0], num_class)
#pred_prob_train = model.predict(d_train).reshape(y_train.shape[0], num_class)
#pred_label = np.argmax(pred_prob, axis=1)
#pred_label_train = np.argmax(pred_prob_train, axis=1)
#error_rate = np.sum(pred_label != y_test) / y_test.shape[0]
#print('Test error using softprob = {}'.format(error_rate))


##########################################################
# make histogram of discriminator value for signal and bkg
##########################################################
#pd.DataFrame({'truth':y_test, 'disc':y_pred}).hist(column='disc', by='truth', bins=50)
y_frame = pd.DataFrame({'truth':y_test, 'disc':pred_label})
y_frame_train = pd.DataFrame({'truth':y_train, 'disc':pred_label_train})

disc_zz    = y_frame[y_frame['truth'] == 1]['disc'].values
disc_zz_train    = y_frame_train[y_frame_train['truth'] == 1]['disc'].values

disc_ttz    = y_frame[y_frame['truth'] == 2]['disc'].values
disc_ttz_train    = y_frame_train[y_frame_train['truth'] == 2]['disc'].values

disc_wz    = y_frame[y_frame['truth'] == 3]['disc'].values
disc_wz_train    = y_frame_train[y_frame_train['truth'] == 3]['disc'].values

disc_twz    = y_frame[y_frame['truth'] == 4]['disc'].values
disc_twz_train    = y_frame_train[y_frame_train['truth'] == 4]['disc'].values

disc_other    = y_frame[y_frame['truth'] == 5]['disc'].values
disc_other_train    = y_frame_train[y_frame_train['truth'] == 5]['disc'].values

disc_signal = y_frame[y_frame['truth'] == 0]['disc'].values
disc_signal_train = y_frame_train[y_frame_train['truth'] == 0]['disc'].values

plt.figure()
bins_label=[-0.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5]
plt.hist(disc_signal, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="WWZ - test", color='b')
plt.hist(disc_signal_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="WWZ - train", linestyle='--', color='b')
plt.hist(disc_zz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="zz - test", color='y')
plt.hist(disc_zz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="zz - train", linestyle='--', color='y')
plt.hist(disc_ttz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="ttz - test", color='g')
plt.hist(disc_ttz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="ttz - train", linestyle='--', color='g')
plt.hist(disc_wz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="wz - test", color='r')
plt.hist(disc_wz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="wz - train", linestyle='--', color='r')
plt.hist(disc_twz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="twz - test", color='m')
plt.hist(disc_twz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="twz - train", linestyle='--', color='m')
plt.hist(disc_other, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="other - test", color='c')
plt.hist(disc_other_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="other - train", linestyle='--', color='c')

#plt.hist(disc_signal_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="WWZ  - train")
plt.yscale("log")
plt.xlim([-0.5, 5.5])
#plt.ylim([0.001, 100.0])
plt.legend(loc="upper right")
plt.xlabel('BDT response (label)')
plt.ylabel('Events')
plt.savefig(plotDir+'training/mydiscriminator_' + test_name +'_logY.png')

plt.figure()
plt.hist(disc_signal, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="WWZ - test", color='b')
plt.hist(disc_signal_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="WWZ - train", linestyle='--', color='b')
plt.hist(disc_zz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="zz - test", color='y')
plt.hist(disc_zz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="zz - train", linestyle='--', color='y')
plt.hist(disc_ttz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="ttz - test", color='g')
plt.hist(disc_ttz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="ttz - train", linestyle='--', color='g')
plt.hist(disc_wz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="wz - test", color='r')
plt.hist(disc_wz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="wz - train", linestyle='--', color='r')
plt.hist(disc_twz, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="twz - test", color='m')
plt.hist(disc_twz_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="twz - train", linestyle='--', color='m')
plt.hist(disc_other, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="other - test", color='c')
plt.hist(disc_other_train, density=True, bins=bins_label, alpha=1.0, histtype="step", lw=2, label="other - train", linestyle='--', color='c')

plt.yscale("linear")
plt.xlim([-0.5, 5.5])
#plt.ylim([0.001, 100.0])
plt.legend(loc="upper right")
plt.xlabel('BDT response')
plt.ylabel('Events')
plt.savefig(plotDir+'training/mydiscriminator_' + test_name +'_linY.png')

os.system("chmod 755 "+plotDir+"training/*")


# Pickle dictionary using protocol 0.
output = open('../models/model_'+test_name+'.pkl', 'wb')
pickle.dump(model, output)
output.close()
model.dump_model('../models/model_'+test_name+'.txt')
model.save_model('../models/model_'+test_name+'.xgb')

print("example input and prediction....")
print("zz sample 1st event input:")
print(df_zz.values[0])
print("y_pred_bkg first 20 events")
print(pred_label[:20])

#plot feature importances

model.feature_names = variables_names

xgb.plot_importance(model, max_num_features=len(variables), xlabel="F score (weight)")
plt.savefig(plotDir+'training/myImportances_Fscore_' + test_name + '.png')
os.system("chmod 755 "+plotDir+"training/*")

xgb.plot_tree( model )
fig = plt.gcf()
fig.set_size_inches(500, 50)
plt.draw()
plt.savefig(plotDir+'training/myTree_' + test_name + '.png')
plt.savefig(plotDir+'training/myTree_' + test_name + '.svg')
os.system("chmod 755 "+plotDir+"training/*")
