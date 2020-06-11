#include "Algorithm.h"
#include <algorithm>
#include "iostream"

ChessState Algorithm::minimax(ChessState state, int turn, int depth)
{
  int length = 0;
  int best = 0;
  int bestval = 0;
  // black wins
  if (state.board[5] == 0)
  {
    state.value = -1000000;
    return state;
  }
  // white wins
  if (state.board[11] == 0)
  {
    state.value = 1000000;
    return state;
  }
  if (depth == 0)
  {
    state.value = ChessState::evaluationFunction(state);
    return state;
  }
  ChessState* moves = state.generateStates(turn, &length);
  for (int i = 0; i < length; i++)
  {
    ChessState s = minimax(moves[i], turn * -1, depth - 1);
    moves[i].value = s.value;
    if (turn == 1 && moves[i].value > moves[best].value)
    {
      best = i;
    }
    else if (turn == -1 && moves[i].value < moves[best].value)
    {
      best = i;
    }
  }
  ChessState s = moves[best];
  std::free(moves);
  return s;
}

ChessState Algorithm::AB_minimax(ChessState state, int turn, int depth, float alpha, float beta)
{
  int length = 0;
  int best = 0;
  int bestval = 0;
  // black wins
  if (state.board[5] == 0)
  {
    state.value = -1000000;
    return state;
  }
  // white wins
  if (state.board[11] == 0)
  {
    state.value = 1000000;
    return state;
  }
  if (depth == 0)
  {
    state.value = ChessState::evaluationFunction(state);
    return state;
  }
  ChessState* moves = state.generateStates(turn, &length);
  for (int i = 0; i < length; i++)
  {
    if (alpha >= beta)
    {
      break;
    }
    ChessState s = AB_minimax(moves[i], turn * -1, depth - 1, alpha, beta);
    moves[i].value = s.value;
    if (turn == 1 && moves[i].value > moves[best].value)
    {
      best = i;
    }
    else if (turn == -1 && moves[i].value < moves[best].value)
    {
      best = i;
    }
    if (turn == 1 && moves[best].value > alpha)
    {
      alpha = moves[best].value;
    }
    else if (turn == -1 && moves[best].value < beta)
    {
      beta = moves[best].value;
    }
  }
  ChessState s = moves[best];
  std::free(moves);
  return s;
}

ChessState Algorithm::MO_minimax(ChessState state, int turn, int depth, float alpha, float beta)
{
  int length = 0;
  int best = 0;
  int bestval = 0;
  // black wins
  if (state.board[5] == 0)
  {
    state.value = -1000000;
    return state;
  }
  // white wins
  if (state.board[11] == 0)
  {
    state.value = 1000000;
    return state;
  }
  ChessState* moves = state.generateStates(turn, &length);
  for (int i = 0; i < length; i++)
  {
    moves[i].value = ChessState::evaluationFunction(moves[i]);
  }
  if (turn == 1)
  {
    std::sort(moves, moves + length, std::greater<ChessState>());
  }
  else if (turn == -1)
  {
    std::sort(moves, moves + length);
  }
  if (depth == 1)
  {
    ChessState s = moves[0];
    std::free(moves);
    return s;
  }
  for (int i = 0; i < length; i++)
  {
    if (alpha >= beta)
    {
      break;
    }
    ChessState s = MO_minimax(moves[i], turn * -1, depth - 1, alpha, beta);
    moves[i].value = s.value;
    if (turn == 1 && moves[i].value > moves[best].value)
    {
      best = i;
    }
    else if (turn == -1 && moves[i].value < moves[best].value)
    {
      best = i;
    }
    if (turn == 1 && moves[best].value > alpha)
    {
      alpha = moves[best].value;
    }
    else if (turn == -1 && moves[best].value < beta)
    {
      beta = moves[best].value;
    }
  }
  ChessState s = moves[best];
  std::free(moves);
  return s;
}
