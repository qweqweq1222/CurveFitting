#pragma once

#include <list>
#include <vector>
#include <QPointF>
#include <QPoint>
#include <QRect>
#include <tuple>
#include <QGraphicsEllipseItem>


// храним все точки, BSP дерево для быстрого поиска точек
class PointsStorage{
public:
    PointsStorage();
    ~PointsStorage();
    void setPoints(std::vector<QPointF> points);
    void buildTree();
    void setSceneRect(QRectF rect);
    std::vector<std::vector<std::tuple<QPointF*, int>>*> getContainers(QRectF bound); // возвращаем список контейнеров, которые заметаются окном просмотра
    void addPoint(QPointF point, int pos = 0); 
    void addPoints(std::vector<QPointF> points, int pos = 0);
    void deletePoint(QPointF point);
    void deletePoints(int start, int end);
    int getContainersCount(); 
    int size();
    int getPointNumber(QPointF p);
    std::vector<QPointF>* getAllItems(); // получить массив всех точек
    void clearTree();
private:
    std::tuple<int, int> findContainer(QPointF point); // определить контейнер, в котором лежит точка
    void rebuildTree(int pos);

    int depth = 5;
    QRectF sceneRect;
    std::vector<QPointF> points_;
    std::vector<QPointF*> allItems;
    std::vector<std::vector<std::vector<std::tuple<QPointF*, int>>>> tree;
};