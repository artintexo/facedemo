#pragma once

#include "facelib/facexport.h"

#include <QList>
#include <QRect>
#include <QString>

namespace facelib {

struct FACEXPORT FaceData {
    FaceData(int age = 0, const QString &gender = "", const QRect &rect = QRect());
    int age;
    QString gender;
    QRect rect;
};

struct FACEXPORT NodeData {
    NodeData(const QString &name = "", const QString &path = "", bool isDir = false, bool isFile = false);
    QString name;
    QString path;
    bool isDir;
    bool isFile;
    QList<FaceData> list;
};

}
