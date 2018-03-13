
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace std;
using namespace rocksdb;
extern "C" {
  void helloDB(const char* dbname);
  int putDB(const char* dbname, const char* key, const char* value);
  int getDB(const char* dbname, const char* key, char* value);
  int delDB(const char* dbname, const char* key);
  int keysDB(const char* dbname, char* keys);
}

class DBWrap {
  DB* _db;
  public:
  DBWrap(const char* name) {
    cout << "This is DBWrap" << endl;
    Options options;
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    // create the DB if it's not already present
    options.create_if_missing = true;
    // open DB
    std::string kDBPath(name);
    Status s = DB::Open(options, kDBPath, &this->_db);
    assert(s.ok());
  }
  int put(const char* key, const char* value) { 
    auto s = this->_db->Put(WriteOptions(), key, value);
    assert(s.ok());
    return s.ok();
  }
  int del(const char* key) { 
    auto s = this->_db->Delete(WriteOptions(), key);
    assert(s.ok());
    return s.ok();
  }
  std::tuple<std::string, int> get(const char* key) { 
    std::string value;
    auto s = this->_db->Get(ReadOptions(), key, &value);
    if(s.ok())
      return std::make_tuple<std::string, int>(std::string(value), int(s.ok()));
    else
      return std::make_tuple(std::string(""), 0);
  }
  std::tuple<std::string,int> keys() {
    std::vector<std::string> keysvec;
    rocksdb::Iterator* it = this->_db->NewIterator(rocksdb::ReadOptions());
    for( it->SeekToFirst(); it->Valid(); it->Next()) {
      auto onekey = it->key().ToString();
      keysvec.push_back(onekey);
      keysvec.push_back("");
    }
    // erase last control string
    keysvec.erase( keysvec.end()-1 );
    std::string keystr = "";
    for(auto key : keysvec) 
      keystr += key;

    cout << "keystr " << keystr << endl;
    return std::make_tuple(keystr,1);
  }
};

static std::map<std::string,DBWrap> dbname_dbwrap;

void helloDB(const char* name) {
  cout << "This is C++ " << name << endl;
  cout << endl;
  auto file_name = std::string(name);
  DBWrap dbwrap(file_name.c_str());
  dbname_dbwrap.insert(std::make_pair(file_name, dbwrap));
}

int putDB(const char* dbname, const char* key, const char* value) {
  //cout << "Try to put to " << dbname << endl;
  //cout << "key" << string(key) << endl;
  auto dbwrap = dbname_dbwrap.find(dbname);
  auto status = dbwrap->second.put(key, value);
  return status;
}

int getDB(const char* dbname, const char* key, char* value) {
  //cout << "Try to put to " << dbname << endl;
  //cout << "key" << string(key) << endl;
  auto dbwrap = dbname_dbwrap.find(dbname);
  auto [response, status] = dbwrap->second.get(key);
  //cout << "Fetch finished" << endl;
  std::memcpy(value, response.c_str(), response.length());
  //cout << "Copy finished" << endl;
  return status;
}
int delDB(const char* dbname, const char* key) {
  auto dbwrap = dbname_dbwrap.find(dbname);
  auto status = dbwrap->second.del(key);
  return status;
}

int keysDB(const char* dbname, char* keys) {
  auto dbwrap = dbname_dbwrap.find(dbname);
  auto [response, status] = dbwrap->second.keys();
  std::memcpy(keys, response.c_str(), response.length());
  return status;
}
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
