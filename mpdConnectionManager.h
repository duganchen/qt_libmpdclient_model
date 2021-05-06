#ifndef MPDCONNECTIONMANAGER_H
#define MPDCONNECTIONMANAGER_H

#include <QObject>

#include "connectionstate.h"
#include "mpd/client.h"
#include "mpdclient/connection.h"
#include "mpdclient/status.h"

class QSocketNotifier;

class MPDConnectionManager : public QObject
{
    Q_OBJECT
public:
    MPDConnectionManager(mpd::Connection &, QObject * = nullptr);
public slots:
    void connectToMPD();
    void onMPDClosed();
    void disconnectFromMPD();
    void disableNotifications();
    void enableNotifications();
signals:
    void errorMessage(const QString &);
    void connectionState(MPDConnection::State);
    void idleQueue();

private:
    mpd::Connection &m_mpd;
    int m_mpdError{MPD_ERROR_SUCCESS};
    QSocketNotifier *m_socketNotifier{};

    void handleIdle(mpd_idle);
    void handleError(mpd_error);
};

#endif