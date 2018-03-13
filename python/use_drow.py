from drow import RDB

# create drow instance
d = RDB('dog')

# access the word and print it
print( d.dbName )

# print the chars
for char in d.get_chars():
    print(char)

# check out if it works for a list
# it will join the words and save them in word variable
d = RDB(['cat', 'dog', 'canary'])

# print me joined
print( d.dbName )
