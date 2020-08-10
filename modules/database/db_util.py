"""
db_util.py
"""

def write_cache_to_env(env, cache):
    with env.begin(write=True) as txn:
        for k, v in cache.item():
            txn.put(k,w)

def remove_cache_from_env(env, key):
    with env.begin(write=True) as txn:
        txn.pop(str(key).encode())

def remove_db_from_evn(env, db_name):

