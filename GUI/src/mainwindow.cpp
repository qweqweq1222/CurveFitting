#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneEvent>
#include <QBrush>
#include <QShortcutEvent>
#include <cmath>
#include <execution>
#include <QGraphicsEllipseItem>
#include <list>

#include <chrono>
#include <thread>
#include <limits>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene.addItem(&item);
    scene.setItemIndexMethod(QGraphicsScene::ItemIndexMethod::BspTreeIndex);

    ui->viewer->setScene(&scene);
    scene.setSceneRect(0, 0, width, hight);

    pointsStorage.setSceneRect({0, 0, width, hight});
    threadsCount = QThread::idealThreadCount() - 1;
    selectors = std::vector<PointsSelecter>(threadsCount); 

    ui->viewer->show();

    zoom = new Graphics_view_zoom(ui->viewer);
    zoom->set_modifiers(Qt::NoModifier);

    model = new QFileSystemModel;
    model->setRootPath(QString("../"));
    model->setNameFilters({"*.jpg", "*.png", "*.jpeg"});
    ui->fileTree->setModel(model);
    QModelIndex idx = model->index("../");
    ui->fileTree->setRootIndex(idx);
    connect(ui->fileTree, &QTreeView::doubleClicked, this, &MainWindow::loadIMG);

    pen.setBrush(Qt::red);
    pen.setWidth(1.5);
    
    penLine.setBrush(Qt::red);
    penLine.setWidth(1);

    sc = new QShortcut(QKeySequence(tr("CTRL+Z")), this);

    qApp->installEventFilter(this);

    ui->splineTypeLabel->setText("Spline type: Cubic");

    connect(sc, &QShortcut::activated, this, &MainWindow::undo);
    //connect(ui->del, &QPushButton::pressed, this, &MainWindow::onDelete);
    connect(ui->redact, &QPushButton::pressed, this, &MainWindow::onRedact);
    connect(ui->apply, &QPushButton::pressed, this, &MainWindow::onApply);
    connect(ui->loadButton, &QPushButton::pressed, this, &MainWindow::onLoad);
    connect(ui->chooseSpline, &QPushButton::pressed, this, &MainWindow::onChooseSpline);
    connect(ui->saveButton, &QPushButton::pressed, this, &MainWindow::onSaveDots);
    connect(ui->interButtn, &QPushButton::pressed, this, &MainWindow::onInterpolate);
    connect(zoom, &Graphics_view_zoom::zoomed, this, &MainWindow::moreDetails);
    connect(zoom, &Graphics_view_zoom::moved, this, &MainWindow::moreDetails);
    connect(ui->viewer->horizontalScrollBar(), &QAbstractSlider::sliderMoved, this, &MainWindow::moreDetails);
    connect(ui->viewer->verticalScrollBar(), &QAbstractSlider::sliderMoved, this, &MainWindow::moreDetails);
    connect(ui->isClosed, &QCheckBox::stateChanged, this, &MainWindow::onCheckClosed);
    connect(ui->generatePointsbutton,&QPushButton::pressed, this, &MainWindow::onGenerate);

    ui->redact->setEnabled(false);
    ui->apply->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete lpos;
    delete sc;
    delete zoom;
}

void MainWindow::loadIMG(){
    if (!model->isDir(ui->fileTree->currentIndex())){
        auto pxmap = QPixmap(model->filePath(ui->fileTree->currentIndex()));
        item.setPixmap(pxmap);            
        ui->viewer->setSceneRect(pxmap.rect());
        ui->viewer->updateSceneRect(ui->viewer->sceneRect());
        ui->viewer->show();
    }
}

void MainWindow::addPoint(QPointF pos){
    points.push_back(pos);
}

void MainWindow::addLine(QPointF lastpos, QPointF pos){
    scene.addLine(lastpos.x(), lastpos.y(), pos.x(), pos.y(), pen);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::GraphicsSceneMouseDoubleClick){
        QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
        if (pointsStorage.size() == 0){
            chooseMod = false;
            redactMod = true;
            ui->apply->setEnabled(true);
        } 
        if (chooseMod){
            choosePoints(e->scenePos());
            return true;
        }
        if (redactMod){
            addUsersPoints(e->scenePos());
            return true;
        }
    }
    return false;
}
    
void MainWindow::undo(){
    if (scene.items().size() > 2){
        auto items = scene.items(Qt::AscendingOrder);
        scene.removeItem(items.last());
        scene.update();
    }
}

void MainWindow::onDelete(){
    deleteMod = ! deleteMod;
}

void MainWindow::deleteItem(QPointF pos){
    auto i = scene.itemAt(pos, QTransform());
            if(i != static_cast<QGraphicsItem*>(&item)){
                scene.removeItem(i);
                scene.update();
            }
}

