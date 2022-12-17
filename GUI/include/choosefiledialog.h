#ifndef CHOOSEFILEDIALOG_H
#define CHOOSEFILEDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileSystemModel>

namespace Ui {
class ChooseFileDialog;
}

class ChooseFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseFileDialog(QWidget *parent = nullptr);
    ~ChooseFileDialog();

    QString getPath();
    
private:
    Ui::ChooseFileDialog *ui;
    QFileSystemModel* model;
    QString path = "";
public slots:
    void onChoice();
};

#endif // CHOOSEFILEDIALOG_H
