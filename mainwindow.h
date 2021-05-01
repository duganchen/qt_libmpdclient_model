#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAbstractItemModel;

class MainWindow: public QMainWindow
{
public:
    MainWindow(QAbstractItemModel *, QWidget * = nullptr, Qt::WindowFlags = Qt::WindowFlags());
};

#endif