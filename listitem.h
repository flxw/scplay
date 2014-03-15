#ifndef LISTITEM_H
#define LISTITEM_H

# include <QString>
# include <QUrl>
# include <QPixmap>

class ListItem
{
public:
    ListItem();

    const QPixmap& getArtwork() const;
    void setArtwork(QPixmap& p);
    bool hasArtwork() const;

    const QUrl& getArtworkUrl() const;
    void setArtworkUrl(QString url);

    const QString& getTitle() const;
    void setTitle(QString t);

    const QString& getUser() const;
    void setUser(QString u);

    int getId() const;
    void setId(int i);

    // --- attributes
protected:
    QPixmap artwork;
    QUrl artworkUrl;

    QString title;
    QString user;

    int id;
};

#endif // LISTITEM_H
