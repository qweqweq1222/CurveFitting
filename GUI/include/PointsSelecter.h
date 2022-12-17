#pragma once

#include <QThread>
#include <QDebug>
#include <QPointF>
#include <QRectF>
#include <QGraphicsEllipseItem>
#include <vector>
#include <list>
#include <tuple>

// проверка точек на попадание в зону просмотра, можно запускать в отдельном потоке
class PointsSelecter : public QThread{
public:
    void setPoints(std::tuple<QPointF*, int> *points, int size);
    void setRectangle(QRectF rect);
    std::tuple<QPointF*, int>* getSelectedPoints(); // возвращает точки, которые попали в Rectangle
    int getSelectedPointsSize();
private:
    void run();

    std::tuple<QPointF*, int>* points_;
    std::tuple<QPointF*, int>* selectedPoints_ = nullptr;
    QRectF boundRect;

    int points_size;
    int selected_size;
};