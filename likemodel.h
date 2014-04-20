#ifndef LIKEMODEL_H
#define LIKEMODEL_H

# include "listmodelbase.h"
# include "sound.h"
# include "soundstorage.h"

class LikeModel : public ListModelBase
{
    Q_OBJECT

public:
    explicit LikeModel(const SoundStorage *storage, QObject *parent = 0);

    Sound getItem(const QModelIndex& index) const;

public slots:
    void updateLikes(QList<int> newLikeIds);
};


#endif // LIKEMODEL_H
