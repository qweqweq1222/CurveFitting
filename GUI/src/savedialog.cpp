#include "savedialog.h"
#include "ui_savedialog.h"

saveDialog::saveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::saveDialog)
{
    ui->setupUi(this);
    model = new QFileSystemModel;
    model->setRootPath(QString("/"));
    model->setNameFilters({"*.jpg", "*.png", "*.jpeg"});
    ui->treeView->setModel(model);
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
    QModelIndex idx = model->index("/");
    ui->treeView->setRootIndex(idx);
    connect(ui->treeView, &QTreeView::clicked, this, &saveDialog::onChoice);
    connect(ui->fileName, &QLineEdit::editingFinished, this, &saveDialog::onEditingFinish);
}

saveDialog::~saveDialog()
{
    delete ui;
}

void saveDialog::onChoice(){
    path = model->filePath(ui->treeView->currentIndex());
}

QString saveDialog::getPath(){
    return path;
}

QString saveDialog::getFileName(){
    return name;
}

void saveDialog::onEditingFinish(){
    name = ui->fileName->text();
}