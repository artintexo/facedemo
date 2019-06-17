#pragma once

#include "facelib/facexport.h"
#include "facelib/nodedata.h"

#include <QList>
#include <QVariant>

namespace facelib {

class FACEXPORT NodeItem {
public:
    NodeItem(const NodeData &data, NodeItem *parent = nullptr);
    ~NodeItem();

    const NodeData &data() const;
    NodeItem *parent() const;
    const QList<NodeItem *> childs() const;
    void append(NodeItem *child);
    int row() const;
    void update(const QList<FaceData> &list);

private:
    NodeData data_;
    NodeItem *parent_;
    QList<NodeItem *> childs_;
};

}
