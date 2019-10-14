if [ -z $1 ]; then echo "sh $0 COMBINECARD.txt"; exit; fi
 # combine -M ProfileLikelihood --significance $1 -t -1 --expectSignal=1 --rMin -50 --rMax 50
combine -M Significance $1 -t -1 --expectSignal=1
combine -M Significance $1 -t -1 --expectSignal=1 --pvalue
# combine -M ProfileLikelihood --significance $1 --rMin -50 --rMax 50
# combine -M Asymptotic $1
