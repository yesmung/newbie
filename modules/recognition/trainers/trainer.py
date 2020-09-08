# -*- coding: utf-8 -*-
import os
import sys
import time
import random
import string
import argparse

import torch
import torch.backends.cudnn as cudnn
import torch.nn.init as init
import torch.optim as optim
import torch.utils.data
import torch.onnx
import pickle
import numpy as np
import io
import json

from base.base_trainer import BaseTrain

from models.utils import CTCLabelConverter, AttnLabelConverter, Averager
from data_loader.dataset import hierarchical_dataset, AlignCollate, Batch_Balanced_Dataset, inference_dataset
from models.model import Model
from trainers.test import validation, recog_loader

from tqdm import tqdm

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# mlflow lib
import mlflow
import mlflow.pytorch
import mlflow.pyfunc

from mlflow.tracking.client import MlflowClient
from mlflow.entities import ViewType


class Trainer(BaseTrain):
    def __init__(self, opt, data_loader, config, use_maindb=True):
        super(Trainer, self).__init__(opt, data_loader)
        self.config = config
        self.data_loader = data_loader
        self.opt = opt

        self.callbacks = []
        self.loss = []
        self.acc = []
        self.val_loss = []
        self.val_acc = []
        self.use_maindb = use_maindb

    def init_callbacks(self):
        self.callbacks.append(
            ModelCheckpoint(
                filepath=os.path.join(self.config.callbacks.checkpoint_dir,
                                      '%s-{epoch:02d}-{val_loss:.2f}.ckpt' % self.config.exp.name),
                monitor=self.config.callbacks.checkpoint_monitor,
                mode=self.config.callbacks.checkpoint_mode,
                save_best_only=self.config.callbacks.checkpoint_save_best_only,
                save_weights_only=self.config.callbacks.checkpoint_save_weights_only,
                verbose=self.config.callbacks.checkpoint_verbose,
            )
        )
        self.callbacks.append(
            TensorBoard(
                log_dir=self.config.callbacks.tensorboard_log_dir,
                write_graph=self.config.callbacks.tensorboard_write_graph,
            )
        )

    def train(self):
        # activate ml run
        mlflow.ActiveRun(self.config.mlrun)

        # load baseparams
        print(self)
        opt = self.opt
        config = self.config
        os.makedirs(config.exp.path + config.exp.name, exist_ok=True)

        """ dataset preparation"""
        opt.select_data = opt.select_data.split(',')
        opt.batch_ratio = opt.batch_ratio.split(',')

        train_dataset = Batch_Balanced_Dataset(opt, config)
        AlignCollate_valid = AlignCollate(imgH=opt.imgH, imgW=opt.imgW, keep_ratio_with_pad=opt.PAD)

        valid_dataset = hierarchical_dataset(root='', opt=opt, select_data=[opt.valid_data])

        valid_loader = torch.utils.data.DataLoader(
            valid_dataset,
            batch_size=opt.batch_size,
            shuffle=True,  # 'True' to check training progress with validation function.
            num_workers=int(opt.workers),
            collate_fn=AlignCollate_valid,
            pin_memory=True)

        print('-' * 80)

        """ model configuration """
        if 'CTC' in opt.Prediction:
            converter = CTCLabelConverter(opt.character)
        else:
            converter = AttnLabelConverter(opt.character)
        opt.num_class = len(converter.character)

        if opt.rgb:
            opt.input_channel = 3
        model = Model(opt)
        print('model input parameters', opt.imgH, opt.imgW, opt.num_fiducial, opt.input_channel, opt.output_channel,
              opt.hidden_size, opt.num_class, opt.batch_max_length, opt.Transformation, opt.FeatureExtraction,
              opt.SequenceModeling, opt.Prediction)

        # weight initialization
        for name, param in model.named_parameters():
            if 'localization_cf2' in name:
                print(f'Skip {name} as it is already initialized')
                continue
            try:
                if 'bias' in name:
                    init.constant_(param, 0.0)
                elif 'weight' in name:
                    init.kaiming_normal_(param)
            except Exception as e:  # for batchnorm.
                if 'weight' in name:
                    param.data.fill_(1)
                continue

        # data parallel for multi-GPU
        model = torch.nn.DataParallel(model).to(device)
        model.train()
        if opt.continue_model != '':
            print('-' * 20)
            print(f'loading pretrained model from {opt.continue_model}')
            model.load_state_dict(torch.load(opt.continue_model))
            print('-' * 20)
        print("Model:")
        print(model)

        # mlflow log
        model_summary_path = self.config.META_DB.path + self.config.META_DB.name + '/model_summary.txt'
        print(model, file=open(model_summary_path, 'a'))
        mlflow.log_artifact(model_summary_path)

        """ setup loss """
        if 'CTC' in opt.Prediction:
            criterion = torch.nn.CTCLoss(zero_infinity=True).to(device)
        else:
            criterion = torch.nn.CrossEntropyLoss(ignore_index=0).to(device)  # ignore [GO] token = ignore index 0

        # loss averager
        loss_avg = Averager()

        # filter that only require gradient decent
        filtered_parameters = []
        params_num = []
        for p in filter(lambda p: p.requires_grad, model.parameters()):
            filtered_parameters.append(p)
            params_num.append(np.prod(p.size()))
        print('Trainable params num : ', sum(params_num))

        # setup optimizer
        if opt.adam:
            optimizer = optim.Adam(filtered_parameters, lr=opt.lr, betas=(opt.beta1, 0.999))
        elif opt.adadelta:
            optimizer = optim.Adadelta(filtered_parameters, lr=opt.lr, rho=opt.rho, eps=opt.eps)
        elif opt.sgd:
            if opt.continue_model != "":
                optimizer = optim.SGD(filtered_parameters, lr=opt.lr, momentum=0.9, nesterov=True)
            else:
                optimizer = optim.SGD(filtered_parameters, lr=opt.lr, momentum=0.9, nesterov=False)

        print(">> Optimizer:")
        print(optimizer)

        """ final options """
        with open(f'{config.exp.path + config.exp.name}/opt.txt', 'a') as opt_file:
            opt_log = '--------------- Options ---------------\n'
            args = vars(opt)
            for k, v in args.items():
                opt_log += f'{str(k)}: {str(v)}\n'
            opt_log += '--------------------------------------\n'
            print(opt_log)
            opt_file.write(opt_log)

        """ start training """
        start_iter = 0

        # log mlflow model
        pbpath = config.exp.path + config.exp.name + '/model'
        os.makedirs(pbpath, exist_ok=True)
        tspath = pbpath + '/model.pth'
        optpath = pbpath + '/opt.json'
        torch.save(model.state_dict(), tspath)
        with open(optpath, 'w') as file:
            json.dump(opt, file)
        print(tspath)
        print(pbpath)
        mlflow.pyfunc.log_model('model', loader_module='./recog_loader.py', data_path=pbpath)

        start_time = time.time()
        best_accuracy = -1
        best_norm_ED = 1e+6
        i = start_iter
        tq = tqdm(total=opt.num_iter)
        while (True):
            tq.update(1)

            # train part
            image_tensors, labels = train_dataset.get_batch()
            image = image_tensors.to(device)
            text, length = converter.encode(labels, batch_max_length=opt.batch_max_length)
            batch_size = image.size(0)

            if 'CTC' in opt.Prediction:
                preds = model(image, text).log_softmax(2)
                preds_size = torch.IntTensor([preds.size(1)] * batch_size).to(device)
                preds = preds.permute(1, 0, 2)  # to use CTCLoss format

                # To avoid ctc_loss issue, disabled cudnn for the computation of the ctc_loss
                # https://github.com/jpuigcerver/PyLaia/issues/16
                torch.backends.cudnn.enabled = False
                cost = criterion(preds, text, preds_size, length)
                torch.backends.cudnn.enabled = True
            else:
                preds = model(image, text[:, :-1])  # align with Attention.forward
                target = text[:, 1:]  # without [GO] Symbol
                cost = criterion(preds.view(-1, preds.shape[-1]), target.contiguous().view(-1))

            model.zero_grad()
            cost.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), opt.grad_clip)  # gradient clippping with 5 (Default)
            optimizer.step()

            loss_avg.add(cost)

            # validation part
            if i % opt.valInterval == 0:
                elapsed_time = time.time() - start_time
                print(f'[{i}/{opt.num_iter}] Loss: {loss_avg.val():0.5f} elapsed_time: {elapsed_time:0.5f}')
                mlflow.log_metrics({'Loss': np.double(loss_avg.val()), 'elapsed_time': elapsed_time}, step=i)

                # for log
                with open(f'{config.exp.path + config.exp.name}/log_train.txt', 'a') as log:
                    log.write(f'[{i}/{opt.num_iter}] Loss: {loss_avg.val():0.5f} elapsed_time: {elapsed_time:0.5f}\n')
                    loss_avg.reset()

                    model.eval()
                    with torch.no_grad():
                        valid_loss, current_accuracy, current_norm_ED, preds, labels, infer_time, lengh_of_data = validation(
                            model, criterion, valid_loader, converter, opt)
                    model.train()

                    mltag = ''
                    for pred, gt in zip(preds[:5], labels[:5]):
                        if 'Attn' in opt.Prediction:
                            pred = pred[:pred.find('[s]')]
                            gt = gt[:gt.find('[s]')]
                        print(f'{pred:20s}, gt: {gt:20s}, {str(pred == gt)}')
                        mltag = mltag + f'{pred:20s}, gt: {gt:20}, {str(pred == gt)}\n'
                        log.write(f'{pred:20s}, gt: {gt:20s}, {str(pred == gt)}\n')
                    mlflow.set_tag(f'{i}/{opt.num_iter}', mltag)

                    valid_log = f'[{i}/{opt.num_iter}] valid loss : {valid_loss:0.5f}'
                    valid_log = f' accuracy: {current_accuracy:0.3f}, norm_ED: {current_norm_ED:0.2f}'
                    print(valid_log)
                    mlflow.log_metrics({'valid_loss': np.double(valid_loss), 'accuracy': np.double(current_accuracy),
                                        'norm_ED': np.double(current_norm_ED)}, step=i)
                    log.write(valid_log + '\n')

                    # keep best accuracy model
                    if current_accuracy > best_accuracy:
                        best_accuracy = current_accuracy
                        # log mlflow model
                        pbpath = config.exp.path + config.exp.name + '/best_accuracy'
                        os.makedirs(pbpath, exist_ok=True)
                        tspath = pbpath + '/model.pth'
                        optpath = pbpath + '/opt.json'
                        torch.save(model.state_dict(), tspath)
                        with open(optpath, 'w') as file:
                            json.dump(opt, file)

                        mlflow.pyfunc.log_model('model', loader_module='./recog_loader.py', data_path=pbpath)

                    if current_norm_ED < best_norm_ED:
                        best_norm_ED = current_norm_ED
                        torch.save(model.state_dict(), f'{config.exp.path + config.exp.name}/best_norm_ED.pth')
                        # log mlflow model
                        pbpath = config.exp.path + config.exp.name + '/best_norm_ED'
                        os.makedirs(pbpath, exist_ok=True)
                        tspath = pbpath + '/model.pth'
                        optpath = pbpath + '/opt.json'
                        torch.save(model.state_dict(), tspath)
                        with open(optpath, 'w') as file:
                            json.dump(opt, file)

                        mlflow.pyfunc.log_model('model', loader_module='./recog_loader.py', data_path=pbpath)

                    best_model_log = f'best_accuracy: {best_accuracy:0.3f}, best_norm_ED: {best_norm_ED:0.2f}'
                    print(best_model_log)

                    log.write(best_model_log + '\n')
            if i == opt.num_iter:
                # log mlflow model
                pbpath = config.exp.path + config.exp.name + '/model'
                os.makedirs(pbpath, exist_ok=True)
                tspath = pbpath + '/model.pth'
                optpath = pbpath + '/opt.json'
                torch.save(model.state_dict(), tspath)
                with open(optpath, 'w') as file:
                    json.dump(opt, file)

                mlflow.pyfunc.log_model('model', loader_module='./recog_loader.py', data_path=pbpath)
                print('end the training')
                sys.exit()
            i += 1

    def inference(self):
        # get opts
        opt = self.opt
        config = self.config

        """ model configuration """
        if 'CTC' in opt.Prediction:
            converter = CTCLabelConverter(opt.character)
        else:
            converter = AttnLabelConverter(opt.character)
        opt.num_class = len(converter.character)

        if opt.rgb:
            opt.input_channel = 3
        model = Model(opt)
        print('model input parameters', opt.imgH, opt.imgW, opt.num_fiducial, opt.input_channel, opt.output_channel,
              opt.hidden_size, opt.num_class, opt.batch_max_length, opt.Transformation, opt.FeatureExtraction,
              opt.SequenceModeling, opt.Prediction
              )
        if opt.cuda:
            model = torch.nn.DataParallel(model).to(device)
        else:
            model = torch.nn.DataParallel(model).to(device)
            model = model.cpu()

        # load model
        print('loading pretrained model from %s' % opt.saved_model)
        if opt.cuda:
            model.load_state_dict(torch.load(opt.saved_model))
        else:
            model.load_state_dict(torch.load(opt.saved_model, map_location=lambda storage, loc: storage))

        # prepare data.
        AlignCollate_demo = AlignCollate(imgH=opt.imgH, imgW=opt.imgW, keep_ratio_with_pad=opt.PAD)

        prefixs = config.META_DB.target_prefix
        prefixs = prefixs.split(',')
        select_data = config.META_DB.dblist
        select_data = select_data.split(',')
        demo_data = inference_dataset(root=opt.train_data, opt=opt, select_data=select_data, prefixs=prefixs,
                                      use_maindb=self.use_maindb)
        print(demo_data)
        demo_loader = torch.utils.data.DataLoader(demo_data,
                                                  batch_size=opt.batch_size,
                                                  shuffle=False,
                                                  num_workers=int(opt.workers),
                                                  collate_fn=AlignCollate_demo,
                                                  pin_memory=True)

        # predict
        model.eval()

        current_time = time.time()
        tq = tqdm(total=len(demo_data))
        with torch.no_grad():
            recogresult = []
            for image_tensors, image_path_list in demo_loader:
                batch_size = image_tensors.size(0)
                image = image_tensors.size(0)
                image = image_tensors.to(device)

                # For max length prediction
                length_for_pred = torch.IntTensor([opt.batch_max_length] * batch_size).to(device)
                text_for_pred = torch.LongTensor(batch_size, opt.batch_max_length).fill_(0).to(device)
                preds = model(image, text_for_pred, is_train=False)

                # select max probability (greedy decoding) then decode index to character
                pred_acc, preds_index = preds.max(2)
                preds_str = converter.decode(preds_index, length_for_pred)

                for img_name, pred, acc in zip(image_path_list, preds_str, pred_acc):
                    tq.update(1)

                    pred_result = pred[:pred.find('[s]')]  # prune after "end of sentence" token([s])
                    pacc = acc[:pred.find('[s]')]

                    recogresult.append((img_name, pred_result, pacc))

        return recogresult
