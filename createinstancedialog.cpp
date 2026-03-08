#include "createinstancedialog.h"
#include "ui_createinstancedialog.h"

CreateInstanceDialog::CreateInstanceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateInstanceDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("增加类对象");
}

CreateInstanceDialog::~CreateInstanceDialog()
{
    delete ui;
}

QString CreateInstanceDialog::cName()
{
    return ui->lineEditCname->text();
}

QString CreateInstanceDialog::iName()
{
    return ui->lineEditOb_name->text();
}

void CreateInstanceDialog::on_AddBtn_clicked()
{
    accept();
}

void CreateInstanceDialog::on_CancleBtn_clicked()
{
    reject();
}
