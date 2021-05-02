#include "mpdConnectionManager.h"
#include "mpdclient/connection.h"
#include <QSocketNotifier>

MPDConnectionManager::MPDConnectionManager(mpd::Connection &mpd, QObject *parent)
    : QObject(parent)
    , m_mpd(mpd)
{
}

void MPDConnectionManager::connectToMPD() {
    emit errorMessage("");
    emit connectionState(MPDConnection::State::Connecting);

    // This is a demo. We harcode the connection values.
    mpd::Connection connection(mpd_connection_new("localhost", 6600, 0));
    
    m_mpd = std::move(connection);
    if (m_mpd.getError() == MPD_ERROR_SUCCESS) {
        m_socketNotifier = new QSocketNotifier(m_mpd.getFD(), QSocketNotifier::Read, this);
        emit connectionState(MPDConnection::State::Connected);
    } else {
        onMPDClosed();
    }
}

void MPDConnectionManager::onMPDClosed() {
    // Handle:
    // * MPD_ERROR_CLOSED, which happens if we've lost the connnection due to, say,
    //   mpd being killed.
    // * connection failures

    // Precondition: MPD's error state isn't MPD_ERROR_SUCCESS
    emit errorMessage(m_mpd.getErrorMessage());
    
    disconnectFromMPD();
}

void MPDConnectionManager::disconnectFromMPD()
{
    if (m_socketNotifier) {
        m_socketNotifier->deleteLater();
        m_socketNotifier = nullptr;
    }

    mpd::Connection disconnected;
    m_mpd = std::move(disconnected);
    emit connectionState(MPDConnection::State::Disconnected);
}