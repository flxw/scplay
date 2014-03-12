#ifndef SOUNDLISTITEM_H
#define SOUNDLISTITEM_H

# include <QString>

class SoundListItem
{
public:
    SoundListItem();

    QString getTitle();
    void setTitle(QString t);

    QString getUser();
    void setUser(QString u);

    int getId();
    void setId(int i);

    // --- attributes
public:
private:
    QString title;
    QString user;
    int id;
};

#endif // SOUNDLISTITEM_H
