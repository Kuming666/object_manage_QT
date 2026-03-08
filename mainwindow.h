#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardItemModel>

#include "classtable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showObjects();
    void showObject_members(int num);
    void showFindOb_members(QList<QPair<int, int>> &results);

    void highlight(int dex);

private slots:
    void showCustomContextMenu(const QPoint &pos);

    void on_actionDeleteSelected_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionAdd_triggered();

    void showClassTableView_changed();

    void on_actionShow_class_triggered();

    void on_actionShow_class_member_triggered();

    void on_actionAddClass_member_triggered();

    void on_actionFind_triggered();

    void on_actionDel_triggered();

    void on_actionDel_member_triggered();

    void on_actionDel_all_triggered();

    void on_actionDel_member_all_triggered();

    void on_actionShowClassSize_triggered();

    void on_AddObInstance_triggered();

    void on_ShowObInstance_triggered();

private:
    Ui::MainWindow *ui;
    classTable table;
    QStandardItemModel *objectsTableModel;
    int curTable;
    int curClass;//在显示类成员时通过编号知道是哪个类
};
#endif // MAINWINDOW_H
