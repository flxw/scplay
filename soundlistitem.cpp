# include "soundlistitem.h"

SoundListItem::SoundListItem()
{
}

// --- public functions
QString SoundListItem::getTitle() {
    return title;
}

void SoundListItem::setTitle(QString t) {
    title = t;
}

QString SoundListItem::getUser() {
    return user;
}

void SoundListItem::setUser(QString u) {
    user = u;
}

int SoundListItem::getId() {
    return id;
}

void SoundListItem::setId(int i) {
    id = i;
}
