#include <iostream>
#include <string>
#include <mecab.h>
#include <string.h>
#include <stdio.h>

#include "wakachi.hpp"

const char* wakati(const char* text) {
  static MeCab::Tagger *tagger = MeCab::createTagger("-Owakati");
  static std::string result    = ""; 
  result                       = std::string( tagger->parse(text) );
  return std::move(result.c_str());  
}

const char* wakati2(const char* text) {
  MeCab::Tagger *tagger = MeCab::createTagger("-Owakati");
  int length      = static_cast<int>(strlen( text )) * 2;
  char* result    = new char[length];
  strcpy(result, tagger->parse(text));
  delete tagger;
  //std::cout << result << std::endl;
  return result;  
}

int test() {
  static int i = 1;
  i++;
  return i;
}
int main(int argc, char **argv) {
  std::string text      = "一夜一夜にひとみごろ";
  std::cout << wakati(text.c_str()) << std::endl;
}
