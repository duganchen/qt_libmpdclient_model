#ifndef MPDCONNECTIONMANAGER_H
#define MPDCONNECTIONMANAGER_H

#include <QObject>
#include "connection.h"

class MPDConnectionManager: public QObject
{
public:
    Q_OBJECT
    MPDConnectionManager(mpd::Connection &);
    void connectToMPD(const char *, unsigned, unsigned);
signals:
    void errorMessage(const QString &);
private:
        mpd::Connection &m_mpdConnection;
};

#endif