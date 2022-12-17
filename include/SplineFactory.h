#pragma once
#include "SplineCreator.h"
#include <map>
#include <string>


// шаблонная фабрика сплайнов по соответствующему типу вызывает соответствующие методы
class SplineFactory{
    std::map<std::string, AbstractSplineCreator*> spline_map;
public:
    template <typename T>
    void registrateSpline(std::string name){
        if (spline_map.find(name) == spline_map.end()){
            spline_map[name] = new SplineCreator<T>();
        }
    }

    Spline* makeSpline(std::string name){
        return spline_map[name]->createSpline();
    }

    ~SplineFactory(){
        for (auto val: spline_map){
            delete val.second;
        }
    }
};