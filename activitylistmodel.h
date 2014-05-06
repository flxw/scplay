#ifndef ACTIVITYLISTMODEL_H
#define ACTIVITYLISTMODEL_H

# include "listmodelbase.h"

class ActivityListModel : public ListModelBase
{
    Q_OBJECT

public:
    explicit ActivityListModel(const SoundStorage *storage, QObject *parent = 0);


    int   rowCount(const QModelIndex&) const;
    Sound getItem(const QModelIndex& index) const;

public slots:
    void updateActivities(QList< QPair<int,QString> > activities);

private:
    QList< QPair<int,QString> > idsAndTypes;
};

#endif // ACTIVITYLISTMODEL_H
