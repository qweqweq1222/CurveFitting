#include "BSpline.h"
#include <iostream>
#include <cmath>
#include <gsl/gsl_bspline.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <stdexcept>

void BSpline::makeSpline(){
    if (points_.empty())
        throw std::range_error("For interpolation points are required!");

    points_.insert(points_.begin(), points_[points_.size()-1]);

    points_.insert(points_.begin(), points_[points_.size()-2]);
    points_.insert(points_.begin(), points_[points_.size()-3]);
    points_.push_back(points_[3]);
    points_.push_back(points_[4]);
    points_.push_back(points_[5]);
    size_t n = points_.size();


    size_t nbreak = n - 2;
    size_t ncoeffs = n;
    size_t i, j;
}
