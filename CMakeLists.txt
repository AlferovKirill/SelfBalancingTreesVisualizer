cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    SelfBalancingTreesVisualizer
    VERSION 0.1.0
    DESCRIPTION "A graphical visualizer for self-balancing binary search trees (AVL, Splay) with extensible architecture, implemented in C++ and Qt/QML. The project lets you explore insert, delete, and search operations step by step, visualize tree rotations, and observe internal state changes."
    HOMEPAGE_URL "https://github.com/AlferovKirill/SelfBalancingTreesVisualizer"
    LANGUAGES CXX
)

set(AUTHOR "Alferov Kirill")

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Quick QuickControls2 LinguistTools)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE LIB_SOURCES "source/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp")

add_library(SelfBalancingTreesVisualizer_lib OBJECT ${LIB_SOURCES})

target_include_directories(SelfBalancingTreesVisualizer_lib ${warning_guard} PUBLIC "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>")

target_compile_features(SelfBalancingTreesVisualizer_lib PUBLIC cxx_std_17)

target_link_libraries(SelfBalancingTreesVisualizer_lib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
)

# ---- Declare executable ----

qt_add_executable(SelfBalancingTreesVisualizer_exe source/main.cpp)

add_executable(SelfBalancingTreesVisualizer::exe ALIAS SelfBalancingTreesVisualizer_exe)

set_property(TARGET SelfBalancingTreesVisualizer_exe PROPERTY OUTPUT_NAME SelfBalancingTreesVisualizer)

target_compile_features(SelfBalancingTreesVisualizer_exe PRIVATE cxx_std_17)

target_link_libraries(SelfBalancingTreesVisualizer_exe PRIVATE SelfBalancingTreesVisualizer_lib)

set_target_properties(SelfBalancingTreesVisualizer_exe PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ---- Declare paths for configure files ----

set(CONFIG_TEMPLATE_DIR ${CMAKE_SOURCE_DIR}/config)
set(CONFIG_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/source/config)

include(cmake/config/project_description.cmake)

# ---- Declare QML modules and assets ----

file(GLOB_RECURSE QML_UI_FILES RELATIVE "${CMAKE_SOURCE_DIR}" CONFIGURE_DEPENDS "qml/ui/*.qml")

qt_add_qml_module(SelfBalancingTreesVisualizer_exe
    URI app
    RESOURCE_PREFIX /
    VERSION 1.0
    QML_FILES ${QML_UI_FILES}
)

add_subdirectory(assets)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT SelfBalancingTreesVisualizer_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of SelfBalancingTreesVisualizer"
  )
endif()

include(cmake/dev-mode.cmake)