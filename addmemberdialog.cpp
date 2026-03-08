#include "addmemberdialog.h"
#include "ui_addmemberdialog.h"

#include <QMessageBox>

AddMemberDialog::AddMemberDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMemberDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("增加类成员");
}

AddMemberDialog::~AddMemberDialog()
{
    delete ui;
}

bool AddMemberDialog::IsNumber_memberExist(classTable &table, int num, int number)
{
    //num为所想选的类编号
    for(int i = 0; i < table.getTotalMemberNumber(num); ++i)
    {
        if(number == table.getOb_member(num, i).member_number)
            return true;
    }
    return false;
}

bool AddMemberDialog::IsName_memberExist(classTable &table, int num, QString name)
{
    for(int i = 0; i < table.getTotalMemberNumber(num); ++i)
    {
        if(name == table.getOb_member(num, i).member_name)
            return true;
    }
    return false;
}

int AddMemberDialog::number()
{
    QString sNumber = ui->lineEditNumber->text();
    bool ok;
    int number = sNumber.toInt(&ok);
    if(ok && number >= 0)
        return number;
    else
        return -1;
}

QString AddMemberDialog::name()
{
    return ui->lineEditName->text();
}

QString AddMemberDialog::dataType()
{
    return ui->lineEditType->text();
}

int AddMemberDialog::byte_number()
{
    QString sNumber = ui->lineEditByte_number->text();
    bool ok;
    int number = sNumber.toInt(&ok);
    if(ok && number >= 0)
        return number;
    else
        return -1;
}

QString AddMemberDialog::accsess()
{
    return ui->comboBoxAccess->currentText();
}

void AddMemberDialog::on_OKButton_clicked()
{
    //增加校验，确保数据已填入
    if(ui->lineEditNumber->text().isEmpty() ||
            ui->lineEditName->text().isEmpty() || ui->lineEditType->text().isEmpty() ||
            ui->lineEditByte_number->text().isEmpty() || ui->comboBoxAccess->currentText().isEmpty())
    {
        QMessageBox::information(this, "新增失败", "请确保数据全部填入");
    }
    //确保编号和字节数是数字
    else if(byte_number() == -1 || number() == -1)
    {
        QMessageBox::information(this, "新增失败", "编号和字节数必须为自然数");
    }
    else
        accept();
}

void AddMemberDialog::on_CancleButton_clicked()
{
    reject();
}
