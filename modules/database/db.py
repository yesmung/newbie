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
DB_BASE = './DB/'
DB_MAIN_PATH= DB_BASE + 'db_main'
MLFLOW_URI = './MLFLOW/'

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
        env = open_env(path=DB_MAIN_PATH)
    db_data = env.open_db(str('db_data').encode())
    cache = {}

    db_name = get_value_by_key(db, 'name')
    db_path = get_value_by_key(db, 'reference')

    cache[str(db_name).encode()] = db_path.encode()
    write_cache_to_db(env, db_data, cache)
    update_time_stemp(env)

    return env


def register_db_by_path(env=None, db_name=None, db_path=None):
    if env is None:
        env = open_env(path=DB_MAIN_PATH)
    db_data = env.open_db(str('db_data').encode())
    cache = {}

    cache[str(db_name).encode()] = db_path.encode()
    write_cache_to_db(env, db_data, cache)
    update_time_stemp(env)

    return env


def unregister_db(env=None, db_name=None):
    if env is None:
        env = open_env(path=DB_MAIN_PATH)
    db_data = env.open_db(str('db_data').encode())

    remove_cache_from_db(env, db_data, db_name)
    update_time_stemp(env)

    return env


def create_meta_db(path='/', name='', description=''):
    cache = {}
    env = lmdb.open(path+name, map_size=MAP_SIZE, max_dbs=MAX_DB, lock=False)
    cache[b'mlflow_uri'] = str(MLFLOW_URI).encode()
    cache[b'mlflow_experiment'] = str('').encode()
    cache[b'name'] = str(name).encode()
    cache[b'created'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'updated'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'description'] = str(description).encode()
    cache[b'reference'] = str(path+name).encode()
    write_cache_to_env(env, cache)

    return env


def create_inference_db(path='/', name='', description='', data_class='INFERENCE', sep='\t'):
    cache = {}
    env = lmdb.open(path + name, map_size=MAP_SIZE, max_dbs=MAX_DB, lock=False)
    cache[b'name'] = str(name).encode()
    cache[b'created'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'updated'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'description'] = str(description).encode()
    cache[b'reference'] = str(path + name).encode()
    cache[b'note'] = str('').encode()
    cache[b'class'] = str(data_class).encode()
    cache[b'sep'] = str(sep).encode()
    write_cache_to_env(env, cache)

    return env


def update_inference(env, index=None, config=None, process=None, module=None, ref=None, sep='\t'):
    db_data = env.open_db(b'db_data')
    cache = {}

    if config is not None:
        ckey = 'config-%09d'%index
        cache[str(ckey).encode()] = config.encode()

    if process is not None:
        ckey = 'process-%09d'%index
        cache[str(ckey).encode()] = process.encode()

    if module is not None:
        ckey = 'module-%09d'%index
        cache[str(ckey).encode()] = module.encode()

    if ref is not None:
        ckey = 'ref-%09d'%index
        cache[str(ckey).encode()] = ref.encode()

    write_cache_to_env(env, cache)

    return env, db_data


