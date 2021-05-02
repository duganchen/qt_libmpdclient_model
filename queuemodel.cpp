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

    m_queueVersion = UINT_MAX;
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

void QueueModel::onQueueVersion(unsigned queueVersion) {
    if (m_queueVersion == queueVersion) {
        return;
    }

    m_queueVersion = queueVersion;

    if (!m_mpd) {
        return;
    }

    auto status = m_mpd.status();
    if (m_mpd.get_error() == MPD_ERROR_CLOSED) {
        emit mpdClosed();
        return;
    }

    auto queueLength = status->get_queue_length();
    if (queueLength < m_songs.size()) {
        m_songs.resize(queueLength);
    }

    auto changes = m_mpd.plchangesposid(queueVersion);
    if (m_mpd.get_error() == MPD_ERROR_CLOSED) {
        emit mpdClosed();
        return;
    }

    // Update the UI one row at a time, as we did in the Python version.
    // Optimize it if you want to...
    for (auto &change: changes) {
        auto song = m_mpd.get_queue_song_id(change.id);

        if (!song && m_mpd.get_error() == MPD_ERROR_CLOSED) {
            emit mpdClosed();
            break;
        }

        if (change.position < m_songs.size()) {
            m_songs[change.position] = std::move(song);
            auto changeIndex = index(change.position, 0);
            emit dataChanged(changeIndex, changeIndex);
        } else {
            beginInsertRows(QModelIndex(), m_songs.size(), m_songs.size());
            m_songs.push_back(std::move(song));
            endInsertRows();
        }
    }
}