#include "mainwindow.h"
#include "queuemodel.h"
#include <QLabel>
#include <QListView>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QueueModel *model, QWidget *parent, Qt::WindowFlags windowFlags)
    : QMainWindow(parent, windowFlags)
{
    auto layout = new QVBoxLayout();
    m_connectButton = new QPushButton("&Connect to MPD");
    connect(m_connectButton, &QPushButton::clicked, [=]() {
        switch (m_connectionState) {
        case MPDConnection::State::Disconnected:
            emit connectClicked();
            break;
        case MPDConnection::State::Connected:
            emit disconnectClicked();
            break;
        default:
            break;
        }
    });
    layout->addWidget(m_connectButton);
    m_progressBar = new QProgressBar();
    layout->addWidget(m_progressBar);
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
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(1);
        break;
    case MPDConnection::State::Connecting:
        m_connectButton->setText("&Connect to MPD");
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(0);
        break;
    case MPDConnection::State::Connected:
        m_connectButton->setText("Dis&connect from MPD");
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(1);
        break;
    };
};