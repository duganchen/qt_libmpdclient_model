#include <QApplication>
#include "mpdclient/connection.h"
#include "mainwindow.h"
#include "queuemodel.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    mpd::Connection connection;
    auto model = new QueueModel(connection);
    MainWindow window(model);
    window.show();
    return app.exec();
}