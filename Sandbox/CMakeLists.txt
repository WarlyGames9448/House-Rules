add_executable(Sandbox
    src/SandboxApp.cpp
)

target_link_libraries(Sandbox
    PRIVATE Fuze
)

target_include_directories(Sandbox PRIVATE include)

set_target_properties(Sandbox PROPERTIES
    BUILD_RPATH "\$ORIGIN"
    INSTALL_RPATH "\$ORIGIN"
)
