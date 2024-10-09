//
// Created by admin on 09/10/2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

class Utils {
  public:
    static std::vector<std::vector<char>> boardToVectorBoard(char board[8][8]);
    static bool isValidSquare(int row, int col);
    static bool isEnemyPiece(char piece, bool isWhite);
    static bool isEmptySquare(char piece);
};

#endif //UTILS_H
