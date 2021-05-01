#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectionstate.h"

class QAbstractItemModel;
class QProgressBar;
class QPushButton;

class MainWindow: public QMainWindow
{
public:
    MainWindow(QAbstractItemModel *, QWidget * = nullptr, Qt::WindowFlags = Qt::WindowFlags());
public slots:
    void setConnectionState(MPDConnection::State);
private:
    QProgressBar *m_progressBar{};
    QPushButton *m_connectButton{};
    QPushButton *m_refreshButton{};
};

#endif