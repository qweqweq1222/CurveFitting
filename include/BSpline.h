#pragma once

#include "AbstractSpline.h"
#include <QLine>
#include <QPointF>

class BSpline : public Spline{
    void makeSpline();
    QPointF eval(double pos){return QPointF(0,0);};
    QPointF evalDer(double pos){return QPointF(0,0);};
    ~BSpline(){};
};