#include "activitylistmodel.h"

ActivityListModel::ActivityListModel(const SoundStorage *storage, QObject *parent)
                  : ListModelBase(storage,parent) {
    connect(soundStorage, SIGNAL(activitiesUpdated(QList<QPair<int,QString> >)),
            this, SLOT(updateActivities(QList<QPair<int,QString> >)));
}


int ActivityListModel::rowCount(const QModelIndex&) const {
    return idsAndTypes.count();
}

Sound ActivityListModel::getItem(const QModelIndex& index) const {
    int idx = index.row();

    if (idx < idsAndTypes.size()) {
        if (idsAndTypes.at(idx).second == QString("track")) {
            return soundStorage->getSoundById(idsAndTypes.at(idx).first);
        } else if (idsAndTypes.at(idx).second == QString("playlist")) {
            return soundStorage->getPlaylistById(idsAndTypes.at(idx).first);
        }
    }
}

void ActivityListModel::updateActivities(QList< QPair<int, QString> > activities) {
    idsAndTypes = activities;

    emit dataChanged(QModelIndex(), QModelIndex());
}