def create_data_db(path='/', name='', description='', data_class='', sep='\t'):
    cache = {}
    env = lmdb.open(path + name, map_size=MAP_SIZE, max_dbs=MAX_DB, lock=False)
    cache[b'name'] = str(name).encode()
    cache[b'created'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'updated'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    cache[b'description'] = str(description).encode()
    cache[b'reference'] = str(path + name).encode()
    cache[b'note'] = str('').encode()
    cache[b'class'] = str(data_class).encode()
    cache[b'sep'] = str(sep).encode()
    write_cache_to_env(env, cache)

    return env


def update_cache_in_queue(q=Queue(), index=None, label='', image=None, text=None, ref='', char=None,
                          char_i=None, char_c=None, word=None, word_i=None, word_c=None, image_c=None,
                          image_w=None, word_ner=None, table=None, db=None, sep='\t'):
    p = Process(target=update_cache, args=({}, index, label, image, text, ref, char, char_i, char_c, word, word_i,
                word_c, image_c, image_w, word_ner, table, db, sep, q))
    p.start()

    return p, q


def update_data(env, index=None, label='', image=None, text=None, ref='', char=None,
                char_i=None, char_c=None, word=None, word_i=None, word_c=None, image_c=None,
                image_w=None, word_ner=None, table=None, db=None, sep='\t'):
    db_data = env.open_db(b'db_data')
    cache = {}

    if label is not None:
        ckey = 'label-%09d'%index
        cache[str(ckey).encode()] = label.encode()

    if ref is not None:
        ckey = 'ref-%09d'%index
        cache[str(ckey).encode()] = ref.encode()

    if char is not None:
        ckey = 'char-%09d'%index
        cache[str(ckey).encode()] = encode_list(char, sep=sep)

    if char_i is not None:
        ckey = 'char_i-%09d'%index
        cache[str(ckey).encode()] = encode_list(char_i, sep=sep)

    if char_c is not None:
        ckey = 'char_c-%09d'%index
        cache[str(ckey).encode()] = encode_pd(char_c, sep=sep)

    if word is not None:
        ckey = 'word-%09d'%index
        cache[str(ckey).encode()] = encode_list(word, sep=sep)

    if word_i is not None:
        ckey = 'word_i-%09d'%index
        cache[str(ckey).encode()] = encode_list(word_i, sep=sep)

    if word_c is not None:
        ckey = 'word_c-%09d'%index
        cache[str(ckey).encode()] = encode_pd(word_c, sep=sep)

    if word_ner is not None:
        ckey = 'word_ner-%09d'%index
        cache[str(ckey).encode()] = encode_list(word_ner, sep=sep)

    if table is not None:
        ckey = 'table-%09d'%index
        cache[str(ckey).encode()] = encode_pdlist(table, sep=sep)

    if db is not None:
        ckey = 'db-%09d'%index
        cache[str(ckey).encode()] = encode_pdlist(db, sep=sep)

    if text is not None:
        ckey = 'text-%09d'%index
        cache[str(ckey).encode()] = text.encode()

    if image is not None:
        temp = BytesIO()
        image.save(temp, 'JPEG')
        ckey = 'img-%09d' % index
        cache[str(ckey).encode()] = temp.getvalue()

    if image_w is not None:
        for ci in range(len(image_w)):
            temp = BytesIO()
            image.save(temp, 'JPEG')
            ckey = 'img_w-%09d-%09d'%(index, ci)
            cache[str(ckey).encode()] = temp.getvalue()

    if image_c is not None:
        for ci in range(len(image_c)):
            temp = BytesIO()
            image.save(temp, 'JPEG')
            ckey = 'img_c-%09d-%09d' % (index, ci)
            cache[str(ckey).encode()] = temp.getvalue()

    write_cache_to_db(env, db_data, cache)

    return env


def update_cache(cache, index=None, label='', image=None, text=None, ref='', char=None,
                 char_i=None, char_c=None, word=None, word_i=None, word_c=None, image_c=None,
                 image_w=None, word_ner=None, table=None, db=None, sep='\t', queue=None):

    if queue is not None:
        cache = {}

    if label is not None:
        ckey = 'label-%09d'%index
        cache[str(ckey).encode()] = label.encode()

    if ref is not None:
        ckey = 'ref-%09d'%index
        cache[str(ckey).encode()] = ref.encode()

    if char is not None:
        ckey = 'char-%09d'%index
        cache[str(ckey).encode()] = encode_list(char, sep=sep)

    if char_i is not None:
        ckey = 'char_i-%09d'%index
        cache[str(ckey).encode()] = encode_list(char_i, sep=sep)

    if char_c is not None:
        ckey = 'char_c-%09d'%index
        cache[str(ckey).encode()] = encode_pd(char_c, sep=sep)

    if word is not None:
        ckey = 'word-%09d'%index
        cache[str(ckey).encode()] = encode_list(word, sep=sep)

    if word_i is not None:
        ckey = 'word_i-%09d'%index
        cache[str(ckey).encode()] = encode_list(word_i, sep=sep)

    if word_c is not None:
        ckey = 'word_c-%09d'%index
        cache[str(ckey).encode()] = encode_pd(word_c, sep=sep)

    if word_ner is not None:
        ckey = 'word_ner-%09d'%index
        cache[str(ckey).encode()] = encode_list(word_ner, sep=sep)

    if table is not None:
        ckey = 'table-%09d'%index
        cache[str(ckey).encode()] = encode_pdlist(table, sep=sep)

    if db is not None:
        ckey = 'db-%09d'%index
        cache[str(ckey).encode()] = encode_pdlist(db, sep=sep)

    if text is not None:
        ckey = 'text-%09d'%index
        cache[str(ckey).encode()] = text.encode()

    if image is not None:
        temp = BytesIO()
        image.save(temp, 'JPEG')
        ckey = 'img-%09d' % index
        cache[str(ckey).encode()] = temp.getvalue()

    if image_w is not None:
        for ci in range(len(image_w)):
            temp = BytesIO()
            image.save(temp, 'JPEG')
            ckey = 'img_w-%09d-%09d'%(index, ci)
            cache[str(ckey).encode()] = temp.getvalue()

    if image_c is not None:
        for ci in range(len(image_c)):
            temp = BytesIO()
            image.save(temp, 'JPEG')
            ckey = 'img_c-%09d-%09d' % (index, ci)
            cache[str(ckey).encode()] = temp.getvalue()

    if queue is not None:
        queue.put(cache)
        return
    else:
        return cache


def get_db_by_name(env=None, db_name=None):
    db_main_data = env.open_db(b'db_data')
    db_path = get_cache_from_db(env=env, db=db_main_data, key=db_name)
    return open_env(db_path)


def read_bulk_data_from_db(env=None, prefix=None, db_name='db_data'):
    # db = env.open_db(str(db_name).encode())
    db = env.open_db(b'db_data')
    list_data = []

    try:
        sep = get_cache_from_env(env, 'sep')
    except:
        sep = ','
        print('!! Warning !! comma <,> separator is used')
        print('@@ Warning @@ You may open an empty DB')
        cache = {}
        cache[b'sep'] = str(sep).encode()

    with env.begin(write=False) as txn:
        for key, value in txn.cursor(db):
            key = key.decode()

            if key.startswith('img'):
                if key[0:5] == 'img_c':
                    if prefix == 'img_c':
                        buf = six.BytesIO()
                        buf.write(value)
                        buf.seek(0)
                        value = Image.open(buf).convert('RGB')
                        list_data.append(value)
                elif key[0:5] == 'img_w':
                    if prefix == 'img_w':
                        buf = six.BytesIO()
                        buf.write(value)
                        buf.seek(0)
                        value = Image.open(buf).convert('RGB')
                        list_data.append(value)
                else:
                    if prefix == 'img':
                        buf = six.BytesIO()
                        buf.write(value)
                        buf.seek(0)
                        value = Image.open(buf).convert('RGB')
                        list_data.append(value)

            elif key.startswith('char'):
                if key[0:6] == 'char_c':
                    if prefix == "char_c":
                        value = decode_pd(value, sep=sep)
                        list_data.append(value)

                elif key[0:6] == 'char_i':
                    if prefix == "char_i":
                        value = decode_list(value, sep=sep)
                        list_data.append(value)
                else:
                    if prefix == 'char':
                        value = decode_list(value, sep=sep)
                        list_data.append(value)

            elif key.startswith('word'):
                if key[0:6] == 'word_c':
                    if prefix == "word_c":
                        value = decode_pd(value, sep=sep)
                        list_data.append(value)
                elif key[0:6] == 'word_i':
                    if prefix == "word_i":
                        value = decode_list(value, sep=sep)
                        list_data.append(value)
                elif key[0:6] == 'word_n':
                    if prefix == "word_ner":
                        value = decode_list(value, sep=sep)
                        list_data.append(value)
                else:
                    if prefix == 'word':
                        value = decode_list(value, sep=sep)
                        list_data.append(value)

            elif key.startswith('table'):
                if prefix == 'table':
                    value = decode_pdlist(value, sep=sep)
                    list_data.append(value)

            elif key.startswith('db'):
                if prefix == 'db':
                    value = decode_pdlist(value, sep=sep)
                    list_data.append(value)

            else:
                if key.startswith(prefix):
                    value = value.decode("utf-8")
                    list_data.append(value)

    return list_data


def read_bulk_key_from_db(env=None, db_name='db_data', prefix='None'):
    db = env.open_db(str(db_name).encode())
    list_data = []

    if prefix == 'None':
        with env.begin(write=False) as txn:
            key_list = [key.decode() for key, _ in txn.cursor(db)]
    else:
        with env.begin(write=False) as txn:
            key_list = [key.decode() for key, _ in txn.cursor(db) if key.decode().startswith(prefix+'-')]
    return key_list

def read_data_from_db_by_index(env=None, prefix=None, index=None, db_name='db_data'):
    db = env.open_db(str(db_name).encode())
    with env.begin(write=False) as txn:
        db_cs = txn.cursor(db)

        if isinstance(index, str):
            key = index
        else:
            key = '%s-%09d'%(prefix, index)

        value = db_cs.get(str(key).encode())
        key = str(key)

        try:
            if key.startswith('img'):
                buf = six.BytesIO()
                buf.write(value)
                buf.seek(0)
                rdata = Image.open(buf).convert('RGB')
            elif key.startswith('char_c'):
                if key.startswith((prefix)):
                    rdata = decode_pd(value)
            else:
                if key.startswith(prefix):
                    rdata = value.decode('utf-8')
        except:
            rdata = '('+prefix+')'

        return rdata


def decode_img(value):
    buf = six.BytesIO()
    buf.write(value)
    buf.seek(0)
    return Image.open(buf).convert('RGB')


def set_dbs_to_meta_db(db_main=None, db_meta=None, db_list=None):
    if db_main is None:
        db_main = open_env(path=DB_MAIN_PATH)

    for db_name in db_list:
        if db_name.startswith('/'):
            db_path = db_name
            db_name = os.path.basename(db_name)
        else:
            db_path = get_db_data_value_by_key(db_main, db_name)

        db_meta = register_db_by_path(env=db_meta, db_name=db_name, db_path=db_path)


def read_bulk_data_from_meta_db(env=None, prefix=None):
    db_name = []
    db_path = []
    db = env.open_db(b'db_data')
    with env.begin(write=False) as txn:
        for key, value in txn.cursor(db):
            db_name.append(key.decode('utf-8'))
            db_path.append(value.decode('utf-8'))

    pdata = []
    ldata = []
    for dpath in db_path:
        db_data = open_env_read(dpath)
        pdata.extend(read_bulk_data_from_db(env=db_data, prefix=prefix))
        ldata.extend(read_bulk_data_from_db(env=db_data, prefix='label'))

    return pdata, ldata


def get_dbs_from_meta_db(env=None):
    if env is None:
        env = open_env(path=DB_MAIN_PATH)
    db = env.open_db(b'db_data')

    list_db_name = []
    list_db_ref = []
    list_db_env = []

    with env.begin(write=False) as txn:
        for key, value in txn.cursor(db):
            key = key.decode('utf-8')
            value = value.decode('utf-8')
            data_db = open_env_read(value)

            list_db_name.append(key)
            list_db_ref.append(value)
            list_db_env.append(data_db)

    return list_db_name, list_db_ref, list_db_env


def print_env(env, save=False, save_path='', wmode='a'):
    if save is True:
        original_out = sys.stdout
        sys.stdout = open(save_path, wmode)

    tb = PrettyTable(['Key', 'Value'])
    tb.align['Key'] = 'l'
    tb.align['Value'] = 'l'

    for key, value in env.begin().cursor():
        key = key.decode('utf-8')
        if key.startswith('db'):
            value = '(database)'
        else:
            try:
                value = value.decode('utf-8')
            except:
                value = '(err)'
        tb.add_row([key, value])
    print(tb)

    if save is True:
        sys.stdout.close()
        sys.stdout = original_out


def print_db(env, db_name=None, save=False, save_path='', wmode='a', short=False):
    if save is True:
        original_out = sys.stdout
        sys.stdout = open(save_path, wmode)
    if db_name is None:
        db = env.open_db(b'db_data')
    else:
        db = env.open_db(str(db_name).encode())

    tb = PrettyTable(['Key', 'Value'])
    tb.align['Key'] = 'l'
    tb.align['Value'] = 'l'

    if short is True:
        with env.begin(write=False) as txn:
            for key, value in txn.cursor(db):
                key = key.decode('utf-8')
                if key[-1] == '0':
                    if key.startswith('img'):
                        value = '(image)'
                    elif key.startswith('word-'):
                        value = decode_list(value)
                    elif key.startswith('word_c-'):
                        value = '(word_c)'
                    elif key.startswith('word_i-'):
                        value = decode_list(value)
                    elif key.startswith('char_i-'):
                        value = decode_list(value)
                    elif key.startswith('char-'):
                        value = decode_list(value)
                    elif key.startswith('char_c-'):
                        value = '(char_c)'
                    elif key.startswith('table-'):
                        value = '(table)'
                    elif key.startswith('db-'):
                        value = '(db)'
                    else:
                        value = value.decode()

                    tb.add_row([key, value])
    else:
        with env.begin(write=False) as txn:
            for key, value in txn.cursor(db):
                key = key.decode('utf-8')
                if key.startswith('img'):
                    value = '(image)'
                elif key.startswith('word-'):
                    value = decode_list(value)
                elif key.startswith('word_c-'):
                    value = '(word_c)'
                elif key.startswith('word_i-'):
                    value = decode_list(value)
                elif key.startswith('char_i-'):
                    value = decode_list(value)
                elif key.startswith('char-'):
                    value = decode_list(value)
                elif key.startswith('char_c-'):
                    value = '(char_c)'
                elif key.startswith('table-'):
                    value = '(table)'
                elif key.startswith('db-'):
                    value = '(db)'
                else:
                    value = value.decode()

                tb.add_row([key, value])
    print(tb)

    if save is True:
        sys.stdout.close()
        sys.stdout = original_out


def update_time_stemp(env):
    cache = {}
    cache[b'updated'] = str(datetime.now().strftime('%Y-%m-%d %H:%M:%S')).encode()
    write_cache_to_env(env, cache)
    return env


def update_key_value(env, key, value):
    cache = {}
    cache[str(key).encode()] = str(value).encode()
    write_cache_to_env(env, cache)
    return env


def print_db_summary(env=None, db_name=None, save=False, save_path='', wmode='a'):
    if save is True:
        original_out = sys.stdout
        sys.stdout = open(save_path, wmode)

    #if db_name is None:
    db = env.open_db(b'db_data')
    #else:
    #    db = env.open_db(str(db_name).encode())

    tb = PrettyTable(['Key', 'Value'])
    tb.align['Key'] = 'l'
    tb.align['Value'] = 'l'

    with env.begin(write=False) as txn:
        keylist = [key.decode() for key, _ in txn.cursor(db)]

    dataclass_name = np.unique([key.split('-',1)[0] for key in keylist])
    number_of_value = len(keylist)
    number_of_dataclass = len(dataclass_name)
    if number_of_dataclass == 0:
        number_of_dataclass = 1

    number_of_data = 0
    for keyl in keylist:
        if keyl.startswith('label'):
            number_of_data = number_of_data + 1

    tb.add_row(['number of data', number_of_data])
    tb.add_row(['number of class', number_of_dataclass])
    tb.add_row(['classes', str(dataclass_name)])

    examples = []

    if number_of_data > 0:
        randomindex = np.random.randint(0, high=number_of_data, size=1)[0]
        tb.add_row(['eg. index',randomindex])
        for dclass in dataclass_name:
            exdata = read_data_from_db_by_index(env=env, prefix=dclass, index=randomindex)
            examples.append(exdata)
            tb.add_row(['eg. '+dclass, exdata])
    print(tb)

    if save is True:
        sys.stdout.close()
        sys.stdout = original_out

    return examples, dataclass_name


def get_db_summary(env=None, db_name=None, prefix=''):
    # if db_name is None:
    db = env.open_db(b'db_data')
    # else:
    #    db = env.open_db(str(db_name).encode())

    with env.begin(write=False) as txn:
        keylist = [key.decode('utf-8') for key, _ in txn.cursor(db)]

    dataclass_name = np.unique([key.split('-',1)[0] for key in keylist])
    number_of_value = len(keylist)
    number_of_dataclass = len(dataclass_name)
    if number_of_dataclass == 0:
        number_of_dataclass = 1

    number_of_data = 0
    for keyl in keylist:
        if keyl.startswith('label'):
            number_of_data = number_of_data + 1

    summarydict = {prefix+'number of data':number_of_data,
                   prefix+'number of class':number_of_dataclass,
                   prefix+'classes':str(dataclass_name)}

    if number_of_data > 0:
        randomindex = np.random.randint(0, high=number_of_data, size=1)[0]
        summarydict[prefix+'eg. index'] = randomindex
        for dclass in dataclass_name:
            exdata = read_data_from_db_by_index(env=env, prefix=dclass, index=randomindex)
            summarydict[prefix+'eg. '+dclass] = exdata

    return summarydict


def get_env_summary(env=None, prefix=''):
    summarydict = {}
    for key, value in env.begin().cursor():
        key = key.decode('utf-8')
        try:
            if key.startswith('db'):
                value = '(database)'
            else:
                value = value.decode('utf-8')
        except:
            value = ''

        summarydict[prefix+key] = value

    return summarydict


def read_bulk_key_from_meta_db(env=None):
    db_name = []
    db_path = []
    db = env.open_db(b'db_data')
    with env.begin(write=False) as txn:
        for key, value in txn.cursor(db):
            db_name.append(key.decode('utf-8'))
            db_path.append(value.decode('utf-8'))

    ldata = []
    for dpath in db_path:
        dname = os.path.basename(dpath)
        db_data = open_env(dpath)
        llist = read_bulk_key_from_db(env=db_data)
        for lind in range(len(llist)):
            llist[lind] = dname+'://'+llist[lind]
        ldata.extend(llist)

    return ldata


def get_key_names_in_db(env, db='db_data', prefix='', include_dbname=False):
    if isinstance(db, str):
        db = env.open_db(str(db).encode())
    dbnames = []
    with env.begin() as txn:
        dbnames = [return_by_prefix(key.decode(), prefix) for key, _ in txn.cursor(db)]

    return [x for x in dbnames if x != None]


def return_by_prefix(key, prefix):
    if key.startswith(prefix):
        return key


def read_all_data_from_meta_db(env, prefix, overridelabel=None):
    db_names, db_paths, dbs = get_dbs_from_meta_db(env=env)
    data = []
    for idx, data_env in enumerate(dbs):
        exdata = read_bulk_key_from_db(env=data_env, prefix=prefix)
        if prefix == 'label' and overridelabel is not None:
            exdata = [overridelabel[idx]]*len(exdata)
        data.extend(exdata)

    return data


def read_all_keys_from_meta_db(env, prefix, overridelabel=None, remove_last=False):
    db_names, db_paths, dbs = get_dbs_from_meta_db(env=env)
    data = []
    pbar = tqdm(len(db_names))
    for data_env, name_env in zip(dbs, db_names):
        pbar.update(1)
        pbar.set_description('read keys from: ',name_env)

        exdata = get_key_names_in_db(env=data_env, prefix=prefix)

        if remove_last is True:
            exdata = exdata[:-1]

        exdatawname = []
        for kname in exdata:
            exdatawname.append([name_env, kname])

        data.extend(exdatawname)

    return data


def label2int(labels):
    categories = np.unique(labels)
    ilabel = np.zeros([len(labels)])
    for idx in range(len(categories)):
        for jdx in range(len(labels)):
            if labels[jdx] == categories[idx]:
                ilabel[jdx] = idx
    return ilabel, categories


def img2array(imgs):
    iimgs = []
    for img in imgs:
        iimgs.append(np.array(img))

    return iimgs


def split_list(data, ratio=[0.7, 0.3], shuffle=True, set_data_length=True, data_length=[0, 0]):
    datalen = len(data)
    si = 0
    spdata = []

    if shuffle is True:
        sdata = []
        np.random.seed(0)
        for el in np.random.permutation(datalen):
            sdata.append(data[el])
        data = sdata

    for ridx in range(len(ratio)):
        fsi = int(datalen*ratio[ridx]+si)
        if fsi > datalen:
            fsi = datalen

        spdata.append(data[si:fsi])
        si = fsi

    if set_data_length is True:
        spsdata = []
        for ridx in range(len(data_length)):
            sd = spdata[ridx]
            sdlen = len(sd)
            sfdata = []
            np.random.seed(0)
            for el in np.random.randint(0, high=sdlen, size=data_length[ridx]):
                sfdata.append(sd[el])
            spsdata.append(sfdata)
        spdata = spsdata

    return spdata


def refresh_main_db(main_db_path=DB_BASE+'*'):
    env = open_env(DB_MAIN_PATH)
    db = env.open_db(str('db_data').encode())
    dbnames = []
    with env.begin(write=False) as txn:
        for key, value in txn.cursor(db):
            if os.path.isdir(value.decode()) == 0:
                print('::: no data exist from db ::: '+ key.decode())
                unregister_db(db_name=os.path.basename(key.decode()))

    dbpath = glob(main_db_path)
    dblist = []
    for dbname in dbpath:
        if os.path.basename(dbname) != 'db_main':
            try:
                data_db = open_env_read(dbname)
                register_db(db=open_env(dbname))
                dblist.append(os.apth.basename(dbname))
            except:
                print('--- no data exist from file system --- '+dbname)
                unregister_db(db_name=os.path.basename(dbname))
    print_db(open_env(DB_MAIN_PATH))
    return dblist


def encode_list(data=None, sep='\t'):
    data = [str(d) for d in data]
    buf = io.StringIO()
    csvwriter = csv.writer(buf, delimiter = sep)
    csvwriter.writerows(data)
    return io.BytesIO(buf.getvalue().encode('utf8')).read()


def decode_list(buf=None, sep='\t'):
    data = buf.decode().replace(sep, '')
    return data.split('\r\n')[:-1]


def encode_pd(data=None, sep='\t'):
    buf = io.StringIO()
    data.to_csv(buf, encoding='utf-8', sep=sep)
    return io.BytesIO(buf.getvalue().encode('utf8')).read()


def decode_pd(buf=None, sep='\t'):
    data = []
    for row in buf.decode().split('\n')[1:-1]:
        data.append(row.split(sep)[1:])
    return data


def get_keys_by_prefix(env, keyprefix):
    db = env.open_db(b'db_data')
    with env.begin() as txn:
        return [key for key, _ in txn.cursor(db) if key.decode().startswith(keyprefix)]


def get_length_by_prefix(env, keyprefix):
    db = env.open_db(b'db_data')
    with env.begin() as txn:
        return len([key for key, _ in txn.cursor(db) if key.decode().startswith(keyprefix)])


def encode_pdlist(data=None, sep='\t'):
    buf = io.BytesIO()
    pickle.dump(data, buf)
    return buf.getvalue()


def decode_pdlist(data=None, sep='\t'):
    return pickle.loads(data)


def encode_pickle(data=None, sep='\t'):
    buf = io.BytesIO()
    pickle.dump(data, buf)
    return buf.getvalue()


def decode_pickle(data=None, sep='\t'):
    return pickle.loads(data)

















