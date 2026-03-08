#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QTextStream>
#include <QStandardItem>
#include <QMessageBox>
#include <QAbstractScrollArea>

#include "object.h"
#include "adddialog.h"
#include "ui_adddialog.h"
#include "readonlydelegate.h"
#include "object_memberdialog.h"
#include "object_member_2dialog.h"
#include "addmemberdialog.h"
#include "finddialog.h"
#include "createinstancedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("类管理系统");

    objectsTableModel = new QStandardItemModel;

    //视图与模型相绑定
    ui->showClassTableView->setModel(objectsTableModel);

    ui->showClassTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    // 将表格的右键信号关联到菜单显示槽函数
    connect(ui->showClassTableView, &QTableView::customContextMenuRequested,
            this, &MainWindow::showCustomContextMenu);

    //编辑后同步数据的信号和槽连接
    connect(ui->showClassTableView->itemDelegate(), &QAbstractItemDelegate::closeEditor,
            this, &MainWindow::showClassTableView_changed);

    //增加快捷键
    ui->actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
    ui->actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));

    // 创建右键删除动作
    QAction* deleteAction = new QAction("删除选中项", this);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::on_actionDeleteSelected_triggered);
    // 将动作添加到右键菜单
    ui->menu_change->addAction(deleteAction);

    curTable = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCustomContextMenu(const QPoint &pos)
{
    // 在鼠标位置显示预定义的右键菜单
    ui->menu_change->exec(ui->showClassTableView->mapToGlobal(pos));
}


