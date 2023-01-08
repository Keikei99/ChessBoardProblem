#include "chessboardproblem.h"
#include <malloc.h>
#include <string.h>
#include <QDebug>

int ChessBoardProblem::Board[8][8]; // 棋盘
// 8个方向
const int ChessBoardProblem::MOVEX[8] = {1, 2, 2, 1, -1, -2, -2, -1};
const int ChessBoardProblem::MOVEY[8] = {2, 1, -1, -2, -2, -1, 1, 2};
Stack ChessBoardProblem::s;
int ChessBoardProblem::order = 1;
int ChessBoardProblem::startX, ChessBoardProblem::startY; // 起始点

ChessBoardProblem::ChessBoardProblem(QObject *parent) : QObject(parent)
{

}

// 初始化状态
void ChessBoardProblem::initState() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Board[i][j] = 0;
        }
    }
    while (!isEmpty(s)) {
        pop(s);
    }
    order = 1;
}

// 初始化栈
void ChessBoardProblem::initStack(Stack &s) {
    if (s.elem == nullptr) {
        s.elem = (point *)malloc(MAX_STACK_SIZE * sizeof(point));
    }
    s.top = 0;
    s.base = 0;
}

// 元素入栈
void ChessBoardProblem::push(Stack &s, point p) {
    s.elem[s.top] = p;
    s.top++;
}

// 元素出栈
void ChessBoardProblem::pop(Stack &s) {
    s.top--;
}

// 获取栈顶元素
point ChessBoardProblem::getTop(Stack s) {
    return s.elem[s.top - 1];
}

// 判断栈是否为空
bool ChessBoardProblem::isEmpty(Stack s) {
    return s.top == s.base;
}

// 交换两个元素的值
void ChessBoardProblem::swap(point &a, point &b) {
    point temp = a;
    a = b;
    b = temp;
}

// 进行划分，如果元素小于基准，则放置与基准的左侧
int ChessBoardProblem::Partition(point direction[], int low, int high) {
    int pivot = low;
    while (low < high) {
        while (low < high && direction[high].weight > direction[pivot].weight)
            --high;
        while (low < high && direction[low].weight <= direction[pivot].weight)
            ++low;
        swap(direction[low], direction[high]);
    }
    swap(direction[low], direction[pivot]);
    return low;
}

// 递归快速排序
void ChessBoardProblem::QSort(point direction[], int low, int high) {
    if (low < high) {
        int pivot = Partition(direction, low, high);
        QSort(direction, low, pivot - 1);
        QSort(direction, pivot + 1, high);
    }
}

// 快速排序函数
void ChessBoardProblem::QuickSort(point direction[], int size) {
    QSort(direction, 0, size - 1);
}

// 确认边界条件
bool ChessBoardProblem::check(int x, int y) {
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

// 马向前进
void ChessBoardProblem::forward(point &now, point direction[], int &index) {
    // 标记棋盘
    Board[direction[index].x][direction[index].y] = ++order;
    // 设置权值
    now.weight = direction[index].weight;
    // 设置index指针
    now.index = direction[index].index;
    push(s, now);
    // 更新该点的信息
    memset(&now, 0, sizeof(now));
    now.x = direction[index].x;
    now.y = direction[index].y;
    // 更新index指针
    index = 0;
}

// 如果马无路可走，则进行回溯操作
void ChessBoardProblem::backward(point &now, int &index) {
    --order;
    // 将棋盘上的这个点设置为0
    Board[now.x][now.y] = 0;
    // 获取该点以便在下一次的循环中重新搜索
    point temp = getTop(s);
    pop(s);
    now.x = temp.x;
    now.y = temp.y;
    index = temp.index;
    // 更新index指针
    ++index;
}

// 计算权值的函数
int ChessBoardProblem::calculateTheWeight(int x, int y) {
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        int x_next = x + MOVEX[i];
        int y_next = y + MOVEY[i];
        if (check(x_next, y_next)) {
            if (Board[x_next][y_next] == 0) {
                sum++;
            }
        }
    }
    return sum;
}

// 马踏棋盘算法
void ChessBoardProblem::chessBoardProblem(int startX, int startY) {
    int index = 0; // 可行方向的index指针
    point now; // 初始点
    point direction[8];
    memset(direction, 0, sizeof(point));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Board[i][j] = 0;
        }
    }
    Board[startX][startY] = order;
    now.x = startX;
    now.y = startY;
    // 将order<64设置为while循环的条件，当order>=64时说明棋盘已经遍历完成
    while (order < 64) {
        int size = 0; // 可行方向的数量
        // 搜索8个方向，并计算它们的权值
        for (int i = 0; i < 8; i++) {
            int x_next = now.x + MOVEX[i];
            int y_next = now.y + MOVEY[i];
            if (check(x_next, y_next) && Board[x_next][y_next] == 0) {
                int weight = calculateTheWeight(x_next, y_next);
                direction[size].x = x_next; // 更新横坐标
                direction[size].y = y_next; // 更新纵坐标
                direction[size].weight = weight; // 更新权值
                ++size;
            }
        }
        // 根据权值对direction数组进行快速升序排序
        QuickSort(direction, size);
        for (int i = 0; i < size; i++)
            direction[i].index = i;
        // 如果有路可走，则向该点前进
        if (order >= 1 && size > 0 && index < size) {
            forward(now, direction, index);
        } else { // 如果无路可走，则进行回溯操作
            backward(now, index);
        }
    }
}
