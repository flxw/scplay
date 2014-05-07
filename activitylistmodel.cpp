#include "activitylistmodel.h"

ActivityListModel::ActivityListModel(const SoundStorage *storage, QObject *parent)
                  : ListModelBase(storage,parent) {
    connect(soundStorage, SIGNAL(activitiesUpdated(QList<QPair<int,QString> >)),
            this, SLOT(updateActivities(QList<QPair<int,QString> >)));
}

Sound ActivityListModel::getItem(const QModelIndex& index) const {
    int idx = index.row();

    if (idx < idsAndTypes.size()) {
        if (isPlaylistSelected(idx)) {
            return soundStorage->getPlaylistById(idsAndTypes.at(idx).first);
        } else {
            return soundStorage->getSoundById(idsAndTypes.at(idx).first);
        }
    }
}

int ActivityListModel::rowCount(const QModelIndex&) const {
    return idsAndTypes.count();
}

bool ActivityListModel::isPlaylistSelected(const int &idx) const {
    if (idsAndTypes.at(idx).second == QString("track")) {
        return false;
    } else if (idsAndTypes.at(idx).second == QString("playlist")) {
        return true;
    }
}

void ActivityListModel::updateActivities(QList< QPair<int, QString> > activities) {
    idsAndTypes = activities;

    emit dataChanged(QModelIndex(), QModelIndex());
}
