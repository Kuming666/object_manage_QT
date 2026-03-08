#ifndef OBJECT_MEMBERDIALOG_H
#define OBJECT_MEMBERDIALOG_H

#include <QDialog>

namespace Ui {
class Object_memberDialog;
}

class Object_memberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Object_memberDialog(QWidget *parent = nullptr);
    ~Object_memberDialog();

    int number();

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::Object_memberDialog *ui;
};

#endif // OBJECT_MEMBERDIALOG_H
