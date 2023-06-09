# JSONCPP

## TODO

- 编写测试用例
- dump实现缩进
- 实现bool、long long、double、string的基本运算，示例如下：
  ```C++
  json j = 42;
  j += 2;
  j = "hello"
  j += " world";
  ```

## 环境

- Ubuntu 22.04
- cmake 3.22.1
- make 4.3
- gtest
- valgrind 3.18.1

使用valgrind测试是否存在内存泄漏

```shell
valgrind --tool=memcheck ./build/bin/jsoncpp_test
```

## 使用方法

动态类型

```C++
Json j;         // 不指定类型时，默认是null类型
Json j7 = {};   // null类型
Json j8{};      // null类型
Json j1(Json::kNull);
Json j2(Json::kInt);
Json j3(Json::kDouble);
Json j4(Json::kString);
Json j5(Json::kArray);
Json j6(Json::kObject);

j = true; // bool类型 

j = 42;     // integer类型

j = 42.42;  // double类型

j = "hello json"; // string类型

j = {42, 3.14, false, "json"};  // array类型

j = {{"type", "json", "value", 42}};  // object类型

```

### array类型

```C++
Json j{"hello"};  // j是array类型，只含一个元素

j.push_back(42);  // j[1] = 42

j[32] = 4224;     // j[2:32]自动填充为null类型

```

### object类型

```C++
Json j{{"type", "json", "value", 42}};  // j是object类型

j["key1"] = true;

j["key2"] = 42;

j["key3"] = 42.42;

j["array"] = {1, 2, "hello", 3, 4, true};

j["array"][2] = 234;

j["object"] = {{"hello", "world"}, {"value", 42}};

j["object"]['object2'] = {{"hello", "world"}, {"value", 42}};  // 多层嵌套
```

### 错误的语法

形如`{{"a", b}, {"c", d}}`的初始化或赋值存在歧义，无法判断其是Array还是Object，因此因避免这种写法。例如：


下面代码编译不通过，`{{"tag", "object"}, {"value", 42}}}`出现歧义

> `{{1, 2}, {"3", 4}}`是Array，不存在歧义

```C++
Json json_array = {json,
                  42,
                  json,
                  24.42,
                  false,
                  {1, 2, 3},
                  {{1, 2}, {"3", 4}},
                  {{"tag", "object"}, {"value", 42}}};
```

可使用如下方法替换上述写法

```C++
Json json_array = {json,
                  42,
                  json,
                  24.42,
                  false,
                  {1, 2, 3},
                  {{1, 2}, {"3", 4}},
                  Json(Json::kObject)};
json_array[7]["tag"] = "object";
json_array[7]["value"] = 42;
```
