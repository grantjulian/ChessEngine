#include "ChessState.h"
#pragma once
class Algorithm
{
public:
  static ChessState minimax(ChessState state, int turn, int depth);
  static ChessState AB_minimax(ChessState state, int turn, int depth, float alpha, float beta);
  static ChessState MO_minimax(ChessState state, int turn, int depth, float alpha, float beta);
};

