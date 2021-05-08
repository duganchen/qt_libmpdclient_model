#include "mainwindow.h"
#include "mpdConnectionManager.h"
#include "mpdclient/connection.h"
#include "queuemodel.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mpd::Connection connection;
    MPDConnectionManager mpdConnectionManager(connection);
    QueueModel model(connection);
    QObject::connect(&model,
                     &QueueModel::mpdClosed,
                     &mpdConnectionManager,
                     &MPDConnectionManager::onMPDClosed);
    MainWindow window(&model);
    QObject::connect(&mpdConnectionManager,
                     &MPDConnectionManager::errorMessage,
                     &window,
                     &MainWindow::setErrorMessage);
    QObject::connect(&mpdConnectionManager,
                     &MPDConnectionManager::connectionState,
                     &window,
                     &MainWindow::setConnectionState);
    QObject::connect(&mpdConnectionManager,
                     &MPDConnectionManager::idleQueue,
                     &model,
                     &QueueModel::onIdleQueue);
    QObject::connect(&window,
                     &MainWindow::connectClicked,
                     &mpdConnectionManager,
                     &MPDConnectionManager::connectToMPD);
    QObject::connect(&window,
                     &MainWindow::disconnectClicked,
                     &mpdConnectionManager,
                     &MPDConnectionManager::disconnectFromMPD);
    QObject::connect(&window, &MainWindow::disconnectClicked, &model, &QueueModel::clear);
    QObject::connect(&model, &QueueModel::errorMessage, &window, &MainWindow::setErrorMessage);
    QObject::connect(&model,
                     &QueueModel::commandAboutToBeExecuted,
                     &mpdConnectionManager,
                     &MPDConnectionManager::disableNotifications);
    QObject::connect(&model,
                     &QueueModel::commandExecuted,
                     &mpdConnectionManager,
                     &MPDConnectionManager::enableNotifications);
    QObject::connect(&mpdConnectionManager,
                     &MPDConnectionManager::connectionState,
                     &model,
                     &QueueModel::onConnectionState);
    window.show();
    return app.exec();
}