#ifndef LIKELISTMODEL_H
#define LIKELISTMODEL_H

# include "mylistmodel.h"
# include "sounditem.h"

class LikeListModel : public MyListModel
{
public:
    explicit LikeListModel(QObject *parent);

    ListItem getItem(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // LIKELISTMODEL_H
