cmake_minimum_required(VERSION 3.19)
project(PhantomGenesis LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(PhantomGenesis
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Map.h Map.cpp
    resources.qrc
    Resource/map1.png

    Resource/map0.png
    Player.h Player.cpp
    Resource/player.png
    King.h King.cpp
    PhantomBase.h PhantomBase.cpp
    FlamePhantom.h FlamePhantom.cpp
    LurkPhantom.h LurkPhantom.cpp
    ActiveObject.h
    GameManager.h GameManager.cpp
    ActiveObject.cpp
    Resource/player.gif
    Resource/black.png
    Resource/portal.png
    Resource/flame.png

)

target_link_libraries(PhantomGenesis
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS PhantomGenesis
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PhantomGenesis
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

add_custom_command(TARGET PhantomGenesis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resource $<TARGET_FILE_DIR:PhantomGenesis>/Resource)

install(SCRIPT ${deploy_script})
