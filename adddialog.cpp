#include "adddialog.h"
#include "ui_adddialog.h"

#include <QMessageBox>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("增加类");
}

AddDialog::~AddDialog()
{
    delete ui;
}

bool AddDialog::IsNumberExist(classTable &table, int number) const
{
    for (int i = 0; i < table.getTotalNumber(); ++i)
    {
        if(number == table.getObject(i).number)
            return true;
    }
    return false;
}

bool AddDialog::IsNameExist(classTable &table, QString name) const
{
    for (int i = 0; i < table.getTotalNumber(); ++i)
    {
        if(name == table.getObject(i).name)
            return true;
    }
    return false;
}

int AddDialog::number()
{
    QString sNumber = ui->textEditNumber->toPlainText();
    bool ok;
    int number = sNumber.toInt(&ok);
    if(ok)
        return number;
    else
        return -1;
}

QString AddDialog::name()
{
    return ui->textEditName->toPlainText();
}

QString AddDialog::base_name()
{
    return ui->textEditBase_name->toPlainText();
}

QString AddDialog::Function()
{
    return ui->textEditFunction->toPlainText();
}

QDate AddDialog::date()
{
    return ui->dateEdit->date();
}

QString AddDialog::author()
{
    return ui->textEditAuthor->toPlainText();
}

void AddDialog::on_btnOK_clicked()
{
    //增加校验，确保编号与类名填写
    if(ui->textEditNumber->toPlainText().isEmpty() ||
            ui->textEditName->toPlainText().isEmpty())
    {
        QMessageBox::information(this, "新增失败", "编号和类名必须填写");
    }
    else if(number() == -1)
        QMessageBox::information(this, "新增失败", "编号必须为自然数");
    else
        accept();
}

void AddDialog::on_btnCancel_clicked()
{
    reject();
}
