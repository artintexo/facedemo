#include "facelib/nodeitem.h"

#include <QStringList>

namespace facelib {

NodeItem::NodeItem(const NodeData &data, NodeItem *parent)
    : data_(data), parent_(parent)
{
}

NodeItem::~NodeItem()
{
    qDeleteAll(childs_);
}

const NodeData &NodeItem::data() const
{
    return data_;
}

NodeItem *NodeItem::parent() const
{
    return parent_;
}

const QList<NodeItem *> NodeItem::childs() const
{
    return childs_;
}

void NodeItem::append(NodeItem *item)
{
    childs_.append(item);
}

int NodeItem::row() const
{
    if (parent_)
        return parent_->childs_.indexOf(const_cast<NodeItem *>(this));

    return 0;
}

void NodeItem::update(const QList<FaceData> &list)
{
    data_.list = list;
}

}
