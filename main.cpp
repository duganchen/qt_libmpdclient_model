#include <QApplication>
#include <QObject>
#include "mpdclient/connection.h"
#include "mpdConnectionManager.h"
#include "mainwindow.h"
#include "queuemodel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    mpd::Connection connection;
    MPDConnectionManager mpdConnectionManager(connection);
    QueueModel model(connection);
    QObject::connect(&model, &QueueModel::mpdClosed, &mpdConnectionManager, &MPDConnectionManager::onMPDClosed);
    MainWindow window(&model);
    QObject::connect(&mpdConnectionManager, &MPDConnectionManager::errorMessage, &window, &MainWindow::setErrorMessage);
    QObject::connect(&mpdConnectionManager, &MPDConnectionManager::connectionState, &window, &MainWindow::setConnectionState);
    QObject::connect(&mpdConnectionManager, &MPDConnectionManager::plchangesposid, &model, &QueueModel::onPlChangesPosId);
    QObject::connect(&window, &MainWindow::connectClicked, &mpdConnectionManager, &MPDConnectionManager::connectToMPD);
    QObject::connect(&window, &MainWindow::disconnectClicked, &mpdConnectionManager, &MPDConnectionManager::disconnectFromMPD);
    QObject::connect(&window, &MainWindow::disconnectClicked, &model, &QueueModel::clear);
    window.show();
    return app.exec();
}