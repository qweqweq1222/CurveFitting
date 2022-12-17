#pragma once

#include "AbstractSpline.h"
#include <QLine>

class AkimaSpline : public Spline{
private:
public:
    void makeSpline();
    QPointF eval(double pos){return QPointF(0,0);};
    QPointF evalDir(double pos){return QPointF(0,0);};
    ~AkimaSpline();
};