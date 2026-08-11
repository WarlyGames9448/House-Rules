add_executable(FuzeEditor
    src/EditorApp.cpp
    src/EditorLayer.cpp
    src/ParticleSystem.cpp
    )

target_link_libraries(FuzeEditor
    PRIVATE Fuze
)

target_include_directories(FuzeEditor PRIVATE include)

set_target_properties(FuzeEditor PROPERTIES
    BUILD_RPATH "\$ORIGIN"
    INSTALL_RPATH "\$ORIGIN"
)
