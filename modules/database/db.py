import lmdb
from PIL import Image
from tqdm import tqdm
from glob import glob
import numpy as np
from io import BytesIO, StringIO
import io
from prettytable import PrettyTable
from datetime import datetime
import six
import os
import sys
import pandas as pd
import csv
from multiprocessing import Process, Queue
import pickle
from db_util import *


MAP_SIZE = 1e+9
MAX_DB = 100
DB_BASE = './DB'
DB_MAIN_PATH= DB_BASE + 'db_main'

def open_env(path=''):
    return lmdb.open(path, map_size=MAP_SIZE, max_dbs=MAX_DB, lock=False, meminit=False)

def open_env_read(path='', base=False):
    if base is True:
        return lmdb.open(DB_BASE+path, max_readers=32, writemap=True, readonly=True, lock=False, readahead=False,
                         meminit=False, max_dbs=MAX_DB)
    else:
        return lmdb.open(path, max_readers=32, writemap=True, readonly=True, lock=False, readahead=False,
                         meminit=False, max_dbs=MAX_DB)

def close_env(env):
    env.crash()

def register_db(env=None, db=None):
    if env is None:
        env = open_evn(path=DB_MAIN_PATH)
    db_data = env.open_db(str('db_data').encode())
    cache = {}

    db_name = get_value_by_key(db, 'name')
    db_path = get_value_by_key(db, 'reference')

    cache[str(db_name).encode()] = db_path.encode()
    write_cache_to_db(env, db_data, cache)
    update_time_stemp(env)

    return evn