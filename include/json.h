// 定义JSON数据结构和相应的操作

#ifndef JSONCPP_INCLUDE_JSON_H_
#define JSONCPP_INCLUDE_JSON_H_

#include <initializer_list>
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace jiayuancs {
namespace jsoncpp {

class Json;
// 具有对称性的运算符通常应为非成员函数
bool operator==(const Json &lhs, const Json &rhs);
bool operator!=(const Json &lhs, const Json &rhs);

std::ostream &operator<<(std::ostream &os, const Json &rhs);

class Json final {
  friend bool operator==(const Json &lhs, const Json &rhs);
  friend bool operator!=(const Json &lhs, const Json &rhs);
  friend std::ostream &operator<<(std::ostream &os, const Json &rhs);

 public:
  typedef std::vector<Json> ArrayType;
  typedef std::map<std::string, Json> ObjectType;
  enum JsonType { kNull, kBool, kInt, kDouble, kString, kArray, kObject };

  // 构造函数
  Json();
  Json(const Json &json);
  Json(JsonType json_type);
  Json(bool value);
  Json(int value);
  Json(long long value);
  Json(double value);
  Json(const char *value);
  Json(const std::string &value);
  Json(const std::initializer_list<Json> &li);
  // 使用初始化列表构造object对象时会与array的构造函数冲突，故删除
  // Json(const std::initializer_list<std::pair<const std::string, Json>> &li);
  Json(const ObjectType &value);

  // 赋值运算符通常是返回该对象的引用
  Json &operator=(const Json &rhs);

  // 析构函数
  ~Json();

  // 运算符重载
  // 定义如下内置类型的转换规则，则operator==会与内置的==运算符冲突，故删除
  // operator bool() const;
  // operator long long() const;
  // operator double() const;
  // operator std::string() const;
  Json &operator[](const int index);
  Json &operator[](const char *key);
  Json &operator[](const std::string &key);

  // 其他函数
  // 序列化字符串，缩进为indent
  std::string dump(unsigned indent = 0) const;
  void dump(std::ostream &os, unsigned indent = 0) const;

  bool IsNull() { return type_ == kNull; }
  bool IsBool() { return type_ == kBool; }
  bool IsInteger() { return type_ == kInt; }
  bool IsDouble() { return type_ == kDouble; }
  bool IsString() { return type_ == kString; }
  bool IsArray() { return type_ == kArray; }
  bool IsObject() { return type_ == kObject; }

  const bool GetBool() const;
  const long long GetInteger() const;
  const double GetDouble() const;
  const std::string &GetString() const;

  // Array操作
  ArrayType &GetArray();
  const ArrayType &GetConstArray() const;

  // Object操作
  ObjectType &GetObject();
  const ObjectType &GetConstObject() const;

 private:
  // 释放内存，类型置为kNull
  void clear();
  // 深拷贝
  void copy(const Json &json);

  JsonType type_;
  union {
    bool bool_value_;
    long long int_value_;
    double double_value_;
    std::string *string_pointer_;
    ArrayType *array_pointer_;
    ObjectType *object_pointer_;
  };
};

}  // namespace jsoncpp
}  // namespace jiayuancs

#endif  // JSONCPP_INCLUDE_JSON_H_
