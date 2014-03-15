# include "listitem.h"

ListItem::ListItem() {}

// --- public functions
const QPixmap &ListItem::getArtwork() const {
    return artwork;
}

void ListItem::setArtwork(QPixmap &p) {
    artwork = p;
}

bool ListItem::hasArtwork() const {
    return !artwork.isNull();
}

const QUrl& ListItem::getArtworkUrl() const {
    return artworkUrl;
}

void ListItem::setArtworkUrl(QString url) {
    artworkUrl = url;
}

const QString& ListItem::getTitle() const {
    return title;
}

void ListItem::setTitle(QString t) {
    title = t;
}

const QString& ListItem::getUser() const {
    return user;
}

void ListItem::setUser(QString u) {
    user =u;
}

int ListItem::getId() const {
    return id;
}

void ListItem::setId(int i) {
    id = i;
}
