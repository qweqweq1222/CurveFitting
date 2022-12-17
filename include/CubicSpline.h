#pragma once

#include "AbstractSpline.h"
#include <QLine>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>
#include <QDebug>

class CubicSpline : public Spline{
private:
    gsl_spline *x_cubic = nullptr;
    gsl_spline *y_cubic = nullptr;
    gsl_interp_accel *acc_x = nullptr;
    gsl_interp_accel *acc_y = nullptr;
public:
    void makeSpline();
    QPointF eval(double pos);
    QPointF evalDer(double pos);

    ~CubicSpline();
};
