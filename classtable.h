#ifndef CLASSTABLE_H
#define CLASSTABLE_H

#include <QVector>

#include "object.h"
#include "objectinstance.h"

class classTable
{
public:
    classTable();

    bool ReadClassFromFile(const QString &ReadFileName);
    bool SaveClassToFile(const QString &SaveFileName);

    void addClass(object &ob);
    void addClass_member(int num, object_member &ob_member);

    void delClass(int dex);
    void delClass_member(int cDex, int mDex);
    void delClass_instance(int dex);

    int getTotalNumber();
    int getTotalMemberNumber(int num);

    int getDexFromNumber (int num);
    int getDexFromMember_number (int cNum, int mNum);

    int getCnumFromMember_number(int mNum);

    int getCNumFromCName(QString cName);
    int getCNumFromMName(QString mName);
    int getMNumFromMName(QString mName);

    int getClassTotalSize(int Cnum);

    QString getClassName(int num);

    object_member & getOb_member(int num, int dex);
    object & getObject(int dex);

    bool addObjectInstance(QString &cName, QString &iName);
    QVector<ObjectInstance> &getObjectInstances();
    bool isInstanceNameExist(QString &cName, QString &iname);
    int getTotalInstanceBytes() const;

    void updateInstanceBytesForClass(int cNumber);

private:
    int totalNumber;
    QVector<object> objects;
    QVector<ObjectInstance> objectInstances;
};

#endif // CLASSTABLE_H
