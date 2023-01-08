#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chessboardproblem.h"
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <string.h>
#include <QTextEdit>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextStream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 给棋盘上色
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ui->chessBoard->setItem(i, j, new QTableWidgetItem(" "));
            if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
                ui->chessBoard->item(i, j)->setBackgroundColor(Qt::black);
            }
        }
    }

    // 将棋盘设置为只读
    ui->chessBoard->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 将棋盘设置为不可选中
    ui->chessBoard->setSelectionMode(QAbstractItemView::NoSelection);

    // 设置进度条
    ui->progressBar->setRange(0, 64);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->label_Show->setText(" ");

    // 起始点赋值，同时用蓝色来标定起始点
    this->start_x = 0;
    this->start_y = 0;
    ui->chessBoard->item(this->start_x, this->start_y)->setBackgroundColor(Qt::blue);
    void(QComboBox::*currentIndexChanged)(int) = &QComboBox::currentIndexChanged;

    // 选定初始横坐标
    connect(ui->CBox_x, currentIndexChanged, this, [=](){
        if (!((start_x % 2 == 0 && start_y % 2 == 0) || (start_x % 2 != 0 && start_y % 2 != 0))) {
            ui->chessBoard->item(start_x, start_y)->setBackgroundColor(Qt::black);
        } else {
            ui->chessBoard->item(start_x, start_y)->setBackgroundColor(Qt::white);
        }
        this->start_x = ui->CBox_x->currentIndex();
        ui->chessBoard->item(this->start_x, this->start_y)->setBackgroundColor(Qt::blue);

    });

    // 选定初始纵坐标
    connect(ui->CBox_y, currentIndexChanged, this, [=](){
        if (!((start_x % 2 == 0 && start_y % 2 == 0) || (start_x % 2 != 0 && start_y % 2 != 0))) {
            ui->chessBoard->item(start_x, start_y)->setBackgroundColor(Qt::black);
        } else {
            ui->chessBoard->item(start_x, start_y)->setBackgroundColor(Qt::white);
        }
        this->start_y = ui->CBox_y->currentIndex();
        ui->chessBoard->item(this->start_x, this->start_y)->setBackgroundColor(Qt::blue);
    });

    // 绑定“开始演示”按钮
    connect(ui->Btn_Begin, &QPushButton::clicked, this, &MainWindow::runTest);

    // 绑定“动画演示”按钮
    connect(ui->Btn_Animation, &QPushButton::clicked, this, &MainWindow::animation);

    // 绑定“重新开始”按钮
    connect(ui->Btn_Restart, &QPushButton::clicked, this, &MainWindow::restart);

    // 绑定菜单“重新开始”按钮
    connect(ui->actionRestart, &QAction::triggered, this, &MainWindow::restart);

    // 绑定菜单“退出程序”按钮
    connect(ui->actionExit, &QAction::triggered, this, [=](){
        this->close();
    });

    // 绑定菜单“帮助文档”按钮
    connect(ui->actionHelp, &QAction::triggered, this, [=](){
        QMainWindow *helpWindow = new QMainWindow(this);
        helpWindow->resize(800, 600);
        helpWindow->setWindowTitle("帮助文档");
        helpWindow->setWindowIcon(QIcon(":/Image/Info.png"));
        QTextEdit *textEdit = new QTextEdit(helpWindow);
        helpWindow->setCentralWidget(textEdit);

        // 读入帮助文档
        QString path = ":/HelpText.txt";
        QFile *file = new QFile(path);
        if (file->open(QIODevice::ReadOnly)) {
            QTextStream in(file);
            QByteArray array ;
            // 判断是否到文件末尾
            while(file->atEnd() == false)
            {
                // 按照行读取
                array += file->readLine();
            }
            QFont font = QFont("微软雅黑",13,2);
            textEdit->setText(array);
            textEdit->setFont(font);
            file->close();
            delete file;
            textEdit->setReadOnly(true);
            helpWindow->show();
        } else {
            QMessageBox::critical(helpWindow, "错误", "未找到帮助文档！");
        }
    });
}

