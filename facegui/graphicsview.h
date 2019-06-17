#pragma once

#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsView(QGraphicsScene *scene = nullptr, QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    void fitInView2(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode);
};
