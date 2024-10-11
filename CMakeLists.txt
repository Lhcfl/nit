cmake_minimum_required(VERSION 3.10)  # 设置CMake的最低版本要求
set(CMAKE_CXX_STANDARD 20)

project(nit VERSION 1.0)  # 定义项目名称和版本号

# 将当前目录下的include目录添加到头文件搜索路径
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# include_directories(${CMAKE_SOURCE_DIR})
SET(BASEPATH "${CMAKE_SOURCE_DIR}")
include_directories("${BASEPATH}")

if(MSVC)
add_compile_options("/source-charset:utf-8")
endif()

if(NOT MSVC)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS, 1)
endif()

# 添加源文件
set(SOURCES
    main.cpp
    apis/apis.cpp
    apis/sha1.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 如果需要链接其他库，请取消注释并修改以下行
# target_link_libraries(${PROJECT_NAME} PRIVATE some_library)