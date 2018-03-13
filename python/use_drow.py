from drow import RDB

# create drow instance
db = RDB('boost.rdb')

# access the word and print it
print( db.dbName )

db.put('key1', 'val1')

val = db.get('key1')
print(val)
