#include "PointsStorage.h"
#include <cmath>
#include <QDebug>
    
PointsStorage::PointsStorage(){
}

PointsStorage::~PointsStorage(){
    for(auto item : allItems){
        delete item;
    }
}
    
std::tuple<int, int> PointsStorage::findContainer(QPointF point){
    double x = point.x();
    double y = point.y();
    double minX = sceneRect.x();
    double maxX = minX + sceneRect.width();
    double minY = sceneRect.y();
    double maxY = minY + sceneRect.height();
    int countI = 0;
    int countJ = 0;
    for (int i = 0; i < depth; i++){
        double midx = (maxX + minX) / 2;
        double midy = (maxY + minY) / 2;
        int n = pow(2, depth - i - 1);
        if(x < midx){
            maxX = midx;
        } else {
            minX = midx;
            countI += n; 
        }
        
        if(y < midy){
            maxY = midy;
        } else {
            minY = midy;
            countJ += n;
        }
    }

    return {countI, countJ};
}

std::vector<std::vector<std::tuple<QPointF*,int>>*> PointsStorage::getContainers(QRectF bound){
    QPointF p1 = bound.topLeft();
    QPointF p2 = bound.bottomRight();

    auto [imin, jmin] = findContainer(p1);
    auto [imax, jmax] = findContainer(p2);
    std::vector<std::vector<std::tuple<QPointF*, int>>*> containers;
    for (int i = imin; i <= imax; i++){
        for (int j = jmin; j <= jmax; j++){
            containers.push_back(&(tree[i][j]));
        }
    }

    return std::move(containers);
}

void PointsStorage::addPoint(QPointF point, int pos){
    points_.push_back(point);
    buildTree();
}

void PointsStorage::addPoints(std::vector<QPointF> points, int pos){
    auto it = std::next(points_.begin(), pos + 1);
    points_.insert(it, points.begin(), points.end());
    buildTree();
}

void PointsStorage::deletePoints(int start, int end){
    points_.erase(std::next(points_.begin(), start + 1), std::next(points_.begin(), end));
    buildTree();
}

void PointsStorage::deletePoint(QPointF point){
    auto comp = [=](QPointF p){
        return p == point;
    };
    auto it = std::find_if(points_.begin(), points_.end(), comp);
    points_.erase(it);
    auto [i, j] = findContainer(point);
    auto comp_ptr = [=](std::tuple<QPointF*, int> el){
        return point == *(std::get<0>(el));
    };
    auto it_tree = std::find_if(tree[i][j].begin(), tree[i][j].end(), comp_ptr);
    tree[i][j].erase(it_tree);
}

void PointsStorage::setPoints(std::vector<QPointF> points){
    if (!points_.empty())
        points_.clear();
    points_ = points;
}

void PointsStorage::buildTree(){
    int n = pow(2, depth);
    tree = std::vector<std::vector<std::vector<std::tuple<QPointF*,int>>>>(n);
    for (int i = 0; i < n; i++){
        tree[i] =  std::vector<std::vector<std::tuple<QPointF*, int>>>(n);
    }

    int count = 0;
    for (auto& point : points_){
        auto [i, j] = findContainer(point);
        tree[i][j].push_back({&point, count});
        count++;
    }
}

void PointsStorage::setSceneRect(QRectF rect){
    sceneRect = rect;
}

int PointsStorage::getContainersCount(){
    return pow(2, depth) * pow(2, depth);
}

int PointsStorage::size(){
    return points_.size();
}

std::vector<QPointF>* PointsStorage::getAllItems(){
    return &points_;
}

void PointsStorage::rebuildTree(int pos){
    auto posInc = [=](std::tuple<QPointF*, int> el){
        int val = std::get<1>(el);
        if (val > pos)
            std::get<1>(el)++;
    };
    int n = pow(2, depth);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            std::for_each(tree[i][j].begin(), tree[i][j].end(), posInc);
        }
    }
}

void PointsStorage::clearTree(){
    int n = pow(2, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (!tree[i][j].empty()){
                tree[i][j].clear();
            }
        }
    }
}

int PointsStorage::getPointNumber(QPointF p){ // возвращение порядкового индекса точки
    auto [i, j] = findContainer(p);
    auto& container = tree[i][j];
    auto comp = [=](std::tuple<QPointF*, int> p_){
        return p == *(std::get<0>(p_));
    };
    auto it = std::find_if(container.begin(), container.end(), comp); 
    return std::get<1>(*it);
}