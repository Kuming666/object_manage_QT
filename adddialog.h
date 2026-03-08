#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

#include "classtable.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();

    bool IsNumberExist(classTable &table, int number) const;
    bool IsNameExist(classTable &table, QString name) const;

    int number();
    QString name();
    QString base_name();
    QString Function();
    QDate date();
    QString author();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