void MainWindow::restart()
{
    // 重置棋盘颜色和Board棋盘
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ui->chessBoard->setItem(i, j, new QTableWidgetItem(" "));
            ui->chessBoard->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
                ui->chessBoard->item(i, j)->setBackgroundColor(Qt::black);
                ui->chessBoard->item(i, j)->setForeground(Qt::white);
            }
            ChessBoardProblem::initState();
        }
    }
    ui->label_Show->setText(" ");
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->Btn_Begin->setEnabled(true);
    ui->Btn_Animation->setEnabled(true);
    ui->CBox_x->setEnabled(true);
    ui->CBox_y->setEnabled(true);
    // 给起始点上色
    ui->chessBoard->item(this->start_x, this->start_y)->setBackgroundColor(Qt::blue);
}

// 运行测试用例
void MainWindow::runTest()
{
    ui->Btn_Begin->setEnabled(false);
    ui->CBox_x->setEnabled(false);
    ui->CBox_y->setEnabled(false);
    ChessBoardProblem::initStack(ChessBoardProblem::s);
    ChessBoardProblem::startX = this->start_x;
    ChessBoardProblem::startY = this->start_y;
    ChessBoardProblem::chessBoardProblem(ChessBoardProblem::startX, ChessBoardProblem::startY);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            ui->chessBoard->setItem(i, j, new QTableWidgetItem(QString::number(ChessBoardProblem::Board[i][j])));
            ui->chessBoard->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
                ui->chessBoard->item(i, j)->setBackgroundColor(Qt::black);
                ui->chessBoard->item(i, j)->setForeground(Qt::white);
            }
        }
    }
}

// 动画演示
void MainWindow::animation()
{
    if (ChessBoardProblem::Board[0][0] == 0) {
        // 弹出错误窗口
        QMessageBox::critical(this, "错误", "请先运行测试！");
    } else {
        ui->Btn_Begin->setEnabled(false);
        ui->Btn_Animation->setEnabled(false);
        ui->Btn_Restart->setEnabled(false);
        ui->actionRestart->setEnabled(false);
        ui->CBox_x->setEnabled(false);
        ui->CBox_y->setEnabled(false);
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (!((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))) {
                    ui->chessBoard->item(i, j)->setBackgroundColor(Qt::black);
                    ui->chessBoard->item(i, j)->setForeground(Qt::white);
                } else {
                    ui->chessBoard->item(i, j)->setBackgroundColor(Qt::white);
                }
            }
        }
        static int num = 1;
        QTimer *timer = new QTimer(this);
        timer->start(250);
        ui->progressBar->setVisible(true);
        ui->label_Show->setText("正在演示");
        connect(timer, &QTimer::timeout, this, [=](){
            findElemAndShow(num);
            ui->progressBar->setValue(num);
            if (num == 65) {
                timer->stop();
                num = 1;
                ui->label_Show->setText("演示完毕");
                ui->Btn_Restart->setEnabled(true);
                ui->actionRestart->setEnabled(true);
            }
        });
    }

}

// 通过查找，定位到需要标红的元素
void MainWindow::findElemAndShow(int &order)
{
    int i, j;
    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j) {
            if (ChessBoardProblem::Board[i][j] == order) {
                QTimer *timer2 = new QTimer(this);
                ui->chessBoard->item(i, j)->setBackgroundColor(Qt::red);
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) {
                    ui->chessBoard->item(i, j)->setForeground(Qt::white);
                }
                timer2->start(250);
                connect(timer2, &QTimer::timeout, this, [=](){
                    ui->chessBoard->item(i, j)->setBackgroundColor(Qt::green);
                    ui->chessBoard->item(i, j)->setForeground(Qt::black);
                    timer2->stop();
                });
            }
        }
    }
    order++;
}

// 关闭窗口事件，弹出询问框
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::Yes == QMessageBox::question(this, "询问", "请问是否退出程序？", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)) {
        event->accept();
    } else {
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
