#ifndef CHOOSESPLINEDIALOG_H
#define CHOOSESPLINEDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class ChooseSplineDialog;
}

class ChooseSplineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseSplineDialog(QWidget *parent = nullptr);
    ~ChooseSplineDialog();

    std::string getSplineType();

private:
    Ui::ChooseSplineDialog *ui;
    std::string splineType;

public slots:
    void onChoose();
};

#endif // CHOOSESPLINEDIALOG_H
