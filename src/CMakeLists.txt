cmake_minimum_required(VERSION 3.22)

project(jsoncpp "CXX")

aux_source_directory(./ SRC)

include_directories(${JSONCPP_HEAD_PATH})

# 生成静态库
set(LIBRARY_OUTPUT_PATH ${JSONCPP_LIB_PATH})
add_library(${JSONCPP_LIB_NAME} ${SRC})
