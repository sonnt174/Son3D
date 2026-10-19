cmake_minimum_required(VERSION 3.16)
project(Son3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# sonGraphic: plain C static library (sources are not modified by this build)
# ---------------------------------------------------------------------------
add_library(sonGraphic STATIC
    sonGraphic/bsp_tree.c
    sonGraphic/link2.c
    sonGraphic/matrix4x4.c
    sonGraphic/point3d.c
    sonGraphic/polygon.c
    sonGraphic/vector3d.c
)

target_include_directories(sonGraphic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sonGraphic)

if(NOT WIN32)
    target_link_libraries(sonGraphic PUBLIC m)
endif()

# ---------------------------------------------------------------------------
# SON_3D: Qt6 Widgets application
# ---------------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(SON_3D
    SON_3D/SON_3D/main.cpp
    SON_3D/SON_3D/son_3d.cpp
    SON_3D/SON_3D/son_3d.h
    SON_3D/SON_3D/SON3DView.cpp
    SON_3D/SON_3D/SON3DView.h
    SON_3D/SON_3D/ISystemDrawing.cpp
    SON_3D/SON_3D/ISystemDrawing.h
    SON_3D/SON_3D/QtDrawing3d.cpp
    SON_3D/SON_3D/QtDrawing3d.h
    SON_3D/SON_3D/son_3d.ui
    SON_3D/SON_3D/son_3d_view.ui
    SON_3D/SON_3D/son_3d.qrc
)

target_include_directories(SON_3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SON_3D/SON_3D)

target_link_libraries(SON_3D PRIVATE sonGraphic Qt6::Widgets)
