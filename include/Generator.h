#pragma once
#include <vector>
#include <QPointF>

// "абстрактный" класс для выставления пользовательской функции в API GUI
class Generator{
public:
    double userFuncEval(double x);
    void generatePoints(double start, double end, int count);
    void generateForInterpolation(std::vector<double> args);
    std::vector<QPointF> getPoints();
    void setFunction(double (*f)(double));
    
private:    
    double (*userFunc)(double x);
    std::vector<QPointF> points_;
};