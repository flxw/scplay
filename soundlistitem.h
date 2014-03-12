#ifndef SOUNDLISTITEM_H
#define SOUNDLISTITEM_H

# include <QString>

class SoundListItem
{
public:
    SoundListItem();

    QString getTitle() const;
    void setTitle(QString t);

    QString getUser() const;
    void setUser(QString u);

    int getId() const;
    void setId(int i);

    // --- attributes
public:
private:
    QString title;
    QString user;
    int id;
};

#endif // SOUNDLISTITEM_H
