#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboardproblem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void runTest();
    void animation();
    void restart();
    void findElemAndShow(int &order);
    int start_x, start_y;
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
