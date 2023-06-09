#include "json.h"

#include <sstream>
#include <stdexcept>

namespace jiayuancs {
namespace jsoncpp {

bool operator==(const Json &lhs, const Json &rhs) {
  if (lhs.type_ != rhs.type_) return false;

  switch (lhs.type_) {
    case Json::kNull:
      return true;
    case Json::kBool:
      return lhs.bool_value_ == rhs.bool_value_;
    case Json::kInt:
      return lhs.int_value_ == rhs.int_value_;
    case Json::kDouble:
      return lhs.double_value_ == rhs.double_value_;
    case Json::kString:
      return *lhs.string_pointer_ == *rhs.string_pointer_;
    case Json::kArray:
      // 逐元素对比
      if (lhs.array_pointer_->size() == rhs.array_pointer_->size()) {
        auto liter = lhs.array_pointer_->cbegin();
        auto riter = rhs.array_pointer_->cbegin();
        for (; liter != lhs.array_pointer_->cend(); ++liter, ++riter) {
          if (*liter != *riter) {
            return false;
          }
        }
        return true;
      }
      return false;
    case Json::kObject:
      // 逐元素比较
      if (lhs.object_pointer_->size() == rhs.object_pointer_->size()) {
        auto liter = lhs.object_pointer_->cbegin();
        auto riter = rhs.object_pointer_->cbegin();
        for (; liter != lhs.object_pointer_->cend(); ++liter, ++riter) {
          if (*liter != *riter) {
            return false;
          }
        }
        return true;
      }
      return false;
    default:
      break;
  }
  return false;
}

bool operator!=(const Json &lhs, const Json &rhs) { return !(lhs == rhs); }

std::ostream &operator<<(std::ostream &os, const Json &rhs) {
  rhs.dump(os);
  return os;
}

Json::Json() : type_(kNull) {}

Json::Json(const Json &json) : type_(json.type_) { copy(json); }

Json::Json(JsonType json_type) : type_(json_type) {
  switch (json_type) {
    case kBool:
      bool_value_ = false;
      break;
    case kInt:
      int_value_ = 0;
      break;
    case kDouble:
      double_value_ = 0.0;
      break;
    case kString:
      string_pointer_ = new std::string();
      break;
    case kArray:
      array_pointer_ = new ArrayType();
      break;
    case kObject:
      object_pointer_ = new ObjectType();
      break;
    default:
      break;
  }
}

Json::Json(bool value) : type_(kBool), bool_value_(value) {}

Json::Json(int value) : type_(kInt), int_value_(value) {}

Json::Json(long long value) : type_(kInt), int_value_(value) {}

Json::Json(double value) : type_(kDouble), double_value_(value) {}

Json::Json(const char *value)
    : type_(kString), string_pointer_(new std::string(value)) {}

Json::Json(const std::string &value)
    : type_(kString), string_pointer_(new std::string(value)) {}

Json::Json(const std::initializer_list<Json> &li)
    : type_(kArray), array_pointer_(new ArrayType(li)) {}

Json::Json(const ObjectType &value)
    : type_(kObject), object_pointer_(new ObjectType(value)) {}

Json &Json::operator=(const Json &rhs) {
  // 处理自我赋值
  if (this == &rhs) return *this;

  // 释放当前对象的空间
  clear();

  copy(rhs);

  return *this;
}

Json::~Json() { clear(); }

Json &Json::operator[](const int index) {
  if (index < 0) {
    throw std::logic_error(
        "function Josn::operator[](const int) requires index > 0");
  }
  if (type_ != kArray) {
    throw std::logic_error(
        "function Josn::operator[](const int) type error, requires array");
  }
  if (index < array_pointer_->size()) {
    return (*array_pointer_)[index];
  }

  // 数组扩容（针对vector）
  array_pointer_->reserve(index + 1);
  while (array_pointer_->size() <= index) {
    array_pointer_->push_back(Json());
  }
  return (*array_pointer_)[index];
}

Json &Json::operator[](const char *key) { return (*this)[std::string(key)]; }

Json &Json::operator[](const std::string &key) {
  if (type_ != kObject) {
    throw std::logic_error(
        "function Json::operator[](const string &) type error, requires "
        "object");
  }
  return (*object_pointer_)[key];
}

std::string Json::dump(unsigned indent) const {
  std::ostringstream oss;
  dump(oss, indent);
  return oss.str();
}

void Json::dump(std::ostream &os, unsigned indent) const {
  switch (type_) {
    case kNull:
      os << "null";
      break;
    case kBool:
      os << std::boolalpha << bool_value_ << std::noboolalpha;
      break;
    case kInt:
      os << int_value_;
      break;
    case kDouble:
      os << double_value_;
      break;
    case kString:
      os << "\"" + (*string_pointer_) + "\"";
      break;
    case kArray:
      os << "[";
      for (auto it = array_pointer_->cbegin(); it != array_pointer_->cend();
           ++it) {
        if (it != array_pointer_->cbegin()) {
          os << ", ";
        }
        it->dump(os, indent);
      }
      os << "]";
      break;
    case kObject:
      os << "{";
      for (auto it = object_pointer_->cbegin(); it != object_pointer_->cend();
           ++it) {
        if (it != object_pointer_->cbegin()) {
          os << ", ";
        }
        os << it->first << " : ";
        it->second.dump(os, indent);
      }
      os << "}";
      break;
    default:
      break;
  }
}

const bool Json::GetBool() const {
  if (type_ != kBool) {
    throw std::logic_error("function Json::GetBool() type error, require bool");
  }
  return bool_value_;
}

const long long Json::GetInteger() const {
  if (type_ != kInt) {
    throw std::logic_error(
        "function Json::GetInteger() type error, require Integer");
  }
  return int_value_;
}

const double Json::GetDouble() const {
  if (type_ != kDouble) {
    throw std::logic_error(
        "function Json::GetDouble() type error, require double");
  }
  return double_value_;
}

const std::string &Json::GetString() const {
  if (type_ != kString) {
    throw std::logic_error(
        "function Json::GetString() type error, require string");
  }
  return *string_pointer_;
}

Json::ArrayType &Json::GetArray() {
  if (type_ != kArray) {
    throw std::logic_error(
        "function Json::GetArray() type error, requires array");
  }
  return *array_pointer_;
}

const Json::ArrayType &Json::GetConstArray() const {
  if (type_ != kArray) {
    throw std::logic_error(
        "function Json::GetConstArray() type error, requires array");
  }
  return *array_pointer_;
}

Json::ObjectType &Json::GetObject() {
  if (type_ != kObject) {
    throw std::logic_error(
        "function Json::GetObject() type error, requires object");
  }
  return *object_pointer_;
}

const Json::ObjectType &Json::GetConstObject() const {
  if (type_ != kObject) {
    throw std::logic_error(
        "function Json::GetConstObject() type error, requires object");
  }
  return *object_pointer_;
}

void Json::clear() {
  switch (type_) {
    case kNull:
    case kBool:
    case kInt:
    case kDouble:
      break;
    case kString:
      delete string_pointer_;
      break;
    // delete数组或对象时，会自动对其中每一个元素调用析构函数
    case kArray:
      delete array_pointer_;
      break;
    case kObject:
      delete object_pointer_;
      break;
    default:
      break;
  }
}

void Json::copy(const Json &json) {
  type_ = json.type_;
  switch (type_) {
    case kBool:
      bool_value_ = json.bool_value_;
      break;
    case kInt:
      int_value_ = json.int_value_;
      break;
    case kDouble:
      double_value_ = json.double_value_;
      break;
    case kString:
      string_pointer_ = new std::string(*json.string_pointer_);
      break;
    case kArray:
      array_pointer_ = new ArrayType(*json.array_pointer_);
      break;
    case kObject:
      object_pointer_ = new ObjectType(*json.object_pointer_);
      break;
    default:
      break;
  }
}

}  // namespace jsoncpp
}  // namespace jiayuancs
