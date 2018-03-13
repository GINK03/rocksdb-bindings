#include <boost/python.hpp>
#include <string>
#include <cstdio>
#include <string>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace rocksdb;
namespace py = boost::python;

class RDB{
  private:
  DB* db;
  public:
  std::string dbName;
  RDB(std::string dbName): dbName(dbName){
    Options options;
    options.IncreaseParallelism();
    options.create_if_missing = true;
    Status s = DB::Open(options, dbName, &(this->db)); 
  };
  RDB(py::list ch);
  void put(std::string key, std::string value);
  std::string get(std::string key);
  void del(std::string key);
  py::list get_chars();
};

void RDB::put(std::string key, std::string value) {
  this->db->Put(WriteOptions(), key, value);
}
std::string RDB::get(std::string key) {
  std::string value;
  auto s = this->db->Get(ReadOptions(), key, &value);
  return std::string(value);
}
void RDB::del(std::string key) {
  auto s = this->db->Delete(WriteOptions(), key);
}

py::list RDB::get_chars(){
  py::list char_vec;
  for (auto c : dbName){
    char_vec.append(c);
  }
  return char_vec;
}


BOOST_PYTHON_MODULE(drow){
  py::class_<RDB>("RDB", py::init<std::string>())
    //.def(py::init<py::list>())
    .def("get_chars", &RDB::get_chars)
    .def("put", &RDB::put)
    .def("get", &RDB::get)
    .def("del", &RDB::del)
    .def_readwrite("dbName", &RDB::dbName);
}
