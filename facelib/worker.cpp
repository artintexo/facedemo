#include "facelib/worker.h"

#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QUrlQuery>

namespace facelib {

Worker::Worker(const QString &token, const QStringList &paths, QObject *parent)
    : QObject(parent), token_(token), paths_(paths)
{
}

void Worker::slotStarted()
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this, &Worker::slotFinished);
    connect(this, &Worker::finished, nam, &QNetworkAccessManager::deleteLater);

    for (const QString &path : paths_) {
        QFile *file = new QFile(path);
        if (!file->exists() || !file->open(QIODevice::ReadOnly)) {
            emit detected(path, QByteArray());
            continue;
        }

        QUrlQuery query;
        query.addQueryItem("fd_min_size", "0");
        query.addQueryItem("fd_max_size", "0");
        query.addQueryItem("fd_threshold", "0.8");
        query.addQueryItem("demographics", "true");

        QUrl url;
        url.setScheme("https");
        url.setHost("backend.facecloud.tevian.ru");
        url.setPath("/api/v1/detect");
        url.setQuery(query);

        QNetworkRequest req;
        req.setUrl(url);
        req.setRawHeader("Accept", "application/json");
        req.setRawHeader("Authorization", QString("Bearer %1").arg(token_).toUtf8());
        req.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
        req.setHeader(QNetworkRequest::ContentLengthHeader, file->size());

        QNetworkReply *reply = nam->post(req, file);

        WorkerData workerData;
        workerData.path = path;
        workerData.file = file;
        workerData.reply = reply;
        pending_[workerData.reply] = workerData;
    }
}

void Worker::slotCancelled()
{
    while (pending_.empty() == false) {
        pending_.first().file->close();
        pending_.first().reply->abort();
    }

    QEventLoop loop;
    loop.processEvents();
}

void Worker::slotFinished(QNetworkReply *reply)
{
    QString path = pending_[reply].path;

    if (reply->isReadable()) {
        if (reply->error() == QNetworkReply::NoError)
            emit detected(path, reply->readAll());
        else
            emit detected(path, QByteArray());
    }

    pending_[reply].file->deleteLater();
    pending_[reply].reply->deleteLater();
    pending_.remove(reply);

    if (pending_.empty() == false) {
        int progressValue = (paths_.count() - pending_.count()) * 100 / paths_.count();
        emit progress(progressValue);
    } else {
        emit progress(100);
        emit finished();
    }
}

}
