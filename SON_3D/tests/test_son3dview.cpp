// Characterization test cho SON3DView: chốt hành vi HIỆN TẠI của constructor
// (10 khối lập phương + góc quay đầu), paintEvent (chuỗi biến đổi ma trận + eye),
// xử lý chuột/bánh xe và các hàm vẽ public.
//
// Chỉ dùng MỘT SON3DView cho cả executable (dựng trong initTestCase): destructor
// gọi bsp_clear(s_world) nhưng không gán NULL nên đối tượng thứ hai sẽ dùng con trỏ treo.
#include <QtTest>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSet>

#include "SON3DView.h"

static const int kW = 201, kH = 101; // cỡ widget lẻ: chốt cả phép chia nguyên height()/2

static void verifyMatrix(const son_matrix4x4_t &got, const son_matrix4x4_t &want)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            QVERIFY2(qAbs(got.m[i][j] - want.m[i][j]) <= 1e-7 * (1.0 + qAbs(want.m[i][j])),
                     qPrintable(QString("m[%1][%2]: got %3, want %4")
                                    .arg(i).arg(j).arg(got.m[i][j], 0, 'g', 17)
                                    .arg(want.m[i][j], 0, 'g', 17)));
}

class TestSON3DView : public QObject
{
    Q_OBJECT

private:
    SON3DView *view = nullptr;
    son_matrix4x4_t ctorMatrix;
    // trạng thái kỳ vọng, cập nhật song song với các sự kiện gửi cho widget
    double xRot = 3 * PI / 4 * RAD2DEG;
    double yRot = -PI / 4 * RAD2DEG;
    double trX = 0, trY = 0, factor = 1.0;

    // Dựng lại đúng chuỗi biến đổi của paintEvent bằng thư viện, không gọi code app.
    son_matrix4x4_t paintMatrix(son_vector3d_t *outEye) const
    {
        son_matrix4x4_t m, inv;
        son_vector3d_t vx = {{kW / 2.0, 0.0, 0.0}};
        son_vector3d_t vy = {{0.0, (double)(kH / 2), 0.0}};
        son_point3d_t eye = {{0.0, 0.0, 70000.0}};

        matrix4_identity(&m);
        matrix4_rotate(&m, &vx, xRot * DEG2RAD);
        matrix4_rotate(&m, &vy, yRot * DEG2RAD);
        matrix4_translate(&m, trX, trY, 0);
        matrix4_inverse(&m, &inv);
        matrix4_mult_vector(&inv, &eye, outEye);
        matrix4_scale(&m, factor * eye.v[2] / 700);
        return m;
    }

    // Buộc widget vẽ một lần (paintEvent chạy thật) rồi trả ảnh.
    QImage renderView()
    {
        QImage img(kW, kH, QImage::Format_RGB32);
        img.fill(Qt::white);
        view->render(&img);
        return img;
    }

    void verifyPaintState()
    {
        son_vector3d_t wantEye;
        const son_matrix4x4_t want = paintMatrix(&wantEye);
        renderView();
        verifyMatrix(QtDrawing3d::m_modelViewMatrix, want);
        for (int i = 0; i < 3; ++i)
            QVERIFY2(qAbs(QtDrawing3d::eye.v[i] - wantEye.v[i]) <= 1e-7 * (1.0 + qAbs(wantEye.v[i])),
                     qPrintable(QString("eye[%1]: got %2, want %3")
                                    .arg(i).arg(QtDrawing3d::eye.v[i], 0, 'g', 17)
                                    .arg(wantEye.v[i], 0, 'g', 17)));
    }

    void sendMouse(QEvent::Type type, Qt::MouseButton button, Qt::MouseButtons buttons,
                   double x, double y)
    {
        QMouseEvent ev(type, QPointF(x, y), QPointF(x, y), button, buttons, Qt::NoModifier);
        QCoreApplication::sendEvent(view, &ev);
    }

