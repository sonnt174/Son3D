// Characterization test cho QtDrawing3d: chốt hành vi HIỆN TẠI của ComputeLight,
// DrawPolygon (thêm vào BSP tĩnh), DrawSence (thứ tự duyệt + màu pixel) và Look().
#include <QtTest>
#include <QImage>
#include <QPainter>

#include "QtDrawing3d.h"

// shade của một polygon có pháp tuyến (0,0,1) với vector sáng unit(400,800,600):
//   dot = 600/sqrt(1160000) = 0.5570860145311556
//   shade = dot*0.8 + 0.2
static const double kShadeZ = 0.6456688116249245;

class TestQtDrawing3d : public QObject
{
    Q_OBJECT

private:
    // Đối tượng duy nhất, cố tình KHÔNG xoá: ~ISystemDrawing gọi bsp_clear(s_world)
    // mà không gán lại NULL, nên huỷ nhiều đối tượng sẽ để lại con trỏ treo.
    QtDrawing3d *d = nullptr;

    void resetWorld()
    {
        if (ISystemDrawing::s_world) {
            d->clear_node();
            ISystemDrawing::s_world = NULL;
        }
        matrix4_identity(&ISystemDrawing::m_modelViewMatrix);
    }

    // Polygon tiện dụng: hình vuông trong mặt phẳng z = @z, góc (@x0,@y0)..(@x1,@y1),
    // thứ tự đỉnh cho pháp tuyến (0,0,1).
    void addSquare(double x0, double y0, double x1, double y1, double z,
                   unsigned char r, unsigned char g, unsigned char b)
    {
        son_point3d_t pts[4] = {{{x0, y0, z}}, {{x1, y0, z}}, {{x1, y1, z}}, {{x0, y1, z}}};
        unsigned char color[3] = {r, g, b};
        d->DrawPolygon(pts, 4, color);
    }

    // Dựng polygon rời (không vào cây) từ 3 đỉnh để gọi ComputeLight.
    double lightOf(const son_point3d_t *pts)
    {
        son_polygon3d_t poly;
        polygon3d_init(&poly);
        polygon3d_add_points(&poly, pts, 3);
        const double shade = d->ComputeLight(&poly);
        polygon3d_clear(&poly);
        return shade;
    }

    QImage render(int w = 120, int h = 120)
    {
        QImage img(w, h, QImage::Format_RGB32);
        img.fill(Qt::white);
        QPainter p(&img);
        d->DrawSence(&p);
        p.end();
        return img;
    }

private slots:
    void initTestCase()
    {
        d = new QtDrawing3d();
        ISystemDrawing::s_world = NULL;
    }

    void init() { resetWorld(); }

    // ---- ComputeLight -------------------------------------------------------

