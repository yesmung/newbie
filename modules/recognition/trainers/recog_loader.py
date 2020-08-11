# -*- coding: utf-8 -*-

import os
import time
import string
import argparse
import json

import torch
import torch.backends.cudnn as cudnn
import torch.utils.data
import numpy as np
from nltk.metrics.distance import edit_distance

from models.utils import CTCLabelConverter, AttnLabelConverter, Averager
from data_loader.dataset import hierarchical_dataset, AlignCollate
from models.model import Model

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

def recog_loader(data_path):
    with open(data_path+'/opt.json', 'r') as file:
        opt = json.load(file)

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

    model = torch.nn.DataParallel(model).to(device)

    # load model
    print('loading pretrained model from %s' % opt.saved_model)
    model.load_state_dict(torch.load(data_path+'/model.pth'))
    return model