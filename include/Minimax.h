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
    const int ISOLATED_PAWN_PENALTY = -50;
    const int DOUBLED_PAWN_PENALTY = -30;
    const int PASSED_PAWN_BONUS = 50;
    const int BACKWARD_PAWN_PENALTY = -20;
    const int CHAIN_PAWN_BONUS = 10;
    const int KING_CASTLING_BONUS = 50;
    const int KING_OPEN_FILE_PENALTY = -40;
    const int KING_PAWN_SHIELD_BONUS = 30;
    const int KING_MISSING_SHIELD_PAWN_PENALTY = -20;
    const int KING_ENEMY_PROXIMITY_PENALTY = -10;
    const int PAWN_ACTIVITY_VALUE = 10;
    const int KNIGHT_ACTIVITY_VALUE = 30;
    const int BISHOP_ACTIVITY_VALUE = 30;
    const int ROOK_ACTIVITY_VALUE = 50;
    const int QUEEN_ACTIVITY_VALUE = 90;
    const int KING_ACTIVITY_VALUE = 10;  // King mobility is generally not as important

    const int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    const int kingMoves[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };


    public:
        int evaluate(char board[8][8]);
        float minimaxAB(char board[8][8], int depth, float alpha, float beta, bool maximizingPlayerWhite, bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool &isWhite);
        Move findBestMove(char board[8][8], int depth, bool isWhiteKingMoved, bool isBlackKingMoved,
            bool isWhiteRookAMoved, bool isWhiteRookHMoved,
            bool isBlackRookAMoved, bool isBlackRookHMoved, bool isWhite);
        int evaluateMaterial(char board[8][8]);
        int evaluatePawnStructure(char board[8][8], bool isWhite) ;
        static bool isIsolatedPawn(const char board[8][8], int row, int col, bool isWhite);
        static bool isDoubledPawn(const char board[8][8], int row, int col, bool isWhite);
        static bool isPassedPawn(const char board[8][8], int row, int col, bool isWhite);
        static bool isBackwardPawn(const char board[8][8], int row, int col, bool isWhite);
        static bool isPartOfPawnChain(const char board[8][8], int row, int col, bool isWhite);
        int evaluateKingSafety(const char board[8][8], bool isWhite) const;
        static bool hasKingCastled(const char board[8][8], bool isWhite);
        static bool isOpenFileNearKing(const char board[8][8], int kingRow, int kingCol, bool isWhite);
        int evaluatePawnShield(const char board[8][8], int kingRow, int kingCol, bool isWhite) const;
        int evaluateEnemyPieceProximity(const char board[8][8], int kingRow, int kingCol, bool isWhite) const;
    int evaluateKnightActivity(const char board[8][8], int row, int col, bool isWhite);
    int evaluateKingActivity(const char board[8][8], int row, int col, bool isWhite);
    int evaluateSlidingPieceActivity(const char board[8][8], int row, int col, const int directions[][2], int numDirections, int maxDistance, bool isWhite);
    int evaluateAllPieceActivity(const char board[8][8]);


};



#endif //MINIMAX_H
