"""
db_util.py
"""


def write_cache_to_env(env, cache):
    with env.begin(write=True) as txn:
        for k, v in cache.items():
            txn.put(k, v)


def remove_cache_from_env(env, key):
    with env.begin(write=True) as txn:
        txn.pop(str(key).encode())


def remove_db_from_evn(env, db_name):
    with env.begin(write=True) as txn:
        db = env.open_db(str(db_name).encode())
        txn.drop(db)


def write_cache_to_db(env, db, cache):
    with env.begin(write=True) as txn:
        db_cs = txn.cursor(db)
        for k, v in cache.items():
            db_cs.put(k, v)


def remove_cache_from_db(env, db, key):
    with env.begin(wrtie=True) as txn:
        db_cs = txn.cursor(db)
        db_cs.pop(str(key).encode())


def get_cache_from_db(env, db, key):
    if isinstance(db, str):
        db = env.open_db(str(db).encode())

    with env.begin(write=True) as txn:
        db_cs = txn.cursor(db)
        return db_cs.get(str(key).encode()).decode()


def get_bulk_cache_from_db(env, db, keys):
    if isinstance(db, str):
        db = env.open_db(str(db).encode())

    with env.begin(write=True) as txn:
        db_cs = txn.cursor(db)
        return [db_cs.get(str(key).encode()).decode() for key in keys]


def get_db_data_value_by_key(env, key):
    return get_cache_from_db(env, 'db_data', key)


def get_db_from_env(env, key):
    with env.begin(write=False) as txn:
        return txn.cursor(str(key).encode())


def get_value_by_key(env, key):
    return get_cache_from_env(env, key)


def get_cache_from_env(env, key):
    with env.begin(write=False) as txn:
        return txn.get(str(key).encode()).decode()


def get_key_count(env):
    with env.begin() as txn:
        return txn.stat()['entries']


def get_key_names(env):
    with env.begin() as txn:
        return [key.decode() for key, _ in txn.cursor()]


def get_key_names_in_db(env, db='db_data'):
    if isinstance(db, str):
        db = env.open_db(str(db).encode())

    with env.begin() as txn:
        return [key.decode() for key, _ in txn.cursor(db)]
