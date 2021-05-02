#ifndef MPDCONNECTIONMANAGER_H
#define MPDCONNECTIONMANAGER_H

#include <QObject>

#include "mpd/client.h"
#include "connectionstate.h"

namespace mpd {
class Connection;
}

class QSocketNotifier;

class MPDConnectionManager: public QObject
{
    Q_OBJECT
public:
    MPDConnectionManager(mpd::Connection &, QObject * = nullptr);
public slots:
    void connectToMPD();
    void onMPDClosed();
    void disconnectFromMPD();
signals:
    void errorMessage(const QString &);
    void connectionState(MPDConnection::State);

private:
    mpd::Connection &m_mpd;
    int m_mpdError{MPD_ERROR_SUCCESS};
};

#endif