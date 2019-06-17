#include "graphicsview.h"

#include <QScrollBar>
#include <QWheelEvent>

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setBackgroundBrush(QBrush(Qt::gray));
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *)
{
    QRectF bounds = scene()->itemsBoundingRect();
    fitInView2(bounds, Qt::KeepAspectRatio);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    const double factorIn = 1.1;
    const double factorOut = 0.9;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0)
        scale(factorIn, factorIn);
    else
        scale(factorOut, factorOut);
}

void GraphicsView::fitInView2(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode)
{
    if (!scene() || rect.isNull())
        return;

    auto unity = transform().mapRect(QRectF(0, 0, 1, 1));
    if (unity.isEmpty())
        return;

    scale(1 / unity.width(), 1 / unity.height());
    auto viewRect = viewport()->rect();
    if (viewRect.isEmpty())
        return;

    auto sceneRect = transform().mapRect(rect);
    if (sceneRect.isEmpty())
        return;

    qreal xratio = 0;
    qreal yratio = 0;
    QScrollBar *hscroll = horizontalScrollBar();
    QScrollBar *vscroll = verticalScrollBar();

    if (vscroll->isVisible())
        xratio = (viewRect.width() + vscroll->width()) / sceneRect.width();
    else
        xratio = viewRect.width() / sceneRect.width();

    if (hscroll->isVisible())
        yratio = (viewRect.height() + hscroll->height()) / sceneRect.height();
    else
        yratio = viewRect.height() / sceneRect.height();

    switch (aspectRatioMode) {
    case Qt::KeepAspectRatio:
        xratio = yratio = qMin(xratio, yratio);
        break;
    case Qt::KeepAspectRatioByExpanding:
        xratio = yratio = qMax(xratio, yratio);
        break;
    case Qt::IgnoreAspectRatio:
        break;
    }

    scale(xratio, yratio);
    centerOn(viewRect.center());
}
