#include "AkimaSpline.h"
//#ifndef __unix__
using boost::math::interpolators::makima;
//#else
//#include <boost/math/interpolators/makima.hpp>
//using boost::math::makima;
//#endif
#include <QDebug>

void AkimaSpline::makeSpline(){
    if (points_.empty())
        throw std::range_error("For interpolation points are required!");

    points_.push_back(points_[0]);

    npoints = points_.size();

    std::vector<double> tx, ty;
    std::vector<double> x, y;
    double count = 1;
    for (auto point : points_){
        x.push_back(point.x());
        y.push_back(point.y());
        tx.push_back(count);
        ty.push_back(count);
        count += 1;
    }   
    auto splineX = makima(std::move(tx), std::move(x));
    auto splineY = makima(std::move(ty), std::move(y));
}
