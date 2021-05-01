#include <QApplication>
#include <QObject>
#include "mpdclient/connection.h"
#include "mpdConnectionManager.h"
#include "mainwindow.h"
#include "queuemodel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    mpd::Connection connection;
    auto mpdConnectionManager = new MPDConnectionManager(connection);
    auto model = new QueueModel(connection);
    QObject::connect(model, &QueueModel::mpdError, mpdConnectionManager, &MPDConnectionManager::setError);
    MainWindow window(model);
    QObject::connect(mpdConnectionManager, &MPDConnectionManager::errorMessage, &window, &MainWindow::setErrorMessage);
    QObject::connect(mpdConnectionManager, &MPDConnectionManager::connectionState, &window, &MainWindow::setConnectionState);
    window.show();
    return app.exec();
}