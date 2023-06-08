cmake_minimum_required(VERSION 3.22)

project(jsoncpp_test "CXX")

# 搜索源文件
aux_source_directory(./ SRC)

# jsoncpp和gtest头文件目录
include_directories(${JSONCPP_HEAD_PATH} ${THIRD_PARTY_GTEST}/include)

# jsoncpp和gtest库目录
link_directories(${JSONCPP_LIB_PATH} ${THIRD_PARTY_GTEST}/lib)
link_libraries(${JSONCPP_LIB_NAME} gtest)

# 可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${TEST_EXE_PATH})
add_executable(${PROJECT_NAME} ${SRC})
