#include "mpdConnectionManager.h"
#include "mpdclient/connection.h"
#include <QSocketNotifier>

MPDConnectionManager::MPDConnectionManager(mpd::Connection &mpd, QObject *parent)
    : QObject(parent)
    , m_mpd(mpd)
{}

void MPDConnectionManager::connectToMPD()
{
    emit errorMessage("");
    emit connectionState(MPDConnection::State::Connecting);

    // This is a demo. We harcode the connection values.
    mpd::Connection connection(mpd_connection_new("localhost", 6600, 0));

    m_mpd = std::move(connection);
    if (m_mpd.get_error() == MPD_ERROR_SUCCESS) {
        m_socketNotifier = new QSocketNotifier(m_mpd.get_fd(), QSocketNotifier::Read, this);
        connect(m_socketNotifier, &QSocketNotifier::activated, [=]() {
            if (!m_mpd) {
                return;
            }
            handleIdle(m_mpd.recv_idle(false));
            if (m_mpd) {
                m_mpd.send_idle();
            }
        });
        emit connectionState(MPDConnection::State::Connected);
        if (!m_mpd.send_idle()) {
            handleError(m_mpd.get_error());
        }
    } else {
        onMPDClosed();
    }
}

void MPDConnectionManager::onMPDClosed()
{
    // Handle:
    // * MPD_ERROR_CLOSED, which happens if we've lost the connnection due to, say,
    //   mpd being killed.
    // * connection failures

    // Precondition: MPD's error state isn't MPD_ERROR_SUCCESS
    emit errorMessage(m_mpd.get_error_message());

    disconnectFromMPD();
}

void MPDConnectionManager::disconnectFromMPD()
{
    if (m_socketNotifier) {
        delete m_socketNotifier;
        m_socketNotifier = nullptr;
    }

    mpd::Connection disconnected;
    m_mpd = std::move(disconnected);
    emit connectionState(MPDConnection::State::Disconnected);
}

void MPDConnectionManager::disableNotifications()
{
    if (!m_mpd || !m_socketNotifier) {
        return;
    }

    m_socketNotifier->setEnabled(false);

    mpd_idle idle = m_mpd.noidle();

    if (!idle) {
        handleError(m_mpd.get_error());
    }

    handleIdle(idle);
}

void MPDConnectionManager::enableNotifications()
{
    if (!m_mpd || !m_socketNotifier) {
        return;
    }

    m_socketNotifier->setEnabled(true);

    if (!m_mpd.send_idle()) {
        handleError(m_mpd.get_error());
    }
}

void MPDConnectionManager::handleIdle(mpd_idle idle)
{
    if (!idle) {
        handleError(m_mpd.get_error());
    }

    if (!m_mpd) {
        return;
    }

    if (idle & MPD_IDLE_QUEUE) {
        emit idleQueue();
    }
}

void MPDConnectionManager::handleError(mpd_error error)
{
    if (!m_mpd) {
        return;
    }
    switch (error) {
    // These two both happen if you lose the connection.
    case MPD_ERROR_STATE:
    case MPD_ERROR_CLOSED:
        onMPDClosed();
        break;
    case MPD_ERROR_SUCCESS:
        break;
    default:
        emit errorMessage(m_mpd.get_error_message());
    };
}
