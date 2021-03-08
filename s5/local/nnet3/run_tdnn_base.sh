#!/bin/bash

#    This is the standard "tdnn" system, built in nnet3 with xconfigs.


# local/nnet3/compare_wer.sh exp/nnet3/tdnn1a_sp
# System                tdnn1a_sp
#WER dev93 (tgpr)                9.18
#WER dev93 (tg)                  8.59
#WER dev93 (big-dict,tgpr)       6.45
#WER dev93 (big-dict,fg)         5.83
#WER eval92 (tgpr)               6.15
#WER eval92 (tg)                 5.55
#WER eval92 (big-dict,tgpr)      3.58
#WER eval92 (big-dict,fg)        2.98
# Final train prob        -0.7200
# Final valid prob        -0.8834
# Final train acc          0.7762
# Final valid acc          0.7301

set -e -o pipefail -u

# First the options that are passed through to run_ivector_common.sh
# (some of which are also used in this script directly).
stage=-10
nj=8
lang=lang_final
train_set=$1
test_sets="test_data_01"
gmm=$2       # this is the source gmm-dir that we'll use for alignments; it
                 # should have alignments for the specified training data.
num_threads_ubm=16
nnet3_affix=       # affix for exp dirs, e.g. it was _cleaned in tedlium.
tdnn_affix=1a  #affix for TDNN directory e.g. "1a" or "1b", in case we change the configuration.

# Options which are not passed through to run_ivector_common.sh
train_stage=-4   #-4
remove_egs=true
srand=0
reporting_email=
# set common_egs_dir to use previously dumped egs.
common_egs_dir=

. ./cmd.sh
. ./path.sh
. ./utils/parse_options.sh

if ! cuda-compiled; then
  cat <<EOF && exit 1
This script is intended to be used with GPUs but you have not compiled Kaldi with CUDA
If you want to use GPUs (and have them), go to src/, and configure and make on a machine
where "nvcc" is installed.
EOF
fi

#local/nnet3/run_ivector_common.sh --stage $stage --nj $nj \
#                                  --train-set $train_set --gmm $gmm \
#                                  --num-threads-ubm $num_threads_ubm \
#                                  --nnet3-affix "$nnet3_affix"



gmm_dir=${gmm}
ali_dir=$3
dir=$4 #exp/nnet3/tdnn_ai_all_ep30_lr0001
#egs_dir=exp/nnet3/tdnn_train_ai_all_qaset_noisemix_sp_ep70_lr0001/egs
train_data_dir=${train_set}
train_ivector_dir=exp/nnet3${nnet3_affix}/ivectors_${train_set}_sp_hires

for f in $train_data_dir/feats.scp \
    $ali_dir/ali.1.gz $gmm_dir/final.mdl; do
  [ ! -f $f ] && echo "$0: expected file $f to exist" && exit 1
done



if [ $stage -le 12 ]; then
  mkdir -p $dir
  echo "$0: creating neural net configs using the xconfig parser";

  num_targets=$(tree-info $gmm_dir/tree |grep num-pdfs|awk '{print $2}')
  echo "num_targets: $num_targets" 

  mkdir -p $dir/configs
  cat <<EOF > $dir/configs/network.xconfig
  #input dim=100 name=ivector
  #input dim=13 name=input
  input dim=39 name=input
  # please note that it is important to have input layer with the name=input
  # as the layer immediately preceding the fixed-affine-layer to enable
  # the use of short notation for the descriptor
  #fixed-affine-layer name=lda input=Append(-2,-1,0,1,2,ReplaceIndex(ivector, t, 0)) affine-transform-file=$dir/configs/lda.mat

                                                                      
  relu-renorm-layer name=tdnn1 dim=650 input=Append(-2,-1,0,1,2)
  relu-renorm-layer name=tdnn2 dim=650 input=Append(-1,0,2)
  relu-renorm-layer name=tdnn3 dim=650 input=Append(-3,0,4)
  relu-renorm-layer name=tdnn4 dim=650 input=Append(-7,0,2)
  relu-renorm-layer name=tdnn5 dim=650 
  
  output-layer name=output dim=$num_targets max-change=1.5
EOF
  steps/nnet3/xconfig_to_configs.py --xconfig-file $dir/configs/network.xconfig --config-dir $dir/configs/
fi





if [ $stage -le 13 ]; then
  #if [[ $(hostname -f) == *.clsp.jhu.edu ]] && [ ! -d $dir/egs/storage ]; then
  #  utils/create_split_dir.pl \
  #   /export/b0{3,4,5,6}/$USER/kaldi-data/egs/tedlium-$(date +'%m_%d_%H_%M')/s5_r2/$dir/egs/storage $dir/egs/storage
  #fi


  steps/nnet3/train_dnn.py --stage=$train_stage \
    --cmd="$train_cmd" \
    --feat.cmvn-opts="--norm-means=false --norm-vars=false" \
    --trainer.srand=$srand \
    --trainer.max-param-change=2.0 \
    --trainer.num-epochs=6 \
    --trainer.optimization.initial-effective-lrate=0.0001 \
    --trainer.optimization.final-effective-lrate=0.000015 \
    --trainer.optimization.minibatch-size=16 \
    --trainer.optimization.num-jobs-final=1 \
    --trainer.optimization.num-jobs-initial=1 \
    --egs.dir=$common_egs_dir \
    --cleanup.remove-egs=$remove_egs \
    --use-gpu="true" \
    --ali-dir=$ali_dir \
    --lang=data/lang_final \
    --feat-dir=$train_data_dir\
    --dir=$dir  || exit 1;

    #--trainer.optimization.minibatch-size=256,128 \
    #--trainer.samples-per-iter=10000 \
fi


exit 0;
if [ $stage -le 14 ]; then
  # note: for TDNNs, looped decoding gives exactly the same results
  # as regular decoding, so there is no point in testing it separately.
  # We use regular decoding because it supports multi-threaded (we just
  # didn't create the binary for that, for looped decoding, so far).
  rm $dir/.error || true 2>/dev/null
  for data in $test_sets; do
    (
      data_affix=$(echo $data | sed s/test_//)
      nj=$(wc -l <data/${data}_hires/spk2utt)
      for lmtype in tgpr bd_tgpr; do
        graph_dir=$gmm_dir/graph_${lmtype}
        steps/nnet3/decode.sh --nj $nj --cmd "$decode_cmd"  --num-threads 4 \
           --online-ivector-dir exp/nnet3${nnet3_affix}/ivectors_${data}_hires \
          ${graph_dir} data/${data}_hires ${dir}/decode_${lmtype}_${data_affix} || exit 1
      done
      steps/lmrescore.sh --cmd "$decode_cmd" data/lang_test_{tgpr,tg} \
        data/${data}_hires ${dir}/decode_{tgpr,tg}_${data_affix} || exit 1
      steps/lmrescore_const_arpa.sh --cmd "$decode_cmd" \
        data/lang_test_bd_{tgpr,fgconst} \
       data/${data}_hires ${dir}/decode_${lmtype}_${data_affix}{,_fg} || exit 1
    ) || touch $dir/.error &
  done
  wait
  [ -f $dir/.error ] && echo "$0: there was a problem while decoding" && exit 1
fi
exit 0;
