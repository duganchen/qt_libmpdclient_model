#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include <memory>
#include <vector>
#include <QAbstractListModel>

#include "mpdclient/connection.h"
#include "mpdclient/song.h"
#include "mpdclient/status.h"

class QueueModel : public QAbstractListModel
{
Q_OBJECT

public:
    QueueModel(mpd::Connection &, QObject * = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const override;
public slots:
    void clear();
    void refresh();
    void onIdleQueue(std::unique_ptr<mpd::Status> &);
signals:
    void mpdClosed();
    void errorMessage(const QString &);

private:
    mpd::Connection &m_mpd;
    std::vector<std::unique_ptr<mpd::Song>> m_songs;
    bool m_isConnected{false};
    unsigned m_queueVersion{UINT_MAX};
};

#endif