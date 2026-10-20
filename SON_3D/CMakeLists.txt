cmake_minimum_required(VERSION 3.16)
project(SON_3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thư viện C sonGraphic (sub-project riêng, có test riêng) — chỉ biên dịch nguồn, không sửa.
set(SONGRAPHIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sonGraphic)
add_library(sonGraphic STATIC
    ${SONGRAPHIC_DIR}/bsp_tree.c
    ${SONGRAPHIC_DIR}/link2.c
    ${SONGRAPHIC_DIR}/matrix4x4.c
    ${SONGRAPHIC_DIR}/point3d.c
    ${SONGRAPHIC_DIR}/polygon.c
    ${SONGRAPHIC_DIR}/vector3d.c
)
target_include_directories(sonGraphic PUBLIC ${SONGRAPHIC_DIR})
if(NOT WIN32)
    target_link_libraries(sonGraphic PUBLIC m)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(SON_3D
    SON_3D/main.cpp
    SON_3D/son_3d.cpp
    SON_3D/son_3d.h
    SON_3D/SON3DView.cpp
    SON_3D/SON3DView.h
    SON_3D/ISystemDrawing.cpp
    SON_3D/ISystemDrawing.h
    SON_3D/QtDrawing3d.cpp
    SON_3D/QtDrawing3d.h
    SON_3D/son_3d.ui
    SON_3D/son_3d_view.ui
    SON_3D/son_3d.qrc
)
target_include_directories(SON_3D PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SON_3D)
target_link_libraries(SON_3D PRIVATE sonGraphic Qt6::Widgets)

# Test: tests/CMakeLists.txt (nằm trong thư mục test) khai test bằng add_test — thêm test không phải sửa file này.
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    enable_testing()
    add_subdirectory(tests)
endif()
