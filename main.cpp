#include <stdio.h>
#include <thread>

using namespace std;

const int STACK_SIZE = 8;
const int SLEEP_DURATION = 20;

int max;
int stacks[3][STACK_SIZE];

void sleep() {
    this_thread::sleep_for(chrono::milliseconds(SLEEP_DURATION));
}

void repeatChar(char* str, int &start, int length, char c) {
    for (int i = start; i < start + length; i++) {
        str[i] = c;
    }
    start += length;
}

char* toBlockStr(int val) {
    char* out = new char[STACK_SIZE * 2 + 2];
    out[STACK_SIZE * 2 + 1] = 0;
    int next = 0;
    if (val == 0) {
        repeatChar(out, next, STACK_SIZE * 2 + 1, ' ');
        out[STACK_SIZE] = '|';
    }
    else {
        repeatChar(out, next, STACK_SIZE - val, ' ');
        out[next++] = '(';
        repeatChar(out, next, val - 1, '[');
        out[next++] = '|';
        repeatChar(out, next, val - 1, ']');
        out[next++] = ')';
        repeatChar(out, next, STACK_SIZE - val, ' ');
    }
    return out;
}

void show() {
    system("clear");
    for (int i = 0; i < 3; i++) {
        printf("%s", toBlockStr(0));
    }
    printf("\n");
    for (int y = STACK_SIZE - 1; y >= 0; y--) {
        for (int x = 0; x < 3; x++) {
            printf("%s", toBlockStr(stacks[x][y]));
        }
        printf("\n");
    }
    sleep();
}

int topIndex(int stk) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[stk][i] != 0) {
            return i;
        }
    }
    return -1;
}

int top(int stk) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[stk][i] != 0) {
            return stacks[stk][i];
        }
    }
    return 0;
}

int pop(int stk) {
    for (int i = STACK_SIZE - 1; i >= 0; i--) {
        if (stacks[stk][i] != 0) {
            int ret = stacks[stk][i];
            stacks[stk][i] = 0;
            return ret;
        }
    }
    throw underflow_error("Stack empty, cannot pop any more.\n");
}

void push(int stk, int val) {
    for (int i = 0; i < STACK_SIZE; i++) {
        if (stacks[stk][i] == 0) {
            stacks[stk][i] = val;
            return;
        }
    }
    throw overflow_error("Stack full, cannot push any more.\n");
}

bool isEmpty(int peg) {
    return top(peg) == 0;
}

bool isValid(int src, int dst) {
    return isEmpty(dst) || top(src) < top(dst);
}

void moveBlock(int src, int dst) {
    show();
    if (isValid(src, dst)) {
        push(dst, pop(src));
    }
    else {
        throw invalid_argument("Can't put a bigger block over a smaller block");
    }
}

bool underBlock(int level, int peg) {
    return top(peg) < stacks[peg][level];
}

void moveStack(int y, int src, int dst, int tmp);
void moveStack(int y, int src, int dst, int tmp) {
    if (underBlock(y, src)) {
        int tmpTop = topIndex(tmp);
        moveStack(y + 1, src, tmp, dst);
        moveBlock(src, dst);
        moveStack(tmpTop + 1, tmp, dst, src);
    }
    else {
        moveBlock(src, dst);
    }
}
void test_push() {
    push(1, 3);
    push(1, 1);
    push(1, 2);
}

void test_pop() {
    test_push();
    show();
    // cout << "Top=" << top(1) << '\n';
    // cout << "Pop=" << pop(1) << '\n';
    // cout << "Top=" << top(1) << '\n';
    // cout << "Pop=" << pop(1) << '\n';  
    // cout << "Top=" << top(1) << '\n';
    // cout << "Pop=" << pop(1) << '\n';  
    // cout << "Top=" << top(1) << '\n';
    // cout << "Pop=" << pop(1) << '\n';  
}

void test_move() {
    test_push();
    show();
    moveBlock(1, 2);
    sleep();
    show();
    moveBlock(1, 2);
    sleep();
    show();
    moveBlock(1, 2);
    sleep();
    show();
    moveBlock(1, 2);
    sleep();
}

void initStack() {
    for (int i = STACK_SIZE; i > 0; i--) {
        push(0, i);
    }
}

int main() {
    initStack();
    moveStack(0, 0, 2, 1);
    show();

    return 0;
}