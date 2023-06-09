# JSONCPP

## TODO

- 编写测试用例
- null类型应可以使用`[]`自动转换为`array`或`object`类型而不抛出异常
- dump实现缩进
- 实现bool、long long、double、string的基本运算，示例如下：
  ```C++
  json j = 42;
  j += 2;
  j = "hello"
  j += " world";
  ```
- 实现JSON解析器，能够从流中读取JSON字符串并解析为`Json`对象

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

// 各类型之间可以通过赋值进行转换

j = true; // bool类型 

j = 42;     // integer类型

j = 42.42;  // double类型

j = "hello json"; // string类型

j = {42, 3.14, false, "json"};  // array类型

j = Json::ObjectType{{"type", "json", "value", 42}};  // object类型
```

### array类型

#### 构造array类型对象

##### 方式一

```C++
Json j{"hello", "world", "!"};  // j是array类型，只含一个元素

j[32] = 4224;     // j[3:32]自动填充为null类型
```

##### 方式二

```C++
Json j(Json::kArray);

j[0] = "hello";
j[1] = "world";
j[2] = "!";
j[32] = 4224;
```

#### 直接操作

array类型基于`vector<json>`，使用如下函数可获取该类型的引用，以便直接操作底层数据

```C++
// ArrayType是vector<json>的别名
Json::ArrayType &const_array_value = j.GetConstArray();  // const版本
Json::ArrayType &array_value = j.GetArray();
```

### object类型

#### 构造object类型对象

##### 方式一

```C++
Json json_object(Json::kObject);
json_object["null"] = Json();
json_object["bool"] = true;
json_object["value"] = 42;
json_object["double"] = 3.14;
json_object["hello"] = "world";
json_object["array"] = {1, 2, 3, 3.4, true};
json_object["object"] = Json(Json::kObject);
```

##### 方式二

```C++
Json::ObjectType value = {{"null", Json()},
                          {"bool", true},
                          {"value", 42},
                          {"double", 3.14},
                          {"hello", "world"},
                          {"array", {1, 2, 3, 3.4, true}},
                          {"object", Json(Json::kObject)}};
Json json_object(value);
```

#### 直接操作

object类型基于`map<string, Json>`，使用如下函数可获取该类型的引用，以便直接操作底层数据

```C++
// ObjectType是map<string, Json>的别名
Json::ObjectType &const_map_value = json_object.GetConstObject();  // const版本
Json::ObjectType &map_value = json_object.GetObject();
```

### 序列化

提供如下两个函数将`Json`对象序列化为字符串

```C++
std::string dump(unsigned indent = 0) const;
void dump(std::ostream &os, unsigned indent = 0) const;
```

`Json`对象可直接输出

```C++
std::cout << json_object;
```
