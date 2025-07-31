file(GLOB_RECURSE QML_UI_FILES RELATIVE "${CMAKE_SOURCE_DIR}/qml" CONFIGURE_DEPENDS "ui/*.qml")

qt_add_qml_module(SelfBalancingTreesVisualizer_exe
    URI app
    RESOURCE_PREFIX /
    VERSION 1.0
    QML_FILES ${QML_UI_FILES}
)
