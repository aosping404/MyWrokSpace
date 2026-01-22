cmake_minimum_required(VERSION 3.10)
project(sdLibrary)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 科大讯飞SDK路径（旧版Windows SDK，可选）
set(KDXF_SDK_DIR ${CMAKE_SOURCE_DIR}/KDXF_SDK)

# 科大讯飞Linux SDK路径（新版AIKit SDK）
set(LINUX_AIKIT_SDK_DIR ${CMAKE_SOURCE_DIR}/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5/Linux_aisound_ece9d3c90_v1.1.5_v2.2.15-rc5)

# 添加SDK头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${KDXF_SDK_DIR}/include
    ${LINUX_AIKIT_SDK_DIR}/include
)

# 根据平台选择库文件
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64位Windows
        set(KDXF_LIB_NAME msc_x64)
        set(KDXF_DLL_NAME msc_x64.dll)
    else()
        # 32位Windows
        set(KDXF_LIB_NAME msc)
        set(KDXF_DLL_NAME msc.dll)
    endif()
    set(KDXF_LIB_DIR ${KDXF_SDK_DIR}/libs)
    set(KDXF_DLL_DIR ${KDXF_SDK_DIR}/dll)
elseif(UNIX AND NOT APPLE)
    # Linux系统 - 使用新版AIKit SDK
    set(LINUX_AIKIT_LIB_DIR ${LINUX_AIKIT_SDK_DIR}/libs)
    set(LINUX_AIKIT_LIB_NAME aikit)
    
    # 旧版SDK（如果存在）
    set(KDXF_LIB_DIR ${KDXF_SDK_DIR}/libs)
    
    message(STATUS "Linux平台：使用AIKit SDK")
    message(STATUS "AIKit SDK库目录: ${LINUX_AIKIT_LIB_DIR}")
    message(STATUS "AIKit SDK资源目录: ${LINUX_AIKIT_SDK_DIR}/aisound/resource")
elseif(APPLE)
    # macOS系统
    set(KDXF_LIB_DIR ${KDXF_SDK_DIR}/libs)
    message(STATUS "macOS平台：请确保SDK库文件存在")
endif()

# 添加SDK库文件目录
if(UNIX AND NOT APPLE)
    # Linux平台使用AIKit SDK
    link_directories(${LINUX_AIKIT_LIB_DIR})
else()
    link_directories(${KDXF_LIB_DIR})
endif()

# 添加可执行文件（如果源文件存在）
if(EXISTS ${CMAKE_SOURCE_DIR}/src/main.cpp)
    add_executable(${PROJECT_NAME}
        src/main.cpp
        src/my_math.cpp
    )
endif()

# 添加Hello World语音合成示例
add_executable(hello_world_tts
    src/hello_world_tts.cpp
)

# 链接库文件
if(WIN32)
    # Windows平台链接.lib文件
    target_link_libraries(${PROJECT_NAME} ${KDXF_LIB_NAME})
    
    # 复制DLL文件到输出目录
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${KDXF_DLL_DIR}/${KDXF_DLL_NAME}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
elseif(UNIX AND NOT APPLE)
    # Linux平台 - 链接AIKit库
    # libaikit.so 是主库，ed84113a3_v1010_aee.so 是依赖库，会自动加载
    
    # 为所有可执行文件链接库和复制文件
    set(ALL_TARGETS hello_world_tts)
    if(TARGET ${PROJECT_NAME})
        list(APPEND ALL_TARGETS ${PROJECT_NAME})
    endif()
    
    foreach(TARGET_NAME ${ALL_TARGETS})
        target_link_libraries(${TARGET_NAME} ${LINUX_AIKIT_LIB_NAME})
        
        # 复制库文件到输出目录（确保运行时能找到依赖库）
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LINUX_AIKIT_LIB_DIR}/libaikit.so
            $<TARGET_FILE_DIR:${TARGET_NAME}>
            COMMENT "复制AIKit主库文件"
        )
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LINUX_AIKIT_LIB_DIR}/ed84113a3_v1010_aee.so
            $<TARGET_FILE_DIR:${TARGET_NAME}>
            COMMENT "复制AIKit依赖库文件"
        )
        
        # 复制资源文件到输出目录
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:${TARGET_NAME}>/aisound/resource
            COMMENT "创建资源文件目录"
        )
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LINUX_AIKIT_SDK_DIR}/aisound/resource
            $<TARGET_FILE_DIR:${TARGET_NAME}>/aisound/resource
            COMMENT "复制AIKit资源文件"
        )
        
        # 设置运行时库搜索路径
        set_target_properties(${TARGET_NAME} PROPERTIES
            INSTALL_RPATH "${LINUX_AIKIT_LIB_DIR}"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endforeach()
    
    message(STATUS "Linux平台：已链接AIKit库 (${LINUX_AIKIT_LIB_NAME})")
elseif(APPLE)
    # macOS平台
    # target_link_libraries(${PROJECT_NAME} ${KDXF_LIB_NAME})
    message(WARNING "macOS平台：请根据实际库文件名修改target_link_libraries")
endif()

# 设置运行时库搜索路径（Linux/macOS）
# 注意：运行时库搜索路径已在上面为每个目标单独设置
if(UNIX AND NOT APPLE)
    message(STATUS "已设置运行时库搜索路径: ${LINUX_AIKIT_LIB_DIR}")
elseif(APPLE)
    # macOS平台
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "${KDXF_LIB_DIR}"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()
