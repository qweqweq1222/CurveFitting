#pragma once
#include <vector>
#include <QPointF>
#include <QDebug>


/* определение интерфейса всех базовых сплайнов */
class Spline{
protected:
    int npoints;
    bool closed_ = false;
public:
    virtual void makeSpline() = 0; 
    void setPoints(std::vector<QPointF>& points); // выставление пользовательских точек
    void setClosed(bool closed); // выставление флага замнкутости 
    std::vector<QPointF> getSpline(); 
    virtual QPointF eval(double pos) = 0; // вычисление значения 
    virtual QPointF evalDer(double pos) = 0; // производной 
    std::vector<QPointF> points_;
    std::vector<QPointF> inter_res_;
    virtual ~Spline() = default;
};