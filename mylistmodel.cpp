# include "mylistmodel.h"

MyListModel::MyListModel(QObject *parent) : QAbstractListModel(parent) {}

QVariant MyListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

int MyListModel::rowCount(const QModelIndex &parent) const {
    return dataIds.count();
}

// --- protected slots
void MyListModel::updateModel(QList<int> newItems) {
    dataIds = newItems;

    emit dataChanged(QModelIndex(), QModelIndex());
}
