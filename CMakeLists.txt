cmake_minimum_required(VERSION 3.19)
project(PhantomGenesis LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

qt_standard_project_setup()

qt_add_resources(RESOURCES
    resources.qrc
)

qt_add_executable(PhantomGenesis
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Map.h Map.cpp
    Player.h Player.cpp
    King.h King.cpp
    PhantomBase.h PhantomBase.cpp
    FlamePhantom.h FlamePhantom.cpp
    LurkPhantom.h LurkPhantom.cpp
    ActiveObject.h
    GameManager.h GameManager.cpp
    ActiveObject.cpp
    Wave.h Wave.cpp
    PlayerWave.h PlayerWave.cpp
    FlameWave.h FlameWave.cpp
    LurkWave.h LurkWave.cpp
    VisionMaskItem.h
    VisionMaskItem.cpp
    DropItem.h DropItem.cpp
    KingWave.h KingWave.cpp
    ${RESOURCES}

)

target_link_libraries(PhantomGenesis
    PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::Multimedia
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


install(SCRIPT ${deploy_script})
