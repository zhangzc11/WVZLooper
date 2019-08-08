#!/bin/bash


INPUTDIR=$1 #/nfs-7/userdata/zhicaiz/babies/
version=$2

for year in 2018 2016 2017 
do 
	rm cmd_${year}.sh
	THISINPUTDIR=${INPUTDIR}WVZ${year}_${version}
	for i in $(ls ${THISINPUTDIR}); do
		if [ $year == '2018' ]; then
			echo python append_xgboost_discriminator_to_tree.py ${THISINPUTDIR} $i yes >> cmd_${year}.sh
		else
			echo python append_xgboost_discriminator_to_tree.py ${THISINPUTDIR} $i no >> cmd_${year}.sh
		fi
	done
done

##then do the following:
#source cmd_2016.sh
#source cmd_2017.sh
#source cmd_2018.sh
