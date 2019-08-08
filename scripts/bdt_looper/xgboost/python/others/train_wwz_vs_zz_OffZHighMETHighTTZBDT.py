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

####################option B:##################
##train two BDTs/: ZZ and ttZ
##use OffZ events for ZZ BDT training
##use events without bVeto for ttZ BDT training
##############################################
doAppend = False

root.gROOT.SetBatch(True)
root.gStyle.SetOptStat(0)
root.gStyle.SetOptFit(111)
root.gStyle.SetPaintTextFormat("2.1f")


test_name = 'xgb_wwz_vs_zz_OffZHighMETHighTTZBDT'

lumi_sf_sig = 1.0 # scale lumi from 2018 sample to full run2
lumi_sf_bkg = 1.0

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"
pwd = os.getcwd()
dataDir = pwd.replace("python", "data/All/OffZHighMETHighTTZBDT/")

os.system("mkdir -p "+plotDir)
os.system("mkdir -p "+plotDir+"training")
os.system("mkdir -p "+plotDir+"results")
os.system("mkdir -p "+plotDir+"scores")
os.system("mkdir -p "+plotDir+"variables")

#signal
signalFileName =  dataDir + 'wwz.root'
signalFile = root.TFile(signalFileName)
signalTree = signalFile.Get('t')
signalTree.Draw('met_pt_corr>>tmp1')
signalHisto = root.gDirectory.Get('tmp1')
signalEvents = lumi_sf_sig*signalHisto.Integral()

#bkg
bkgFileName    = dataDir + 'zz.root'
bkgFile = root.TFile(bkgFileName)
bkgTree = bkgFile.Get('t')
bkgTree.Draw('met_pt_corr>>tmp2')
bkgHisto = root.gDirectory.Get('tmp2')
bkgEvents = lumi_sf_bkg*bkgHisto.Integral()


print('[INFO]: S =' + str(signalEvents) + '; B =' + str(bkgEvents) +"; S/sqrt(B) = " + str(signalEvents/math.sqrt(bkgEvents)))

##Define variables to be used
#variables = ['met_pt_corr','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','MllN', 'pt_zeta', 'pt_zeta_vis','M4l','Pt4l']
#variables_names = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','lep34MT','phi0','theta0','phi','theta1','theta2','Mll34', 'pt_zeta', 'pt_zeta_vis','M4l','Pt4l']
variables = ['met_pt_corr','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','MllN', 'pt_zeta', 'pt_zeta_vis','M4l','Pt4l']
variables_names = ['met_pt','lep3Pt','lep4Pt','ZPt','lep3dZ', 'lep4dZ','lep3MT','lep4MT','Mll34', 'pt_zeta', 'pt_zeta_vis','M4l','Pt4l']

print(len(variables))

##Getting ROOT files into pandas
df_signal = uproot.open(signalFileName)['t'].pandas.df(variables, flatten=False)
df_bkg = uproot.open(bkgFileName)['t'].pandas.df(variables, flatten=False)



#getting a numpy array from two pandas data frames
x = np.concatenate([df_bkg.values,df_signal.values])
#creating numpy array for target variables
y = np.concatenate([np.zeros(len(df_bkg)),
                        np.ones(len(df_signal))])

###plot correlation
file_sig = root.TFile(signalFileName)
tree_sig = file_sig.Get("t")
file_bkg = root.TFile(bkgFileName)
tree_bkg = file_sig.Get("t")
h2_corr_sig = root.TH2F("h2_corr_sig", "h2_corr_sig", len(variables), 0, len(variables), len(variables), 0, len(variables))
h2_corr_bkg = root.TH2F("h2_corr_bkg", "h2_corr_bkg", len(variables), 0, len(variables), len(variables), 0, len(variables))

for idx1 in range(len(variables)):
        for idx2 in range(len(variables)):
                tree_sig.Draw(variables[idx1]+":"+variables[idx2]+">>temp_sig")
                tree_bkg.Draw(variables[idx1]+":"+variables[idx2]+">>temp_bkg")
                sig_hist = root.gDirectory.Get('temp_sig')
                h2_corr_sig.SetBinContent(idx1+1, idx2+1, sig_hist.GetCorrelationFactor())
                bkg_hist = root.gDirectory.Get('temp_bkg')
                h2_corr_bkg.SetBinContent(idx1+1, idx2+1, bkg_hist.GetCorrelationFactor())
                root.gDirectory.Delete('temp_sig')
                root.gDirectory.Delete('temp_bkg')
