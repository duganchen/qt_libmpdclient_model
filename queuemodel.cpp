#include "queuemodel.h"

#include "mpdclient/connection.h"
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

    return QString(m_songs[index.row()]->get_tag(MPD_TAG_TITLE, 0));
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
    if (!m_mpd) {
        return;
    }

    beginResetModel();
    m_songs = m_mpd.list_queue_meta();
    endResetModel();
    if (m_mpd.get_error() == MPD_ERROR_CLOSED) {
        emit mpdClosed();
    }
}