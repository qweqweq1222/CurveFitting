#include "Generator.h"

double Generator::userFuncEval(double x){
    return (*userFunc)(x); 
}

void Generator::generatePoints(double start, double end, int count){
    double step = (end - start) / count;
    for (int i = 0; i < count; i ++){
        points_.push_back(QPointF(start + i * step, (*userFunc)(start + i * step)));
    }
}

std::vector<QPointF> Generator::getPoints(){
    return std::move(points_);
}

void Generator::setFunction(double (*f)(double)){
    userFunc = f;
}

void Generator::generateForInterpolation(std::vector<double> args){
    for (auto arg : args){
        points_.push_back(QPointF(arg, (*userFunc)(arg)));
    }
}
