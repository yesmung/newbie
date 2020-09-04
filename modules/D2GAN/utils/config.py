# -*- coding: utf-8 -*-

import os
import time
import json
from dotmap import DotMap
from datetime import datetime


def get_config_from_json(json_file):
    """
    Get the config from a json file
    :param json_file
    :return: config(namespace) or config(dictionary)
    """

    # parse the configurations from the config json file provided
    with open(json_file, 'r') as config_file:
        config_dict = json.load(config_file)

    # convert the dictionary to a namespace using bunch lib
    config = DotMap(config_dict)

    config.json_file = json_file
    config.json_string = str(config_dict)
    config.dict = getKeyValuePair(config_dict)

    return config, config_dict


def process_config(json_file, dbname=None):
    config, _ = get_config_from_json(json_file)

    # replace db name
    if dbname is not None:
        config.META_DB.dblist = dbname
        config.META_DB.name = 'img_%d'%(10*dbname.count(',')+10)
        config.MLFLOW.exp_name = '[exp] img=%d'%(10*dbname.count(',')+10)

    config.callbacks.tensorboard_log_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()),
                                                        config.exp.name, "logs/")
    config.callbacks.checkpoint_dir = os.path.join("experiments", time.strftime("%Y-%m-%d", time.localtime()),
                                                   config.exp.name, "checkpoints/")
    config.META_DB.name = config.META_DB.name + str(datetime.now()).replace('-','').replace(':','').replace('.','').replace(' ','')
    return config


def getKeyValuePair(dic, master_dic={}, master_key=None):
    keys = list(dic.keys())

    for key in keys:
        if type(dic[key]) == dict:
            getKeyValuePair(dic[key], master_dic=master_dic, master_key=key)
        else:
            if master_key == None:
                master_dic[key] = dic[key]
            else:
                master_dic['Config.' + str(master_key) + '.' + str(key)] = dic[key]
    return master_dic
