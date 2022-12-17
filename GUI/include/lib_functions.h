#pragma once
#include "mainwindow.h"

#include "Generator.h"
#include "Interpolator.h"
#include "SplineTypes.h"

class SplineRedactor{
public:
    SplineRedactor();
    ~SplineRedactor() = default;
    int curveFit(int argc, char *argv[], double (*userFunc)(double));
    void setUserFunction(double (*userFunc)(double)); // выставление пользовательской функции (Generator.h)
    void setUserPoints(std::vector<double> usrPts);
    void setUserPoint(double val);

private:
    QApplication* a;
    MainWindow* w;
};
