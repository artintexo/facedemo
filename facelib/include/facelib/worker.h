#pragma once

#include <QMap>
#include <QObject>
#include <QStringList>

class QFile;
class QNetworkReply;

namespace facelib {

struct WorkerData {
    QString path;
    QFile *file;
    QNetworkReply *reply;
};

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(const QString &token, const QStringList &paths, QObject *parent = nullptr);

signals:
    void progress(int value);
    void detected(const QString &path, const QByteArray &json);
    void finished();

public slots:
    void slotStarted();
    void slotCancelled();

private slots:
    void slotFinished(QNetworkReply *reply);

private:
    QString token_;
    QStringList paths_;
    QMap<QNetworkReply *, WorkerData> pending_;
};

}
