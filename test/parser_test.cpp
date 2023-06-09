// 测试JSON解析器相关操作

#include "parser.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace jiayuancs::jsoncpp;
using namespace std;

TEST(ParserTest, NullTest) {
  Json json = "hello world";

  json = Parser("null").Parse();
  EXPECT_TRUE(json.IsNull());
  EXPECT_EQ(json, Json());

  EXPECT_THROW(Parser("nul").Parse(), logic_error);
  EXPECT_THROW(Parser("n").Parse(), logic_error);
  EXPECT_THROW(Parser("Null").Parse(), logic_error);
};

TEST(ParserTest, BoolTest) {
  Json json;

  json = Parser("true").Parse();
  EXPECT_TRUE(json.IsBool());
  EXPECT_EQ(json, true);

  json = Parser("false").Parse();
  EXPECT_TRUE(json.IsBool());
  EXPECT_EQ(json, false);

  EXPECT_THROW(Parser("tru").Parse(), logic_error);
  EXPECT_THROW(Parser("fal").Parse(), logic_error);
};

TEST(ParserTest, IntegerTest) {
  Json json;

  json = Parser("42").Parse();
  EXPECT_TRUE(json.IsInteger());
  EXPECT_EQ(json, 42);

  json = Parser("-42").Parse();
  EXPECT_TRUE(json.IsInteger());
  EXPECT_EQ(json, -42);

  EXPECT_THROW(Parser("----").Parse(), logic_error);
  EXPECT_THROW(Parser("-").Parse(), logic_error);
  EXPECT_THROW(Parser("- 3").Parse(), logic_error);
};

TEST(ParserTest, DoubleTest) {
  Json json;

  json = Parser("42.24").Parse();
  EXPECT_TRUE(json.IsDouble());
  EXPECT_EQ(json, 42.24);

  json = Parser("-42.24").Parse();
  EXPECT_TRUE(json.IsDouble());
  EXPECT_EQ(json, -42.24);

  EXPECT_THROW(Parser("-.").Parse(), logic_error);
  EXPECT_THROW(Parser("---....").Parse(), logic_error);
  EXPECT_THROW(Parser("-f.23").Parse(), logic_error);
};

TEST(ParserTest, StringTest) {
  Json json;

  json = Parser("\"hello world\"").Parse();
  EXPECT_TRUE(json.IsString());
  EXPECT_EQ(json, "hello world");

  json = Parser("\"hel\\a\\b\\c\\dlo\\tworld\\b\\n\\fdddd\"").Parse();
  EXPECT_TRUE(json.IsString());
  EXPECT_EQ(json, "hela\bcdlo\tworld\b\n\fdddd");

  EXPECT_EQ(Parser("\"\"").Parse(), "");

  EXPECT_THROW(Parser("").Parse(), logic_error);
  EXPECT_THROW(Parser("\"hello ").Parse(), logic_error);
  EXPECT_THROW(Parser("\"hello\\\"").Parse(), logic_error);
  EXPECT_THROW(Parser("\"").Parse(), logic_error);
};

TEST(ParserTest, ArrayTest) {
  Json json;

  json = Parser(
             "[null, true, 12, 24.12, \"hello world\", {{\"tag\", \"json\"}, "
             "{\"value\",  42}, {\"array\", [null  , true, 12 , 24.12]}}]")
             .Parse();
  EXPECT_TRUE(json.IsArray());

  Json target = {Json(), true, 12, 24.12, "hello world", Json()};
  target[5] = Json::ObjectType{
      {"tag", "json"}, {"value", 42}, {"array", {Json(), true, 12, 24.12}}};
  EXPECT_EQ(json, target);

  EXPECT_THROW(Parser("[").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, 2, 3").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, 2, ").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, 2 3").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, true 3").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, - true 3").Parse(), logic_error);
  EXPECT_THROW(Parser("[1, - true 3}").Parse(), logic_error);
};

TEST(ParserTest, ObjectTest) {
  Json json;

  json = Parser(
             "\"{\"knull\": null,\"kInt\": 432, \"kDouble\": "
             "234.23,\"kString\": \"hello world\",\"kArray\": [1, true, "
             "3],\"kObject\": {\"knull\": null,\"kInt\": 432,\"kDouble\": "
             "234.23,\"kString\": \"hello world\",\"kArray\": [1, true, 3]}}\"")
             .Parse();
  EXPECT_TRUE(json.IsObject());

  Json target =
      Json::ObjectType{{"knull", Json()},        {"kInt", 432},
                       {"kDouble", 234.23},      {"kString", "hello world"},
                       {"kArray", {1, true, 3}}, {"kObject", Json()}};
  target["kObject"] = Json::ObjectType{{"knull", Json()},
                                       {"kInt", 432},
                                       {"kDouble", 234.23},
                                       {"kString", "hello world"},
                                       {"kArray", {1, true, 3}}};
  cout << target << endl;
  EXPECT_EQ(json, target);

  EXPECT_THROW(Parser("{").Parse(), logic_error);
  EXPECT_THROW(Parser("{\"1\": 2, ").Parse(), logic_error);
  EXPECT_THROW(Parser("{\"1\": 2 : \"2\" :334}").Parse(), logic_error);
  EXPECT_THROW(Parser("{\"1\", 2}").Parse(), logic_error);
  EXPECT_THROW(Parser("{\"1\": 2, true}").Parse(), logic_error);
  EXPECT_THROW(Parser("{\"1\": 2, \"true\"}").Parse(), logic_error);
  EXPECT_THROW(Parser("{1, 2}").Parse(), logic_error);
  EXPECT_THROW(Parser("{1: 2}").Parse(), logic_error);
};
