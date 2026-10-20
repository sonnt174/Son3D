# Smoke test (acceptance của task sửa QPainter, docs/19 §23): app khởi động offscreen và còn sống sau 3 giây (timeout = 124,
# không crash), và Qt KHÔNG báo vẽ lên widget ngoài paintEvent.
add_test(NAME smoke_paints_inside_paint_event
         COMMAND sh -c "out=$(QT_QPA_PLATFORM=offscreen timeout 3 \"$<TARGET_FILE:SON_3D>\" 2>&1); code=$?; echo \"$out\"; [ $code -eq 124 ] || { echo \"app thoát sớm: $code\"; exit 1; }; echo \"$out\" | grep -qE 'QPainter::begin|paintEngine: Should no longer be called' && exit 1; exit 0")

# Characterization test cho lớp vẽ QtDrawing3d (biên dịch thẳng nguồn cần thiết).
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(test_qtdrawing3d
    test_qtdrawing3d.cpp
    ../SON_3D/QtDrawing3d.cpp
    ../SON_3D/ISystemDrawing.cpp
)
target_include_directories(test_qtdrawing3d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SON_3D)
target_link_libraries(test_qtdrawing3d PRIVATE sonGraphic Qt6::Widgets Qt6::Test)

add_test(NAME qtdrawing3d COMMAND test_qtdrawing3d)
set_tests_properties(qtdrawing3d PROPERTIES ENVIRONMENT QT_QPA_PLATFORM=offscreen)