    void sendWheel(int dx, int dy)
    {
        QWheelEvent ev(QPointF(10, 10), QPointF(10, 10), QPoint(0, 0), QPoint(dx, dy),
                       Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
        QCoreApplication::sendEvent(view, &ev);
    }

    // Ảnh để gọi trực tiếp các hàm vẽ public, gốc toạ độ ở tâm (như paintEvent).
    QImage drawOnImage(std::function<void(QPainter *)> body, int size = 160)
    {
        QImage img(size, size, QImage::Format_RGB32);
        img.fill(Qt::white);
        QPainter p(&img);
        p.translate(size / 2, size / 2);
        body(&p);
        p.end();
        return img;
    }

    static void collectPolygons(const bsp_node *n, int *count, QSet<int> *coords)
    {
        if (!n)
            return;
        // danh sách là vòng tròn với node lính canh: duyệt đúng count phần tử
        node2 *it = head_node(&n->polygons);
        for (unsigned k = 0; k < n->polygons.count; ++k, it = next_node(&n->polygons, it)) {
            const son_polygon3d_t *poly = &((const polygon3d_node *)it)->polygon3d;
            son_point3d_t pts[MAX_POINT_PER_POLY];
            polygon3d_get_points(poly, pts);
            for (unsigned i = 0; i < poly->n_ver; ++i)
                for (int k = 0; k < 3; ++k)
                    coords->insert(qRound(pts[i].v[k]));
            ++(*count);
        }
        collectPolygons(n->front, count, coords);
        collectPolygons(n->back, count, coords);
    }

private slots:
    void initTestCase()
    {
        ISystemDrawing::s_world = NULL;
        view = new SON3DView();
        matrix4_cpy(&QtDrawing3d::m_modelViewMatrix, &ctorMatrix);
        view->resize(kW, kH);
    }

    // ---- constructor --------------------------------------------------------

    // Góc khởi tạo: quay 3π/4·RAD2DEG quanh Ox rồi −π/4·RAD2DEG quanh Oy
    // (rotate() nhân lại với DEG2RAD).
    void ctor_pinsInitialRotation()
    {
        son_matrix4x4_t want;
        son_vector3d_t vx = {{1.0, 0.0, 0.0}};
        son_vector3d_t vy = {{0.0, 1.0, 0.0}};
        matrix4_identity(&want);
        matrix4_rotate(&want, &vx, 3 * PI / 4 * RAD2DEG * DEG2RAD);
        matrix4_rotate(&want, &vy, -PI / 4 * RAD2DEG * DEG2RAD);
        verifyMatrix(ctorMatrix, want);
    }

    // 10 khối (drawCube(0..9)), 6 mặt mỗi khối: 60 polygon được thêm vào cây BSP
    // tĩnh, sau khi bị cắt theo các mặt phẳng chia thành 570 polygon. Cạnh dd = 10
    // rồi += i nên mọi toạ độ đỉnh thuộc {0} ∪ {10,11,13,16,20,25,31,38,46,55}.
    void ctor_buildsTenCubesInWorld()
    {
        int count = 0;
        QSet<int> coords;
        collectPolygons(ISystemDrawing::s_world, &count, &coords);

        QCOMPARE(count, 570);
        QList<int> got = coords.values();
        std::sort(got.begin(), got.end());
        QCOMPARE(got, QList<int>({0, 10, 11, 13, 16, 20, 25, 31, 38, 46, 55}));
    }

    // ---- paintEvent ---------------------------------------------------------

    void paint_buildsModelViewMatrixAndEye()
    {
        verifyPaintState();
    }

    // Ảnh render của cảnh mặc định: chốt vài pixel mẫu (gốc toạ độ ở tâm widget,
    // nền đen của palette, cảnh vẽ qua DrawSence).
    void paint_pinsRenderedPixels()
    {
        QImage img = renderView();
        QString sig;
        for (int x : {0, 40, 100, 160, 200})
            for (int y : {0, 25, 50, 75, 100})
                sig += QString("%1 ").arg(img.pixel(x, y) & 0xffffff, 6, 16, QChar('0'));
        QCOMPARE(sig.trimmed(),
                 QString("000000 000000 000000 000000 000000 4c001e 000000 000000 000000 000000 "
                         "e5e500 e5e500 000000 000000 000000 e5e500 000000 000000 000000 000000 "
                         "000000 000000 000000 000000 000000"));
    }

    // drawAxises: hai đường vàng dài 2·9999 qua gốc toạ độ.
    void drawAxises_drawsLongYellowCross()
    {
        QImage img = drawOnImage([this](QPainter *p) { view->drawAxises(p); });
        QCOMPARE(img.pixelColor(80, 20), QColor(Qt::yellow));  // trục dọc
        QCOMPARE(img.pixelColor(150, 80), QColor(Qt::yellow)); // trục ngang
        QCOMPARE(img.pixelColor(81, 20), QColor(Qt::white));
    }

    // ---- chuột --------------------------------------------------------------

    // Nhấn trái rồi kéo: m_yRot += dx, m_xRot += dy.
    void mouse_leftDragRotates()
    {
        sendMouse(QEvent::MouseButtonPress, Qt::LeftButton, Qt::LeftButton, 100, 50);
        sendMouse(QEvent::MouseMove, Qt::NoButton, Qt::LeftButton, 130, 70);
        yRot += 30;
        xRot += 20;
        verifyPaintState();

        // mouseReleaseEvent không xoá trạng thái nhấn: kéo tiếp vẫn quay.
        sendMouse(QEvent::MouseButtonRelease, Qt::LeftButton, Qt::NoButton, 130, 70);
        sendMouse(QEvent::MouseMove, Qt::NoButton, Qt::LeftButton, 125, 80);
        yRot -= 5;
        xRot += 10;
        verifyPaintState();
    }

    // Nhấn phải rồi kéo: m_translateX/Y += dx/dy (góc quay không đổi).
    void mouse_rightDragTranslates()
    {
        sendMouse(QEvent::MouseButtonPress, Qt::RightButton, Qt::RightButton, 10, 20);
        sendMouse(QEvent::MouseMove, Qt::NoButton, Qt::RightButton, 50, 44);
        trX += 40;
        trY += 24;
        verifyPaintState();
    }

    // ---- bánh xe ------------------------------------------------------------

    // Lăn dọc: numSteps = (angleDelta.y()/8)/15 => m_factor *= 2^(0.1·steps).
    void wheel_verticalScalesFactor()
    {
        sendWheel(0, 240);
        factor *= (float)pow(2, 2.0 * 0.1f);
        verifyPaintState();
    }

    // Có cả hai thành phần (x != 0 nhưng y != 0) => KHÔNG phải lăn ngang: dùng y.
    void wheel_bothAxesUsesVertical()
    {
        sendWheel(240, 120);
        factor *= (float)pow(2, 1.0 * 0.1f);
        verifyPaintState();
    }

    // Lăn ngang thuần (y == 0): tỷ lệ không đổi.
    void wheel_horizontalDoesNotScale()
    {
        sendWheel(480, 0);
        verifyPaintState();
    }

    // ---- rotate / translate / scale -----------------------------------------

    void api_translateThenScaleThenRotate()
    {
        son_matrix4x4_t want;
        son_vector3d_t vz = {{0.0, 0.0, 1.0}};
        matrix4_identity(&want);
        matrix4_identity(&QtDrawing3d::m_modelViewMatrix);

        view->translate(3, -5, 7);
        matrix4_translate(&want, 3, -5, 7);
        verifyMatrix(QtDrawing3d::m_modelViewMatrix, want);

        view->scale(4);
        matrix4_scale(&want, 4);
        verifyMatrix(QtDrawing3d::m_modelViewMatrix, want);

        // rotate() nhận độ: 90 độ quanh Oz.
        view->rotate(90, vz);
        matrix4_rotate(&want, &vz, 90 * DEG2RAD);
        verifyMatrix(QtDrawing3d::m_modelViewMatrix, want);
    }

    // ---- hàm vẽ public ------------------------------------------------------

    // drawLine3d chiếu hai đầu qua modelView rồi bỏ z: (0,0,0)-(40,20,0) thành
    // đoạn thẳng từ tâm tới (+40,+20).
    void drawLine3d_drawsProjectedSegment()
    {
        matrix4_identity(&QtDrawing3d::m_modelViewMatrix);
        QImage img = drawOnImage([this](QPainter *p) {
            view->drawLine3d(0, 0, 0, 40, 20, 0, p);
        });
        QCOMPARE(img.pixelColor(80 + 20, 80 + 10), QColor(Qt::black));
        QCOMPARE(img.pixelColor(80 + 40, 80 + 20), QColor(Qt::black));
        QCOMPARE(img.pixelColor(80 + 20, 80 + 20), QColor(Qt::white));
    }

    // m_triangle = (50,0,0), (0,50,0), (0,0,50) => tam giác tô xanh trong góc
    // dương, cạnh huyền x + y = 50.
    void drawTriangle_fillsInitTriangleProjection()
    {
        matrix4_identity(&QtDrawing3d::m_modelViewMatrix);
        QImage img = drawOnImage([this](QPainter *p) { view->drawTriangle(p); });
        QCOMPARE(img.pixelColor(80 + 10, 80 + 10), QColor(Qt::blue));
        QCOMPARE(img.pixelColor(80 + 45, 80 + 45), QColor(Qt::white));
        QCOMPARE(img.pixelColor(80 - 10, 80 + 10), QColor(Qt::white));
    }

    // drawGrid(size, step): i = step..size (bao gồm size), mỗi i vẽ 2 đường song song
    // Ox tại z = ±i và 2 đường song song Oz tại x = ±i, độ dài 2·size.
    // Ma trận y' = y + z để các đường z = ±i không chồng lên nhau.
    void drawGrid_drawsLinesAtEveryStepUpToSize()
    {
        matrix4_identity(&QtDrawing3d::m_modelViewMatrix);
        QtDrawing3d::m_modelViewMatrix.m[1][2] = 1.0;
        QImage img = drawOnImage([this](QPainter *p) { view->drawGrid(20, 10, p); });

        QCOMPARE(img.pixelColor(80 + 15, 80 + 10), QColor(Qt::black)); // z = +10
        QCOMPARE(img.pixelColor(80 + 15, 80 - 20), QColor(Qt::black)); // z = -20
        QCOMPARE(img.pixelColor(80 + 10, 80 + 15), QColor(Qt::black)); // x = +10
        QCOMPARE(img.pixelColor(80 - 20, 80 + 15), QColor(Qt::black)); // x = -20
        QCOMPARE(img.pixelColor(80 + 15, 80 + 15), QColor(Qt::white)); // giữa lưới
        QCOMPARE(img.pixelColor(80 + 25, 80 + 10), QColor(Qt::white)); // ngoài size
        QCOMPARE(img.pixelColor(80 + 15, 80 + 30), QColor(Qt::white)); // ngoài size
    }
};

QTEST_MAIN(TestSON3DView)
#include "test_son3dview.moc"
