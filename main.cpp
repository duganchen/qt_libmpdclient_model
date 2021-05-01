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
    QObject::connect(&model, &QueueModel::mpdError, &mpdConnectionManager, &MPDConnectionManager::setError);
    MainWindow window(&model);
    QObject::connect(&mpdConnectionManager, &MPDConnectionManager::errorMessage, &window, &MainWindow::setErrorMessage);
    QObject::connect(&mpdConnectionManager, &MPDConnectionManager::connectionState, &window, &MainWindow::setConnectionState);
    QObject::connect(&window, &MainWindow::connectClicked, &mpdConnectionManager, &MPDConnectionManager::connectToMPD);
    window.show();
    return app.exec();
}