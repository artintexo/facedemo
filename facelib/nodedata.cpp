#include "facelib/nodedata.h"

namespace facelib {

FaceData::FaceData(int age, const QString &gender, const QRect &rect)
    : age(age), gender(gender), rect(rect)
{
}

NodeData::NodeData(const QString &name, const QString &path, bool isDir, bool isFile)
    : name(name), path(path), isDir(isDir), isFile(isFile)
{
}

}
