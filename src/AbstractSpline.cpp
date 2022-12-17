#include "AbstractSpline.h"

void Spline::setPoints(std::vector<QPointF>& points){
    points_ = points;
}

std::vector<QPointF> Spline::getSpline(){
    return std::move(inter_res_);
}

void Spline::setClosed(bool closed){
    closed_ = closed;
}
