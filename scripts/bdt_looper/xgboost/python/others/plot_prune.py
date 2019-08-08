import numpy as np
import matplotlib.pyplot as plt
import os

test_name = 'xgb_wwz_vs_ttz_nbAll_full'

plotDir = "/home/users/zhicaiz/public_html/WWZ/BDT/"

name = []
AUC = []
with open("result_prune_ttZ.txt") as f:
    lines = f.readlines()
    for line in lines:
        line_items = line.strip('\n').split()
        name.append(line_items[0])
        AUC.append(float(line_items[1]))

plt.figure()
plt.plot(name, AUC, lw=2)
plt.xticks(rotation=90)
plt.xlabel('cumulative removed features (left to right)')
plt.ylabel('AUC after removal')
plt.savefig(plotDir+'training/AUC_vs_removed_features_'+test_name+'.png', bbox_inches='tight')
os.system("chmod 755 "+plotDir+"training/*")
