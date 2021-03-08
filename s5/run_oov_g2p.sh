#! /bin/bash

#export KALDI_ROOT=/home2/mjlee/work/kaldi
export KALDI_ROOT=/media/myungsungkwak/msdisk/speech/kaldi
sequitur=$KALDI_ROOT/tools/sequitur-g2p/g2p.py

source $KALDI_ROOT/tools/env.sh

if [ $# != 1 ]; then
    echo "Usage: run_oov_g2p.sh <input file>"
    exit 1;
fi

##########################################################################
# oov_list      : input list of oov ($1)
# n_oov         : number of oov in the $oov_list
# n_handle_oov  : number of oov per process
# n_tmp_f       : number of file to split into of $oov_list
# start_line    : start line of oov_list for splitted file
# nj            : number of jobs
###########################################################################

oov_list=$1

declare -i n_oov
declare -i n_handle_oov
declare -i n_tmp_f
declare -i start_line
declare -i nj

n_oov=($(cat $1 | wc -l))
n_handle_oov=500
n_tmp_f=$(( $n_oov / $n_handle_oov))
nj=50


if [ $n_oov -gt $n_handle_oov ]; then
   if [ $(($n_oov % $n_handle_oov)) -eq 0 ]; then
      n_tmp_f=$(($n_tmp_f))
   else
      n_tmp_f=$(($n_tmp_f + 1))
   fi
 
##########################################################################
# split $oov_list into small oov_list (ex. oov_list.1 ... oov_list.10)
##########################################################################
   for n in $(seq $n_tmp_f); do
       if [ $n -eq $n_tmp_f ]; then
          start_line=$(($n * $n_handle_oov))
          if [ $(($n_oov % $n_handle_oov)) -eq 0 ]; then
              head -n $start_line $oov_list | tail -n $n_handle_oov \
                  > $oov_list.$n
          else
              head -n $start_line $oov_list | \
               tail -n $(($n_oov % $n_handle_oov)) > $oov_list.$n
          fi
       else
          start_line=$(($n * $n_handle_oov))
          head -n $start_line $oov_list | tail -n $n_handle_oov \
              > $oov_list.$n
       fi
   done

##########################################################################
# g2p process
##########################################################################
   for n in $(seq $n_tmp_f); do
        if [ $n -eq $n_tmp_f ]; then
           g2p.py --encoding=utf-8 --model=oov/model-kor-13 \
                  --apply=$oov_list.$n > oov/$oov_list.$n._res &
        else
           g2p.py --encoding=utf-8 --model=oov/model-kor-13 \
                  --apply=$oov_list.$n > oov/$oov_list.$n._res &
        fi
        if [ `jobs | wc -l` -ge $nj ]; then
           wait
        fi

   done
   wait
  
   for n in $(seq $n_tmp_f); do
          cat oov/$oov_list.$n._res >> oov/$oov_list._g2p
   done
else
    echo "oov_list : ${oov_list}"
    g2p.py --encoding=utf-8 --model=./oov/model-kor-13 \
           --apply=$oov_list > ./oov/oov._g2p
fi


