#pragma once

#include "facelib/nodedata.h"
#include "facelib/nodeitem.h"

#include <QAbstractItemModel>
#include <QStringList>

namespace facelib {

class NodeModel : public QAbstractItemModel {
public:
    NodeModel(const QStringList &paths, QObject *parent = nullptr);
    ~NodeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QStringList getSortedPaths() const;
    void updateItem(const QString &path, const QList<FaceData> &list);

private:
    void setupModel(const QStringList &paths);
    void loadData(const QString &path, NodeItem *parent);
    bool isJpeg(const QString &path) const;
    void populateList(NodeItem *item, QStringList &list) const;
    NodeItem *lookup(const QString &path, NodeItem *parent) const;
    NodeItem *rootItem;
};

}
