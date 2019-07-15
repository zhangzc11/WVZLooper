rm 4l2v.root incl.root
./doAnalysis -i /nfs-7/userdata/phchang/babies/Truth2018_v0.1.12.3/wwz_4l2v_amcatnlo_1.root  -t t -o 4l2v.root
./doAnalysis -i /nfs-7/userdata/phchang/babies/Truth2018_v0.1.12.3/wwz_amcatnlo_1.root  -t t -o incl.root
python plot.py
