### package installation for xgboost training

first, go to a directory where you want to install anaconda - it takes about 7G space...

install anaconda - you only need to do this once!

```
cp /nfs-7/userdata/zhicaiz/Anaconda3-2019.03-Linux-x86_64.sh ./
./Anaconda3-2019.03-Linux-x86_64.sh
```

now you need to remove all the stuff in ~/.bashrc about conda: starting from >>> conda initialize >>> to the end

and then when you want to use conda, do:

```
source YOUR_DIR_TO_CONDA/etc/profile.d/conda.sh; conda activate
```

(don't source setup.sh in the main looper before you do this)

whne you want to get out of conda, do :

```
conda deactivate
```

then install the following packages...
install root for python3:

```
conda uninstall cfitsio
conda uninstall curl
conda uninstall krb5
conda install -c conda-forge krb5=1.16.3
conda install -c conda-forge curl=7.64.1
conda install -c conda-forge cfitsio=3.470
conda install -c conda-forge root=6.18.00
```

install other packages

```
pip install root_pandas
conda install -c conda-forge xgboost
pip install matplotlib
pip install uproot
conda install -c conda-forge tensorflow
conda install -c conda-forge keras
```


### To convert xgboost models into c++ header file, do

```
cd python
source convert_to_c.sh
```

this will generate the corresponding c++ header files that can be directly used to evaluate BDT on the fly in the looper, for example:

```
#include "wwz_vs_zz_emuHighTTZBDT.h"

// other code....
std::vector<float> test_sample_ZZ{bdt.met_pt(),bdt.lep3Pt(),bdt.lep4Pt(),bdt.ZPt(),bdt.lep3dZ(),bdt.lep4dZ(),bdt.lep3MT(),bdt.lep4MT(),bdt.lep34MT(),bdt.phi0(),bdt.theta0(),bdt.phi(),bdt.theta1(),bdt.theta2(),bdt.MllN(),bdt.pt_zeta(),bdt.pt_zeta_vis()};
float BDT_zz_emuHighTTZBDT = wwz_vs_zz_emuHighTTZBDT(test_sample_ZZ, true)[0]; // true means get probability, which is what we use to get BDT, false means to get weight

```

all the header files of the existing model can be found in models/cpp/ , with the corresponding input variables in python/train_*.py