void MainWindow::on_actionOpen_triggered()
{
    QString curPath = QDir::currentPath();
    //弹出文件选择窗口
    QString read_fileName = QFileDialog::getOpenFileName(this, "请打开文件", curPath, "文本文件(*.txt);;所有文件(*.*)");

    if (read_fileName.isEmpty())
        return;
    else
    {
        table.ReadClassFromFile(read_fileName);
        showObjects();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString curPath =QDir::currentPath();
    QString save_fileName = QFileDialog::getSaveFileName(this, "请保存文件", curPath, "文本文件(*.txt);;所有文件(*.*)");
    if (save_fileName.isEmpty())
        return;
    else
    {
        table.SaveClassToFile(save_fileName);
    }
}

void MainWindow::on_actionAdd_triggered()
{
    AddDialog addDialog(this);
    int ret = addDialog.exec();
    if(ret == QDialog::Accepted)
    {
        //检测编号是否重复
        int tempNumber = addDialog.number();
        QString tempName = addDialog.name();
        if(addDialog.IsNumberExist(table, tempNumber))
        {
            QMessageBox::information(this, "创建失败", "当前编号已被使用");
        }
        else if(addDialog.IsNameExist(table, tempName))
        {
            QMessageBox::information(this, "创建失败", "当前类名已被使用");
        }
        else
        {
            object tempObject;
            tempObject.number = tempNumber;
            tempObject.name = tempName;
            tempObject.base_name = addDialog.base_name();
            tempObject.function = addDialog.Function();
            tempObject.date = addDialog.date();
            tempObject.author = addDialog.author();
            //放入objects容器中
            table.addClass(tempObject);
            showObjects();
        }
    }
}

void MainWindow::on_actionAddClass_member_triggered()
{
    Object_memberDialog ob_memberDialog(this);
    int ret = ob_memberDialog.exec();
    if(ret == QDialog::Accepted)
    {
        int num = ob_memberDialog.number();
        if(table.getDexFromNumber(num) == -1)
        {
            showObjects();
            ui->labelClass->setText("没有找到所输入的类");
            ui->labelClass->show();
        }
        else
        {
            AddMemberDialog add_memberDialog(this);
            int ret_1 = add_memberDialog.exec();
            if(ret_1 == QDialog::Accepted)
            {
                int tempNumber = add_memberDialog.number();
                QString tempName = add_memberDialog.name();
                if(add_memberDialog.IsNumber_memberExist(table, num, tempNumber))
                {
                    QMessageBox::information(this, "创建失败", "当前类编号已被使用");
                }
                else if(add_memberDialog.IsName_memberExist(table, num, tempName))
                {
                    QMessageBox::information(this, "创建失败", "当前类成员名已被使用");
                }
                else
                {
                object_member tempOb_member;
                tempOb_member.member_number = tempNumber;
                tempOb_member.member_name = tempName;
                tempOb_member.member_type = add_memberDialog.dataType();
                tempOb_member.byte_number = add_memberDialog.byte_number();
                tempOb_member.member_access = add_memberDialog.accsess();
                table.addClass_member(num, tempOb_member);
                showObject_members(num);

                //更新字节数
                table.updateInstanceBytesForClass(num);
                }
            }
        }
    } 
}


void MainWindow::on_actionShow_class_triggered()
{
    showObjects();
}

void MainWindow::showObjects()
{
    //隐藏当前所处类提示
    ui->labelClass->hide();

    curTable = 1;
    objectsTableModel->clear();
    objectsTableModel->setColumnCount(6);//设置六列

    //显示表头文本
    QStringList tempList;
    tempList.append("编号");
    tempList.append("类名");
    tempList.append("基类名");
    tempList.append("功能");
    tempList.append("日期");
    tempList.append("作者");
    objectsTableModel->setHorizontalHeaderLabels(tempList);
    int RowCnt = table.getTotalNumber();
    objectsTableModel->setRowCount(RowCnt);

    //遍历所有类，将数据插入模型
    for(int i=0; i<RowCnt; ++i)
    {
        object tempObject = table.getObject(i);
        //都要转换为字符串
        QStandardItem *tempItem = new QStandardItem(QString::number(tempObject.number));
        objectsTableModel->setItem(i, 0, tempItem);
        tempItem = new QStandardItem(tempObject.name);
        objectsTableModel->setItem(i, 1, tempItem);
        tempItem = new QStandardItem(tempObject.base_name);
        objectsTableModel->setItem(i, 2, tempItem);
        tempItem = new QStandardItem(tempObject.function);
        objectsTableModel->setItem(i, 3, tempItem);
        tempItem = new QStandardItem(tempObject.date.toString("yyyy-MM-dd"));
        objectsTableModel->setItem(i, 4, tempItem);
        tempItem = new QStandardItem(tempObject.author);
        objectsTableModel->setItem(i, 5, tempItem);
    }

    // 创建只读代理实例
    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate(this);
    // 设置代理，使其只读
    ui->showClassTableView->setItemDelegateForColumn(0, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(1, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(2, readOnlyDelegate);
}

void MainWindow::on_actionShow_class_member_triggered()
{
    Object_memberDialog ob_memberDialog(this);
    int ret = ob_memberDialog.exec();
    if(ret == QDialog::Accepted)
    {
        int num = ob_memberDialog.number();
        showObject_members(num);
    }
}

void MainWindow::showObject_members(int num)
{
    curTable = 2;
    curClass = num;//保存类编号数据
    objectsTableModel->clear();
    objectsTableModel->setColumnCount(5);//设置五列

    //显示表头文本
    QStringList tempList;
    tempList.append("成员编号");
    tempList.append("成员名");
    tempList.append("数据类型");
    tempList.append("字节数");
    tempList.append("可访问性");
    objectsTableModel->setHorizontalHeaderLabels(tempList);
    //确定多少行
    int RowCnt = table.getTotalMemberNumber(num);
    objectsTableModel->setRowCount(RowCnt);

    //展示当前是在哪个类中
    ui->labelClass->show();
    QString sTip = "当前类： ";
    sTip += table.getClassName(num);
    if(table.getClassName(num) == "NULL")
        ui->labelClass->setText("没有找到所输入的类");
    else
        ui->labelClass->setText(sTip);

    //将数据插入模型
    for(int i=0; i<RowCnt; ++i)
    {
        object_member tempOb_member=table.getOb_member(num, i);
        QStandardItem *tempItem = new QStandardItem(QString::number(tempOb_member.member_number));
        objectsTableModel->setItem(i, 0, tempItem);
        tempItem = new QStandardItem(tempOb_member.member_name);
        objectsTableModel->setItem(i, 1, tempItem);
        tempItem = new QStandardItem(tempOb_member.member_type);
        objectsTableModel->setItem(i, 2, tempItem);
        tempItem = new QStandardItem(QString::number(tempOb_member.byte_number));
        objectsTableModel->setItem(i, 3, tempItem);
        tempItem = new QStandardItem(tempOb_member.member_access);
        objectsTableModel->setItem(i, 4, tempItem);
    }

    // 创建只读代理实例
    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate(this);
    // 为第0列（号码列）以及类型与字节数设置代理，使其只读
    ui->showClassTableView->setItemDelegateForColumn(0, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(2, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(3, readOnlyDelegate);
}


void MainWindow::showClassTableView_changed()
{
    QModelIndex index = ui->showClassTableView->currentIndex();
    int row = index.row(); //哪一行
    int col = index.column(); //哪一列
    QVariant data; //数据
    data = objectsTableModel->data(index);

    switch (curTable)
    {
    case 1 :
        //定义了局部变量则必须要用大括号
    {
        object & ob = table.getObject(row); //获取当前运动员，一定要是引用
        switch (col)
        {
        case 1 :
            ob.name = data.toString();
            break;
        case 2 :
            ob.base_name = data.toString();
            break;
        case 3 :
            ob.function = data.toString();
            break;
        case 4 :
            ob.date = data.toDate();
            break;
        case 5 :
            ob.author = data.toString();
            break;
        default:
            break;
        }
        break;
    }
    case 2 :
    {
        object_member & ob_member = table.getOb_member(curClass, row);
        bool ok;//toInt参数
        //字节数输入错误时，保存原数据
        int tempByte_number = ob_member.byte_number;
        switch (col)
        {
        case 1:
            ob_member.member_name = data.toString();
            break;
        case 2:
            ob_member.member_type = data.toString();
            break;
        case 3:
            data.toInt(&ok);
            //确保修改的是数字
            if(ok)
            {
                ob_member.byte_number = data.toInt();
                break;
            }
            else
            {
                QMessageBox::warning(this, "输入错误", "字节数必须为自然数");
                ob_member.byte_number = tempByte_number;
                showObject_members(curClass);
                break;
            }
        case 4:
            ob_member.member_access = data.toString();
            break;
        }
        break;
    }
    case 4:
    {
        QVector<ObjectInstance> & obIntances = table.getObjectInstances();
        if(col == 2)
        {
            obIntances[row].instanceName = data.toString();
        }
    }
    default:
        break;
    }
}

void MainWindow::on_actionDel_triggered()
{
    if(table.getTotalNumber() == 0)
    {
        QMessageBox::warning(this, "删除失败", "当前没有可删除的类");
    }
    else
    {
        Object_memberDialog ob_memberDialog(this);
        int ret = ob_memberDialog.exec();
        if(ret == QDialog::Accepted)
        {
            int num = ob_memberDialog.number();
            int dex = table.getDexFromNumber(num);
            if(dex == -1)
            {
                QMessageBox::warning(this, "删除失败", "没有找到该类");
            }
            else
            {
                table.delClass(dex);
                showObjects();
            }
        }
    }
}

void MainWindow::on_actionDel_member_triggered()
{
    Object_member_2Dialog ob_member_2Dialog(this);
    int ret = ob_member_2Dialog.exec();
    if(ret == QDialog::Accepted)
    {
        int cNum = ob_member_2Dialog.cNumber();
        int mNum = ob_member_2Dialog.mNumber();
        int cDex = table.getDexFromNumber(cNum);
        int mDex = table.getDexFromMember_number(cNum, mNum);
        if(cDex == -1)
        {
            QMessageBox::information(this, "删除失败", "没有找到该类");
        }
        else if(mDex == -1)
        {
            QMessageBox::information(this, "删除失败", "没有找到该类成员");
        }
        else
        {
            table.delClass_member(cDex, mDex);
            showObject_members(cNum);
        }
    }
}

void MainWindow::on_actionDel_all_triggered()
{
    if(table.getTotalNumber() == 0)
    {
        QMessageBox::warning(this, "删除失败", "当前没有可删除的类");
    }
    else
    {
        int ret = QMessageBox::question(this, "删除所有类", "确定要删除所有类吗？",
                                        QMessageBox::Yes, QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            for(int i=table.getTotalNumber() - 1; i>=0; --i)
                table.delClass(i);
        }
        showObjects();
    }
}

void MainWindow::on_actionDel_member_all_triggered()
{
    Object_memberDialog omDialog(this);
    //输入特定类成员编号
    int ret_1 = omDialog.exec();
    if(ret_1 == QDialog::Accepted)
    {
        int Cnum = omDialog.number();
        int Cdex = table.getDexFromNumber(Cnum);
        if(Cdex == -1)
        {
            QMessageBox::information(this, "删除失败", "没有找到该类");
        }
        else
        {
            QString Cname = table.getClassName(Cnum);
            int ret_2 = QMessageBox::question(this, "删除所有类成员", QString("确定要删除类 %1 的所有类成员吗？").arg(Cname),
                                              QMessageBox::Yes, QMessageBox::No);
            if(ret_2 == QMessageBox::Yes)
            {
                for(int i=table.getTotalMemberNumber(Cnum) - 1; i>=0; --i)
                {
                    table.delClass_member(Cdex, i);
                }
            }
            showObject_members(Cnum);
        }
    }
}

void MainWindow::on_actionDeleteSelected_triggered()
{
    QModelIndexList selection = ui->showClassTableView->selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        QMessageBox::information(this, "提示", "请先选择要删除的项");
        return;
    }

    // 按行号从大到小删除，避免索引错乱
    // Set可以每行只被添加一次
    QSet<int> rowsSet;
    for (int i = 0; i < selection.size(); ++i)
    {
        const QModelIndex &index = selection.at(i);
        rowsSet.insert(index.row());
    }
    //转换为List
    QList<int> rows = rowsSet.toList();
    std::sort(rows.begin(), rows.end(), std::greater<int>());

    if (curTable == 1)
    {
        int ret = QMessageBox::question(this, "确认删除",
            QString("确定要删除选中的 %1 个类吗？").arg(rows.size()),
            QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            foreach (int row, rows)
            {
                table.delClass(row);
            }
            showObjects();
        }
    }
    else if (curTable == 2)
    {
        int ret = QMessageBox::question(this, "确认删除",
            QString("确定要删除选中的 %1 个类成员吗？").arg(rows.size()),
            QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            int cDex = table.getDexFromNumber(curClass);
            foreach (int row, rows)
            {
                table.delClass_member(cDex, row);
            }
            showObject_members(curClass);
        }
    }
    else if (curTable == 4)
    {
        int ret = QMessageBox::question(this, "确认删除",
            QString("确定要删除选中的 %1 个类实例吗?").arg(rows.size()),
            QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            foreach (int row, rows)
            {
                table.delClass_instance(row);
            }
            on_ShowObInstance_triggered();
        }
    }
}

void MainWindow::on_actionFind_triggered()
{
    FindDialog findDialog(table, this);
    int ret = findDialog.exec();
    if(ret == QDialog::Accepted)
    {
        int dex = findDialog.getFindIndex();
        switch (dex)
        {
        case 0:
        {
            int number = findDialog.Number();
            int cDex = table.getDexFromNumber(number);
            if (cDex != -1)
            {
                // 切换到类表视图
                showObjects();

                highlight(cDex);
             }
             else
             {
                 QMessageBox::information(this, "查找结果", "未找到指定编号的类");
             }
             break;
        }
        case 1:
        {
            auto results = findDialog.getResults();  // 获取所有匹配结果
            if (results.isEmpty())
            {
                QMessageBox::information(this, "查找结果", "未找到指定编号的类成员");
            }
            else
            {
                showFindOb_members(results);
            }
            break;
        }
        case 2:
        {
            QString cName = findDialog.Name();
            int number = table.getCNumFromCName(cName);
            int cDex = table.getDexFromNumber(number);
            if (cDex != -1)
            {
                // 切换到类表视图
                showObjects();

                highlight(cDex);
             }
             else
             {
                 QMessageBox::information(this, "查找结果", "未找到指定名字的类");
             }
             break;
        }
        case 3:
        {
            QString mName = findDialog.Name();
            int cNum = table.getCNumFromMName(mName);
            if(cNum != -1)
            {
                showObject_members(cNum);

                int mNum =table.getMNumFromMName(mName);
                int mDex = table.getDexFromMember_number(cNum, mNum);
                highlight(mDex);
            }
            else
            {
                QMessageBox::information(this, "查找结果", "未找到指定名字的类成员");
            }
            break;
        }
        default:
            break;
        }
    }
}


void MainWindow::showFindOb_members(QList<QPair<int, int>> &results)
{
    curTable = 3;
    objectsTableModel->clear();
    objectsTableModel->setColumnCount(6);//设置六列

    //显示表头文本
    QStringList tempList;
    tempList.append("成员编号");
    tempList.append("成员名");
    tempList.append("数据类型");
    tempList.append("字节数");
    tempList.append("可访问性");
    tempList.append("所处类");
    objectsTableModel->setHorizontalHeaderLabels(tempList);

    for (const auto& pair : results)
    {
        int row = objectsTableModel->rowCount();
        int cNum = pair.first;    // 类编号
        int mDex = pair.second;  // 成员索引
        object_member member = table.getOb_member(cNum, mDex);
        QString cName = table.getClassName(cNum);  // 获取类名

        // 填充每列数据
        objectsTableModel->setItem(row, 0, new QStandardItem(QString::number(member.member_number)));
        objectsTableModel->setItem(row, 1, new QStandardItem(member.member_name));
        objectsTableModel->setItem(row, 2, new QStandardItem(member.member_type));
        objectsTableModel->setItem(row, 3, new QStandardItem(QString::number(member.byte_number)));
        objectsTableModel->setItem(row, 4, new QStandardItem(member.member_access));
        objectsTableModel->setItem(row, 5, new QStandardItem(cName));
    }

        // 设置所有列只读
        ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate(this);
        for (int i = 0; i < 6; ++i)
        {
            ui->showClassTableView->setItemDelegateForColumn(i, readOnlyDelegate);
        }

        // 显示结果数量
        ui->labelClass->show();
        ui->labelClass->setText(QString("找到 %1 个匹配的类成员").arg(results.size()));
}

void MainWindow::highlight(int dex)
{
    // 定位到目标行
    QModelIndex index = objectsTableModel->index(dex, 0);
    ui->showClassTableView->scrollTo(index, QAbstractItemView::PositionAtCenter); // 滚动到可见区域
    ui->showClassTableView->setCurrentIndex(index); // 设置当前选中项

    //设置临时高亮颜色
    QPalette palette = ui->showClassTableView->palette();
    palette.setColor(QPalette::Highlight, Qt::yellow);
    palette.setColor(QPalette::HighlightedText, Qt::black);
    ui->showClassTableView->setPalette(palette);
}

void MainWindow::on_actionShowClassSize_triggered()
{
    Object_memberDialog dialog(this);
    dialog.setWindowTitle("查询类占用信息");
    int ret = dialog.exec();
    if (ret == QDialog::Accepted)
    {
        int classNumber = dialog.number();
        int dex = table.getDexFromNumber(classNumber);
        if (dex == -1)
        {
            QMessageBox::information(this, "提示", "未找到该类");
            return;
        }

        // 获取类名和总占用大小
        QString className = table.getClassName(classNumber);
        int totalSize = table.getClassTotalSize(classNumber);

        // 显示结果
        QMessageBox::information(this, "类占用信息",
            QString("类名：%1\n类编号：%2\n总占用字节数：%3 字节")
            .arg(className)
            .arg(classNumber)
            .arg(totalSize));
    }
}

void MainWindow::on_AddObInstance_triggered()
{
    CreateInstanceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString Cname = dialog.cName();
        QString instanceName = dialog.iName();

        // 调用classTable添加实例
        if (table.addObjectInstance(Cname, instanceName))
        {
            on_ShowObInstance_triggered();
        }
        else
        {
            QMessageBox::information(this, "失败", "类不存在或实例名重复");
        }
    }
}

void MainWindow::on_ShowObInstance_triggered()
{
    curTable = 4;
    objectsTableModel->clear();
    objectsTableModel->setColumnCount(3);
    objectsTableModel->setHorizontalHeaderLabels({"所属类名", "所属类编号", "实例名", "所占字节数"});

    //填充数据
    QVector<ObjectInstance> instances = table.getObjectInstances();
    for (int i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];
        objectsTableModel->setItem(i, 0, new QStandardItem(table.getClassName(instance.classNumber)));
        objectsTableModel->setItem(i, 1, new QStandardItem(QString::number(instance.classNumber)));
        objectsTableModel->setItem(i, 2, new QStandardItem(instance.instanceName));
        objectsTableModel->setItem(i, 3, new QStandardItem(QString::number(instance.instanceTotalByte)));
     }

    //在右下角label显示统计信息
    int totalCount = instances.size();
    int totalBytes = table.getTotalInstanceBytes();
    ui->labelClass->setText(
        QString("对象总数：%1 个 | 总字节数：%2 字节")
        .arg(totalCount).arg(totalBytes)
    );
    ui->labelClass->show();

    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate(this);
    ui->showClassTableView->setItemDelegateForColumn(0, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(1, readOnlyDelegate);
    ui->showClassTableView->setItemDelegateForColumn(3, readOnlyDelegate);

}
