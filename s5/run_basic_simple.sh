#!/bin/bash

# Warning-- this recipe is now out of date.  See ../s5c/ for the latest recipe.


. ./path.sh
. ./cmd.sh

stage=1
nj=2                                                                    
data=./speechDATA
lang=lm
dict=dict
train_data=train_data_01
test_data=test_data_01
gmm_model_name=gmm_ms
tdnn_model_name=tdnn_base_ms

if [ $stage -le -1 ]; then
  echo -e "\033[33;40m ..... [START] 데이터 다운로드 및 압축해제\033[0m"
  local/download_from_openslr.sh ./speechDATA 
  echo -e "\033[33;40m ..... [END] 데이터 다운로드 및 압축해제\033[0m"
fi

if [ $stage -le 0 ]; then
  echo -e "\033[33;40m ..... [START] 데이터 준비\033[0m"
  for part in train_data_01 test_data_01; do
    # use underscore-separated names in data directories.
    local/data_prep.sh $data/$part data/$(echo $part | sed s/-/_/g)
  done
  echo -e "\033[33;40m ..... [END] 데이터 준비\033[0m"
fi

if [ $stage -le 1 ]; then
  echo -e "\033[33;40m ..... [START] 필수 데이터 생성 : lang 파트\033[0m"
  # update segmentation of transcripts
  for part in train_data_01 test_data_01; do
    local/updateSegmentation.sh data/$part data/local/lm
  done
  echo -e "\033[33;40m ..... [END] 필수 데이터 생성 : lang 파트\033[0m"
fi


exit


if [ $stage -le 2 ]; then
  echo -e "\033[33;40m ..... [START] 언어 모델 생성 : dictionary\033[0m"
  # prepare dictionary and language model 
  local/prepare_dict.sh data/local/lm data/local/dict
  echo -e "\033[33;40m ..... [END] 언어 모델 생성 : dictionary\033[0m"
fi

if [ $stage -le 3 ]; then
  echo -e "\033[33;40m ..... [START] lang (L.fst 등)\033[0m"
  utils/prepare_lang.sh data/local/dict "!SIL" data/local/lang_tmp data/lang_final
  echo -e "\033[33;40m ..... [END] lang (L.fst 등)\033[0m"
fi

if [ $stage -le 4 ]; then
  echo -e "\033[33;40m ..... [START] lang (G.fst 등)\033[0m"
  local/format_lms.sh --src-dir data/lang_final data/local/lm
  echo -e "\033[33;40m ..... [END] lang (G.fst 등)\033[0m"
fi

## mfccdir should be some place with a largish disk where you
if [ $stage -le 5 ]; then
## want to store MFCC features.
  mfccdir=`pwd`/mfcc
  echo -e "\033[33;40m ..... [START] Feature extraction - MFCC (train data) \033[0m"
  steps/make_mfcc.sh --cmd "$train_cmd" --nj $nj data/$train_data exp/make_mfcc_dir/$train_data $mfccdir || exit 1;
  echo -e "\033[33;40m ..... [END] Feature extraction - MFCC (train data) \033[0m"

  echo -e "\033[33;40m ..... [START] CMVN (train data) \033[0m"
  steps/compute_cmvn_stats.sh data/$train_data exp/make_mfcc_dir/$train_data $mfccdir || exit 1;
  echo -e "\033[33;40m ..... [END] CMVN (train data) \033[0m"

  ## mfccdir should be some place with a largish disk where you
  ## want to store MFCC features.
  echo -e "\033[33;40m ..... [START] Feature extraction - MFCC (test data) \033[0m"
  steps/make_mfcc.sh --cmd "$train_cmd" --nj $nj data/$test_data exp/make_mfcc_dir/$test_data $mfccdir || exit 1;
  echo -e "\033[33;40m ..... [END] Feature extraction - MFCC (test data) \033[0m"
  
    echo -e "\033[33;40m ..... [START] CMVN (test data) \033[0m"
  steps/compute_cmvn_stats.sh data/$test_data exp/make_mfcc_dir/$test_data $mfccdir || exit 1;
    echo -e "\033[33;40m ..... [END] CMVN (test data) \033[0m"
fi


