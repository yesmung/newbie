# -*- coding: utf-8 -*-

import os
import re
import six
import math
import lmdb
import torch

from configparser import ConfigParser
path_config_file = os.getcwd().split('docrv2_sroie')[0] + 'docrv2_sroie/' + 'config.ini'
common_variable = ConfigParser()
common_variable.read(path_config_file)
import sys
sys.path.append(common_variable['database']['module_path'])
from db_util import *
from db import *
DB_MAIN_PATH = common_variable['database']['DB_MAIN_PATH']

from natsort import natsorted
from PIL import Image, ImageOps

import numpy as np
from torch.utils.data import Dataset, ConcatDataset, Subset
from torch._utils import _accumulate
import torchvision.transforms as transforms

def decode_list(buf=None, sep='\t'):
    data = buf.decode().replace(sep, '').replace(' ','')
    return data.split('\r\n')[:-1]

class Batch_Balanced_Dataset(object):

    def __init__(self, opt, config):
        """
        Modulate the data ratio in the batch.
        For example, when select_data is "MJ-ST" and batch_ratio is "0.5-0.5",
        the 50% of the batch is filled with MJ and the other 50% of the batch is filled with ST.
        """

        # Set main db path
        if config.META_DB.DB_MAIN_PATH != '':
            DB_MAIN_PATH = config.META_DB.DB_MAIN_PATH
        print("-" * 80)
        print(f'dataset_root: {opt.train_data}\nopt.select_data: {opt.select_data}\nopt.batch_ratio: {opt.batch_ratio}')
        assert len(opt.select_data) == len(opt.batch_ratio)

        _AlignCollate = AlignCollate(imgH=opt.imgH, imgW=opt.imgW, keep_ratio_with_pad=opt.PAD)
        self.data_loader_list = []
        self.dataloader_iter_list = []
        batch_size_list = []
        Total_batch_size = 0
        for selected_d, batch_ratio_d in zip(opt.select_data, opt.batch_ratio):
            _batch_size = max(round(opt.batch_size * float(batch_ratio_d)), 1)
            print("-" * 80)
            _dataset = hierarchical_dataset(root=opt.train_data, opt=opt, select_data=[selected_d])
            total_number_dataset = len(_dataset)

            """
            The total number of data can be modified with opt.total_data_usage_ratio.
            ex) opt.total_data_usage_ratio = 1 indicates 100% usage, and 0.2 indicates 20% usage.
            See 4.2 section in our paper.
            """
            number_dataset = int(total_number_dataset * float(opt.total_data_usage_ratio))
            dataset_split = [number_dataset, total_number_dataset - number_dataset]
            indices = range(total_number_dataset)
            _dataset, _ = [Subset(_dataset, indices[offset - length:offset])
                           for offset, length in zip(_accumulate(dataset_split), dataset_split)]
            selected_d_log = f'num total samples of {selected_d}: {total_number_dataset} x {opt.total_data_usage_ratio} (total_data_usage_ratio) = {len(_dataset)}\n'
            selected_d_log += f'num samples of {selected_d} per batch: {opt.batch_size} x {float(batch_ratio_d)} (batch_ratio) = {_batch_size}'
            print(selected_d_log)
            batch_size_list.append(str(_batch_size))
            Total_batch_size += _batch_size

            _data_loader = torch.utils.data.DataLoader(_dataset, batch_size=_batch_size,
                                                       shuffle=True,
                                                       num_workers=int(opt.workers),
                                                       collate_fn=_AlignCollate,
                                                       pin_memory=True)
            self.data_loader_list.append(_data_loader)
            self.dataloader_iter_list.append(iter(_data_loader))
        print("-" * 80)
        print('Total_batch_size: ', '+'.join(batch_size_list), '=', str(Total_batch_size))
        opt.batch_size = Total_batch_size
        print("-" * 80)

    def get_batch(self):
        balanced_batch_images = []
        balanced_batch_texts = []

        for i, data_loader_iter in enumerate(self.dataloader_iter_list):
            try:
                image, text = data_loader_iter.next()
                balanced_batch_images.append(image)
                balanced_batch_texts += text
            except StopIteration:
                self.dataloader_iter_list[i] = iter(self.data_loader_list[i])
                image, text = self.dataloader_iter_list[i].next()
                balanced_batch_images.append(image)
                balanced_batch_texts += text
            except ValueError:
                pass

        balanced_batch_images = torch.cat(balanced_batch_images, 0)

        return balanced_batch_images, balanced_batch_texts


def hierarchical_dataset(root, opt, select_data='/'):
    """ select_data='/' contains all sub-directory of root directory """
    dataset_list = []
    print(f'dataset_root:     {root}\t dataset: {select_data[0]}')

    for dbname in select_data:
        dataset = LmdbDataset(dbname, opt)
        print(f'data-db-name:\t/{dbname}\t num samples: {len(dataset)}')
        dataset_list.append(dataset)

    concatenated_dataset = ConcatDataset(dataset_list)

    return concatenated_dataset