h2_corr_sig.GetZaxis().SetRangeUser(-1.0, 1.0)
h2_corr_bkg.GetZaxis().SetRangeUser(-1.0, 1.0)

for idx in range(len(variables)):
        h2_corr_sig.GetXaxis().SetBinLabel(idx+1, variables_names[idx])
        h2_corr_sig.GetYaxis().SetBinLabel(idx+1, variables_names[idx])
        h2_corr_bkg.GetXaxis().SetBinLabel(idx+1, variables_names[idx])
        h2_corr_bkg.GetYaxis().SetBinLabel(idx+1, variables_names[idx])

myC = root.TCanvas( "myC", "myC", 200, 10, 900, 800 )
myC.SetHighLightColor(2)
myC.SetFillColor(0)
myC.SetBorderMode(0)
myC.SetBorderSize(2)
myC.SetLeftMargin( 0.12 )
myC.SetRightMargin( 0.12 )
myC.SetBottomMargin( 0.12 )
myC.SetTopMargin( 0.12 )
myC.SetFrameBorderMode(0)
myC.SetFrameBorderMode(0)

stops = np.array([0.00, 0.34, 0.61, 0.84, 1.00])
red= np.array([0.50, 0.50, 1.00, 1.00, 1.00])
green = np.array([ 0.50, 1.00, 1.00, 0.60, 0.50])
blue = np.array([1.00, 1.00, 0.50, 0.40, 0.50])
root.TColor.CreateGradientColorTable(len(stops), stops, red, green, blue, 255)
root.gStyle.SetNumberContours(255)

h2_corr_sig.Draw("COLZTEXT")
h2_corr_sig.SetTitle("")
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_sig.pdf')
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_sig.png')
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_sig.C')
h2_corr_bkg.Draw("COLZTEXT")
h2_corr_bkg.SetTitle("")
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_bkg.pdf')
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_bkg.png')
myC.SaveAs(plotDir+'variables/'+test_name + '_correlation_matrix_bkg.C')
os.system("chmod 755 "+plotDir+"variables/*")

####Plot input variables######

