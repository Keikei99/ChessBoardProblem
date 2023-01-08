#ifndef CHESSBOARDPROBLEM_H
#define CHESSBOARDPROBLEM_H

#include <QObject>

// 定义点
typedef struct point {
    int x, y; // 初始点的横坐标和纵坐标
    int index; // 方向指针
    int weight; // 该点的权值
    point() {} // 默认构造函数
} point;



#define MAX_STACK_SIZE 70

// 定义栈
typedef struct Stack {
    point *elem;
    int base, top;
} Stack;



class ChessBoardProblem : public QObject
{
    Q_OBJECT
public:
    explicit ChessBoardProblem(QObject *parent = nullptr);
    ChessBoardProblem(int start_x, int start_y);
    static int Board[8][8]; // 棋盘
    static const int MOVEX[8];
    static const int MOVEY[8];
    static Stack s;
    static int order;
    static int startX, startY; // 起始点

    static void initStack(Stack &s);
    static void push(Stack &s, point p);
    static void pop(Stack &s);
    static point getTop(Stack s);
    static bool isEmpty(Stack s);
    static void swap(point &a, point &b);
    static int Partition(point direction[], int low, int high);
    static void QSort(point direction[], int low, int high);
    static void QuickSort(point direction[], int size);
    static bool check(int x, int y);
    static void forward(point &now, point direction[], int &index);
    static void backward(point &now, int &index);
    static int calculateTheWeight(int x, int y);
    static void chessBoardProblem(int startX, int startY);

    static void initState();
signals:

public slots:
};

#endif // CHESSBOARDPROBLEM_H
