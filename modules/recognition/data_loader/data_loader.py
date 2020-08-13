# -*- coding: utf-8 -*-

from base.base_data_loader import BaseDataLoader

import os
import sys
import numpy as np
sys.path.append('/home/dk/docr2/modules/database')
from db_util import *
from db import *
DB_MAIN_PATH = '/home/dk/docr2/DB/db_main'

import mlflow
import mlflow.pytorch

class DataLoader(BaseDataLoader):
    def __init__(self, config):
        super(DataLoader, self).__init__(config)

        # set main db path
        if config.META_DB.DB_MAIN_PATH != '':
            DB_MAIN_PATH = config.META_DB.DB_MAIN_PATH

        # create meta db
        self.db_meta = self.set_meta_db()

        if config.MLFLOW.module_name != "None":
            # log meta db to mlflow
            self.log_meta_db_to_mlflow()

    def set_meta_db(self):
        # 새로운 meta db 생성
        path = self.config.META_DB.path
        name = self.config.META_DB.name
        dbinfofile = path+name+self.config.META_DB.log
        self.dbinfofile = dbinfofile

        description = self.config.META_DB.description

        # load meta db
        if self.config.META_DB.load_existing_meta_db is False:
            db_meta = create_meta_db(path=path, name=name, description=description)

            db_list_string = self.config.META_DB.dblist
            db_list = db_list_string.split(',')

            db_meta = set_dbs_to_meta_db(db_meta=db_meta, db_list=db_list)
        else:
            db_meta = open_env(path=path+name)

        # data summary file 기록
        print_env(db_meta, save=True, save_path=dbinfofile, wmode='w')
        print_db(db_meta, save=True, save_path=dbinfofile)

        return db_meta

    def log_meta_db_to_mlflow(self):
        db_meta = self.db_meta

        mlflow.ActiveRun(self.config.mlrun)

        # get/set meta db info
        dbinfofile = self.dbinfofile
        path = self.config.META_DB.path
        name = self.config.META_DB.name

        list_db_name, list_db_ref, list_db_env = get_dbs_from_meta_db(env=db_meta)
        print_env(db_meta)
        print_db(db_meta)
        envsm_main = get_env_summary(db_meta, prefix='__meta__/')
        mlflow.log_params(envsm_main)
        mlflow.log_param('__meta__/-', '-'*200)

        # get/set data db info
        for idx in range(len(list_db_name)):
            envsm = get_env_summary(list_db_env[idx], prefix='_'+list_db_name[idx]+'/')
            mlflow.log_params(envsm)

            print_env(list_db_env[idx])
            mlflow.log_param('_' + list_db_name[idx]+'/-', '-'*200)
            print_env(list_db_env[idx], save=True, save_path=dbinfofile)

        mlflow.log_artifact(path+name)
        with open(dbinfofile, 'r') as file:
            data_info_string = file.read().replace('\n', '')

        mlflow.set_tag('data_summary', data_info_string)

    def save_to_db(self, predictions=None):
        # save to database
        config = self.config

        # db 불러오기
        path = config.inference.data_db_path
        name = config.inference.data_db_name
        if config.inference.create_new_data_db == True:
            # 빈 data db 생성
            description = config.inference.data_db_description
            datadb = create_data_db(path=path, name=name, description=description, data_class='INFERENCE')
        else:
            datadb = open_env(path+name)
        print_env(datadb)

        # sort data
        indexmat = []
        for pidx in predictions:
            indexmat.append(int(pidx[0]))
        indexnum = list(set(indexmat))

        wordbag = [[]] * (np.max(indexnum)+1)
        accvvals = [[]] * (np.max(indexnum)+1)
        for pidx in predictions:
            wordbag[int(pidx[0])] = wordbag[int(pidx[0])] + [str(pidx[1])]
            accvvals[int(pidx[0])] = accvvals[int(pidx[0])] + [str(pidx[2].cpu().numpy())]

        if config.inference.save_prefix == 'word':
            for widx, words in enumerate(wordbag):
                accs = accvvals[widx]
                update_data(datadb, index=widx, label=None, ref=None, word=words, word_i=accs)
        elif config.inference.save_prefix == 'char':
            for widx, words in enumerate(wordbag):
                update_data(datadb, index=widx, label=None, ref=None, char=words)
        else:
            print('prefix error')

def textfilter(text):
    startind = len(text)
    flag = 0
    for idx in range(len(text)-2):
        if text[idx].isdigit() == False:
            if text[idx] == text[idx+1] and text[idx] == text[idx+2]:
                startind = idx
                break
    filtered = text[:startind]

    if startind != len(text):
        flag = 1
    return filtered
