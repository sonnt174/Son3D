# Smoke test (acceptance của task sửa QPainter, docs/19 §23): app khởi động offscreen và còn sống sau 3 giây (timeout = 124,
# không crash), và Qt KHÔNG báo vẽ lên widget ngoài paintEvent.
add_test(NAME smoke_paints_inside_paint_event
         COMMAND sh -c "out=$(QT_QPA_PLATFORM=offscreen timeout 3 \"$<TARGET_FILE:SON_3D>\" 2>&1); code=$?; echo \"$out\"; [ $code -eq 124 ] || { echo \"app thoát sớm: $code\"; exit 1; }; echo \"$out\" | grep -qE 'QPainter::begin|paintEngine: Should no longer be called' && exit 1; exit 0")
