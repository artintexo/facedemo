#pragma once

#include "facelib/faceobject.h"

#include <QObject>

class FaceCli : public QObject {
    Q_OBJECT
public:
    FaceCli();
    ~FaceCli() override;
    void Run();

signals:
    void quit();

private slots:
    void detected(const QString &path, const QByteArray &json);

private:
    facelib::FaceObject *faceObject_;
};
