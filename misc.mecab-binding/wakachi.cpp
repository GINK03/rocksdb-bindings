#include <iostream>
#include <string>
#include <mecab.h>
#include "wakachi.hpp"

const char* wakati(const char* text) {
  static MeCab::Tagger *tagger = MeCab::createTagger("-Owakati");
  static std::string result    = ""; 
  result                       = std::string( tagger->parse(text) );
  return std::move(result.c_str());  
}

int main(int argc, char **argv) {
  std::string text      = "一夜一夜にひとみごろ";
  std::cout << wakati(text.c_str()) << std::endl;
}
