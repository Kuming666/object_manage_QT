#include "objectinstance.h"

ObjectInstance::ObjectInstance()
{

}

ObjectInstance::ObjectInstance(int classNumber, const QString &instanceName, int totalByte)
    : classNumber(classNumber), instanceName(instanceName), instanceTotalByte(totalByte)
{

}
