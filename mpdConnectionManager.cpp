#include "mpdConnectionManager.h"
#include <mpd/client.h>

MPDConnectionManager::MPDConnectionManager(mpd::Connection &mpd, QObject *parent)
    : QObject(parent)
    , m_mpd(mpd)
{
}

void MPDConnectionManager::connectToMPD() {
    mpd::Connection connection(mpd_connection_new("localhost", 6600, 0));
    m_mpd = std::move(connection);
    setError(m_mpd.getError());
}

void MPDConnectionManager::setError(int mpdError) {
    if (m_mpdError == mpdError) {
        return;
    }

    if (!m_mpd) {
        return;
    }

    m_mpdError = mpdError;

    if (m_mpdError != MPD_ERROR_SUCCESS) {
        emit errorMessage(m_mpd.getErrorMessage());
        mpd::Connection disconnected;
        m_mpd = std::move(disconnected);
    }
}