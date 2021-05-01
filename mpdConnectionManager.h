#ifndef MPDCONNECTIONMANAGER_H
#define MPDCONNECTIONMANAGER_H

#include <QObject>

#include "mpdclient/connection.h"
#include "mpd/client.h"
#include "connectionstate.h"

class MPDConnectionManager: public QObject
{
    Q_OBJECT
public:
    MPDConnectionManager(mpd::Connection &, QObject * = nullptr);
public slots:
    void connectToMPD();
    void setError(int);
signals:
    void errorMessage(const QString &);
    void connectionState(MPDConnection::State);

private:
    mpd::Connection &m_mpd;
    int m_mpdError{MPD_ERROR_SUCCESS};
};

#endif