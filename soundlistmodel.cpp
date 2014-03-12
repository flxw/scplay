# include "soundlistmodel.h"

# include <QFile>
# include <QJsonDocument>
# include <QJsonArray>
# include <QJsonObject>

SoundListModel::SoundListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    parseJson();
}


QVariant SoundListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= soundItems.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        SoundListItem sound = soundItems.at(index.row());
        return sound.getTitle();
    } else {
        return QVariant();
    }
}

QVariant SoundListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return QVariant();
}

int SoundListModel::rowCount(const QModelIndex &parent) const {
    return soundItems.count();
}

// --- private functions
void SoundListModel::parseJson() {
    QFile fewlikes(":/fewlikes.json");

    if (!fewlikes.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString json = fewlikes.readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    QJsonArray jsonArray = jsonDocument.array();
    QJsonObject songObject;

    for (QJsonArray::const_iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
        SoundListItem listItem;

        songObject = (*it).toObject();
        listItem.setId(songObject["id"].toInt());
        listItem.setTitle(songObject["title"].toString());
        listItem.setUser(songObject["user"].toObject()["username"].toString());

        soundItems.append(listItem);
    }
}
