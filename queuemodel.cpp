#include "queuemodel.h"

#include "connection.h"
#include <mpd/client.h>

QueueModel::QueueModel(mpd::Connection &mpd, QObject *parent)
    : QAbstractListModel(parent)
    , m_mpd(mpd)
{
}

int QueueModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_songs.size();
}

QVariant QueueModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (index.row() < 0) {
        return QVariant();
    }

    if (index.row() > m_songs.size() - 1) {
        return QVariant();
    }

    return QString(m_songs[index.row()]->getTag(MPD_TAG_TITLE, 0));
}

void QueueModel::setConnected(bool isConnected) {
    if (isConnected == m_isConnected) {
        return;
    }

    beginResetModel();

    if (isConnected && m_mpd) {
        m_songs = m_mpd.listQueueMeta();
        emit mpdError(m_mpd.getError());
    } else {
        m_songs.clear();
    }

    endResetModel();

    m_isConnected = isConnected;
}