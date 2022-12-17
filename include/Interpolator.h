#pragma once
#include <vector>
#include <QPointF>
#include <QRectF>
#include "AbstractSpline.h"
#include "SplineFactory.h"
#include <queue>
#include <cstddef>


// API пользователя для работы с сплайнами
class ScreenView{
public:
    QRectF boundRect;
    int xres = 1920; 
    int yres = 1080; 
    char* img;
    ScreenView();
    ~ScreenView();
};

class Interpolator{
public:
    Interpolator();
    ~Interpolator();
    Interpolator(const Interpolator& val);

    void interpolate(std::string spline_type, std::string smoothness = "c2");
    void setPoints(std::vector<QPointF>& points);
    void setPointsMove(std::vector<QPointF> points);
    void setClosed(bool closed);
    std::vector<QPointF> getSpline();
    QPointF eval(double pos);
    double evalDer(double pos);
    void deletePoint(int index);
    void addPoint(int index, QPointF p);
    double GetClosest(double x, double y); // вычисление ближайшей точки сплайна к заданной точке 
    std::vector<int> Filter(int group_size, double angle); // фильтрация массива точек
    friend double CompareSplines(Interpolator& int1, Interpolator&  int2, int samplesCount); // вычисление метрики RMSE между сплайнами 
    char* rasterize(ScreenView& screen); // растеризация кривой
    std::vector<QPointF> Simplify(std::vector<QPointF>& pointList, float epsilon = 0.05); // алгоритм RDP

private:
    friend class MetaSpline;
    float PerpendicularDistance(const QPointF& p, const QPointF& line_p1, const QPointF& line_p2); // функция для алгоритма RDP 
    bool closed_ = false;
    SplineFactory factory;
    Spline *spline = nullptr;
    std::vector<QPointF> points_;
    std::string splineType_;
};



double CompareSplines(Interpolator& int1, Interpolator&  int2, int samplesCount);