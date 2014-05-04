#include "listmodelbase.h"

ListModelBase::ListModelBase(const SoundStorage* storage, QObject *parent) : QAbstractListModel(parent), soundStorage(storage) {}

Sound ListModelBase::getItem(const QModelIndex& index) const {
    return Sound();
}

// --- public methods
QVariant ListModelBase::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

QVariant ListModelBase::data(const QModelIndex &index, int role) const {
    return QVariant();
}

int ListModelBase::rowCount(const QModelIndex&) const {
    return itemIds.count();
}
