from rdb import RDB

import hashlib 

import random

import concurrent.futures

from timeit import default_timer as timer

def _map(arg):
  key = arg
  # create drow instance
  db = RDB(f'/tmp/boost_{key:09d}.rdb')
  for i in range(100000):
    value = f'{random.random()}'
    ha   = hashlib.sha256(bytes(value, 'utf8')).hexdigest()
    db.put(ha, value)
    val = db.get(ha)

for num in range(20,100, 10):
  start = timer()
  args = list(range(num))
  with concurrent.futures.ProcessPoolExecutor(max_workers=num) as exe:
    exe.map(_map, args)
  end = timer()
  print(f'elapsed {num*100000} {end - start}')
