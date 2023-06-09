// 测试Json数据结构的基本使用

#include "json.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace jiayuancs::jsoncpp;
using namespace std;

TEST(JsonConstructor, TypeConstructorInitState) {
  Json json_default;
  EXPECT_TRUE(json_default.IsNull());

  Json json_null(Json::kNull);
  EXPECT_TRUE(json_null.IsNull());

  Json json_bool(Json::kBool);
  EXPECT_TRUE(json_bool.IsBool());
  EXPECT_FALSE(json_bool.GetBool());
  EXPECT_EQ(json_bool, false);

  Json json_int(Json::kInt);
  EXPECT_TRUE(json_int.IsInteger());
  EXPECT_EQ(json_int.GetInteger(), 0);
  EXPECT_EQ(json_int, 0);

  Json json_double(Json::kDouble);
  EXPECT_TRUE(json_double.IsDouble());
  EXPECT_DOUBLE_EQ(json_double.GetDouble(), 0.0);
  EXPECT_EQ(json_double, 0.0);

  Json json_string(Json::kString);
  EXPECT_TRUE(json_string.IsString());
  EXPECT_EQ(json_string.GetString(), "");
  EXPECT_EQ(json_string, "");

  Json json_array(Json::kArray);
  EXPECT_TRUE(json_array.IsArray());
  const Json::ArrayType &array_value = json_array.GetConstArray();
  EXPECT_EQ(array_value.size(), 0);

  Json json_object(Json::kObject);
  EXPECT_TRUE(json_object.IsObject());
  const Json::ObjectType &object_value = json_object.GetConstObject();
  EXPECT_EQ(object_value.size(), 0);
};

// 测试类型转换
TEST(JsonTypeTest, TypeConversion) {
  Json json;  // 不指定类型时，默认是null类型

  // null转换为array
  json[1] = 12;
  EXPECT_TRUE(json.IsArray());
  EXPECT_EQ(json, Json({Json(), 12}));
  EXPECT_NE(json, Json({Json(), 13}));
  json = {};

  // null转换为object
  json["hello"] = "json";
  EXPECT_TRUE(json.IsObject());
  EXPECT_EQ(json, Json(Json::ObjectType{{"hello", "json"}}));
  EXPECT_NE(json, Json(Json::ObjectType{{"hello", "world"}}));

  // 转换为bool类型
  json = true;
  EXPECT_TRUE(json.IsBool());
  EXPECT_EQ(json, true);
  EXPECT_NE(json, false);

  // 转换为integer类型
  json = 42;
  EXPECT_TRUE(json.IsInteger());
  EXPECT_EQ(json, 42);
  EXPECT_NE(json, 24);

  // 转换为double类型
  json = 42.42;
  EXPECT_TRUE(json.IsDouble());
  EXPECT_EQ(json, 42.42);
  EXPECT_NE(json, 42.24);

  // 转换为string类型
  json = "hello json";
  EXPECT_TRUE(json.IsString());
  EXPECT_EQ(json, "hello json");
  EXPECT_NE(json, "hello world");

  // 转换为array类型
  json = {42, 3.14, false, "json"};
  EXPECT_TRUE(json.IsArray());
  EXPECT_EQ(json, Json({42, 3.14, false, "json"}));
  EXPECT_NE(json, Json({42, 3.14, false, "jsan"}));

  // 转换为object类型
  json = Json::ObjectType{{"type", "json"}, {"value", 42}};
  EXPECT_TRUE(json.IsObject());
  EXPECT_EQ(json, Json(Json::ObjectType{{"type", "json"}, {"value", 42}}));
  EXPECT_NE(json, Json(Json::ObjectType{{"type", "json"}, {"value", 42.2}}));
};

// 测试null类型
TEST(JsonTypeTest, NullType) {
  Json json_default;
  Json json_null(Json::kNull);
  Json json_null_2{};  // 等价 Json json_null_2 = {};

  EXPECT_TRUE(json_default.IsNull());
  EXPECT_TRUE(json_null.IsNull());
  EXPECT_TRUE(json_null_2.IsNull());
  EXPECT_EQ(json_default, json_null);
  EXPECT_EQ(json_null_2, json_default);
  EXPECT_EQ(json_default.dump(), "null");
  EXPECT_EQ(json_null.dump(), "null");
  EXPECT_EQ(json_null_2.dump(), "null");

  cout << json_default << " == " << json_null << endl;
};

// 测试array类型
TEST(JsonTypeTest, ArrayType) {
  Json json_default = {
      Json(), true, 1, 2.0, "3", {"array", 1, 2, 3}, Json(Json::kObject)};

  EXPECT_THROW(json_default.GetObject(), logic_error);
  EXPECT_THROW(json_default["hello"], logic_error);

  Json json_array(Json::kArray);
  json_array[5] = {"array", 1, 2, 3};
  json_array[0] = Json();
  json_array[1] = true;
  json_array[2] = 1;
  json_array[3] = 2.0;
  json_array[4] = "3";
  json_array[6] = Json(Json::kObject);

  EXPECT_EQ(json_array, json_default);
  EXPECT_EQ(json_array.GetConstArray(), json_default.GetConstArray());
  EXPECT_EQ(json_array.dump(), json_default.dump());

  json_default[2] = "hello";
  EXPECT_FALSE(json_default == json_array);

  json_default = {};
  EXPECT_TRUE(json_default.IsNull());
}

// 测试object类型
TEST(JsonTypeTest, ObjectType) {
  Json::ObjectType value = {{"null", Json()},
                            {"bool", true},
                            {"value", 42},
                            {"double", 3.14},
                            {"hello", "world"},
                            {"array", {1, 2, 3, 3.4, true}},
                            {"object", Json(Json::kObject)}};
  Json json_object(value);
  EXPECT_EQ(json_object.GetConstObject(), value);

  Json json_default(Json::kObject);
  json_default["null"] = Json();
  json_default["bool"] = true;
  json_default["value"] = 42;
  json_default["double"] = 3.14;
  json_default["hello"] = "world";
  json_default["array"] = {1, 2, 3, 3.4, true};
  EXPECT_NE(json_default, json_object);
  json_default["object"] = Json(Json::kObject);
  EXPECT_EQ(json_default, json_object);

  json_default = {};
  EXPECT_TRUE(json_default.IsNull());
};

// 测试序列化为字符串
TEST(JsonDumpTest, DumpTest) {
  Json json(Json::kObject);
  json["hello"] = "world";
  json["kint"] = 42;
  json["kdouble"] = 23.4;
  json["kbool"] = false;
  json["knull"] = Json();
  json["karray"] = {1,
                    2,
                    3,
                    "ceshi",
                    34.4,
                    true,
                    Json(),
                    {{1, 2}, {"3", 4}},
                    {"nihao", "shijie", {1, 2, 3}, "hello"},
                    Json(Json::kObject)};

  Json json_array = {json, 42, 24.42, false, {1, 2, 3}, Json(Json::kObject)};
  json_array[5]["tag"] = "object";
  json_array[5]["value"] = 42;

  cout << json_array << endl;
};
