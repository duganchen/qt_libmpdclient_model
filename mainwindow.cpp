#include "mainwindow.h"
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QAbstractItemModel *model, QWidget *parent, Qt::WindowFlags windowFlags): QMainWindow(parent, windowFlags)
{
    auto layout = new QVBoxLayout();
    layout->addWidget(new QPushButton("&Connect to MPD"));
    auto widget = new QWidget();
    auto view = new QListView();
    view->setModel(model);
    layout->addWidget(view);
    widget->setLayout(layout);
    setCentralWidget(widget);
}