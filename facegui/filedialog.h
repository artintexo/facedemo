#pragma once

#include <QFileDialog>

class FileDialog : public QFileDialog {
    Q_OBJECT
public:
    FileDialog(QWidget *parent = nullptr);
    QStringList paths() const;

protected:
    void accept() override;

private:
    QStringList paths_;
};
