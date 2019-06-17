#include "facelib/faceobject.h"
#include "facelib/nodedata.h"
#include "facelib/nodeitem.h"
#include "facelib/nodemodel.h"
#include "facelib/worker.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

namespace facelib {

FaceObject::FaceObject(QObject *parent)
    : QObject(parent), token_(""), model_(nullptr)
{
}

FaceObject::~FaceObject()
{
    delete model_;
}

void FaceObject::setToken(const QString &token)
{
    token_ = token;
}

QAbstractItemModel *FaceObject::model() const
{
    return model_;
}

void FaceObject::createModel(const QStringList &paths)
{
    if (model_ != nullptr)
        delete model_;

    model_ = new NodeModel(paths, this);
}

void FaceObject::startDetect()
{
    QThread *thread_ = new QThread();
    Worker *worker_ = new Worker(token_, model_->getSortedPaths());
    worker_->moveToThread(thread_);

    connect(this, &FaceObject::cancelled, worker_, &Worker::slotCancelled);
    connect(worker_, &Worker::detected, this, &FaceObject::slotDetected);
    connect(worker_, &Worker::progress, this, &FaceObject::slotProgress);
    connect(worker_, &Worker::finished, this, &FaceObject::slotFinished);

    connect(thread_, &QThread::started, worker_, &Worker::slotStarted);
    connect(worker_, &Worker::finished, thread_, &QThread::quit);
    connect(worker_, &Worker::finished, worker_, &Worker::deleteLater);
    connect(thread_, &QThread::finished, thread_, &QThread::deleteLater);

    thread_->start();
}

void FaceObject::cancelDetect()
{
    emit cancelled();
}

void FaceObject::slotDetected(const QString &path, const QByteArray &json)
{
    if (json.count() > 0) {
        QJsonDocument doc = QJsonDocument::fromJson(json);
        int statusCode = doc.object().value("status_code").toInt(-1);
        if (statusCode == 200) {
            QList<FaceData> list;
            QJsonArray array = doc.object().value("data").toArray();
            for (const QJsonValue value : array) {
                QJsonValue bbox = value.toObject().value("bbox");
                QJsonValue demographics = value.toObject().value("demographics");
                FaceData faceData;
                faceData.age = static_cast<int>(demographics.toObject().value("age").toObject().value("mean").toDouble());
                faceData.gender = demographics.toObject().value("gender").toString();
                faceData.rect = QRect(bbox.toObject().value("x").toInt(),
                                      bbox.toObject().value("y").toInt(),
                                      bbox.toObject().value("width").toInt(),
                                      bbox.toObject().value("height").toInt());

                list.append(faceData);
            }

            if (list.empty() == false)
                model_->updateItem(path, list);
        }
    }

    emit detected(path, json);
}

void FaceObject::slotProgress(int value)
{
    emit progress(value);
}

void FaceObject::slotFinished()
{
    emit finished();
}

}
