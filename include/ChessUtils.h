//
// Created by admin on 08/10/2024.
//

#ifndef CHESSUTILS_H
#define CHESSUTILS_H

#include <iostream>

class ChessUtils {
    public:
        void printBoard(int color1, int color2, char chessBoard[8][8]);
        std::pair<int, int> chessboardToArrCords(std::string strCoords);
        void move(char (&board)[8][8], std::pair<int, int> old, std::pair<int, int> newC,
            bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        bool verifyMove(std::pair<int, int> old, std::pair<int, int> newC, char board[8][8],
            bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool isWhite);
        std::pair<int, int> findKing(char board[8][8], bool white);
        bool checkExists(char board[8][8], bool isKingWhite, bool &isWhiteKingMoved,
            bool &isBlackKingMoved, bool &isWhiteRookAMoved,
            bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        bool canKingMove(bool white, char board[8][8], bool &isWhiteKingMoved,
            bool &isBlackKingMoved, bool &isWhiteRookAMoved,
            bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        bool canOthersSaveKing(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        bool canMove(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        bool solvesCheck(bool white, char board[8][8], std::pair<int, int> old, std::pair<int, int> newC,
            bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved);
};


#endif //CHESSUTILS_H
