#include "facegui.h"
#include "filedialog.h"
#include "graphicsview.h"

#include "facelib/faceobject.h"
#include "facelib/nodedata.h"
#include "facelib/nodeitem.h"

#include <QBoxLayout>
#include <QDebug>
#include <QFile>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGroupBox>
#include <QLineEdit>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QTreeView>

#include <numeric>

FaceGui::FaceGui()
    : QWidget(nullptr)
{
    // start group
    startBtn_ = new QPushButton("Start");
    startBtn_->setEnabled(false);
    progressBar_ = new QProgressBar();

    QHBoxLayout *startLayout = new QHBoxLayout();
    startLayout->addWidget(startBtn_);
    startLayout->addWidget(progressBar_);

    QGroupBox *startGroup = new QGroupBox("Start face detection");
    startGroup->setLayout(startLayout);

    // token group
    tokenEdit_ = new QLineEdit();

    QFormLayout *tokenLayout = new QFormLayout();
    tokenLayout->addRow("Token:", tokenEdit_);

    QGroupBox *tokenGroup = new QGroupBox("Input token");
    tokenGroup->setLayout(tokenLayout);

    // select group
    selectBtn_ = new QPushButton("Select");
    treeView_ = new QTreeView();

    QVBoxLayout *selectLayout = new QVBoxLayout();
    selectLayout->addWidget(selectBtn_);
    selectLayout->addWidget(treeView_);

    QGroupBox *selectGroup = new QGroupBox("Select images");
    selectGroup->setLayout(selectLayout);

    // view group
    graphicsScene_ = new QGraphicsScene();
    graphicsView_ = new GraphicsView(graphicsScene_);

    QVBoxLayout *viewLayout = new QVBoxLayout();
    viewLayout->addWidget(graphicsView_);

    // face object
    faceObject_ = new facelib::FaceObject(this);

    // connections
    connect(startBtn_, &QPushButton::clicked, this, &FaceGui::slotStartBtnClicked);
    connect(tokenEdit_, &QLineEdit::textChanged, this, &FaceGui::slotCheckStartCondition);
    connect(selectBtn_, &QPushButton::clicked, this, &FaceGui::slotSelectBtnClicked);
    connect(treeView_, &QTreeView::clicked, this, &FaceGui::slotTreeViewClicked);

    connect(faceObject_, &facelib::FaceObject::progress, this, &FaceGui::slotProgress);
    connect(faceObject_, &facelib::FaceObject::finished, this, &FaceGui::slotFinished);

    // main
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(startGroup);
    rightLayout->addStrut(1);
    rightLayout->addWidget(tokenGroup);
    rightLayout->addStrut(1);
    rightLayout->addWidget(selectGroup);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(viewLayout, 7);
    mainLayout->addLayout(rightLayout, 3);

    setLayout(mainLayout);
    setWindowTitle("Face GUI");
    resize(1300, 700);
}

void FaceGui::resizeEvent(QResizeEvent *event)
{
    graphicsView_->fitInView2(graphicsScene_->itemsBoundingRect(), Qt::KeepAspectRatio);
    QWidget::resizeEvent(event);
}

void FaceGui::slotStartBtnClicked()
{
    if (startBtn_->text() == "Start") {
        startBtn_->setText("Cancel");
        progressBar_->reset();
        tokenEdit_->setEnabled(false);
        selectBtn_->setEnabled(false);
        faceObject_->startDetect();
    } else if (startBtn_->text() == "Cancel") {
        startBtn_->setEnabled(false);
        faceObject_->cancelDetect();
    }
}

void FaceGui::slotSelectBtnClicked()
{
    FileDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        QStringList paths = dialog.paths();
        faceObject_->createModel(paths);
        treeView_->setModel(faceObject_->model());
        treeView_->expandAll();
        progressBar_->reset();
        graphicsScene_->clear();
        slotCheckStartCondition();
    }
}

void FaceGui::slotCheckStartCondition()
{
    tokenEdit_->setText(tokenEdit_->text().trimmed());

    if (tokenEdit_->text().count() > 0)
        faceObject_->setToken(tokenEdit_->text());

    if (tokenEdit_->text().count() > 0 && treeView_->model() != nullptr)
        startBtn_->setEnabled(true);
    else
        startBtn_->setEnabled(false);
}

void FaceGui::slotProgress(int value)
{
    progressBar_->setValue(value);
}

void FaceGui::slotFinished()
{
    startBtn_->setText("Start");
    startBtn_->setEnabled(true);
    tokenEdit_->setEnabled(true);
    selectBtn_->setEnabled(true);
}

void FaceGui::slotTreeViewClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    facelib::NodeItem *item = static_cast<facelib::NodeItem *>(index.internalPointer());
    facelib::NodeData data = item->data();

    if (data.isFile && QFile::exists(data.path) && data.list.empty() == false) {
        // find font size
        QString maxString = "";
        int minRectWidth = std::numeric_limits<int>::max();
        int maxRectHeight = 0;
        for (auto &nodeData : data.list) {
            QString title = QString("%1: %2").arg(nodeData.gender).arg(nodeData.age);
            maxString = (maxString.count() > title.count()) ? maxString : title;
            minRectWidth = std::min(minRectWidth, nodeData.rect.width());
            maxRectHeight = std::max(maxRectHeight, nodeData.rect.height());
        }

        QFont font;
        int fontSize = maxRectHeight;
        font.setPixelSize(fontSize);
        QFontMetrics fontMetrics(font);
        while (fontMetrics.width(maxString) > minRectWidth) {
            fontSize -= 5;
            font.setPixelSize(fontSize);
            fontMetrics = QFontMetrics(font);
        }

        // draw items
        graphicsScene_->clear();
        QGraphicsPixmapItem *pixmapItem = graphicsScene_->addPixmap(QPixmap(data.path));

        QPen rectPen;
        rectPen.setBrush(QBrush(Qt::red));
        rectPen.setWidth(3);
        rectPen.setCosmetic(true);

        for (auto &faceData : data.list) {
            QString title = QString("%1: %2").arg(faceData.gender).arg(faceData.age);
            QPointF topLeft = pixmapItem->mapToScene(faceData.rect.x(), faceData.rect.y());
            QPointF bottomRight = pixmapItem->mapToScene(faceData.rect.x() + faceData.rect.width(), faceData.rect.y() + faceData.rect.height());

            QRectF rect(topLeft.x(), topLeft.y(), bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
            graphicsScene_->addRect(rect, rectPen);

            QPointF point(topLeft);
            QFontMetrics fontm(font);
            point.setY(point.y() - fontm.height() - 1);

            QPen textPen;
            textPen.setBrush(QBrush(Qt::red));
            textPen.setWidth(1);
            textPen.setCosmetic(true);

            QGraphicsSimpleTextItem *textItem = graphicsScene_->addSimpleText(title, font);
            textItem->setPos(point);
            textItem->setPen(textPen);
            textItem->setBrush(QBrush(Qt::red));
        }

        graphicsScene_->setSceneRect(graphicsScene_->itemsBoundingRect());
        graphicsView_->fitInView2(graphicsScene_->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}
