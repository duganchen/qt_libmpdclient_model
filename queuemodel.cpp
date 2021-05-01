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

void QueueModel::clear()
{
    if (m_songs.empty()) {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, m_songs.size() - 1);
    m_songs.clear();
    endRemoveRows();
}

void QueueModel::refresh() {

    beginResetModel();
    m_songs = m_mpd.listQueueMeta();
    endResetModel();
    emit mpdError(m_mpd.getError());
}