#include "parser.h"

#include <cstring>

namespace jiayuancs {
namespace jsoncpp {

Parser::~Parser() {
  if (need_delete_) {
    delete in_str_;
  }
}

Json Parser::Parse() {
  int token = GetNextToken();
  switch (token) {
    case 'n':
      in_str_->putback(token);
      return ParseNull();
    case 't':
      in_str_->putback(token);
      return ParseBool(true);
    case 'f':
      in_str_->putback(token);
      return ParseBool(false);
    case '-':
      return ParseNumber(false);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      in_str_->putback(token);
      return ParseNumber(true);
    case '\"':
      return Json(ParseString());
    case '[':
      return ParseArray();
    case '{':
      return ParseObject();
    case EOF:
      ThrowError("expected more characters, but got eof");
    default:
      break;
  }

  ThrowError("unexpected character ", token);
  return Json();
}

void Parser::SkipSpace() {
  char tmp = ' ';
  while (in_str_->get(tmp)) {
    if (tmp == ' ' || tmp == '\t' || tmp == '\n' || tmp == '\r') {
      // 记录行号，便于排错
      if (tmp == '\n') {
        ++line_no_;
      }
    } else {
      in_str_->putback(tmp);
      break;
    }
  }
}

int Parser::GetNextToken() {
  SkipSpace();
  return in_str_->get();
}

void Parser::ThrowError(const char *info_str) {
  std::ostringstream error_info;
  error_info << "syntax error in line " << line_no_ << ": " << info_str;
  throw std::logic_error(error_info.str());
}

void Parser::ThrowError(const char *info_str, const char value) {
  std::ostringstream error_info;
  error_info << "syntax error in line " << line_no_ << ": " << info_str << "\""
             << value << "\"";
  throw std::logic_error(error_info.str());
}

void Parser::ThrowError(const char *info_str, const char *value) {
  std::ostringstream error_info;
  error_info << "syntax error in line " << line_no_ << ": " << info_str
             << value;
  throw std::logic_error(error_info.str());
}

Json Parser::ParseNull() {
  char buf[5];
  in_str_->read(buf, 4);
  buf[4] = '\0';
  if (strcmp(buf, "null") == 0) {
    return Json();
  }

  ThrowError("expected null, but was ", buf);
  return Json();
}

Json Parser::ParseBool(bool value) {
  char buf[6];
  if (value) {
    in_str_->read(buf, 4);
    buf[4] = '\0';
    if (strcmp(buf, "true") == 0) {
      return Json(true);
    }
  } else {
    in_str_->read(buf, 5);
    buf[5] = '\0';
    if (strcmp(buf, "false") == 0) {
      return Json(false);
    }
  }

  ThrowError("expected bool value (true or false), but was ", buf);
  return Json();
}

Json Parser::ParseNumber(bool positive) {
  int token = EOF;
  long long numerator = 0;    // 分子数值
  long long denominator = 1;  // 分母数值
  long long sign = positive ? 1 : -1;
  bool dot_flag = false;     // 是否已读取到小数点
  bool number_char = false;  // 是否读取到数字字符

  while ((token = in_str_->get()) &&
         (token >= '0' && token <= '9' || token == '.')) {
    if (token == '.') {
      if (dot_flag == true) {  // 多次出现小数点，数字不合法
        ThrowError("invalid number");
      }
      dot_flag = true;
      continue;
    }

    numerator *= 10;
    numerator += token - '0';
    if (dot_flag) {
      denominator *= 10;
    }
    number_char = true;
  }

  if (token != EOF) {
    in_str_->putback(token);
  }

  if (!number_char) {  // 未读取到数字字符
    ThrowError("invalid number");
  }

  numerator *= sign;
  if (dot_flag) {  // 浮点数
    return Json((double)numerator / (double)denominator);
  }
  return Json(numerator);  // 整数
}

std::string Parser::ParseString() {
  int token = EOF;
  bool valid = false;
  std::string str_value = "";
  while ((token = in_str_->get()) != EOF) {
    if (token == '\"') {
      valid = true;
      break;
    }

    if (token != '\\') {
      str_value += token;
    } else {  // 转义字符
      token = in_str_->get();
      switch (token) {
        case '\"':
          str_value += "\"";
          break;
        case '\\':
          str_value += "\\";
          break;
        case '/':
          str_value += "/";
          break;
        case 'b':
          str_value += "\b";
          break;
        case 'f':
          str_value += "\f";
          break;
        case 'n':
          str_value += "\n";
          break;
        case 'r':
          str_value += "\r";
          break;
        case 't':
          str_value += "\t";
          break;
        case 'u':
          // TODO: 处理JSON数据中的\u转义字符
          ThrowError("this escape character is not currently supported");
          break;
        default:
          str_value += token;
          break;
      }
    }
  }

  if (valid) {
    return str_value;
  }

  ThrowError("invalid string");
  return "";
}

Json Parser::ParseArray() {
  int token = GetNextToken();

  Json json_array = Json(Json::kArray);
  if (token == ']') {
    return json_array;
  }

  in_str_->putback(token);

  for (int idx = 0;; ++idx) {
    json_array[idx] = Parse();

    token = GetNextToken();

    if (token == ']') {
      break;
    }

    if (token != ',') {
      ThrowError("invalid array");
    }
  }

  return json_array;
}

Json Parser::ParseObject() {
  int token = GetNextToken();

  Json json_object = Json(Json::kObject);
  if (token == '}') {
    return json_object;
  }

  in_str_->putback(token);

  std::string key;
  for (;;) {
    if ((token = GetNextToken()) != '\"') {
      ThrowError("expected \'\"\' in object");
    }

    // 解析key
    key = ParseString();

    if ((token = GetNextToken()) != ':') {
      ThrowError("expected \':\' in object");
    }

    // 解析value
    json_object[key] = Parse();

    token = GetNextToken();
    if (token == '}') {
      break;
    }
    if (token != ',') {
      ThrowError("expected \',\' in object");
    }
  }

  return json_object;
}

}  // namespace jsoncpp
}  // namespace jiayuancs
