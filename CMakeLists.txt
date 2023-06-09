cmake_minimum_required(VERSION 3.22)

project(jsoncpp "CXX")

# 设置C++标准(应确保使用的所有第三方库都不超过该标准)
# 本项目基于C++11，但是gtest要求版本不得低于C++14，所以这里指定为C++14
set(CMAKE_CXX_STANDARD 14)

# 构建模式：Debug, Release, ...
set(CMAKE_BUILD_TYPE Debug)

# 是否构建测试代码(ON/OFF)
option(BUILD_TEST_CODE "是否构建测试代码" ON)

# ------------------- JSONCPP ----------------------

# 静态库生成路径
set(JSONCPP_LIB_PATH ${CMAKE_BINARY_DIR}/lib)
# 头文件目录
set(JSONCPP_HEAD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 静态库名称
set(JSONCPP_LIB_NAME ${PROJECT_NAME})

add_subdirectory(src)

# ------------------- For Test ----------------------
# 测试程序生成路径
set(TEST_EXE_PATH ${CMAKE_BINARY_DIR}/bin)

# 第三方库 gtest
set(THIRD_PARTY_GTEST ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gtest)

# 构建测试代码
if(${BUILD_TEST_CODE})
  message(STATUS "构建测试代码")
  add_subdirectory(test)
endif()
