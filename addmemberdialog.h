#ifndef ADDMEMBERDIALOG_H
#define ADDMEMBERDIALOG_H

#include <QDialog>

#include "classtable.h"

namespace Ui {
class AddMemberDialog;
}

class AddMemberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMemberDialog(QWidget *parent = nullptr);
    ~AddMemberDialog();

    bool IsNumber_memberExist(classTable &table, int num, int number);
    bool IsName_memberExist(classTable &table, int num, QString name);

    int number();
    QString name();
    QString dataType();
    int byte_number();
    QString accsess();

private slots:
    void on_OKButton_clicked();

    void on_CancleButton_clicked();

private:
    Ui::AddMemberDialog *ui;
};

#endif // ADDMEMBERDIALOG_H
