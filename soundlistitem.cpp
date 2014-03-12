# include "soundlistitem.h"

SoundListItem::SoundListItem()
{
}

// --- public functions
QString SoundListItem::getTitle() const {
    return title;
}

void SoundListItem::setTitle(QString t) {
    title = t;
}

QString SoundListItem::getUser() const {
    return user;
}

void SoundListItem::setUser(QString u) {
    user = u;
}

int SoundListItem::getId() const {
    return id;
}

void SoundListItem::setId(int i) {
    id = i;
}
