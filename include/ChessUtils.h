//
// Created by admin on 08/10/2024.
//

#ifndef CHESSUTILS_H
#define CHESSUTILS_H

#include <iostream>

class ChessUtils {
    public:
        static void printBoard(int color1, int color2, char chessBoard[8][8]);
        static std::pair<int, int> chessboardToArrCords(const std::string& strCoords);
        static void move(char (&board)[8][8], std::pair<int, int> old, std::pair<int, int> newC,
                         bool &isWhiteKingMoved, bool &isBlackKingMoved,
                         bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                         bool &isBlackRookAMoved, bool &isBlackRookHMoved);
        static bool verifyMove(std::pair<int, int> old, std::pair<int, int> newC, char board[8][8],
                               bool& isWhiteKingMoved, bool& isBlackKingMoved,
                               bool& isWhiteRookAMoved, bool& isWhiteRookHMoved,
                               bool& isBlackRookAMoved, bool& isBlackRookHMoved, bool isWhite, bool checkOnWhite, bool checkOnBlack);
        static std::pair<int, int> findKing(char board[8][8], bool white);
        static bool checkExists(char board[8][8], bool isKingWhite, bool &isWhiteKingMoved,
            bool &isBlackKingMoved, bool &isWhiteRookAMoved,
            bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool checkOnWhite, bool checkOnBlack);
        static bool canKingMove(bool white, char board[8][8], bool &isWhiteKingMoved,
            bool &isBlackKingMoved, bool &isWhiteRookAMoved,
            bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool checkOnWhite, bool checkOnBlack);
        static bool canOthersSaveKing(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool checkOnWhite, bool checkOnBlack);
        static bool canMove(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool checkOnWhite, bool checkOnBlack);
        static bool solvesCheck(bool white, char board[8][8], std::pair<int, int> old, std::pair<int, int> newC,
            bool &isWhiteKingMoved, bool &isBlackKingMoved,
            bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
            bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool checkOnWhite, bool checkOnBlack);
};


#endif //CHESSUTILS_H
