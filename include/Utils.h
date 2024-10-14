//
// Created by admin on 09/10/2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <PGNMove.h>

class Utils {
  public:
    static std::vector<std::vector<char>> boardToVectorBoard(char board[8][8]);
    static bool isValidSquare(int row, int col);
    static bool isEnemyPiece(char piece, bool isWhite);
    static bool isEmptySquare(char piece);
    static std::string convertMoveToPGN(const PGNMove& move);
    static std::vector<PGNMove> detectMovesFromHistory(const std::vector<std::vector<std::vector<char>>>& history);
    static std::vector<std::string> convertMoveHistoryToPGN(const std::vector<std::vector<std::vector<char>>>& history);
    static void writePGNToFile(const std::string& filename, const std::vector<std::string>& pgnMoves);
};

#endif //UTILS_H
