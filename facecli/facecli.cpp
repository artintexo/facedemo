#include "facecli.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QStringList>

FaceCli::FaceCli()
    : QObject(nullptr)
{
    faceObject_ = new facelib::FaceObject(this);

    connect(this, &FaceCli::quit, qApp, &QCoreApplication::quit, Qt::QueuedConnection);
    connect(faceObject_, &facelib::FaceObject::detected, this, &FaceCli::detected);
    connect(faceObject_, &facelib::FaceObject::finished, qApp, &QCoreApplication::quit);
}

FaceCli::~FaceCli()
{
    delete faceObject_;
}

void FaceCli::Run()
{
    const QStringList &args = qApp->arguments();

    if (args.count() < 3) {
        qWarning() << "Usage: facecli [token] [path 1] ... [path N]";
        qWarning() << "Example:\n\t facecli K3EHVe7cdEyagrfVb8Z4 image1.jpeg ../ImageFolder image2.jpeg";
        qWarning() << "";
        emit quit();
    } else {
        faceObject_->setToken(args[1]);
        faceObject_->createModel(args.mid(2));
        faceObject_->startDetect();
    }
}

void FaceCli::detected(const QString &path, const QByteArray &json)
{
    if (json.count() > 0) {
        QJsonDocument data = QJsonDocument::fromJson(json);
        qDebug() << path << ":";
        QFile output;
        output.open(1, QIODevice::WriteOnly);
        output.write(data.toJson(QJsonDocument::Indented));
    }
}
