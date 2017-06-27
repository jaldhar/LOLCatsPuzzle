//
// Created by jaldhar on 6/26/17.
//

#include "LOLCatsPuzzle.h"

#include <algorithm>
using namespace std;

#include "LOLCatsPuzzle.h"

LOLCatsPuzzle::LOLCatsPuzzle() {
    for (int i = 0; i < BOARDHEIGHT; i++) {
        for (int j = 0; j < BOARDWIDTH; j++) {
            _board[i][j] = -1;
        }
    }
}

LOLCatsPuzzle::~LOLCatsPuzzle()=default;

int LOLCatsPuzzle::height() const {
    return BOARDHEIGHT;
}

int LOLCatsPuzzle::width() const {
    return BOARDWIDTH;
}

pair<int, int> LOLCatsPuzzle::convert(int y, int x) {
    return make_pair(_board[y][x] / BOARDWIDTH, _board[y][x] % BOARDWIDTH);
}

bool LOLCatsPuzzle::move(int y1, int x1, int y2, int x2) {
    if (y1 == y2 && x1 == x2)
        return false;

    swap(_board[y1][x1], _board[y2][x2]);

    return true;
}

void LOLCatsPuzzle::reset() {

    for (int i = 0; i < BOARDHEIGHT; i++) {
        for (int j = 0; j < BOARDWIDTH; j++) {
            _board[i][j] = i * BOARDWIDTH + j;
        }
    }
}

void LOLCatsPuzzle::shuffle() {
    array<int, BOARDWIDTH * BOARDHEIGHT> a;

    for (int i = 0; i < BOARDHEIGHT; i++) {
        for (int j = 0; j < BOARDWIDTH; j++) {
            a[i * BOARDWIDTH + j] = _board[i][j];
        }
    }

    random_shuffle(begin(a), end(a));

    for (size_t i = 0; i < a.size(); i++) {
        _board[i / BOARDWIDTH][i % BOARDWIDTH] = a[i];
    }
}

bool LOLCatsPuzzle::success() {
    int current = 0;

    for (int i = 0; i < BOARDHEIGHT; i++) {
        for (int j = 0; j < BOARDWIDTH; j++) {
            if (_board[i][j] != current)
                return false;

            current++;
        }
    }
    return true;
}
