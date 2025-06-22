cmake_minimum_required(VERSION 3.19)
project(PhantomGenesis LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia)

qt_standard_project_setup()

qt_add_executable(PhantomGenesis
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Map.h Map.cpp
    resources.qrc
    Player.h Player.cpp

    King.h King.cpp
    PhantomBase.h PhantomBase.cpp
    FlamePhantom.h FlamePhantom.cpp
    LurkPhantom.h LurkPhantom.cpp
    ActiveObject.h
    GameManager.h GameManager.cpp
    ActiveObject.cpp

    Resource/black.png
    Resource/portal.png
    Resource/flame.png
    Wave.h Wave.cpp
    PlayerWave.h PlayerWave.cpp
    Resource/ruins.png
    Resource/mapExit.png Resource/mazeExit.png
    Resource/flamePhantomBase.png
    Resource/flamePhantom.png
    Resource/player.png
    FlameWave.h FlameWave.cpp
    Resource/flameWave.png
    Resource/lurkPhantomBase.png
    Resource/lurkPhantom.png
    LurkWave.h LurkWave.cpp
    Resource/lurkWave.png
    VisionMaskItem.h
    VisionMaskItem.cpp
    DropItem.h DropItem.cpp
    Resource/lurkPhantomDrop.png
    Resource/flamePhantomDrop.png
    Resource/PhantomGenesis.ico
    Resource/startPage.gif
    Resource/startPage.mp3
    Resource/trapped0.mp3
    Resource/deadEnd.flac
    Resource/king.png
    KingWave.h KingWave.cpp
    Resource/kingWave.png
    Resource/trapped0.flac
    Resource/portalStart.png
    Resource/portalEnd.png
    Resource/fog.png
    Resource/flameWaveHit.wav Resource/kingWaveHit.wav Resource/lurkWaveHit.wav Resource/playerWaveHit.wav


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

add_custom_command(TARGET PhantomGenesis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resource $<TARGET_FILE_DIR:PhantomGenesis>/Resource)

install(SCRIPT ${deploy_script})
