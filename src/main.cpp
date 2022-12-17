#include <iostream>
#include <chrono>
#include <QDebug>
#include <cmath>
#include <vector>
#include "Generator.h"

double f(double t){
    return sin(t);
}

int main(){
    Generator g;
    g.setFunction(&f);
    g.generatePoints(0, 3, 100);
    auto points = g.getPoints();
    for (auto point : points){
        qDebug() << point;
    }
    return 0;
}