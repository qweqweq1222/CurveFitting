#pragma once

#include "AbstractSpline.h"

// "абстрактный" класс, который используется при создании сплайнов 
class AbstractSplineCreator{
public:
    virtual Spline* createSpline() = 0;
};