#ifndef OBJECTINSTANCE_H
#define OBJECTINSTANCE_H

#include "QString"

class ObjectInstance
{
public:
    ObjectInstance();
    ObjectInstance(int classNumber, const QString& instanceName, int totalByte);

    int classNumber;
    QString instanceName;
    int instanceTotalByte;
};

#endif // OBJECTINSTANCE_H
