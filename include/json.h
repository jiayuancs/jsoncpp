#ifndef JSONCPP_INCLUDE_JSON_H_
#define JSONCPP_INCLUDE_JSON_H_

#include <map>
#include <string>
#include <vector>

namespace jiayuancs {
namespace jsoncpp {

class Json {
 public:
  Json();

 private:
  enum { kNull, kBool, kInt, kDouble, kString, kArray, kObject } type_;
  union {
    bool bool_value_;
    long long int_value_;
    double double_value_;
    std::string *string_pointer_;
    std::vector<Json> *array_pointer_;
    std::map<std::string, Json> *object_pointer_;
  };
};

}  // namespace jsoncpp
}  // namespace jiayuancs

#endif  // JSONCPP_INCLUDE_JSON_H_
