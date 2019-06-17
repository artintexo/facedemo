#pragma once

#include <QWidget>

namespace facelib {
class FaceObject;
}

class GraphicsView;
class QGraphicsScene;
class QGroupBox;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QTreeView;

class FaceGui : public QWidget {
    Q_OBJECT
public:
    FaceGui();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void slotStartBtnClicked();
    void slotSelectBtnClicked();
    void slotCheckStartCondition();
    void slotProgress(int value);
    void slotFinished();
    void slotTreeViewClicked(const QModelIndex &index);

private:
    QPushButton *startBtn_;
    QProgressBar *progressBar_;
    QLineEdit *tokenEdit_;
    QPushButton *selectBtn_;
    QTreeView *treeView_;
    QGraphicsScene *graphicsScene_;
    GraphicsView *graphicsView_;
    facelib::FaceObject *faceObject_;
};
