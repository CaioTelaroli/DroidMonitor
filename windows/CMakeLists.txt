add_executable(DroidMonitorHost
    src/main.cpp
    src/Protocol.cpp
    src/TcpClient.cpp
)

target_include_directories(DroidMonitorHost
    PRIVATE include
)

target_compile_definitions(DroidMonitorHost
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

target_link_libraries(DroidMonitorHost
    PRIVATE ws2_32
)

if(MSVC)
    target_compile_options(DroidMonitorHost
        PRIVATE /W4 /permissive-
    )
endif()