#include <boost/python.hpp>
#include <string>

namespace py = boost::python;

class RDB{
  public:
  std::string dbName;
  RDB(std::string dbName): dbName(dbName){};
  RDB(py::list ch);
  py::list get_chars();
};

RDB::RDB(py::list l){
  std::string w {""};
  std::string gap {" "};
  std::string token;
  for (int i = 0; i < len(l) ; i++){
     token = py::extract<std::string>(l[i]);
     if (i == len(l) -1){
       w += token;
       break;
      }
      w += token + gap;
  }
  this -> dbName = w;
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
    .def(py::init<py::list>())
    .def("get_chars", &RDB::get_chars)
    .def_readwrite("dbName", &RDB::dbName);
}