class LmdbDataset(Dataset):

    def __init__(self, root, opt):

        root = os.path.dirname(DB_MAIN_PATH) + '/' + root
        print(root)

        self.root = root
        self.opt = opt
        self.env = lmdb.open(root, max_readers=32, max_dbs=1000, readonly=True, lock=False, readahead=False, meminit=False)
        if not self.env:
            print('cannot create lmdb from %s' % (root))
            sys.exit(0)

        dbsummary = get_db_summary(env=self.env)
        nSamples = dbsummary['number of data']
        self.nSamples = nSamples
        self.filtered_index_list = [index for index in range(self.nSamples)]

    def __len__(self):
        return self.nSamples

    def __getitem__(self, index):
        assert index <= len(self), 'index range error'
        index = self.filtered_index_list[index]

        db = self.env.open_db(b'db_data')
        with self.env.begin(write=False).cursor(db) as txn:
            label_key = 'text-%09d'.encode() % index
            label = txn.get(label_key).decode()

            if self.opt.upper_case_only is True:
                label = label.upper()
            #label = label.replace(' ','')

            img_key = 'img-%09d'.encode() % index
            imgbuf = txn.get(img_key)

            buf = six.BytesIO()
            buf.write(imgbuf)
            buf.seek(0)

            try:
                if self.opt.rgb:
                    img = Image.open(buf).convert('RGB')  # for color image
                else:
                    img = Image.open(buf).convert('L')

            except IOError:
                print(f'Corrupted image for {index}')
                # make dummy image and dummy label for corrupted image.
                if self.opt.rgb:
                    img = Image.new('RGB', (self.opt.imgW, self.opt.imgH))
                else:
                    img = Image.new('L', (self.opt.imgW, self.opt.imgH))
                label = '[dummy_label]'

            img = ImageOps.invert(img)

            # We only train and evaluate on alphanumerics (or pre-defined character set in train.py)
            out_of_char = f'[^{self.opt.character}]'
            label = re.sub(out_of_char, '', label)

        return (img, label)

def inference_dataset(root, opt, select_data='', prefixs='', use_maindb=True):
    """ select_data='/' contains all sub-directory of root directory """
    dataset_list = []
    print(f'dataset_root:      {root}\t dataset: {select_data[0]}\t prefix: {prefixs[0]}')

    for dbname, prefix in zip(select_data, prefixs):
        dataset = InferenceLmdbDataset(dbname, opt, prefix, use_maindb=use_maindb)

        lindataset = []
        for das in dataset:
            lindataset.extend(das)
        dataset = lindataset

        print(f'data-db-name:\t/{dbname}\t num samples: {len(dataset)}')
        dataset_list.append(dataset)

    concatenated_dataset = ConcatDataset(dataset_list)

    return concatenated_dataset

class InferenceLmdbDataset(Dataset):

    def __init__(self, root, opt, prefix, use_maindb=True):
        if use_maindb is True:
            root = os.path.dirname(DB_MAIN_PATH) + '/' + root
        print(root)
        self.prefix = prefix
        self.root = root
        self.opt = opt
        self.env = lmdb.open(root, max_readers=32, readonly=True, lock=False, readahead=False, meminit=False, max_dbs=2)
        if not self.env:
            print('cannot create lmdb from %s' % (root))
            sys.exit(0)

        keys = get_keys_by_prefix(self.env, prefix)
        nSamples = len(keys)
        self.nSamples = nSamples
        self.keys = keys
        self.filtered_index_list = [index for index in range(self.nSamples)]

    def __len__(self):
        return self.nSamples

    # get image from coordinate
    def __getitem__(self, index):
        assert index <= len(self), 'index range error'
        index = self.filtered_index_list[index]

        db = self.env.open_db(b'db_data')
        with self.env.begin(write=False).cursor(db) as txn:
            label = '%d' % index
            img_key = 'img-%09d' % index
            imgbuf = txn.get(img_key.encode())

            buf = six.BytesIO()
            buf.write(imgbuf)
            buf.seek(0)

            try:
                if self.opt.rgb:
                    img = Image.open(buf).convert('RGB') # for color image
                else:
                    img = Image.open(buf).convert('L')
            except IOError:
                print(f'Corrupted image for {index}')
                # make dummy image and dummy label for corrupted image.
                if self.opt.rgb:
                    img = Image.new('RGB', (self.opt.imgW, self.opt.imgH))
                else:
                    img = Image.new('L', (self.opt.imgW, self.opt.imgH))
                label = '[dummy_label]'

            cood_key = self.keys[index]
            value = txn.get(cood_key)
            wcood = decode_pd(value, sep='\t')

            img = ImageOps.invert(img)

            imgs = []
            for thebox in wcood:
                thecood = np.array(thebox[0:4]).astype(np.float32)
                try:
                    pdd = 3
                    thecood[1] = thecood[1] - pdd
                    thecood[3] = thecood[3] + pdd
                    cim = img.crop(thecood)
                except:
                    thecood = np.array(thebox[0:4]).astype(np.float32)
                    cim = img.crop(thecood)
                imgs.append((cim, label))
            out_of_char = f'[^{self.opt.character}]'
            label = re.sub(out_of_char, '', label)

        return imgs

