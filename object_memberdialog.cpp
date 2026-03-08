#include "object_memberdialog.h"
#include "ui_object_memberdialog.h"

Object_memberDialog::Object_memberDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Object_memberDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("选择想进行操作的类");
}

Object_memberDialog::~Object_memberDialog()
{
    delete ui;
}

int Object_memberDialog::number()
{
    QString sNumber = ui->numberLineEdit->text();
    int number = sNumber.toInt();
    return number;
}

void Object_memberDialog::on_OKButton_clicked()
{
    accept();
}

void Object_memberDialog::on_CancelButton_clicked()
{
    reject();
}
