// 测试Json数据结构的基本使用

#include "json.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

using namespace jiayuancs::jsoncpp;
using std::string;
using std::vector;

TEST(JsonTest, IntegerTest) {
  Json json;
  EXPECT_EQ(json.IsNull(), true);

  int value = 42;
  json = value;
  EXPECT_EQ(json.IsNull(), false);
  EXPECT_EQ(json.IsInteger(), true);
  EXPECT_EQ(json.GetInteger(), value);
  EXPECT_EQ(json, value);
};

TEST(JsonTest, StringTest) {
  string value = "hello world";

  // 使用初始化列表得到的是数组类型
  Json json{value};
  EXPECT_EQ(json.IsArray(), true);

  json = value;
  EXPECT_EQ(json.IsString(), true);
  EXPECT_EQ(json, value);
};
