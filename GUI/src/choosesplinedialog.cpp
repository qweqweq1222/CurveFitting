#include "choosesplinedialog.h"
#include "ui_choosesplinedialog.h"
#include <QDebug>

ChooseSplineDialog::ChooseSplineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseSplineDialog)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &ChooseSplineDialog::onChoose);
}

ChooseSplineDialog::~ChooseSplineDialog()
{
    delete ui;
}

void ChooseSplineDialog::onChoose(){
    QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->radioButtons->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i)
    {
        group.addButton(allButtons[i],i);
    }
    splineType = group.checkedButton()->objectName().toStdString();
}

std::string ChooseSplineDialog::getSplineType(){
    return splineType;
}