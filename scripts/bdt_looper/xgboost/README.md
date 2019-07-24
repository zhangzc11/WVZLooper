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

