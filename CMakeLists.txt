#设置最低的编译版本
cmake_minimum_required(VERSION 3.10)
#项目名称
project(freeRTOS)
#设置c/c++标准
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#头文件路径
include_directories(
    ${TOOLCHAIN_INC_DIR}
    "src/Configuration Files/"
    "src/External dependence/"
    "src/FreeRTOS/include"
)

#添加-D
add_definitions(-D_CRT_SECURE_NO_WARNINGS 
                -D_DEBUG -D_CONSOLE 
                -D_WIN32_WINNT=0x0601 
                -DWINVER=0x400 
                -D_VC80_UPGRADE=0x0600 
                -D_MBCS)
#关闭警告
add_compile_options(-Wno-int-to-void-pointer-cast -Wno-format)

#添加需要链接的库文件目录 新建好了 链接的时候用
LINK_DIRECTORIES(
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64" 
"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/lib/x64"
"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.24.28314/atlmfc/lib/x64"
"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/lib/x64"
"C:/Program Files (x86)/Windows Kits/10/lib/10.0.18362.0/ucrt/x64"
"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/lib"
"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.7.2/lib/um/x64")
if(AS_A_SUBPROJECT)
    message(STATUS "freeRTOS 作为子项目编译")
    #枚举源文件
    file(GLOB_RECURSE SOURCES "src/FreeRTOS/src/*.c")
    # 指定生成 ncrapi_lvgl_sim_kernel 链接库
    add_library (freeRTOS ${SOURCES})
else() 
    message(STATUS "freeRTOS 作为独立项目编译")
    #枚举源文件
    file(GLOB_RECURSE SOURCES  "src/*.c" "src/*.cpp")
    #生成可执行文件
    add_executable(main ${SOURCES})
    #添加一个run命令
    add_custom_target (run COMMAND ninja all  COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)
endif()










