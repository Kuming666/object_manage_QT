#ifndef CREATEINSTANCEDIALOG_H
#define CREATEINSTANCEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateInstanceDialog;
}

class CreateInstanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateInstanceDialog(QWidget *parent = nullptr);
    ~CreateInstanceDialog();

    QString cName();
    QString iName();

private slots:
    void on_AddBtn_clicked();

    void on_CancleBtn_clicked();

private:
    Ui::CreateInstanceDialog *ui;
};

#endif // CREATEINSTANCEDIALOG_H
