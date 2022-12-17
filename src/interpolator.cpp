#include "Interpolator.h"
#include "BSpline.h"
#include "AkimaSplineGSL.h"
#include "LinearGSL.h"
#include "SplineTypes.h"
#include "CubicSpline.h"
#include "SteffenSpline.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_bspline.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>

Interpolator::Interpolator(){
    factory.registrateSpline<BSpline>(SplineType::BSpline);
    factory.registrateSpline<AkimaSplineGSL>(SplineType::Akima);
    factory.registrateSpline<CubicSpline>(SplineType::Cubic);
    factory.registrateSpline<SteffenSpline>(SplineType::Steffen);
    factory.registrateSpline<LinearGSL>(SplineType::Linear);
    
}

Interpolator::~Interpolator(){
    if(spline){
        delete spline;
        spline = nullptr;
    }
}

void Interpolator::interpolate(std::string spline_type, std::string smoothness){
    splineType_ = spline_type;
    if(spline){
        delete spline;
        spline = nullptr;
    }
    spline = factory.makeSpline(spline_type);
    spline->setClosed(closed_);
    spline->setPoints(points_);
    spline->makeSpline();
}

void Interpolator::setPoints(std::vector<QPointF>& points){
    if (!points_.empty())
        points_.clear();
    points_ = points;    
}

void Interpolator::setPointsMove(std::vector<QPointF> points){
    points_ = points;    
}

std::vector<QPointF> Interpolator::getSpline(){
    return std::move(spline->getSpline());
}

QPointF Interpolator::eval(double pos){
    return spline->eval(pos);
}

double Interpolator::evalDer(double pos){
    auto p = spline->evalDer(pos);
    if (p.x() == 0){
        return std::nan("0");
    }
    return p.y() / p.x();
}

double dst(QPointF p1, QPointF p2) {
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y(); 
    return sqrt(dx*dx + dy*dy);
}

double Interpolator::GetClosest(double x, double y) {

    double t = 0;
    double step = 0.01;
    double min = dst(QPointF(x,y), eval(t));
    double r = 0;
    double param = 0.0;
    while(t < points_.size() - 1 - step)
    {
        t += step;
        r = dst(QPointF(x,y), eval(t));
        if (min > r){
            min = r;
            param = t;
        }
    }
    
    return param;
}

void Interpolator::setClosed(bool closed){
    closed_ = closed;
}

void Interpolator::deletePoint(int index){
    if (!spline)
        throw std::logic_error("Spline was not created");
    if (points_.empty())
        throw std::logic_error("No points");
    if (points_.size() <= index)
        throw std::range_error("Index out of rnage");


    points_.erase(std::next(points_.begin(), index));
    interpolate(splineType_);
}

void Interpolator::addPoint(int index, QPointF p){
    if (!spline)
        throw std::logic_error("Spline was not created");

    points_.insert(std::next(points_.begin(), index), p);
    interpolate(splineType_);
}

double CompareSplines(Interpolator& int1, Interpolator&  int2, int samplesCount){
    int size1 = int1.points_.size();
    int size2 = int2.points_.size();

    double d1 = ((double)size1 - 1) / (double)samplesCount;
    double d2 = ((double)size2 - 1) / (double)samplesCount;

    double rmse = 0;
    for (int i = 0; i < samplesCount - 1; i++){
        auto p1 = int1.eval(i * d1);
        auto p2 = int2.eval(i * d2);

        auto p3 = p2 - p1;

        rmse += p3.x() * p3.x() + p3.y() * p3.y();    
    }

    rmse /= samplesCount;
    rmse = sqrt(rmse);
    return rmse;
}


double Angle(QPointF& start, QPointF& end)
{
    if(start.y() == end.y())
        return 0;
    
    double dy = end.y() - start.y();
    double dx = end.x() - start.x();

    return start.x() != end.x() ? atan(dy / dx) : (M_PI / 2);
}

/* number of fit coefficients */
#define NCOEFFS  12

/* nbreak = ncoeffs + 2 - k = ncoeffs - 2 since k = 4 */
#define NBREAK   (NCOEFFS - 2)


std::vector<int> Interpolator::Filter(int group_size, double angle){
    return {};
}
/*__________________________________________________________________*/



Interpolator::Interpolator(const Interpolator& val){
    factory.registrateSpline<BSpline>(SplineType::BSpline);
    factory.registrateSpline<AkimaSplineGSL>(SplineType::Akima);
    factory.registrateSpline<CubicSpline>(SplineType::Cubic);
    factory.registrateSpline<SteffenSpline>(SplineType::Steffen);
    factory.registrateSpline<LinearGSL>(SplineType::Linear);
    for (auto val_p : val.points_){
        points_.push_back(val_p);
    }
    closed_ = val.closed_;
    splineType_ = val.splineType_;
    spline = factory.makeSpline(splineType_);
    spline->setClosed(closed_);
    spline->setPoints(points_);
    spline->makeSpline();
}

char* Interpolator::rasterize(ScreenView& screen){
    double Lx = screen.boundRect.x();
    double Ly = screen.boundRect.y();
    double Rx = Lx + screen.boundRect.width();
    double Ry = Ly + screen.boundRect.height();
    for (double t = 0; t < points_.size() - 1; t += 0.001){
        auto p = eval(t);
        double x = p.x();
        double y = p.y();

        double x_screen = (x - Lx) * screen.xres / (Rx - Lx);
        double y_screen = (y - Ly) * screen.yres / (Ry - Ly);

        if(Lx <= x && x < Rx && Ly <= y && y < Ry)  {
            screen.img[int(floor(x_screen)) + screen.xres * int(floor(y_screen))] = 1; 
        }
    }
    return screen.img;
}

ScreenView::ScreenView(){
    img = new char[xres * yres];
}
ScreenView::~ScreenView(){
    delete img;
}
std::vector<QPointF> Interpolator::Simplify(std::vector<QPointF>& pointList, float epsilon)
{
        std::vector<QPointF> resultList;

        float dmax = 0;
        int index = 0;
        for (int i = 1; i < pointList.size() - 1; ++i)
        {
            float d = PerpendicularDistance(pointList[i], pointList[0], pointList[pointList.size() - 1]);
            if (d > dmax) {
                index = i;
                dmax = d;
            }
        }

        if (dmax > epsilon)
        {
            std::vector<QPointF> pre_part, next_part;
            for (int i = 0; i <= index; ++i)	pre_part.push_back(pointList[i]);
            for (int i = index; i < pointList.size(); ++i)	next_part.push_back(pointList[i]);
            std::vector<QPointF> resultList1 = Simplify(pre_part, epsilon);
            std::vector<QPointF> resultList2 = Simplify(next_part, epsilon);

            resultList.insert(resultList.end(), resultList1.begin(), resultList1.end());
            resultList.insert(resultList.end(), resultList2.begin()+1, resultList2.end());
        }
        else
        {
            resultList.push_back(pointList[0]);
            resultList.push_back(pointList[pointList.size() - 1]);
        }
            
        return resultList;
}

float Interpolator::PerpendicularDistance(const QPointF& p, const QPointF& line_p1, const QPointF& line_p2)
{
	QPointF vec1 = QPointF(p.x() - line_p1.x(), p.y() - line_p1.y());
	QPointF vec2 = QPointF(line_p2.x() - line_p1.x(), line_p2.y() - line_p1.y());
	float d_vec2 = sqrt(vec2.x()*vec2.x() + vec2.y()*vec2.y());
	float cross_product = vec1.x()*vec2.y() - vec2.x()*vec1.y();
	float d = abs(cross_product / d_vec2);
	return d;
}

