#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileSystemModel>

namespace Ui {
class saveDialog;
}

class saveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit saveDialog(QWidget *parent = nullptr);
    ~saveDialog();

    QString getPath();
    QString getFileName();
private:
    Ui::saveDialog *ui;
    QFileSystemModel* model;
    QString path;
    QString name;
private slots:
    void onChoice();
    void onEditingFinish();
};

#endif // SAVEDIALOG_H
