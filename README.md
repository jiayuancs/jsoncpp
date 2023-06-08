# JSONCPP

## TODO

- 定义Json变量时应可以指定类型
- 编写测试用例
- 实现bool、long long、double、string的基本运算，示例如下：
  ```C++
  json j = 42;
  j += 2;
  j = "hello"
  j += " world";
  ```

## 使用方法

动态类型

```C++
Json j;   // 默认是null类型

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
