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