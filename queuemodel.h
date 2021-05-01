#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include <memory>
#include <vector>
#include <QAbstractListModel>

namespace mpd {
class Connection;
class Song;
} // namespace mpd


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
signals:
    void mpdError(int);

private:
    mpd::Connection &m_mpd;
    std::vector<std::unique_ptr<mpd::Song>> m_songs;
    bool m_isConnected{false};
};

#endif