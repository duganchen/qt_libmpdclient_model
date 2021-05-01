#include "mainwindow.h"
#include <QListView>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QAbstractItemModel *model, QWidget *parent, Qt::WindowFlags windowFlags): QMainWindow(parent, windowFlags)
{
    auto layout = new QVBoxLayout();
    m_connectButton = new QPushButton("&Connect to MPD");
    layout->addWidget(m_connectButton);
    m_progressBar = new QProgressBar();
    layout->addWidget(m_progressBar);
    auto view = new QListView();
    view->setModel(model);
    layout->addWidget(view);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);
}

void MainWindow::setConnectionState(MPDConnection::State state)
{
    switch (state) {
        case MPDConnection::State::Disconnected:
            m_connectButton->setText("&Connect to MPD");
            m_connectButton->setEnabled(true);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(1);
            break;
        case MPDConnection::State::Connecting:
            m_connectButton->setText("&Connect to MPD");
            m_connectButton->setEnabled(false);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(0);
            break;
        case MPDConnection::State::Connected:
            m_connectButton->setText("Dis&connect from MPD");
            m_connectButton->setEnabled(true);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(1);
            break;
       };
}