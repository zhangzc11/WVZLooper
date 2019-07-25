
for model in \
wwz_vs_zz_emuHighTTZBDT \
wwz_vs_zz_OffZ \
wwz_vs_zz_emu \
wwz_vs_zz_OffZHighTTZBDT \
wwz_vs_ttzzz_emu \
wwz_vs_ttzzz_bVeto \
wwz_vs_ttzzz_OffZ \
wwz_vs_ttz_nbAll \
wwz_vs_ttz_emu \
wwz_vs_ttz_bVeto \
wwz_vs_ttz_OffZ 

do
	python raw_to_cpp.py --xgb_dump=../models/model_xgb_${model}.txt --num_classes=1 --func_name=${model}
	mv ${model}.h ../models/cpp/
done
