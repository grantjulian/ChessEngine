// ChessEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ChessState.h"
#include "Algorithm.h"
unsigned long long pos_converter(const char* location)
{
  char col_char = location[0];
  char row_char = location[1];
  short col = (int)col_char - (int)('a');
  short row = atoi(&row_char) - 1;
  //std::cout << col << ", " << row << std::endl;
  return (1ull << (7 - col)) << ((7 - row) * 8);
}
void move_helper(ChessState *s, const char* loc1, const char* loc2)
{
  unsigned long long pos1 = pos_converter(loc1);
  unsigned long long pos2 = pos_converter(loc2);
  if (loc1 == "KC")
  {

  }
  short piece;
  for (int i = 0; i < 12; i++)
  {
    if (pos1 & s->board[i])
    {
      piece = i;
    }
  }
  s->clearPiece(pos1);
  s->clearPiece(pos2);
  s->board[piece] |= pos2;
}

int main()
{
  ChessState state = ChessState();
  state.initializeBoard();
  state.printBoard();
  state = Algorithm::MO_minimax(state, 1, 8, -999999.f, 999999.f);
  //while (true)
  //{
  //  state = Algorithm::MO_minimax(state, 1, 8, -999999.f, 999999.f);
  //  state.printBoard();
  //  char move1[2];
  //  char move2[2];
  //  std::cout << std::endl << "starting position: ";
  //  std::cin >> move1;
  //  if (move1[0] == 'k' && move1[1] == 'c')
  //  {
  //    std::cout << std::endl << "starting position 1: ";
  //    std::cin >> move1;
  //    std::cout << "ending postion 1: ";
  //    std::cin >> move2;
  //    move_helper(&state, move1, move2);
  //    std::cout << std::endl << "starting position 2: ";
  //    std::cin >> move1;
  //    std::cout << "ending postion 2: ";
  //    std::cin >> move2;
  //    move_helper(&state, move1, move2);
  //  }
  //  else
  //  {
  //    std::cout << "ending postion: ";
  //    std::cin >> move2;
  //    move_helper(&state, move1, move2);
  //  }
  //  state.printBoard();
  //}

}

