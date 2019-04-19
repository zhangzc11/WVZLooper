#g++ -g -std=c++1y  Controll.C makeHists.C Analysis.C -I$ROOTSYS/include `root-config --libs ` -lMinuit -o Analysis.exe

./Analysis.exe Filelist_bkg.list MC
rm -r results
mkdir results
mv MC*results.root results
chmod 755 results
chmod 755 results/*
cd results
sh ../hadd.sh
cd ..
python makeplot.py
rm -r ../public_html/plots6
cp -r plots ../public_html/plots6
chmod 755 ../public_html/plots6
chmod 755 ../public_html/plots6/*
