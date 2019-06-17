#include "facelib/nodemodel.h"
#include "facelib/nodedata.h"
#include "facelib/nodeitem.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImageReader>

namespace facelib {

NodeModel::NodeModel(const QStringList &paths, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new NodeItem(NodeData("Name"));
    setupModel(paths);
}

NodeModel::~NodeModel()
{
    delete rootItem;
}

int NodeModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QStringList NodeModel::getSortedPaths() const
{
    QStringList list;
    populateList(rootItem, list);
    list.sort();
    return list;
}

void NodeModel::updateItem(const QString &path, const QList<FaceData> &list)
{
    NodeItem *item = lookup(path, rootItem);

    if (item) {
        item->update(list);
        emit dataChanged(QModelIndex(), QModelIndex());
    } else {
        qWarning() << "model has no item with path:" << path;
    }
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() != 0)
        return QVariant();

    NodeItem *item = static_cast<NodeItem *>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
        return item->data().name;
    case Qt::ToolTipRole:
        return item->data().path;
    default:
        return QVariant();
    }
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    NodeItem *item = static_cast<NodeItem *>(index.internalPointer());

    if (item->data().isDir)
        return Qt::ItemIsEnabled;
    else if (item->data().isFile && item->data().list.empty() == false)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::NoItemFlags;
}

QVariant NodeModel::headerData(int, Qt::Orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return rootItem->data().name;
    default:
        return QVariant();
    }
}

QModelIndex NodeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    NodeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NodeItem *>(parent.internalPointer());

    NodeItem *childItem = parentItem->childs().value(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex NodeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    NodeItem *childItem = static_cast<NodeItem *>(index.internalPointer());
    NodeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    NodeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NodeItem *>(parent.internalPointer());

    return parentItem->childs().count();
}

void NodeModel::setupModel(const QStringList &paths)
{
    for (const QString &path : paths)
        loadData(path, rootItem);
}

void NodeModel::loadData(const QString &path, NodeItem *parent)
{
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        if (fileInfo.isDir()) {
            NodeData data(fileInfo.fileName(), fileInfo.absoluteFilePath(), true, false);
            NodeItem *parentItem = new NodeItem(data, parent);
            parent->append(parentItem);

            QDir dir(fileInfo.absoluteFilePath());
            for (const QString &entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
                loadData(dir.filePath(entry), parentItem);
            for (const QString &entry : dir.entryList(QDir::Files))
                loadData(dir.filePath(entry), parentItem);
        } else if (fileInfo.isFile()) {
            if (isJpeg(fileInfo.absoluteFilePath()) == true) {
                NodeData data(fileInfo.fileName(), fileInfo.absoluteFilePath(), false, true);
                NodeItem *childItem = new NodeItem(data, parent);
                parent->append(childItem);
            }
        }
    } else {
        qWarning() << "not file or folder:" << fileInfo.absoluteFilePath();
    }
}

bool NodeModel::isJpeg(const QString &path) const
{
    QImageReader reader(path);
    QByteArray byteArray = reader.format();
    if (byteArray == "jpeg")
        return true;

    return false;
}

void NodeModel::populateList(NodeItem *item, QStringList &list) const
{
    for (NodeItem *item : item->childs()) {
        if (item->childs().count() > 0)
            populateList(item, list);
        else
            list.append(item->data().path);
    }
}

NodeItem *NodeModel::lookup(const QString &path, NodeItem *parent) const
{
    NodeItem *output = nullptr;

    for (NodeItem *item : parent->childs()) {
        if (item->childs().empty() == false)
            output = lookup(path, item);
        else if (item->data().path == path)
            output = item;

        if (output)
            return output;
    }

    return output;
}

}
