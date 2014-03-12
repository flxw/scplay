#ifndef LIKELISTMODEL_H
#define LIKELISTMODEL_H

# include "soundlistmodel.h"
# include "soundlistitem.h"

class LikeListModel : public SoundListModel
{
public:
    LikeListModel(QObject *parent);

protected:
    void fillModel();

protected slots:
    void updateModel(QList<SoundListItem> newItems);

};

#endif // LIKELISTMODEL_H
