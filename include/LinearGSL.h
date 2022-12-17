#pragma once

#include "AbstractSpline.h"
#include <QLine>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>
#include <QDebug>

class LinearGSL : public Spline{
private:
    gsl_spline *x_linear, *y_linear;
    gsl_interp_accel *acc_x, *acc_y;
public:
    void makeSpline();
    QPointF eval(double pos);
    QPointF evalDer(double pos);

    ~LinearGSL();
};

