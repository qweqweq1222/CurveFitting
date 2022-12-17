#include "PointsSelecter.h"
#include <QDebug>

void PointsSelecter::setPoints(std::tuple<QPointF*, int> *points, int size){
    points_ = points;
    points_size = size;
    if (selectedPoints_ != nullptr)
        delete[] selectedPoints_;
    selectedPoints_ = new std::tuple<QPointF*, int>[points_size];
}
 
void PointsSelecter::setRectangle(QRectF rect){
    boundRect = rect;
}

std::tuple<QPointF*, int>* PointsSelecter::getSelectedPoints(){
    return selectedPoints_;
}

void PointsSelecter::run(){ // необходимо определить при наследовании от QThread (она запускается при создании thread)
    double Lx = boundRect.x();
    double Ly = boundRect.y();
    double Rx = Lx + boundRect.width();
    double Ry = Ly + boundRect.height();
    int count = 0;
    for (int i = 0; i < points_size; i++){
        auto& [point, size] = points_[i];
        double x = point->x();
        double y = point->y();
        if(Lx <= x && x < Rx && Ly <= y && y < Ry)  {
            selectedPoints_[count] = {point, size};
            count++;
        }   
    }
    selected_size = count;
}

int PointsSelecter::getSelectedPointsSize(){
    return selected_size;
}