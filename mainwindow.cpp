#include "mainwindow.h"
#include "queuemodel.h"
#include <QLabel>
#include <QListView>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QueueModel *model, QWidget *parent, Qt::WindowFlags windowFlags): QMainWindow(parent, windowFlags)
{
    auto layout = new QVBoxLayout();
    m_connectButton = new QPushButton("&Connect to MPD");
    connect(m_connectButton, &QPushButton::clicked, [=]() { emit connectClicked(); });
    layout->addWidget(m_connectButton);
    m_progressBar = new QProgressBar();
    layout->addWidget(m_progressBar);
    m_refreshButton = new QPushButton("&Refresh MPD Queue");
    m_refreshButton->setEnabled(false);
    connect(m_refreshButton, &QPushButton::clicked, model, &QueueModel::refresh);
    layout->addWidget(m_refreshButton);
    auto view = new QListView();
    view->setModel(model);
    layout->addWidget(view);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    m_statusLabel = new QLabel();
    statusBar()->addPermanentWidget(m_statusLabel);
}

void MainWindow::setErrorMessage(const QString &message)
{
    m_statusLabel->setText(message);
}

void MainWindow::setConnectionState(MPDConnection::State state)
{
    if (m_connectionState == state) {
        return;
    }

    m_connectionState = state;

    switch (m_connectionState) {
        case MPDConnection::State::Disconnected:
            m_connectButton->setText("&Connect to MPD");
            m_refreshButton->setEnabled(false);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(1);
            break;
        case MPDConnection::State::Connecting:
            m_connectButton->setText("&Connect to MPD");
            m_refreshButton->setEnabled(false);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(0);
            break;
        case MPDConnection::State::Connected:
            m_connectButton->setText("Dis&connect from MPD");
            m_refreshButton->setEnabled(true);
            m_progressBar->setMinimum(0);
            m_progressBar->setMaximum(1);
            break;
       };
};