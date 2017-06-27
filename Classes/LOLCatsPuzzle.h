//
// Created by jaldhar on 6/26/17.
//

#ifndef LOLCATSPUZZLE_H
#define LOLCATSPUZZLE_H

#include <array>
#include <utility>

class LOLCatsPuzzle {
public:
    LOLCatsPuzzle();
    virtual ~LOLCatsPuzzle();
    int                 height() const;
    int                 width() const;
    std::pair<int,int>  convert(int y, int x);
    bool                move(int y1, int x1, int y2, int x2);
    void                reset();
    void                shuffle();
    bool                success();
private:
    static const int BOARDWIDTH  = 4;
    static const int BOARDHEIGHT = 6;
    std::array<std::array<int, BOARDWIDTH>, BOARDHEIGHT> _board;

};


#endif // LOLCATSPUZZLE_H
