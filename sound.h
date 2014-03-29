#ifndef SOUND_H
#define SOUND_H

# include <QString>
# include <QUrl>
# include <QPixmap>

class Sound
{
public:
    Sound();

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

#endif // SOUND_H
