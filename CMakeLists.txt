# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.8)

# 定义项目名称
project(raytracer)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)

# 添加头文件目录（修正了变量名的拼写错误）
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置可执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 使用 GLOB 收集源文件（注意：GLOB 通常不推荐用于大型项目）
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")  # 假设源文件是 .cpp 文件

# 添加可执行目标
add_executable(raytracer ${SOURCES})

# 查找并链接 SDL2 库
find_package(SDL2 REQUIRED)
target_include_directories(raytracer PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(raytracer ${SDL2_LIBRARIES})