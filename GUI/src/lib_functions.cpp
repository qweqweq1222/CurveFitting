#include "lib_functions.h"

#include <QApplication>
#include <QDebug>
#include <fstream>

SplineRedactor::SplineRedactor(){
}

int SplineRedactor::curveFit(int argc, char *argv[], double (*userFunc)(double)){
    a = new QApplication(argc, argv);
    w = new MainWindow;
    w->setUserPoints();
    w->setUserFunction(userFunc);
    w->show();
    return a->exec();
}

void SplineRedactor::setUserFunction(double (*userFunc)(double)){
    w->setUserFunction(userFunc);
}

void SplineRedactor::setUserPoints(std::vector<double> usrPts){
    std::ofstream fout("/tmp/pointsTechnical.txt");
    for (auto val: usrPts){
        fout << val << std::endl;
    }
}

void SplineRedactor::setUserPoint(double val){
    w->setUserPoint(val);
}