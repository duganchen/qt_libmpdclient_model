#ifndef MPDCONNECTIONMANAGER_H
#define MPDCONNECTIONMANAGER_H

#include <QObject>

#include "connection.h"

class MPDConnectionManager: public QObject
{
    Q_OBJECT
public:
    MPDConnectionManager(mpd::Connection &, QObject * = nullptr);
    void connectToMPD();
    void setError(int);
signals:
    void errorMessage(const QString &);
private:
    mpd::Connection &m_mpd;
    int m_mpdError{0};
};

#endif