void MainWindow::onLoad(){
    fileDilalog = new ChooseFileDialog;

    QString path;
    if(fileDilalog->exec() == QDialog::Accepted)
        if(fileDilalog->getPath() != ""){
            path = fileDilalog->getPath();
        }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    
    QTextStream in(&file);
    std::vector<QPointF> ps;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        auto nums = line.split(" ");
        auto x = nums[0].toDouble();
        auto y = nums[1].toDouble();
        ps.push_back(QPointF(x, y));
    }
    file.close();
    pointsStorage.setPoints(std::move(ps));
    pointsStorage.buildTree();
    try{
        interpolator.setPoints(*(pointsStorage.getAllItems()));
        interpolator.setClosed(closed);
        interpolator.interpolate(splineType, Smoothness::c2);
        interpolated = true;
    }
    catch(std::exception const& e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }
        
    QMessageBox msgBox;
    msgBox.setText("Points are loaded");
    msgBox.exec();

    delete fileDilalog;
}

void MainWindow::onChooseSpline(){
    splineDilalog = new ChooseSplineDialog;
    splineDilalog->exec();
    splineType = splineDilalog->getSplineType();
    ui->splineTypeLabel->setText(QString::fromStdString("Spline type: " + splineType));
    delete splineDilalog;
}
void MainWindow::onSaveSpline(){
    savedialog = new saveDialog;
    savedialog->exec();
    delete savedialog;
    }

