#include "LinearGSL.h"

void LinearGSL::makeSpline(){
    if (points_.empty())
        throw std::range_error("For interpolation points are required!");

    if (points_.size() < 3)
        throw std::range_error("For interpolation minimum 3 points are required!");
    
    double count = 0;
    if (closed_){
        points_.insert(points_.begin(), points_[points_.size() - 1]);
        points_.insert(points_.begin(), points_[points_.size() - 2]);
        points_.insert(points_.begin(), points_[points_.size() - 3]);
        points_.push_back(points_[3]);
        points_.push_back(points_[4]);
        points_.push_back(points_[5]);
        count = -3;
    }

    std::vector<double> t;
    std::vector<double> x, y;
    for (auto point : points_){
        x.push_back(point.x());
        y.push_back(point.y());
        t.push_back(count);
        count += 1;
    }

    int N = x.size();

    acc_x = gsl_interp_accel_alloc();
    acc_y = gsl_interp_accel_alloc();
    x_linear = gsl_spline_alloc(gsl_interp_linear, N);
    y_linear = gsl_spline_alloc(gsl_interp_linear, N);

    gsl_spline_init(x_linear, t.data(), x.data(), N);
    gsl_spline_init(y_linear, t.data(), y.data(), N);
}

LinearGSL::~LinearGSL(){
    gsl_spline_free(x_linear);
    gsl_spline_free(y_linear);
    gsl_interp_accel_free(acc_x);
    gsl_interp_accel_free(acc_y);
}

QPointF LinearGSL::eval(double pos){
    double x = gsl_spline_eval(x_linear, pos, acc_x);
    double y = gsl_spline_eval(y_linear, pos, acc_y);

    return QPointF(x, y);
}   

QPointF LinearGSL::evalDer(double pos){
    double x = gsl_spline_eval_deriv(x_linear, pos, acc_x);
    double y = gsl_spline_eval_deriv(y_linear, pos, acc_y);

    return QPointF(x, y);
}   