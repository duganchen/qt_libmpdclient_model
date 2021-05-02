#include "queuemodel.h"

#include "mpdclient/connection.h"
#include <mpd/client.h>

QueueModel::QueueModel(mpd::Connection &mpd, QObject *parent)
    : QAbstractListModel(parent)
    , m_mpd(mpd)
{}

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

void QueueModel::refresh()
{
    if (!m_mpd) {
        return;
    }

    auto songs = m_mpd.list_queue_meta();
    switch (m_mpd.get_error()) {
    case MPD_ERROR_SUCCESS:
        break;
    case MPD_ERROR_CLOSED:
        emit mpdClosed();
        return;
    default:
        emit m_mpd.get_error_message();
    };

    beginResetModel();
    m_songs = std::move(songs);
    endResetModel();
}

void QueueModel::onIdleQueue()
{
    if (!m_mpd) {
        return;
    }

    if (m_queueVersion == UINT_MAX) {
        return;
    }

    auto status = m_mpd.status();
    switch (m_mpd.get_error()) {
    case MPD_ERROR_SUCCESS:
        break;
    case MPD_ERROR_CLOSED:
        emit mpdClosed();
        return;
    default:
        emit m_mpd.get_error_message();
    };

    unsigned queueVersion = status->get_queue_version();

    if (queueVersion == m_queueVersion) {
        return;
    }

    auto changes = m_mpd.plchangesposid(m_queueVersion);
    switch (m_mpd.get_error()) {
    case MPD_ERROR_SUCCESS:
        break;
    case MPD_ERROR_CLOSED:
        emit mpdClosed();
        return;
    default:
        emit m_mpd.get_error_message();
    };

    m_queueVersion = queueVersion;

    unsigned queueLength = status->get_queue_length();
    if (queueLength < m_songs.size()) {
        beginRemoveRows(QModelIndex(), queueLength, m_songs.size());
        m_songs.resize(queueLength);
        endRemoveRows();
    }

    // Update the UI one row at a time, as we did in the Python version.
    // Optimize it if you want to...
    for (auto &change : changes) {
        auto song = m_mpd.get_queue_song_id(change.id);
        mpd_error error = m_mpd.get_error();
        if (MPD_ERROR_CLOSED == error) {
            emit mpdClosed();
            break;
        } else if (MPD_ERROR_SUCCESS != error) {
            emit errorMessage(m_mpd.get_error_message());
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