# JSONCPP

## TODO

- [ ] 增加dump缩进支持
- [ ] 增加bool、long long、double、string等类型的基本运算支持，示例如下：
  ```C++
  json j = 42;
  j += 2;
  j = "hello"
  j += " world";
  ```
- [ ] 增加`\u`转义字符支持
- [ ] 增加注释支持(`//`和`/**/`)
- [ ] 增加单引号字符串支持(`''`)

## 环境

- Ubuntu 22.04
- cmake 3.22.1
- make 4.3
- g++ 11.3.0
- gtest
- valgrind 3.18.1

使用gtest进行单元测试

使用valgrind测试是否存在内存泄漏

```shell
valgrind --tool=memcheck ./build/bin/jsoncpp_test
```

## 编译

默认构建类型为`Debug`，且自动构建测试代码，可在根目录的`CMakeLists.txt`中修改：

```cmake
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

option(BUILD_TEST_CODE "是否构建测试代码" ON)
# option(BUILD_TEST_CODE "是否构建测试代码" OFF)
```

构建`jsoncpp`的命令如下：

```shell
git clone https://github.com/jiayuancs/jsoncpp.git
cd jsoncpp
mkdir build && cd build
cmake ..
make
```

编译得到的静态库文件存放在`build/lib`目录下，名为`libjsoncpp.a`

如果设置了`BUILD_TEST_CODE=ON`，则生成的可执行测试程序存放在`build/bin`目录下，名为`jsoncpp_test`

## 介绍

`jsoncpp`是一个基于C++11的JSON库，主要包括两个类：

- `jiayuancs::jsoncpp::Json`类：实现了JSON数据结构和相应的操作，位于头文件`json.h`
- `jiayuancs::jsoncpp::Parser`类：实现反序列化，位于头文件`parser.h`

一个`Json`对象具有6种类型，各类型与C++中类型对应关系如下：

- `null`类型
- `bool`类型：基于`bool`类型
- `number`类型：该类型细分为两种类型
  - `integer`类型：基于`long long`类型
  - `double`类型：基于`double`类型
- `string`类型：基于`std::string`类型
- `array`类型：基于`std::vector<Json>`类型
- `object`类型：基于`std::map<std::string, Json>`类型

## 使用方法

各类型的`Json`对象之间可以直接赋值

```C++
Json j;                 // 不指定类型时，默认是null类型
Json j1 = {};           // null类型
Json j2{};              // null类型
Json j3(Json::kNull);   // null类型
Json j4(Json::kBool);   // bool类型
Json j5(Json::kInt);    // integer类型
Json j6(Json::kDouble); // double类型
Json j7(Json::kString); // string类型
Json j8(Json::kArray);  // array类型
Json j9(Json::kObject); // object类型

// 各类型之间可以直接赋值（使用深拷贝）

j = true; // bool类型 

j = 42;     // integer类型

j = 42.42;  // double类型

j = "hello json"; // string类型

j = {42, 3.14, false, "json"};  // array类型

j = Json::ObjectType{{"type", "json"}, {"value", 42}};  // object类型
```

比较`Json`对象

```C++
// 只有当j1与j2的类型相同，且值相同时，条件才为真
j1 == j2;

// 等价于 !(j1 == j2)
j1 != j2;

// 所有的null类型都是相等的
Json() == Json();  // return true
```

### array类型

#### 构造array类型对象

##### 方式一

```C++
Json j{"hello", "world", "!", 42};  // j是array类型

j[32] = 4224;     // j[4:32]自动填充为null类型
```

##### 方式二

```C++
Json j;  // 或者明确指明类型 Json j(Json::kArray);

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
Json json_object;  // 或者明确指明类型 Json json_object(Json::kObject);
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

`Json`对象提供如下两个成员函数实现将当前对象序列化为字符串

```C++
std::string dump(unsigned indent = 0) const;
void dump(std::ostream &os, unsigned indent = 0) const;
```

示例：

```C++
string str_value = json_object.dump();
```

`Json`对象可直接输出

```C++
std::cout << json_object;
```

### 反序列化

类`Parser`用于反序列化，有两种方法：

方法一：JSON字符串反序列化为`Json`对象

```C++
// string
Json json_object = Parser("[null, true, 12, 24.12]").Parse();
```

方法二：从输入流(`istream`)中读取JSON字符串，反序列化为`Json`对象

```C++
// istream (以ifstream为例)
ifstream ifs("./data.json");
Json json_object = Parser(ifs).Parse();
```

### 注意

`jsoncpp`使用**前缀匹配**，前缀匹配成功时会自动返回，不再读取后面的字符

```C++
Json json;

json = Parser("nulll").Parse();  // 匹配前缀null，得到null类型

json = Parser("truea").Parse();  // 匹配前缀true，得到bool类型

json = Parser("falsee").Parse(); // 匹配前缀false，得到bool类型

json = Parser("43224fa34").Parse();  // 匹配前缀43224，得到integer类型

json = Parser("432.24fa34").Parse(); // 匹配前缀432.24，得到double类型

json = Parser("\"hello world\"sdfj").Parse();  // 匹配前缀"hello world"，得到string类型

json = Parser("[1, 2, 3]sdfj").Parse();  // 匹配前缀"[1, 2, 3]"，得到array类型

json = Parser("{\"type\": 23}sdfj").Parse();  // 匹配前缀"{"type": 23}"，得到object类型
```
