#ifndef OBJECT_MEMBER_2DIALOG_H
#define OBJECT_MEMBER_2DIALOG_H

#include <QDialog>

namespace Ui {
class Object_member_2Dialog;
}

class Object_member_2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Object_member_2Dialog(QWidget *parent = nullptr);
    ~Object_member_2Dialog();

    int cNumber();
    int mNumber();

private slots:
    void on_OKbtn_clicked();

    void on_Canclebtn_clicked();

private:
    Ui::Object_member_2Dialog *ui;
};

#endif // OBJECT_MEMBER_2DIALOG_H
