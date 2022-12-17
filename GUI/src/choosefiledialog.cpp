#include "choosefiledialog.h"
#include "ui_choosefiledialog.h"
#include <QDebug>

ChooseFileDialog::ChooseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseFileDialog)
{
    ui->setupUi(this);
    model = new QFileSystemModel;
    model->setRootPath(QString("/"));
    model->setNameFilters({"*.txt"});
    ui->treeView->setModel(model);
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
    QModelIndex idx = model->index("/home");
    ui->treeView->setRootIndex(idx);
    connect(ui->treeView, &QTreeView::clicked, this, &ChooseFileDialog::onChoice);
}


ChooseFileDialog::~ChooseFileDialog()
{
    delete ui;
    delete model;
}

void ChooseFileDialog::onChoice(){
    if (!model->isDir(ui->treeView ->currentIndex()))
        path = model->filePath(ui->treeView->currentIndex());
    else 
        path = "";
}

QString ChooseFileDialog::getPath(){
    return path;
}