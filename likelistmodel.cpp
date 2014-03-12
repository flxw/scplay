# include "likelistmodel.h"

# include <QFile>
# include <QJsonDocument>
# include <QJsonArray>
# include <QJsonObject>

LikeListModel::LikeListModel(QObject *parent) : SoundListModel(parent)
{
    fillModel();
}

// --- protected functions
void LikeListModel::fillModel() {
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

void LikeListModel::updateModel() {
}