for idx in range(len(variables)):
        plt.figure()
        plt.hist(df_signal[df_signal[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=4, label="WWZ")
        plt.hist(df_bkg[df_bkg[variables[idx]] > -999][variables[idx]], density=True, bins=50, alpha=1.0, histtype="step", lw=4, label="ZZ")
        plt.legend(loc="upper center", fontsize = 50)
        plt.xlabel(variables[idx], fontsize=50)
        plt.ylabel('Events', fontsize=30)
        plt.xticks(fontsize=35)
        plt.yticks(fontsize=30)
        fig = plt.gcf()
        fig.set_size_inches(16, 12)
        plt.draw()
        plt.savefig(plotDir+'variables/'+test_name + '_' + variables[idx]+'.png')

os.system("chmod 755 "+plotDir+"variables/*")


# split data into train and test sets
seed = 7
test_size = 0.4
sample_size = 1.0
x_train, x_test, y_train, y_test = train_test_split(x, y, train_size = sample_size*(1-test_size), test_size=sample_size*test_size, random_state=seed)

# fit model no training data
#model = xgb.XGBClassifier(max_depth=8, n_estimators=1000, gamma=1, learning_rate = .99)
#model = xgb.XGBClassifier(max_depth=4, learning_rate=0.05, n_estimators=400, verbosity=2, n_jobs=4, reg_lambda=0.5, booster='gblinear')
#model = xgb.XGBClassifier(max_depth=3, learning_rate=0.05, n_estimators=600, verbosity=2, n_jobs=4, reg_lambda=0.5)
model = xgb.XGBClassifier(max_depth=3, learning_rate=0.1, n_estimators=300, verbosity=2, n_jobs=4, reg_lambda=1.0)
#model = xgb.XGBClassifier(max_depth=8, learning_rate=0.1, n_estimators=1000, verbosity=2, n_jobs=4, booster='gblinear')
#model = xgb.XGBClassifier(max_depth=3, learning_rate=0.1, n_estimators=100, verbosity=2, n_jobs=4)
model.fit(x_train, y_train)

#print( dir(model) )
#print(model)
# make predictions for test data
y_pred = model.predict_proba(x_test)[:, 1]
y_pred_train = model.predict_proba(x_train)[:, 1]
y_pred_bkg = model.predict_proba(df_bkg.values)[:, 1]
predictions = [round(value) for value in y_pred]
# evaluate predictions
accuracy = accuracy_score(y_test, predictions)
print("Accuracy: %.2f%%" % (accuracy * 100.0))
AUC = roc_auc_score(y_test, y_pred)
print("AUC: "+str(AUC))
#get roc curve
#roc = roc_curve(y_test, y_pred)
fpr, tpr, thr = roc_curve(y_test, y_pred)


significance = []
effSignal = []
effBkg = []
thresholds = []

ctr = 0
for i in range(len(fpr)):
    if fpr[i] > 1e-5 and tpr[i] > 1e-5:
        #print fpr[i], tpr[i] 
        #significance.append(math.sqrt(lumi)*4.8742592356*0.006431528796*tpr[i]/math.sqrt(fpr[i]*0.9935684712))
        significance.append(signalEvents*tpr[i]/math.sqrt(fpr[i]*bkgEvents))
        effSignal.append(tpr[i])
        effBkg.append(fpr[i])
        thresholds.append(thr[i])
        #print significance[ctr], ' ' , fpr[ctr], ' ', tpr[ctr]
        ctr = ctr + 1


max_significance = max(significance)
idx_max_significance = np.argmax(np.array(significance))
best_threshold = thresholds[idx_max_significance]
best_effSignal = effSignal[idx_max_significance]
best_effBkg = effBkg[idx_max_significance]

print("max_significance: "+str(max_significance))
print("best_threshold: "+str(best_threshold))
print("best_effSignal: "+str(best_effSignal))
print("best_effBkg: "+str(best_effBkg))

idx_WP90 = 0
minD0p9 = 999.0
for idx in range(len(effSignal)):
	if abs(effSignal[idx] - 0.90) < minD0p9:
		idx_WP90 = idx
		minD0p9 = abs(effSignal[idx] - 0.90)

WP90_significance = significance[idx_WP90]
WP90_threshold = thresholds[idx_WP90]
WP90_effSignal = effSignal[idx_WP90]
WP90_effBkg = effBkg[idx_WP90]

print("WP90_significance: "+str(WP90_significance))
print("WP90_threshold: "+str(WP90_threshold))
print("WP90_effSignal: "+str(WP90_effSignal))
print("WP90_effBkg: "+str(WP90_effBkg))

idx_WP80 = 0
minD0p8 = 999.0
for idx in range(len(effSignal)):
	if abs(effSignal[idx] - 0.80) < minD0p8:
		idx_WP80 = idx
		minD0p8 = abs(effSignal[idx] - 0.80)

WP80_significance = significance[idx_WP80]
WP80_threshold = thresholds[idx_WP80]
WP80_effSignal = effSignal[idx_WP80]
WP80_effBkg = effBkg[idx_WP80]

print("WP80_significance: "+str(WP80_significance))
print("WP80_threshold: "+str(WP80_threshold))
print("WP80_effSignal: "+str(WP80_effSignal))
print("WP80_effBkg: "+str(WP80_effBkg))


##########################################################
# make histogram of discriminator value for signal and bkg
##########################################################
#pd.DataFrame({'truth':y_test, 'disc':y_pred}).hist(column='disc', by='truth', bins=50)
y_frame = pd.DataFrame({'truth':y_test, 'disc':y_pred})
y_frame_train = pd.DataFrame({'truth':y_train, 'disc':y_pred_train})
disc_bkg    = y_frame[y_frame['truth'] == 0]['disc'].values
disc_bkg_train    = y_frame_train[y_frame_train['truth'] == 0]['disc'].values
disc_signal = y_frame[y_frame['truth'] == 1]['disc'].values
disc_signal_train = y_frame_train[y_frame_train['truth'] == 1]['disc'].values
plt.figure()
plt.hist(disc_signal, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="WWZ - test")
plt.hist(disc_signal_train, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="WWZ  - train")
plt.hist(disc_bkg, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="ZZ - test")
plt.hist(disc_bkg_train, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="ZZ - train")
plt.yscale("log")
plt.xlim([0.0, 1.0])
plt.ylim([0.001, 1000.0])
plt.legend(loc="upper center")
plt.xlabel('BDT response')
plt.ylabel('Events')
plt.axvline(x=WP90_threshold, color="black", linestyle='--')
plt.axvline(x=WP80_threshold, color="black")
plt.text(0.5,0.7,'WP90: disc > %.4f'%WP90_threshold, fontsize=12, horizontalalignment='center', verticalalignment='center', transform=plt.gca().transAxes)
plt.text(0.5,0.6,'WP80: disc > %.4f'%WP80_threshold, fontsize=12, horizontalalignment='center', verticalalignment='center', transform=plt.gca().transAxes)
plt.savefig(plotDir+'training/mydiscriminator_' + test_name +'_logY.png')

plt.figure()
plt.hist(disc_signal, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="WWZ - test")
plt.hist(disc_signal_train, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="WWZ  - train")
plt.hist(disc_bkg, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="ZZ - test")
plt.hist(disc_bkg_train, density=True, bins=100, alpha=1.0, histtype="step", lw=2, label="ZZ - train")
plt.yscale("linear")
plt.xlim([0.0, 1.0])
#plt.ylim([0.001, 100.0])
plt.legend(loc="upper center")
plt.xlabel('BDT response')
plt.ylabel('Events')
plt.axvline(x=WP90_threshold, color="black", linestyle='--')
plt.axvline(x=WP80_threshold, color="black")
plt.text(0.5,0.7,'WP90: disc > %.4f'%WP90_threshold, fontsize=12, horizontalalignment='center', verticalalignment='center', transform=plt.gca().transAxes)
plt.text(0.5,0.6,'WP80: disc > %.4f'%WP80_threshold, fontsize=12, horizontalalignment='center', verticalalignment='center', transform=plt.gca().transAxes)
plt.savefig(plotDir+'training/mydiscriminator_' + test_name +'_linY.png')

os.system("chmod 755 "+plotDir+"training/*")


#plot roc curve
plt.figure()
lw = 2
plt.plot(fpr, tpr, color='darkorange',
         lw=lw, label='ROC curve')
plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.ylabel('Signal Efficiency')
plt.xlabel('Background Efficiency')
plt.axhline(y=0.9, color="black", linestyle='--')
plt.text(0.5,0.1,'WP80: bkg eff = %.4f'%WP80_effBkg, fontsize=12)
plt.text(0.5,0.2,'WP90: bkg eff = %.4f'%WP90_effBkg, fontsize=12)
#plt.text(0.5,0.3,'WP90: S/sqrt(B) = %.2f'%WP90_significance, fontsize=12)
plt.text(0.5,0.3,'AUC = %.4f'%AUC, fontsize=12)
#plt.title('Receiver operating characteristic example')
#plt.legend(loc="lower right")
#plt.show()
plt.savefig(plotDir+'training/myroc_' + test_name + '.png')
os.system("chmod 755 "+plotDir+"training/*")

# Pickle dictionary using protocol 0.
output = open('../models/model_'+test_name+'.pkl', 'wb')
pickle.dump(model, output)
output.close()
model.get_booster().dump_model('../models/model_'+test_name+'.txt')
model.get_booster().save_model('../models/model_'+test_name+'.xgb')

print("example input and prediction....")
print("bkg sample 1st event input:")
print(df_bkg.values[0])
print("y_pred_bkg first 20 events")
print(y_pred_bkg[:20])

#plot feature importances

model.get_booster().feature_names = variables_names

xgb.plot_importance(model, max_num_features=len(variables), xlabel="F score (weight)")
plt.savefig(plotDir+'training/myImportances_Fscore_' + test_name + '.png')
os.system("chmod 755 "+plotDir+"training/*")

#xgb.plot_tree( model.get_booster() )
xgb.plot_tree( model )
fig = plt.gcf()
#fig.set_size_inches(500, 50)
plt.draw()
plt.savefig(plotDir+'training/myTree_' + test_name + '.png')
plt.savefig(plotDir+'training/myTree_' + test_name + '.svg')
os.system("chmod 755 "+plotDir+"training/*")
