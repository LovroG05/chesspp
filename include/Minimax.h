//
// Created by admin on 08/10/2024.
//

#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
#include <map>
#include "Move.h"

class Minimax {
    std::map<char, int> piece_values;
    public:
        int evaluate(char board[8][8]);
        float minimaxAB(char board[8][8], int depth, float alpha, float beta, bool maximizingPlayerWhite, bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool &isWhite);
    Move findBestMove(char board[8][8], int depth, bool &isWhiteKingMoved, bool &isBlackKingMoved,
        bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
        bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool &isWhite);


};



#endif //MINIMAX_H
