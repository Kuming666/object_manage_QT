#include "object_member_2dialog.h"
#include "ui_object_member_2dialog.h"

Object_member_2Dialog::Object_member_2Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Object_member_2Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("删除类成员");
}

Object_member_2Dialog::~Object_member_2Dialog()
{
    delete ui;
}

int Object_member_2Dialog::cNumber()
{
    return ui->lineEditCnumber->text().toInt();
}

int Object_member_2Dialog::mNumber()
{
    return ui->lineEditMnumber->text().toInt();
}

void Object_member_2Dialog::on_OKbtn_clicked()
{
    accept();
}

void Object_member_2Dialog::on_Canclebtn_clicked()
{
    reject();
}
