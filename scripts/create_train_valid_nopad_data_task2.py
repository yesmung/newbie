import os
from configparser import ConfigParser
path_config_file = os.getcwd().split('docrv2_sroie')[0] + 'docrv2_sroie/' + 'config.ini'
common_variable = ConfigParser()
common_variable.read(path_config_file)

print(common_variable.sections())
print(common_variable['database']['module_path'])
print(common_variable['database']['DB_BASE'])

import sys
sys.path.append(common_variable['database']['module_path'])
from db_util import *
from db import *

import tensorflow
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import copy

from parse import parse
from PIL import Image, ImageFont, ImageDraw, ImageEnhance
from glob import glob
from tqdm import tqdm

def get_coords_from_image(txt_file):
    lps = []

    try:
        with open(txt_file, 'r') as f:
            coords_data = f.read().split('\n')
            for ii in range(len(coords_data)):
                if len(coords_data[ii]) > 0:
                    ps = parse('{},{},{},{},{},{},{},{},{}', coords_data[ii])
                    ps = list(ps)
                    ps[:-1] = list(map(int, ps[:-1]))
                    lps.append(ps)
    except:
        with open(txt_file, 'r', encoding='ISO-8859-1') as f:
            coords_data = f.read().split('\n')
            for ii in range(len(coords_data)):
                if len(coords_data[ii]) > 0:
                    ps = parse('{},{},{},{},{},{},{},{},{}', coords_data[ii])
                    ps = list(ps)
                    ps[:-1] = list(map(int, ps[:-1]))
                    lps.append(ps)
    return lps


def create_database(db_idx, db_name=None):
    path = common_variable['database']['DB_BASE']
    os.makedirs(path, exist_ok=True)

    name = db_name + "_%04d"%db_idx
    description = 'data db from train_task2 all coords images'
    datadb = create_data_db(path=path, name=name, description=description, data_class='DATA')
    print("... created db : ", name)
    return datadb


def update_database(datadb, imgs, coords, refname, db_start_idx):
    for idx, img in enumerate(imgs):
        chars = []
        chars.append(coords[idx][8])
        char_table = pd.DataFrame(coords[idx])
        char_table = char_table.T
        char_table.rename(columns={0:'x1', 1:'y1', 2:'x2', 3:'y2', 4:'x3',
                                   5:'y3', 6:'x4', 7:'y4', 8:'char'}, inplace=True)
        
        char_table.x1 = char_table.x1.astype(int)
        char_table.y1 = char_table.y1.astype(int)
        char_table.x2 = char_table.x2.astype(int)
        char_table.y2 = char_table.y2.astype(int)
        char_table.x3 = char_table.x3.astype(int)
        char_table.y3 = char_table.y3.astype(int)
        char_table.x4 = char_table.x4.astype(int)
        char_table.y4 = char_table.y4.astype(int)
        
        update_data(datadb, 
                    index=db_start_idx+idx, 
                    label='task12', 
                    image=img, 
                    text=chars[0], 
                    ref=refname, 
                    char_c=None,
                    char=None)

def get_crop_image_by_coords(img_path, coords, num_images=1):
    img = Image.open(img_path)
    o_width = img.size[0]
    o_height = img.size[1]
    
    transpose_x1_coords = np.transpose(coords)[0]
    transpose_y1_coords = np.transpose(coords)[1]
    transpose_x1_coords = list(map(int, transpose_x1_coords)) 
    transpose_y1_coords = list(map(int, transpose_y1_coords))
    
    transpose_x2_coords = np.transpose(coords)[2]
    transpose_y2_coords = np.transpose(coords)[3]
    transpose_x2_coords = list(map(int, transpose_x2_coords)) 
    transpose_y2_coords = list(map(int, transpose_y2_coords))    
    
    transpose_x3_coords = np.transpose(coords)[4]
    transpose_y3_coords = np.transpose(coords)[5]
    transpose_x3_coords = list(map(int, transpose_x3_coords)) 
    transpose_y3_coords = list(map(int, transpose_y3_coords))

    transpose_x4_coords = np.transpose(coords)[6]
    transpose_y4_coords = np.transpose(coords)[7]
    transpose_x4_coords = list(map(int, transpose_x4_coords)) 
    transpose_y4_coords = list(map(int, transpose_y4_coords))
    
    transpose_chars = np.transpose(coords)[8]
    
    sample_list = []
    coords_list = []
    for idx in range(num_images):
        base_idx = idx

        new_x1 = transpose_x1_coords[base_idx]
        new_y1 = transpose_y1_coords[base_idx]

        new_x2 = transpose_x2_coords[base_idx]
        new_y2 = transpose_y2_coords[base_idx]

        new_x3 = transpose_x3_coords[base_idx]
        new_y3 = transpose_y3_coords[base_idx]

        new_x4 = transpose_x4_coords[base_idx]
        new_y4 = transpose_y4_coords[base_idx]
    
        crop_img = img.crop((new_x1,
                             new_y1,
                             new_x3,
                             new_y3))
        sample_list.append(crop_img)
        coords_list.append([new_x1, new_y1, new_x2, new_y2, new_x3, new_y3, new_x4, new_y4,
                           transpose_chars[base_idx]])

    return sample_list, coords_list

def main():
    """
    task2 : create train dataset
    """
    datadir = '/media/myungsungkwak/msdisk/docrv2_sroie/data/train_task2_crop_sample/'
    img_files = glob(datadir+'*.jpg')
    txt_files = glob(datadir+'*.txt')

    crop_images = []
    coords_table = []

    datadb = create_database(1, "recog_train_task2_all_pad")
    num_images = 1
    db_start_idx = 0
    for idx in tqdm(range(len(img_files))):
        #     print(idx, " : ", img_files[idx])
        refname = os.path.basename(img_files[idx])[:-4]
        txt_filepath = os.path.join(datadir, refname+'.txt')
        coords_table.append(get_coords_from_image(txt_filepath))
        
        num_images = len(coords_table[idx])

        crop_img, crop_coords = get_crop_image_by_coords(img_files[idx], coords_table[idx], num_images=num_images)
        update_database(datadb, crop_img, crop_coords, refname, db_start_idx)
        db_start_idx = db_start_idx + num_images

    del coords_table[:][:]

if __name__ == '__main__':
    main()