class RawDataset(Dataset):

    def __init__(self, root, opt):
        self.opt = opt
        self.image_path_list = []
        for dirpath, dirnames, filenames in os.walk(root):
            for name in filenames:
                _, ext = os.path.splitext(name)
                ext = ext.lower()
                if ext == '.jpg' or ext == '.jpeg' or ext == '.png':
                    self.image_path_list.append(os.path.join(dirpath, name))

        self.image_path_list = natsorted(self.image_path_list)
        self.nSamples = len(self.image_path_list)

    def __len__(self):
        return self.nSamples

    def __getitem__(self, index):

        try:
            if self.opt.rgb:
                img = Image.open(self.image_path_list[index]).convert('RGB')  # for color image
            else:
                img = Image.open(self.image_path_list[index]).convert('L')

        except IOError:
            print(f'Corrupted image for {index}')
            # make dummy image and dummy label for corrupted image.
            if self.opt.rgb:
                img = Image.new('RGB', (self.opt.imgW, self.opt.imgH))
            else:
                img = Image.new('L', (self.opt.imgW, self.opt.imgH))

        return (img, self.image_path_list[index])


class ResizeNormalize(object):

    def __init__(self, size, interpolation=Image.BICUBIC):
        self.size = size
        self.interpolation = interpolation
        self.toTensor = transforms.ToTensor()

    def __call__(self, img):
        img = img.resize(self.size, self.interpolation)
        img = self.toTensor(img)
        img.sub_(0.5).div_(0.5)
        return img


class NormalizePAD(object):

    def __init__(self, max_size, PAD_type='right'):
        self.toTensor = transforms.ToTensor()
        self.max_size = max_size
        self.max_width_half = math.floor(max_size[2] / 2)
        self.PAD_type = PAD_type

    def __call__(self, img):
        img = self.toTensor(img)
        img.sub_(0.5).div_(0.5)
        c, h, w = img.size()
        Pad_img = torch.FloatTensor(*self.max_size).fill_(0)
        Pad_img[:, :, :w] = img  # right pad
        if self.max_size[2] != w:  # add border Pad
            Pad_img[:, :, w:] = img[:, :, w - 1].unsqueeze(2).expand(c, h, self.max_size[2] - w)

        return Pad_img


class AlignCollate(object):

    def __init__(self, imgH=32, imgW=100, keep_ratio_with_pad=False):
        self.imgH = imgH
        self.imgW = imgW
        self.keep_ratio_with_pad = keep_ratio_with_pad

    def __call__(self, batch):
        batch = filter(lambda x: x is not None, batch)
        images, labels = zip(*batch)

        if self.keep_ratio_with_pad:  # same concept with 'Rosetta' paper
            resized_max_w = self.imgW
            #input_channel = 3 if images[0].mode == 'RGB' else 1
            transform = NormalizePAD((1, self.imgH, resized_max_w))
            #transform = NormalizePAD((input_channel, self.imgH, resized_max_w))

            resized_images = []
            for image in images:
                w, h = image.size
                ratio = w / float(h)
                if math.ceil(self.imgH * ratio) > self.imgW:
                    resized_w = self.imgW
                else:
                    resized_w = math.ceil(self.imgH * ratio)

                resized_image = image.resize((resized_w, self.imgH), Image.BICUBIC)
                resized_images.append(transform(resized_image))
                # resized_image.save('./image_test/%d_test.jpg' % w)

            image_tensors = torch.cat([t.unsqueeze(0) for t in resized_images], 0)

        else:
            transform = ResizeNormalize((self.imgW, self.imgH))
            image_tensors = [transform(image) for image in images]
            image_tensors = torch.cat([t.unsqueeze(0) for t in image_tensors], 0)

        return image_tensors, labels


def tensor2im(image_tensor, imtype=np.uint8):
    image_numpy = image_tensor.cpu().float().numpy()
    if image_numpy.shape[0] == 1:
        image_numpy = np.tile(image_numpy, (3, 1, 1))
    image_numpy = (np.transpose(image_numpy, (1, 2, 0)) + 1) / 2.0 * 255.0
    return image_numpy.astype(imtype)


def save_image(image_numpy, image_path):
    image_pil = Image.fromarray(image_numpy)
    image_pil.save(image_path)