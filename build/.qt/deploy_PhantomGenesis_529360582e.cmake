include("D:/game/qt/PhantomGenesis/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/PhantomGenesis-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtmultimedia")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/game/qt/PhantomGenesis/build/PhantomGenesis.exe
    GENERATE_QT_CONF
)
