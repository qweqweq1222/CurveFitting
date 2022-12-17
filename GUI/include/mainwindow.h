#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <vector>

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QFileSystemModel>
#include <QGraphicsScene>
#include <QPen>
#include <QShortcut>
#include <QPainterPath>

#include "graphics_view_zoom.h"
#include "choosefiledialog.h"
#include "choosesplinedialog.h"
#include "savedialog.h"
#include "SplineTypes.h"

#include "Interpolator.h"
#include "Generator.h"
#include "PointsStorage.h"
#include "PointsSelecter.h"
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserFunction(double (*userFunc)(double));
    void setUserPoints();
    void setUserPoint(double val);

private:
    Ui::MainWindow *ui;
    QFileSystemModel* model;
    Graphics_view_zoom* zoom;
    ChooseFileDialog *fileDilalog;
    ChooseSplineDialog *splineDilalog;
    saveDialog *savedialog;

    double width = 800;
    double hight = 600;

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    QPainterPath painter;
    QGraphicsPathItem* pathPtr = nullptr;
    
    QPainterPath painterFunc;
    QGraphicsPathItem* pathPtrFunc = nullptr;

    QPointF* lpos = nullptr;
    bool deleteMod = false;
    bool redactMod = false;
    bool chooseMod = false;

    QPen pen;
    QPen penLine;
    QPen penFunc;

    QShortcut *sc;
    QChart *chart;

    bool eventFilter(QObject *obj, QEvent *event);
    void deleteItem(QPointF pos);
    void addUsersPoints(QPointF pos);
    std::vector<QPointF> usersPoints;

    Interpolator interpolator;
    Generator generator;
    double (*userFunc_)(double x) = nullptr;
    std::vector<double> usersKnots;
    bool isFunctionDrawable = false;

    bool interpolated = false;
    bool closed = false;
    PointsStorage pointsStorage;
    std::vector<PointsSelecter> selectors;

    std::string splineType = SplineType::Cubic;

    std::vector<QPointF> points;
    std::vector<QPointF> spline;
    std::vector<QGraphicsItem*> items;

    std::map<QGraphicsItem*, int> itemsMap;
    std::vector<std::tuple<QGraphicsItem*, int>> visibleItems;

    const int visibleItemsCount = 25;
    int threadsCount;

    double square;
    double scale = 1;

    void choosePoints(QPointF pos);
    std::vector<QGraphicsItem*> bounds;
    int start_pos = -2; 
    int end_pos = -2;

    void drawSpline(double start, double stop, double step);
    void drawFunc(double start, double stop, double step);
    double RMSE(double start, double end, double step);
    double rmse = 0;

private slots:
    void loadIMG();
    void addPoint(QPointF pos);
    void addLine(QPointF lastpos, QPointF pos);
    void undo();
    void onDelete();
    void onLoad();
    void onChooseSpline();
    void onSaveSpline();
    void onSaveDots();
    void onInterpolate();
    void moreDetails(); // получить "корректное" изображение сплайна при зуме
    void onApply(); 
    void onRedact();
    void onCheckClosed(int state);
    void onGenerate();
};

#endif // MAINWINDOW_H