void MainWindow::onSaveDots(){
    savedialog = new saveDialog;
    savedialog->exec();
    auto path = savedialog->getPath();
    auto name = savedialog->getFileName();

    QFile file(path + "/" + name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    auto ptr = pointsStorage.getAllItems();
    for (auto& point : *ptr){
        out << QString::number(point.x(), 103, 12) << " " << QString::number(point.y(), 103, 12) << "\n";
    }
    file.close();

    delete savedialog;
}

void MainWindow::onInterpolate(){
    try{
        interpolator.setPoints(*(pointsStorage.getAllItems()));
        interpolator.setClosed(closed);
        interpolator.interpolate(splineType, Smoothness::c2);

        if (isFunctionDrawable){
            rmse = RMSE(pointsStorage.getAllItems()->front().x(), pointsStorage.getAllItems()->back().x(), 0.1);
        }

        ui->rmse->setText("RMSE: " + QString::number(rmse));

        interpolated = true;
    }
    catch(std::range_error const& e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }  
    catch(std::exception const& e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }

    int start = std::numeric_limits<int>::max();
    int end = 0;
    if (!visibleItems.empty()){
        for (auto& el : visibleItems){
            if (std::get<1>(el) != -1 && std::get<0>(el)){
                if (end < std::get<1>(el))
                    end = std::get<1>(el);
                if (start > std::get<1>(el))
                    start = std::get<1>(el);
            }
        }
    } else {
        start = 0;
        end = pointsStorage.size() - 1;
    }
    double step = double (end - start) / 1000;
    drawSpline(start, end, step);
}

void MainWindow::moreDetails(){
    if (pointsStorage.size() == 0)
        return;

    auto sceneRect = ui->viewer->mapToScene(ui->viewer->rect()).boundingRect();  // получаем размер всего экрана 
    square = sceneRect.width() * sceneRect.height(); 
    scale = sqrt(square / width / hight); 
    pen.setWidthF(1.5 * scale < 0.3 ? 0.3 : scale);

    if (!visibleItems.empty()){
        for (auto& item : visibleItems){
            if (redactMod && std::get<1>(item) == -1) // точки добавлены пользователем, но кнопка apply не нажата (они не внесличь еще в pointsStorage)
                continue;
            scene.removeItem(std::get<0>(item));
            if (!std::get<0>(item))
                delete std::get<0>(item);
        }
        visibleItems.clear();
    }

    auto containers = pointsStorage.getContainers(sceneRect); // получаем все контейнеры в окне просмотра 
    if (containers.size() > pointsStorage.getContainersCount() / 2){ 
        std::vector<QPointF> *allItems = pointsStorage.getAllItems();
        int offset = allItems->size() / visibleItemsCount; // через сколько точек будем отображать "видимые" точки
        offset = offset ? offset : 1;
        if (offset <= 1){ // редактировать можем только при определенном приближении 
            ui->redact->setEnabled(true);
            ui->apply->setEnabled(true);
        } else {
            ui->redact->setEnabled(false);
            ui->apply->setEnabled(false);
        }
        for (int i = 0; i < allItems->size(); i += offset){
            QPointF p = (*allItems)[i];
            QGraphicsEllipseItem* el = new QGraphicsEllipseItem(p.x() - 5, p.y() - 5, 10, 10); //отрисовка выбранных точек
            el->setPen(pen);
            el->setTransformOriginPoint(p.x(), p.y());
            el->setScale(scale);
            scene.addItem(el);
            visibleItems.push_back({el, i});
        }
        
        double step = (double)allItems->size() / 1000.0;
        drawSpline(0, allItems->size() - 1, step);
        if (isFunctionDrawable)
            drawFunc(0, allItems->size() - 1, step);
        return;
    }

    std::vector<std::tuple<QPointF*, int>> pts; // указатель на конкретную точку и ее порядковый номер
    int count = 0;
    int s = 0;

    for (auto& container : containers){  // отбираем точки, которые лежат в подобласти прямоугольника просмотра
        if (container->size() > 0){
            selectors[count].setPoints(container->data(), container->size()); // selectors наследуется от QThread -> многопоточно
            selectors[count].setRectangle(sceneRect);
            selectors[count].start(); 
            count++;
            s++;
        }

        if (count == threadsCount - 1){ // когда запустились все потоки -> ждем пока они завершаются 
            count = 0;
            for (auto& sel : selectors){
                sel.wait();
            }
            for (auto& sel : selectors){
                auto selected = sel.getSelectedPoints();
                int selected_size = sel.getSelectedPointsSize();
                for (int j = 0; j < selected_size; j++){
                    pts.push_back(selected[j]);
                }
            }
            
        }

    }

    for (auto& sel : selectors){            
        sel.wait();
    }
    for (int i = 0; i < count; i++){
        auto& sel = selectors[i];
        auto selected = sel.getSelectedPoints();
        int selected_size = sel.getSelectedPointsSize();
        for (int j = 0; j < selected_size; j++){
            pts.push_back(selected[j]);   
        }
    }

    auto comp = [=](std::tuple<QPointF*, int> a, std::tuple<QPointF*, int> b){
        return std::get<1>(a) < std::get<1>(b);  // tuple(1) -> сортировка по порядковому номеру 
    };
    std::sort(std::execution::par_unseq, pts.begin(), pts.end(), comp); // многопоточная сортировка (индексы перемешены)
    
    int offset = round((double)pts.size() / (double)visibleItemsCount);

    if (offset <= 1){
        ui->redact->setEnabled(true);
        ui->apply->setEnabled(true);
    } else {
        ui->redact->setEnabled(false);
        ui->apply->setEnabled(false);
    }
    
    if (offset == 0)
        return;
    
    count = 0;
    for (auto& val : pts){ //выводим на экран точки кратные оффсету
        if (count == offset){ 
            auto& [point, num] = val;
            QGraphicsEllipseItem* item = new QGraphicsEllipseItem(point->x() - 5, point->y() - 5, 10, 10); 
            item->setPen(pen);
            item->setTransformOriginPoint(point->x(), point->y());
            if (num == start_pos || num == end_pos){
                item->setScale(scale * 1.5);
            } else{
                item->setScale(scale);
            }
            scene.addItem(item);
            visibleItems.push_back({item, num});
            count = 0;
        }
        count++;
    }

    auto [ps, start] = pts.front();  
    auto [pe, end] = pts.back(); 
    double step = (double)(end - start) / 1000;
    drawSpline(start, end, step);
    if (isFunctionDrawable)
        drawFunc(start, end, step);
}

void MainWindow::onRedact(){
    QMessageBox msgBox;
    msgBox.setText("Choose 2 points to insert new points between!");
    msgBox.exec();

    chooseMod = true;
}

void MainWindow::onApply(){
    if (pointsStorage.size() == 0){
        pointsStorage.setPoints(usersPoints);
        pointsStorage.buildTree();
        if(!usersPoints.empty()){
            usersPoints.clear();
        }
        start_pos = -2;
        end_pos = -2;
        redactMod = false;
        return;
    }

    if (bounds.size() < 2)
        return;

    if(std::min(start_pos, end_pos) == 0 && std::max(start_pos, end_pos) == pointsStorage.size() - 1){
        std::reverse(usersPoints.begin(), usersPoints.end());
        pointsStorage.addPoints(usersPoints, std::max(start_pos, end_pos));
    } else {
        if (start_pos > end_pos){
            std::reverse(usersPoints.begin(), usersPoints.end());
            pointsStorage.addPoints(usersPoints, end_pos);
        } else {
            pointsStorage.addPoints(usersPoints, start_pos);
        }
    }

    if(!usersPoints.empty()){
        usersPoints.clear();
    }
    if(!bounds.empty()){
        bounds.clear();
    }
    redactMod = false;
    start_pos = -2;
    end_pos = -2;
    onInterpolate();
    moreDetails();
}

void MainWindow::addUsersPoints(QPointF pos){
    usersPoints.push_back(pos);
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(pos.x() - 5, pos.y() - 5, 10, 10); 
    item->setPen(pen);
    item->setTransformOriginPoint(pos.x(), pos.y());
    item->setScale(scale);
    scene.addItem(item);
    if (pointsStorage.size() == 0){
        visibleItems.push_back({item, visibleItems.size()}); 
    } else {
        visibleItems.push_back({item, -1}); //-1 -- для точек, добавленных пользователем
    }
    scene.update();
}

void MainWindow::choosePoints(QPointF pos){
    if (bounds.size() < 2){
        auto itm = scene.itemAt(pos, QTransform());

        auto comp = [=](std::tuple<QGraphicsItem*, int> a){
            return std::get<0>(a) == itm;
        };
        auto it = std::find_if(visibleItems.begin(), visibleItems.end(), comp);
        if (it == visibleItems.end())
            return;

        if (bounds.size() == 0){
            start_pos = std::get<1>(*it);
        }
        if (bounds.size() == 1){
            end_pos = std::get<1>(*it);
        }
        itm->setScale(scale * 1.5);
        bounds.push_back(itm);
    }
    if (bounds.size() == 2){
        int min = std::min(start_pos, end_pos);
        int max = std::max(start_pos, end_pos);
        if (max - min == 1){
            chooseMod = false;
            redactMod = true;
            QMessageBox msgBox;
            msgBox.setText("Now you can add points!");
            msgBox.exec();
        }
        if (max - min > 1){
            chooseMod = false;
            redactMod = true;
            QMessageBox msgBox;
            msgBox.setText("Now you can add points!\nPoints between choosen were deleted!");
            msgBox.exec();
            if (min == 0 && max == pointsStorage.size() - 1)
                return;
            pointsStorage.deletePoints(min, max);
            moreDetails();
            if (start_pos > end_pos){
                start_pos -= (max - min) - 1;
            } else {
                end_pos -= (max - min) - 1;
            }

        }

    }
}

void MainWindow::drawSpline(double start, double end, double step){
    if (interpolated){
        if (pathPtr)
            scene.removeItem(pathPtr);
        painter = QPainterPath();
        painter.moveTo(interpolator.eval(start));
        for (double t = start; t <= end + closed; t += step){ 
            QPointF p = interpolator.eval(t);                
            painter.lineTo(p);
        }
        QPointF p = interpolator.eval(end + closed);                
        painter.lineTo(p);
        penLine.setWidthF(scale);
        pathPtr = scene.addPath(painter, penLine);
    }
    scene.update();
}

void MainWindow::drawFunc(double start, double end, double step){
    if (pathPtrFunc)
        scene.removeItem(pathPtrFunc);

    start += 200;
    end += 200;
    
    painterFunc = QPainterPath();
    painterFunc.moveTo(start, generator.userFuncEval(start));
    for (double t = start; t <= end; t += step){ 
        double f = generator.userFuncEval(t);
        painterFunc.lineTo(t, f);
    }
    double f = generator.userFuncEval(end);
    painterFunc.lineTo(QPointF(end, f));
    penFunc.setWidthF(scale);
    penFunc.setColor(Qt::blue);
    pathPtrFunc = scene.addPath(painterFunc, penFunc);
    scene.update();
}

double MainWindow::RMSE(double start, double end, double step){
    double rmse = 0;
    for (double t = start; t < end; t += step){
        rmse += pow((interpolator.eval(t - 200).y() - generator.userFuncEval(t)), 2);
    }

    rmse = sqrt(rmse);
    return rmse;
}

void MainWindow::onCheckClosed(int state){
    closed = (bool) state;
    interpolator.setClosed(closed);
    try{
        interpolator.setPoints(*(pointsStorage.getAllItems()));
        interpolator.interpolate(splineType, Smoothness::c2);
        interpolated = true;
    }
    catch(std::range_error const& e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }  
    catch(std::exception const& e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
        return;
    }
}

void MainWindow::onGenerate(){
    if (!userFunc_){
        QMessageBox msgBox;
        msgBox.setText("User funciton is undefined!");
        msgBox.exec();
        return;
    }

    if (!visibleItems.empty()){
        for (auto& item : visibleItems){
            scene.removeItem(std::get<0>(item));
            if (!std::get<0>(item))
                delete std::get<0>(item);
        }
        visibleItems.clear();
    }

    generator.setFunction(userFunc_);
    generator.generateForInterpolation(usersKnots);
    pointsStorage.setPoints(generator.getPoints());
    pointsStorage.buildTree();
    double step = (usersKnots.back() - usersKnots.front()) / usersKnots.size();
    isFunctionDrawable = true;
    moreDetails();
}

void MainWindow::setUserFunction(double (*userFunc)(double)){
    userFunc_ = userFunc; 
}

void MainWindow::setUserPoints(){
    QFile file("/tmp/pointsTechnical.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        auto t = line.toDouble();
        usersKnots.push_back(t);
    }
    file.close();
}

void MainWindow::setUserPoint(double val){
    usersKnots.push_back(val);
}