//
// Created by admin on 09/10/2024.
//

#include "../include/Utils.h"

std::vector<std::vector<char>> Utils::boardToVectorBoard(char board[8][8]){
  std::vector<std::vector<char>> vecBoard(8, std::vector<char>(8));
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      vecBoard[i][j] = board[i][j];
    }
  }
  return vecBoard;
}

// Function to check if a square is within bounds
bool Utils::isValidSquare(int row, int col) {
  return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Check if a square is empty or occupied by an opponent
bool Utils::isEnemyPiece(char piece, bool isWhite) {
  return (isWhite && islower(piece)) || (!isWhite && isupper(piece));
}

// Check if a square is empty
bool Utils::isEmptySquare(char piece) {
  return piece == ' ';
}