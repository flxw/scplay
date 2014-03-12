#ifndef LIKELISTMODEL_H
#define LIKELISTMODEL_H

# include "soundlistmodel.h"

class LikeListModel : public SoundListModel
{
public:
    LikeListModel(QObject *parent);

protected:
    void fillModel();

protected slots:
    void updateModel();

};

#endif // LIKELISTMODEL_H
