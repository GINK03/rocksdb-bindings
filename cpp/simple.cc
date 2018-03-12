
#include <cstdio>
#include <string>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace std;
using namespace rocksdb;


int main() {
  cout << "hoge" << endl;

  DB* db;
  Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  std::string kDBPath = "test.rdb";
  Status s = DB::Open(options, kDBPath, &db);
  cout << s.ok() << endl;
  //assert(s.ok());

  // Put key-value
  s = db->Put(WriteOptions(), "key1", "value");
  assert(s.ok());

  // get value
  std::string value;
  s = db->Get(ReadOptions(), "key1", &value);
  assert(s.ok());
  assert(value == "value");

  // memcopyを防ぎ、pinnableでデータを受け取る
  string loc;
  PinnableSlice pinnable(&loc);
  s = db->Get(ReadOptions(), db->DefaultColumnFamily(), "key1", &pinnable);
  // copy to localstring
  cout << "pinnable-loc:" << loc << endl;
 
  // dbをiterate, scanする
  auto it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    cout << it->key().ToString() << ": " << it->value().ToString() << endl;
  }
  assert(it->status().ok());
  delete it;

  // キーの生存確認
  s = db->Get(ReadOptions(), db->DefaultColumnFamily(), "key1", &pinnable);
  cout << "undef of key1:" << s.IsNotFound() << endl;
  s = db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable);
  cout << "undef of key2:" << s.IsNotFound() << endl;

  // keyをデリート
  db->Delete(rocksdb::WriteOptions(), "key1");

}
