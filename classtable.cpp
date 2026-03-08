 #include "classtable.h"
#include <QFile>
#include <QTextCodec>

classTable::classTable()
{
    totalNumber = 0;
}

bool classTable::ReadClassFromFile(const QString &ReadFileName)
{
    QFile file(ReadFileName);
    if(!file.exists())
        return false;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream read(&file);
    read.setCodec(QTextCodec::codecForName("system"));//显示文字

    //初始化
    objects.clear();
    totalNumber = 0;
    objectInstances.clear();

    //逐个读取类信息
    read >> totalNumber;
    object tempObject;
    if (totalNumber > 0)
    {
        for(int i=0; i<totalNumber; ++i)
        {
            tempObject.read_object(read);
            objects.push_back(tempObject);
        }
    }

    // 读取实例信息
    int instanceCount;
    read >> instanceCount;
    for(int i=0; i<instanceCount; ++i)
    {
        int classNum;
        QString name;
        int totalByte;
        read >> classNum >> name >> totalByte;
        objectInstances.append(ObjectInstance(classNum, name, totalByte));
    }

    file.close();
    return true;
}

bool classTable::SaveClassToFile(const QString &SaveFileName)
{
    QFile file(SaveFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
           return false;
    QTextStream save(&file);
    save.setCodec(QTextCodec::codecForName("system"));

    //逐个写入类信息
    save << totalNumber <<endl;
    if(totalNumber > 0)
    {
        for(int i=0; i<totalNumber; ++i)
        {
            objects[i].save_object(save);
        }
    }

    // 保存实例总数和实例信息
    save << objectInstances.size() << endl;
    for(const auto& instance : objectInstances)
    {
        save << instance.classNumber << "\t";
        save << instance.instanceName << "\t";
        save << instance.instanceTotalByte << endl;
    }

    file.close();
    return true;
}

void classTable::addClass(object &ob)
{
    objects.push_back(ob);
    totalNumber += 1;
}

void classTable::addClass_member(int num, object_member &ob_member)
{
    int dex = getDexFromNumber(num);
    objects[dex].member.push_back(ob_member);
    objects[dex].member_count += 1;
}

void classTable::delClass(int dex)
{
    objects.remove(dex);
    totalNumber -= 1;
}

void classTable::delClass_member(int cDex, int mDex)
{
    int classNumber = objects[cDex].number;
    objects[cDex].member.remove(mDex);
    objects[cDex].member_count -= 1;
    updateInstanceBytesForClass(classNumber);
}

void classTable::delClass_instance(int dex)
{
    objectInstances.remove(dex);
}

int classTable::getTotalNumber()
{
    totalNumber = objects.size();
    return totalNumber;
}

int classTable::getTotalMemberNumber(int num)
{
    for(int i=0; i<totalNumber; ++i)
    {
        if(objects[i].number == num)
            return objects[i].member_count;
    }
    return 0;
}

int classTable::getDexFromNumber(int num)
{
    for(int i=0; i<totalNumber; ++i)
    {
        if(objects[i].number == num)
            return i;
    }
    return -1;
}

int classTable::getDexFromMember_number(int cNum, int mNum)
{
    int dex = getDexFromNumber(cNum);
    if(dex != -1)
    {
        for(int i=0; i<objects[dex].member_count; ++i)
        {
            if(objects[dex].member[i].member_number == mNum)
                return i;
        }
        return -1;
    }
    return -1;
}

int classTable::getCnumFromMember_number(int mNum)
{
    for(int i=0; i<totalNumber; ++i)
    {
        for(int j=0; j<objects[i].member_count; ++j)
        {
            if(objects[i].member[j].member_number == mNum)
                return objects[i].number;
        }
    }
    return -1;
}

int classTable::getCNumFromCName(QString cName)
{
    for(int i=0; i<totalNumber; ++i)
    {
        if(objects[i].name == cName)
            return objects[i].number;
    }
    return -1;
}

int classTable::getCNumFromMName(QString mName)
{
    for(int i=0; i<totalNumber; ++i)
    {
        for(int j=0; j<objects[i].member_count; ++j)
        {
            if(objects[i].member[j].member_name == mName)
                return objects[i].number;
        }
    }
    return -1;
}

int classTable::getMNumFromMName(QString mName)
{
    for(int i=0; i<totalNumber; ++i)
    {
        for(int j=0; j<objects[i].member_count; ++j)
        {
            if(objects[i].member[j].member_name == mName)
                return objects[i].member[j].member_number;
        }
    }
    return -1;
}

QString classTable::getClassName(int num)
{
    for(int i=0; i<totalNumber; ++i)
    {
        if(objects[i].number == num)
            return objects[i].name;
    }
    return "NULL";
}

object_member &classTable::getOb_member(int num, int dex)
{
    int i =getDexFromNumber(num);
    return objects[i].member[dex];
}

object &classTable::getObject(int dex)
{
    return objects[dex];
}

int classTable::getClassTotalSize(int Cnum)
{
    int dex = getDexFromNumber(Cnum);
    if (dex == -1)
    {
        return -1;
    }
    int baseSize = 0;
    QString baseName = objects[dex].base_name;
    if (!baseName.isEmpty())
    {
        int baseNum = getCNumFromCName(baseName);
        if (baseNum != -1)
        {
            baseSize = getClassTotalSize(baseNum);
        }
    }
    int totalSize = baseSize;
    const int alignment = 4; // 假设4字节对齐
    for (int i = 0; i < objects[dex].member_count; ++i)
    {
        int memberSize = objects[dex].member[i].byte_number;
        // 计算当前成员的对齐偏移（确保地址是alignment的倍数）
        int padding = (alignment - (totalSize % alignment)) % alignment;
        totalSize += padding + memberSize; // 加上对齐填充和成员大小
    }
    totalSize = (totalSize + alignment - 1) / alignment * alignment;
    return totalSize;
}

bool classTable::addObjectInstance(QString &cName, QString &iName)
{
    //检查关联的类是否存在
    int cNum = getCNumFromCName(cName);
    if (getDexFromNumber(cNum) == -1)
    {
        return false;
    }
    //检查同一类下是否有重复的实例名
    if (isInstanceNameExist(cName, iName))
    {
        return false;
    }
    //用已有方法计算该类的总字节数
    int classTotalByte = getClassTotalSize(cNum);
    //存储实例信息
    objectInstances.append(ObjectInstance(cNum, iName, classTotalByte));
    return true;
}

QVector<ObjectInstance> &classTable::getObjectInstances()
{
    return objectInstances;
}

bool classTable::isInstanceNameExist(QString &cName, QString& iName)
{
    int cNum = getCNumFromCName(cName);
    for (const auto& instance : objectInstances)
    {
        if (instance.classNumber == cNum && instance.instanceName == iName)
        {
            return true;
        }
    }
    return false;
}

int classTable::getTotalInstanceBytes() const
{
    int total = 0;
    for (const auto& instance : objectInstances)
    {
        total += instance.instanceTotalByte;
    }
    return total;
}

void classTable::updateInstanceBytesForClass(int cNumber)
{
    int newTotalByte = getClassTotalSize(cNumber);
    if (newTotalByte == -1) return;

    for (auto& instance : objectInstances)
    {
         if (instance.classNumber == cNumber)
         {
              instance.instanceTotalByte = newTotalByte;
         }
    }
}
