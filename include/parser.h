// JSON解析器相关数据结构和操作

#ifndef JSONCPP_INCLUDE_PARSER_H_
#define JSONCPP_INCLUDE_PARSER_H_

#include <sstream>

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
  inline void SkipSpace();
  inline int GetNextToken();
  inline void ThrowError(const char *info_str);
  inline void ThrowError(const char *info_str, const char value);
  inline void ThrowError(const char *info_str, const char *value);

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
