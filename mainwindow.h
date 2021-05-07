#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectionstate.h"
#include <QMainWindow>

class QAbstractItemModel;
class QLabel;
class QProgressBar;
class QPushButton;
class QueueModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QueueModel *, QWidget * = nullptr, Qt::WindowFlags = Qt::WindowFlags());
public slots:
    void setConnectionState(MPDConnection::State);
    void setErrorMessage(const QString &);
signals:
    void connectClicked();
    void disconnectClicked();

private:
    QProgressBar *m_progressBar{};
    QPushButton *m_connectButton{};
    QLabel *m_statusLabel{};
    MPDConnection::State m_connectionState{MPDConnection::State::Disconnected};
};

#endif