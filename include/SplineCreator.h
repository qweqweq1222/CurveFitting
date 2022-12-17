#pragma once
#include "AbstractSplineCreator.h"
#include "AbstractSpline.h"


template<class T>
class SplineCreator : public AbstractSplineCreator{
public:
    Spline* createSpline(){
        return new T;
    }
};