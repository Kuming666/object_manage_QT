#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(classTable& table, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog),
    table(table)
{
    ui->setupUi(this);
    this->setWindowTitle("查找");

    findIndex = 0;
}

FindDialog::~FindDialog()
{
    delete ui;
}

int FindDialog::Number()
{
    return ui->lineEdit->text().toInt();
}

QString FindDialog::Name()
{
    return ui->lineEdit->text();
}

int FindDialog::getFindIndex()
{
    return findIndex;
}

QList<QPair<int, int>> FindDialog::getResults() const
{
    return results;  // 返回所有匹配结果
}

void FindDialog::on_Findbtn_clicked()
{
    results.clear();  // 清空之前的结果
        if (findIndex == 1) // 当查找类型为"类成员编号"时
        {
            int targetNum = Number();
            for (int i = 0; i < table.getTotalNumber(); ++i)
            {
                int cNum = table.getObject(i).number;  // 当前类编号
                for (int j = 0; j < table.getTotalMemberNumber(cNum); ++j)
                {
                    if (table.getOb_member(cNum, j).member_number == targetNum)
                    {
                        results.append(QPair<int, int>(cNum, j));
                    }
                }
            }
        }
    accept();
}

void FindDialog::on_Canclebtn_clicked()
{
    reject();
}

void FindDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "类编号")
    {
        findIndex = 0;
    }
    else if(arg1 == "类成员编号")
    {
        findIndex = 1;
    }
    else if(arg1 == "类名")
    {
        findIndex = 2;
    }
    else if(arg1 == "类成员名")
    {
        findIndex = 3;
    }
}
