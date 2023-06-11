// JSON解析器相关数据结构和操作

#ifndef JSONCPP_INCLUDE_PARSER_H_
#define JSONCPP_INCLUDE_PARSER_H_

#include <sstream>
#ifndef NDEBUG
#include <iostream>
#endif  // NDEBUG

#include "json.h"

namespace jiayuancs {
namespace jsoncpp {

class Parser final {
 public:
  Parser(std::istream &is) : in_str_(&is), need_delete_(false), line_no_(1) {}
  Parser(const std::string &is)
      : in_str_(new std::istringstream(is)), need_delete_(true), line_no_(1) {}
  ~Parser();

  Json Parse();

 private:
  void SkipSpace();
  // 内联函数应定义(而不是仅声明)在头文件中
  int GetNextToken() {
    SkipSpace();
#ifndef NDEBUG
    int ch_debug = in_str_->peek();
    if (ch_debug == EOF) {
      std::clog << "EOF";
    } else {
      std::clog << static_cast<char>(ch_debug);
    }
    std::clog << std::endl;
#endif  // NDEBUG
    return in_str_->get();
  }
  void ThrowError(const char *info_str);
  void ThrowError(const char *info_str, const char value);
  void ThrowError(const char *info_str, const char *value);

  Json ParseNull();
  Json ParseBool(bool value);
  Json ParseNumber(bool positive);
  std::string ParseString();
  Json ParseArray();
  Json ParseObject();

  std::istream *in_str_;
  bool need_delete_;
  int line_no_;
};

}  // namespace jsoncpp
}  // namespace jiayuancs

#endif  // JSONCPP_INCLUDE_PARSER_H_
