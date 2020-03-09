#设置最低的编译版本
cmake_minimum_required(VERSION 3.10)
#项目名称
project(ncrapi_lvgl_sim_kernel)
#设置c/c++标准
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#头文件路径
include_directories(
    ${TOOLCHAIN_INC_DIR}
    "include"
    "include/freeRTOS"
    "include/demo"
)
#添加-D
add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_DEBUG -D_CONSOLE -D_WIN32_WINNT=0x0601 -DWINVER=0x400 -D_VC80_UPGRADE=0x0600 -D_MBCS)
#关闭警告
add_compile_options(-Wno-int-to-void-pointer-cast -Wno-format)
set(MODE 1)# 1正常编译 2生成静态库
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) #设置二进制文件存放路径 
#SET(LIB_OUTPUT_PATH "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64") #设置各种库存放路径
if(MODE EQUAL 1)
    #枚举头文件
    #file(GLOB_RECURSE INCLUDES "include/*.h" "include/*.hpp")
    #枚举源文件
    file(GLOB_RECURSE SOURCES  "src/*.c" "src/*.cpp")
    #添加需要链接的库文件目录 新建好了 链接的时候用
    LINK_DIRECTORIES(
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64" 
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/lib/x64"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/atlmfc/lib/x64"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/lib/x64"
    "C:/Program Files (x86)/Windows Kits/10/lib/10.0.18362.0/ucrt/x64"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/lib"
    "C:/Program Files (x86)/Windows Kits/NETFXSDK/4.7.2/lib/um/x64")
    #生成可执行文件
    add_executable(main ${SOURCES})
    target_link_libraries(main )#不链接pthread easylog编译报错
    #添加一个run命令
    add_custom_target (run COMMAND ninja all  COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)
else() 
    #要建立的静态库
    file(GLOB_RECURSE LIBLVGL "include/ncrapi_lvgl_sim_kernel/lvgl/*.c")
    file(GLOB_RECURSE LIBDRIVERS "include/ncrapi_lvgl_sim_kernel/lv_drivers/*.c")
    message(STATUS "LIBLVGL_PATH:${LIBLVGL}")
    message(STATUS "LIBDRIVERS_PATH:${LIBDRIVERS}")
    #file(GLOB_RECURSE LIBRESOURCES "./src/system/logo.c" "./src/system/field.c")
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
        message(STATUS "linux")
        #寻找sdl2
        find_package(SDL2 REQUIRED SDL2)
        include_directories(${SDL2_INCLUDE_DIRS})
        add_library(lvgl_gcc STATIC ${LIBLVGL})
        add_library(drivers_gcc STATIC ${LIBDRIVERS})
        #add_library(resources STATIC ${LIBRESOURCES}_linux)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
        message(STATUS "windows")
        #添加-D
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        add_library(lvgl_clang_win STATIC ${LIBLVGL})
        add_library(drivers_clang_win STATIC ${LIBDRIVERS})
        #add_library(resources STATIC ${LIBRESOURCES}_win)
    else()
        message(STATUS "无法判断属于哪种系统!")
    endif()
endif()