    // Pháp tuyến (0,0,1), modelView = identity: shade = dot*0.8 + 0.2, không bị kẹp.
    void computeLight_pinsExactShade()
    {
        son_point3d_t pts[3] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}};
        QVERIFY(qAbs(lightOf(pts) - kShadeZ) < 1e-12);
    }

    // Pháp tuyến (0,0,-1): dot < 0 => shade âm => kẹp lên biên dưới 0.3.
    void computeLight_clampsToAmbient()
    {
        son_point3d_t pts[3] = {{{0, 0, 0}}, {{1, 1, 0}}, {{1, 0, 0}}};
        QVERIFY(qAbs(lightOf(pts) - 0.3) < 1e-12);
    }

    // Pháp tuyến đúng hướng vector sáng (2,4,3) => dot = 1 => shade 1.0 => kẹp xuống 0.9.
    void computeLight_clampsToMax()
    {
        son_point3d_t pts[3] = {{{0, 0, 0}}, {{4, -2, 0}}, {{4, 1, -4}}};
        QVERIFY(qAbs(lightOf(pts) - 0.9) < 1e-12);
    }

    // modelViewMatrix được áp vào pháp tuyến trước khi lấy dot: ma trận đổi dấu z
    // biến pháp tuyến (0,0,1) thành (0,0,-1) => shade bị kẹp về 0.3.
    void computeLight_usesModelViewMatrix()
    {
        ISystemDrawing::m_modelViewMatrix.m[2][2] = -1.0;
        son_point3d_t pts[3] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}};
        QVERIFY(qAbs(lightOf(pts) - 0.3) < 1e-12);
    }

    // Pháp tuyến biến đổi bằng phần tuyến tính (3x3) của modelViewMatrix rồi chuẩn hoá:
    // tịnh tiến (cột 3) và phóng to đều (paintEvent nhân cả ma trận với m_factor*100)
    // không đổi ánh sáng của polygon có pháp tuyến (0,0,1).
    void computeLight_ignoresTranslationAndUniformScale()
    {
        son_point3d_t pts[3] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}};
        ISystemDrawing::m_modelViewMatrix.m[0][3] = 40.0;
        ISystemDrawing::m_modelViewMatrix.m[1][3] = -25.0;
        ISystemDrawing::m_modelViewMatrix.m[2][3] = 7.0;
        QVERIFY(qAbs(lightOf(pts) - kShadeZ) < 1e-12);

        matrix4_identity(&ISystemDrawing::m_modelViewMatrix);
        matrix4_scale(&ISystemDrawing::m_modelViewMatrix, 100.0);
        QVERIFY(qAbs(lightOf(pts) - kShadeZ) < 1e-12);
    }

    // ---- DrawPolygon -> BSP tĩnh --------------------------------------------

    void drawPolygon_addsPolygonWithColorToWorld()
    {
        QVERIFY(bsp_empty(ISystemDrawing::s_world) == son_TRUE);
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);

        QVERIFY(ISystemDrawing::s_world != NULL);
        QCOMPARE((int)ISystemDrawing::s_world->polygons.count, 1);

        polygon3d_node *pn =
            (polygon3d_node *)head_node(&ISystemDrawing::s_world->polygons);
        QCOMPARE((int)pn->polygon3d.n_ver, 4);
        QCOMPARE((int)pn->polygon3d.color[0], 200);
        QCOMPARE((int)pn->polygon3d.color[1], 100);
        QCOMPARE((int)pn->polygon3d.color[2], 50);

        son_point3d_t pts[MAX_POINT_PER_POLY];
        polygon3d_get_points(&pn->polygon3d, pts);
        QCOMPARE(pts[0].v[0], 10.0);
        QCOMPARE(pts[2].v[1], 60.0);
        QCOMPARE(pts[3].v[2], 0.0);
    }

    // Polygon đồng phẳng được xếp vào cùng danh sách của node gốc.
    void drawPolygon_coplanarGoesToSameNode()
    {
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);
        addSquare(30, 30, 80, 80, 0, 0, 200, 0);

        QCOMPARE((int)ISystemDrawing::s_world->polygons.count, 2);
        QVERIFY(ISystemDrawing::s_world->front == NULL);
        QVERIFY(ISystemDrawing::s_world->back == NULL);
    }

    void drawPolygon_splitsFrontAndBack()
    {
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);
        addSquare(30, 30, 80, 80, 5, 0, 200, 0);
        addSquare(30, 30, 80, 80, -5, 0, 0, 200);

        QCOMPARE((int)ISystemDrawing::s_world->polygons.count, 1);
        QVERIFY(ISystemDrawing::s_world->front != NULL);
        QVERIFY(ISystemDrawing::s_world->back != NULL);
    }

    // ---- DrawSence(QPainter *) ---------------------------------------------

    // Một polygon: vùng được tô đúng chỗ và màu là color * shade (bị cắt về int).
    void drawSence_fillsProjectedPolygonWithShadedColor()
    {
        ISystemDrawing::eye.v[0] = 0;
        ISystemDrawing::eye.v[1] = 0;
        ISystemDrawing::eye.v[2] = 7000;
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);

        QImage img = render();
        QCOMPARE(img.pixelColor(30, 30), QColor(129, 64, 32));
        QCOMPARE(img.pixelColor(100, 100), QColor(Qt::white));
        // ngoài biên polygon thì không bị tô
        QCOMPARE(img.pixelColor(5, 30), QColor(Qt::white));
    }

    // Cả hai polygon trong danh sách của một node đều được vẽ, theo thứ tự danh sách,
    // mỗi polygon tô bằng MÀU CỦA CHÍNH NÓ (không bị polygon sau tô lại).
    void drawSence_drawsEveryPolygonOfNodeInListOrder()
    {
        ISystemDrawing::eye.v[0] = 0;
        ISystemDrawing::eye.v[1] = 0;
        ISystemDrawing::eye.v[2] = 7000;
        addSquare(10, 10, 40, 40, 0, 200, 100, 50);
        addSquare(60, 60, 90, 90, 0, 0, 200, 0);

        QCOMPARE((int)ISystemDrawing::s_world->polygons.count, 2);
        QImage img = render();
        QCOMPARE(img.pixelColor(20, 20), QColor(129, 64, 32)); // vùng polygon 1, màu 1
        QCOMPARE(img.pixelColor(70, 70), QColor(0, 129, 0)); // vùng polygon 2
        QCOMPARE(img.pixelColor(50, 50), QColor(Qt::white)); // giữa hai vùng
    }

    // eye ở phía dương (side_eye > 0): back -> node -> front, nên nhánh front vẽ cuối.
    void drawSence_eyeInFrontDrawsFrontSubtreeLast()
    {
        ISystemDrawing::eye.v[0] = 0;
        ISystemDrawing::eye.v[1] = 0;
        ISystemDrawing::eye.v[2] = 7000;
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);
        addSquare(30, 30, 80, 80, 5, 0, 200, 0);   // front
        addSquare(0, 0, 25, 25, -5, 0, 0, 200);    // back

        QImage img = render();
        QVERIFY(ISystemDrawing::s_world->front != NULL);
        QCOMPARE(img.pixelColor(45, 45), QColor(0, 129, 0));   // front đè node
        QCOMPARE(img.pixelColor(20, 20), QColor(129, 64, 32)); // node đè back
        QCOMPARE(img.pixelColor(5, 5), QColor(0, 0, 129));     // chỉ back
    }

    // eye ở phía âm (side_eye < 0): front -> node -> back, nên node đè nhánh front
    // và nhánh back vẽ cuối.
    void drawSence_eyeBehindDrawsBackSubtreeLast()
    {
        ISystemDrawing::eye.v[0] = 0;
        ISystemDrawing::eye.v[1] = 0;
        ISystemDrawing::eye.v[2] = -7000;
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);
        addSquare(30, 30, 80, 80, 5, 0, 200, 0);   // front
        addSquare(0, 0, 25, 25, -5, 0, 0, 200);    // back

        QImage img = render();
        QCOMPARE(img.pixelColor(45, 45), QColor(129, 64, 32)); // node đè front
        QCOMPARE(img.pixelColor(70, 70), QColor(0, 129, 0));   // chỉ front
        QCOMPARE(img.pixelColor(20, 20), QColor(0, 0, 129));   // back đè node
    }

    // Cây rỗng: không vẽ gì.
    void drawSence_emptyWorldDrawsNothing()
    {
        QImage img = render(20, 20);
        QCOMPARE(img.pixelColor(10, 10), QColor(Qt::white));
    }

    // modelViewMatrix được dùng để chiếu điểm: dịch x thêm 40 thì vùng tô dịch theo.
    // Pháp tuyến là HƯỚNG: phép tịnh tiến không đổi ánh sáng, shade vẫn = kShadeZ.
    void drawSence_projectsThroughModelViewMatrix()
    {
        ISystemDrawing::eye.v[0] = 0;
        ISystemDrawing::eye.v[1] = 0;
        ISystemDrawing::eye.v[2] = 7000;
        ISystemDrawing::m_modelViewMatrix.m[0][3] = 40.0;
        addSquare(10, 10, 60, 60, 0, 200, 100, 50);

        QImage img = render();
        QCOMPARE(img.pixelColor(30, 30), QColor(Qt::white));
        QCOMPARE(img.pixelColor(70, 30), QColor(129, 64, 32)); // = color * kShadeZ
    }

    // ---- Look() -------------------------------------------------------------

    void look_buildsViewMatrixFromEye()
    {
        ISystemDrawing::eye.v[0] = 300;
        ISystemDrawing::eye.v[1] = 400;
        ISystemDrawing::eye.v[2] = 0;
        d->Look();

        const double expect[4][4] = {
            {   0.0, -0.8, -0.6, 0.0},
            {   0.0,  0.6, -0.8, 0.0},
            {  -1.0,  0.0,  0.0, 0.0},
            {   0.0,  0.0, 499.0, 1.0},
        };
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                QVERIFY2(qAbs(ISystemDrawing::m_modelViewMatrix.m[i][j] - expect[i][j]) < 1e-9,
                         qPrintable(QString("m[%1][%2] = %3")
                                        .arg(i).arg(j)
                                        .arg(ISystemDrawing::m_modelViewMatrix.m[i][j])));
    }
};

QTEST_MAIN(TestQtDrawing3d)
#include "test_qtdrawing3d.moc"
