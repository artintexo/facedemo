#include "filedialog.h"

#include <QListView>
#include <QTreeView>

FileDialog::FileDialog(QWidget *parent)
    : QFileDialog(parent, "Choose directories and files")
{
    setOption(QFileDialog::DontUseNativeDialog, true);
    setMimeTypeFilters(QStringList() << "image/jpeg");

    QListView *listView = findChild<QListView *>("listView");
    if (listView)
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QTreeView *treeView = findChild<QTreeView *>("treeView");
    if (treeView)
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

QStringList FileDialog::paths() const
{
    return paths_;
}

void FileDialog::accept()
{
    paths_ = selectedFiles();
    emit filesSelected(paths_);
    QDialog::accept();
}
