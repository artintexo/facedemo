#pragma once

#include "facelib/facexport.h"

#include <QObject>

class QAbstractItemModel;

namespace facelib {

class NodeModel;

class FACEXPORT FaceObject : public QObject {
    Q_OBJECT
public:
    FaceObject(QObject *parent = nullptr);
    ~FaceObject() override;

public:
    void setToken(const QString &token);
    QAbstractItemModel *model() const;
    void createModel(const QStringList &paths);
    void startDetect();
    void cancelDetect();

signals:
    void cancelled();
    void detected(const QString &, const QByteArray &);
    void progress(int);
    void finished();

private slots:
    void slotDetected(const QString &path, const QByteArray &json);
    void slotProgress(int value);
    void slotFinished();

private:
    QString token_;
    NodeModel *model_;
};

}
