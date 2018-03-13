from rdb import RDB

# create drow instance
db = RDB('/tmp/boost.rdb')

# access the word and print it
print( db.dbName )

db.put('key1', 'val1')

val = db.get('key1')
print(val)
db.put('key2', 'val2')

print(db.keys())
val = db.delete('key1')