if [ $stage -le 6 ]; then
  echo -e "\033[33;40m ..... [START] Train monophone \033[0m"
  steps/train_mono.sh --nj $nj --cmd "$train_cmd" \
    data/$train_data data/lang_final exp/mono0a_"$gmm_model_name" || exit 1;
  echo -e "\033[33;40m ..... [END] Train monophone \033[0m"

  echo -e "\033[33;40m ..... [START] Align monophone \033[0m"
  steps/align_si.sh --nj $nj --cmd "$train_cmd" \
     data/$train_data data/lang_final exp/mono0a_"$gmm_model_name" exp/mono0a_"$gmm_model_name"_ali || exit 1;
  echo -e "\033[33;40m ..... [END] Align monophone \033[0m"
fi
exit

if [ $stage -le 7 ]; then
  echo -e "\033[33;40m ..... [START] Train triphone - 1\033[0m"
  steps/train_deltas.sh --cmd "$train_cmd" \
    2500 20000 data/$train_data data/lang_final exp/mono0a_"$gmm_model_name"_ali exp/tri1_"$gmm_model_name" || exit 1;
  echo -e "\033[33;40m ..... [END] Train triphone - 1\033[0m"
 # utils/mkgraph.sh $data/lang_test exp/tri1_ai_10 exp/tri1_ai/graph

  echo -e "\033[33;40m ..... [START] Align triphone - 1\033[0m"
  steps/align_si.sh --nj $nj --cmd "$train_cmd" \
   data/$train_data data/lang_final exp/tri1_"$gmm_model_name" exp/tri1_"$gmm_model_name"_ali || exit 1;
  echo -e "\033[33;40m ..... [END] Align triphone - 1\033[0m"
fi

if [ $stage -le 8 ]; then
  echo -e "\033[33;40m ..... [START] Train triphone - 2\033[0m"
  steps/train_deltas.sh --cmd "$train_cmd" \
    2500 20000 data/$train_data data/lang_final exp/tri1_"$gmm_model_name"_ali exp/tri2_"$gmm_model_name" || exit 1;
  echo -e "\033[33;40m ..... [END] Train triphone - 2\033[0m"

  echo -e "\033[33;40m ..... [START] Align triphone - 2\033[0m"
  steps/align_si.sh --nj $nj --cmd "$train_cmd" \
    data/$train_data data/lang_final exp/tri2_"$gmm_model_name" exp/tri2_"$gmm_model_name"_ali || exit 1;
  echo -e "\033[33;40m ..... [END] Align triphone - 2\033[0m"
fi

echo -e "\033[33;40m ..... READY TO TRAIN TDNN\033[0m"
#exit

if [ $stage -le 9 ]; then
  echo -e "\033[33;40m ..... [START] Train TDNN\033[0m"
  local/nnet3/run_tdnn_base.sh data/$train_data exp/tri2_"$gmm_model_name" \
      exp/tri2_"$gmm_model_name"_ali exp/nnet3/$tdnn_model_name  || exit 1;
  echo -e "\033[33;40m ..... [END] Train TDNN\033[0m"
fi
echo -e "\033[33;40m ..... END TRAIN TDNN\033[0m"
exit

acwt=0.083
decodef=decode_"$testfile"_"$graph"_acwt"$acwt"   #_"$info"

echo "PATH:"$PATH

if [ $stage -le 10 ]; then
  echo -e "\033[33;40m ..... [START] Make graph\033[0m"
  utils/mkgraph.sh data/lang_final_test_tgsmall exp/nnet3/tdnn_base_ms exp/nnet3/tdnn_base_ms/ms_graph || exit 1;
  echo -e "\033[33;40m ..... [END] Make graph\033[0m"
fi

if [ $stage -le 11 ]; then
  echo -e "\033[33;40m ..... [START] Decoding\033[0m"
  steps/nnet3/decode.sh --nj 4 --cmd "$decode_cmd"  --num-threads 2 --use-gpu false\
      exp/nnet3/tdnn_base_ms/ms_graph data/test_data_01 exp/nnet3/tdnn_base_ms/$decodef || exit 1
  echo -e "\033[33;40m ..... [END] Decoding\033[0m"
fi