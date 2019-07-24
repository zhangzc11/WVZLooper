rm 4l2v.root incl.root
# ./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2018_v0.1.12.6/wwz_4l2v_amcatnlo_1.root  -t t -o 4l2v.root
# ./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2018_v0.1.12.6/wwz_amcatnlo_1.root  -t t -o incl.root
# ./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2017_v0.1.12.7/wwz_4l2v_amcatnlo_1.root  -t t -o 4l2v.root
# ./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2017_v0.1.12.7/wwz_amcatnlo_1.root  -t t -o incl.root
./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2018_v0.1.12.7/wwz_4l2v_amcatnlo_1.root  -t t -o 4l2v.root
./doAnalysis -i /nfs-7/userdata/phchang/babies/WVZ2018_v0.1.12.7/wwz_amcatnlo_1.root  -t t -o incl.root
python plot.py
