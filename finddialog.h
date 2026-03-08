#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

#include "classtable.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(classTable& table, QWidget *parent = nullptr);
    ~FindDialog();

    int Number();
    QString Name();

    int getFindIndex();
    QList<QPair<int, int>> getResults() const; // <类编号, 成员索引>

private slots:
    void on_Findbtn_clicked();

    void on_Canclebtn_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::FindDialog *ui;

    int findIndex;

    classTable& table;  // 存储类表引用
    QList<QPair<int, int>> results; // 存储所有匹配结果
};

#endif // FINDDIALOG_H
