#include "ChessState.h"
#include <iostream>
#include <intrin.h>

void ChessState::initializeBoard()
{
  // most significant bit is bottom left square in chess board
  // white pieces
  board[0] = 0x00ff000000000000; // white pawns
  board[1] = 0x4200000000000000; // white knights
  board[2] = 0x2400000000000000; // white bishops
  board[3] = 0x8100000000000000; // white rooks
  board[4] = 0x1000000000000000; // white queen
  board[5] = 0x0800000000000000; // white king
  // black pieces
  board[6] = 0x000000000000ff00; // black pawns
  board[7] = 0x0000000000000042; // black knights
  board[8] = 0x0000000000000024; // black bishops
  board[9] = 0x0000000000000081; // black rooks
  board[10] = 0x0000000000000010; // black queen
  board[11] = 0x0000000000000008; // black king
}

void ChessState::clearPiece(unsigned long long pos)
{
  for (int i = 0; i < 12; i++)
  {
    board[i] &= ~pos;
  }
}

void ChessState::placePiece(short id, unsigned long long pos)
{
  clearPiece(pos);
  board[id] |= pos;
}

unsigned long long ChessState::validWhiteMoves()
{
  unsigned long long valid_moves = 0xffffffffffffffff;
  valid_moves ^= board[0];
  valid_moves ^= board[1];
  valid_moves ^= board[2];
  valid_moves ^= board[3];
  valid_moves ^= board[4];
  valid_moves ^= board[5];
  return valid_moves;
}

unsigned long long ChessState::validBlackMoves()
{
  unsigned long long valid_moves = 0xffffffffffffffff;
  valid_moves ^= board[6];
  valid_moves ^= board[7];
  valid_moves ^= board[8];
  valid_moves ^= board[9];
  valid_moves ^= board[10];
  valid_moves ^= board[11];
  return valid_moves;
}

ChessState* ChessState::generateStates(int turn, int* length)
{
  // initialize states to be a certain size
  ChessState* states = (ChessState*)std::malloc(sizeof(ChessState) * 80);
  unsigned long long validmoves;
  unsigned long long opppieces;
  unsigned long long pawns;
  unsigned long long knights;
  unsigned long long bishops;
  unsigned long long rooks;
  unsigned long long queens;
  unsigned long long kings;
  if (turn == 1)
  {
    validmoves = validWhiteMoves();
    opppieces = ~validBlackMoves();
    pawns = board[0];
    knights = board[1];
    bishops = board[2];
    rooks = board[3];
    queens = board[4];
    kings = board[5];
  }
  else //(turn == -1)
  {
    validmoves = validBlackMoves();
    opppieces = ~validWhiteMoves();
    pawns = board[6];
    knights = board[7];
    bishops = board[8];
    rooks = board[9];
    queens = board[10];
    kings = board[11];
  }

  unsigned index = 0;
  unsigned short inc = 0;
  unsigned long long pos = 1;
  unsigned long long attacked_squares = 0;

  this->move_number++;

  if (turn == 1)
  {
    while (inc < 64)
    {
      pos = 1ull << inc;
      // white pawn moves
      // check if there is a pawn at position
      if ((pawns & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (((pos >> 8) & (validmoves & ~opppieces)) != 0)
        {
          // if the pawn has reached the back rank
          if (((pos >> 8) & 0xff) != 0)
          {
            states[index] = *this;
            states[index].board[0] &= ~pos;
            states[index].board[4] |= pos >> 8;
            index++;
            states[index] = *this;
            states[index].board[0] &= ~pos;
            states[index].board[1] |= pos >> 8;
            index++;
          }
          else
          {
            states[index] = *this;
            states[index].board[0] = (board[0] ^ pos) | (pos >> 8);
            index += 1;
          }
          if (pos >= (1ull << 48) && (((pos >> 16) & (validmoves & ~opppieces)) != 0))
          {
            states[index] = *this;
            states[index].board[0] = (board[0] ^ pos) | (pos >> 16);
            index += 1;
          }
        }
        if (((pos >> 9) & opppieces) != 0 && col != 7)
        {
          states[index] = *this;
          states[index].board[0] &= ~pos;
          states[index].clearPiece(pos >> 9);
          states[index].board[0] |= (pos >> 9);
          index++;
        }
        if (((pos >> 7) & opppieces) != 0 && col != 0)
        {
          states[index] = *this;
          states[index].board[0] &= ~pos;
          states[index].clearPiece(pos >> 7);
          states[index].board[0] |= (pos >> 7);
          index++;
        }
        if (col != 7)
        {
          attacked_squares |= pos >> 9;
        }
        if (col != 0)
        {
          attacked_squares |= pos >> 7;
        }
      }
      else if ((knights & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (col < 7 && row < 6 && ((pos >> 17) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos >> 17);
          states[index].board[1] |= pos >> 17;
          attacked_squares |= pos >> 17;
          index += 1;
        }
        if (col < 6 && row < 7 && ((pos >> 10) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos >> 10);
          states[index].board[1] |= pos >> 10;
          attacked_squares |= pos >> 10;
          index += 1;
        }
        if (col < 6 && row > 0 && ((pos << 6) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos << 6);
          states[index].board[1] |= pos << 6;
          attacked_squares |= pos << 6;
          index += 1;
        }
        if (col < 7 && row > 1 && ((pos << 15) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos << 15);
          states[index].board[1] |= pos << 15;
          attacked_squares |= pos << 15;
          index += 1;
        }
        if (col > 0 && row > 1 && ((pos << 17) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos << 17);
          states[index].board[1] |= pos << 17;
          attacked_squares |= pos << 17;
          index += 1;
        }
        if (col > 1 && row > 0 && ((pos << 10) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos << 10);
          states[index].board[1] |= pos << 10;
          attacked_squares |= pos << 10;
          index += 1;
        }
        if (col > 1 && row < 7 && ((pos >> 6) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos >> 6);
          states[index].board[1] |= pos >> 6;
          attacked_squares |= pos >> 6;
          index += 1;
        }
        if (col > 0 && row < 6 && ((pos >> 15) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[1] &= ~pos;
          states[index].clearPiece(pos >> 15);
          states[index].board[1] |= pos >> 15;
          attacked_squares |= pos >> 15;
          index += 1;
        }
      }
      else if ((bishops & pos) != 0)
      {
        short dir[4] = { 9, -7, -9, 7 };
        for (int j = 0; j < 4; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if ((dir[j] == 9 || dir[j] == -7) && inc2 % 8 == 0)
            {
              break;
            }
            if ((dir[j] == -9 || dir[j] == 7) && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[2] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[2] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[2] &= ~pos;
              states[index].board[2] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((rooks & pos) != 0)
      {
        short dir[4] = { 8,1,-8,-1 };
        for (int j = 0; j < 4; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if (dir[j] == 1 && inc2 % 8 == 0)
            {
              break;
            }
            if (dir[j] == -1 && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[3] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[3] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[3] &= ~pos;
              states[index].board[3] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((queens & pos) != 0)
      {
        short dir[8] = { 8,1,-8,-1, 9, -7, -9, 7 };
        for (int j = 0; j < 8; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if ((dir[j] == 1 || dir[j] == 9 || dir[j] == -7) && inc2 % 8 == 0)
            {
              break;
            }
            if ((dir[j] == -1 || dir[j] == -9 || dir[j] == 7) && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[4] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[4] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[4] &= ~pos;
              states[index].board[4] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((kings & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (!this->white_king_moved)
        {
          // short side castle
          if (((validmoves & ~opppieces) & 0x0600000000000000ull) == 0x0600000000000000ull && (this->attacked_squares & 0x0e00000000000000ull) == 0 &&  this->board[3] & 0x0100000000000000ull)
          {
            states[index] = *this;
            states[index].board[5] &= ~pos;
            states[index].board[5] |= (pos >> 2);
            states[index].board[3] &= ~(pos >> 3);
            states[index].board[3] |= (pos >> 1);
            states[index].white_king_moved = true;
            index++;
          }
          // long side castle
          if (((validmoves & ~opppieces) & 0x7000000000000000ull) == 0x7000000000000000ull && (this->attacked_squares & 0x3800000000000000ull) == 0 && this->board[3] & 0x8000000000000000ull)
          {
            states[index] = *this;
            states[index].board[5] &= ~pos;
            states[index].board[5] |= (pos << 2);
            states[index].board[3] &= ~(pos << 4);
            states[index].board[3] |= (pos << 1);
            states[index].white_king_moved = true;
            index++;
          }
        }
        if (col < 7 && row < 7 && ((pos >> 9) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos >> 9);
          states[index].board[5] |= pos >> 9;
          states[index].white_king_moved = true;
          attacked_squares |= pos >> 9;
          index += 1;
        }
        if (col < 7 && ((pos >> 1) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos >> 1);
          states[index].board[5] |= pos >> 1;
          states[index].white_king_moved = true;
          attacked_squares |= pos >> 1;
          index += 1;
        }
        if (col < 7 && row > 0 && ((pos << 7) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos << 7);
          states[index].board[5] |= pos << 7;
          states[index].white_king_moved = true;
          attacked_squares |= pos << 7;
          index += 1;
        }
        if (row > 0 && ((pos << 8) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos << 8);
          states[index].board[5] |= pos << 8;
          states[index].white_king_moved = true;
          attacked_squares |= pos << 8;
          index += 1;
        }
        if (col > 0 && row > 0 && ((pos << 9) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos << 9);
          states[index].board[5] |= pos << 9;
          states[index].white_king_moved = true;
          attacked_squares |= pos << 9;
          index += 1;
        }
        if (col > 0 && ((pos << 1) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos << 1);
          states[index].board[5] |= pos << 1;
          states[index].white_king_moved = true;
          attacked_squares |= pos << 1;
          index += 1;
        }
        if (col > 0 && row < 7 && ((pos >> 7) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos >> 7);
          states[index].board[5] |= pos >> 7;
          states[index].white_king_moved = true;
          attacked_squares |= pos >> 7;
          index += 1;
        }
        if (row < 7 && ((pos >> 8) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[5] &= ~pos;
          states[index].clearPiece(pos >> 8);
          states[index].board[5] |= pos >> 8;
          states[index].white_king_moved = true;
          attacked_squares |= pos >> 8;
          index += 1;
        }
      }
      inc++;
    }
  }
  else if (turn == -1)
  {
    while (inc < 64)
    {
      pos = 1ull << inc;
      // black pawn moves
      // check if there is a pawn at position
      if ((pawns & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (((pos << 8) & (validmoves & ~opppieces)) != 0)
        {
          if (((pos << 8) & 0xff00000000000000) != 0)
          {
            states[index] = *this;
            states[index].board[6] &= ~pos;
            states[index].board[10] |= pos << 8;
            index += 1;
            states[index] = *this;
            states[index].board[6] &= ~pos;
            states[index].board[10] |= pos << 8;
            index += 1;
          }
          else
          {
            states[index] = *this;
            states[index].board[6] = (board[6] ^ pos) | (pos << 8);
            index += 1;
          }
          if (pos < (1ull << 16) && (((pos << 16) & (validmoves & ~opppieces)) != 0))
          {
            states[index] = *this;
            states[index].board[6] = (board[6] ^ pos) | (pos << 16);
            index += 1;
          }
        }
        if (((pos << 9) & opppieces) != 0 && col != 0)
        {
          states[index] = *this;
          states[index].board[6] &= ~pos;
          states[index].clearPiece(pos << 9);
          states[index].board[6] |= (pos << 9);
          index++;
        }
        if (((pos << 7) & opppieces) != 0 && col != 7)
        {
          states[index] = *this;
          states[index].board[6] &= ~pos;
          states[index].clearPiece(pos << 7);
          states[index].board[6] |= (pos << 7);
          index++;
        }
        if (col != 0)
        {
          attacked_squares |= pos << 9;
        }
        if (col != 7)
        {
          attacked_squares |= pos << 7;
        }
      }
      else if ((knights & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (col < 7 && row < 6 && ((pos >> 17) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos >> 17);
          states[index].board[7] |= pos >> 17;
          attacked_squares |= pos >> 17;
          index += 1;
        }
        if (col < 6 && row < 7 && ((pos >> 10) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos >> 10);
          states[index].board[7] |= pos >> 10;
          attacked_squares |= pos >> 10;
          index += 1;
        }
        if (col < 6 && row > 0 && ((pos << 6) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos << 6);
          states[index].board[7] |= pos << 6;
          attacked_squares |= pos << 6;
          index += 1;
        }
        if (col < 7 && row > 1 && ((pos << 15) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos << 15);
          states[index].board[7] |= pos << 15;
          attacked_squares |= pos << 15;
          index += 1;
        }
        if (col > 0 && row > 1 && ((pos << 17) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos << 17);
          states[index].board[7] |= pos << 17;
          attacked_squares |= pos << 17;
          index += 1;
        }
        if (col > 1 && row > 0 && ((pos << 10) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos << 10);
          states[index].board[7] |= pos << 10;
          attacked_squares |= pos << 10;
          index += 1;
        }
        if (col > 1 && row < 7 && ((pos >> 6) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos >> 6);
          states[index].board[7] |= pos >> 6;
          attacked_squares |= pos >> 6;
          index += 1;
        }
        if (col > 0 && row < 6 && ((pos >> 15) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[7] &= ~pos;
          states[index].clearPiece(pos >> 15);
          states[index].board[7] |= pos >> 15;
          attacked_squares |= pos >> 15;
          index += 1;
        }
      }
      else if ((bishops & pos) != 0)
      {
        short dir[4] = { 9, -7, -9, 7 };
        for (int j = 0; j < 4; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if ((dir[j] == 9 || dir[j] == -7) && inc2 % 8 == 0)
            {
              break;
            }
            if ((dir[j] == -9 || dir[j] == 7) && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[8] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[8] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[8] &= ~pos;
              states[index].board[8] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((rooks & pos) != 0)
      {
        short dir[4] = { 8,1,-8,-1 };
        for (int j = 0; j < 4; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if (dir[j] == 1 && inc2 % 8 == 0)
            {
              break;
            }
            if (dir[j] == -1 && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[9] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[9] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[9] &= ~pos;
              states[index].board[9] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((queens & pos) != 0)
      {
        short dir[8] = { 8,1,-8,-1, 9, -7, -9, 7 };
        for (int j = 0; j < 8; j++)
        {
          short inc2 = inc;
          while (true)
          {
            inc2 += dir[j];
            unsigned long long pos2 = 1ull << inc2;
            if (inc2 < 0 || inc2 >= 64 || (pos2 & validmoves) == 0)
            {
              break;
            }
            if ((dir[j] == 1 || dir[j] == 9 || dir[j] == -7) && inc2 % 8 == 0)
            {
              break;
            }
            if ((dir[j] == -1 || dir[j] == -9 || dir[j] == 7) && inc2 % 8 == 7)
            {
              break;
            }
            if ((pos2 & opppieces) != 0)
            {
              states[index] = *this;
              states[index].board[10] &= ~pos;
              states[index].clearPiece(pos2);
              states[index].board[10] |= pos2;
              attacked_squares |= pos2;
              index++;
              break;
            }
            if ((pos2 & validmoves) != 0)
            {
              states[index] = *this;
              states[index].board[10] &= ~pos;
              states[index].board[10] |= pos2;
              attacked_squares |= pos2;
              index++;
            }
          }
        }
      }
      else if ((kings & pos) != 0)
      {
        short row = 7 - (inc / 8), col = 7 - (inc % 8);
        if (!this->black_king_moved)
        {
          // short side castle
          if (((validmoves & ~opppieces) & 6ull) == 6ull && (this->attacked_squares & 0xeull) == 0 && this->board[9] & 1ull)
          {
            states[index] = *this;
            states[index].board[11] &= ~pos;
            states[index].board[11] |= (pos >> 2);
            states[index].board[9] &= ~1ull;
            states[index].board[9] |= 4ull;
            states[index].black_king_moved = true;
            index++;
          }
          // long side castle
          if (((validmoves & ~opppieces) & 0x70ull) == 0x70ull && (this->attacked_squares & 0x38ull) == 0 && this->board[9] & 0x80ull)
          {
            states[index] = *this;
            states[index].board[11] &= ~pos;
            states[index].board[11] |= (pos << 2);
            states[index].board[9] &= ~0x80ull;
            states[index].board[9] |= 0x10ull;
            states[index].black_king_moved = true;
            index++;
          }
        }
        if (col < 7 && row < 7 && ((pos >> 9) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos >> 9);
          states[index].board[11] |= pos >> 9;
          states[index].black_king_moved = true;
          attacked_squares |= pos >> 9;
          index += 1;
        }
        if (col < 7 && ((pos >> 1) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos >> 1);
          states[index].board[11] |= pos >> 1;
          states[index].black_king_moved = true;
          attacked_squares |= pos >> 1;
          index += 1;
        }
        if (col < 7 && row > 0 && ((pos << 7) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos << 7);
          states[index].board[11] |= pos << 7;
          states[index].black_king_moved = true;
          attacked_squares |= pos << 7;
          index += 1;
        }
        if (row > 0 && ((pos << 8) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos << 8);
          states[index].board[11] |= pos << 8;
          states[index].black_king_moved = true;
          attacked_squares |= pos << 8;
          index += 1;
        }
        if (col > 0 && row > 0 && ((pos << 9) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos << 9);
          states[index].board[11] |= pos << 9;
          states[index].black_king_moved = true;
          attacked_squares |= pos << 9;
          index += 1;
        }
        if (col > 0 && ((pos << 1) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos << 1);
          states[index].board[11] |= pos << 1;
          states[index].black_king_moved = true;
          attacked_squares |= pos << 1;
          index += 1;
        }
        if (col > 0 && row < 7 && ((pos >> 7) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos >> 7);
          states[index].board[11] |= pos >> 7;
          states[index].black_king_moved = true;
          attacked_squares |= pos >> 7;
          index += 1;
        }
        if (row < 7 && ((pos >> 8) & validmoves) != 0)
        {
          states[index] = *this;
          states[index].board[11] &= ~pos;
          states[index].clearPiece(pos >> 8);
          states[index].board[11] |= pos >> 8;
          states[index].black_king_moved = true;
          attacked_squares |= pos >> 8;
          index += 1;
        }
      }
      inc++;
    }
  }
  for (int i = 0; i < index; i++)
  {
    states[i].attacked_squares = attacked_squares;
  }
  *length = index;
  this->move_number--;
  return states;
}

void ChessState::printBoard()
{
  char empty1 = (char)177;
  char empty2 = (char)178;
  char topleft = (char)201;
  char topright = (char)187;
  char botleft = (char)200;
  char botright = (char)188;
  char horizontal = (char)205;
  char vert = (char)186;
  // go through every tile
  std::cout << std::endl;
  std::cout << topleft;
  for (int i = 0; i < 16; i++)
  {
    std::cout << horizontal;
  }
  std::cout << topright;
  for (int i = 7; i >= 0; i--) // row (top row)
  {
    std::cout << std::endl;
    std::cout << vert;
    for (int j = 0; j < 8; j++) // col (left column)
    {
      char temp = ((i + j) % 2 == 0) ? empty1 : empty2;
      for (int piece = 0; piece < 12; piece++)
      {
        if ((board[piece] >> (63 - (i * 8 + j))) % 2 == 1)
        {
          temp = pieces[piece];
        }
      }
      std::cout << temp << ' ';
    }
    std::cout << vert;
    std::cout << i + 1;
  }
  std::cout << std::endl;
  std::cout << botleft;
  for (int i = 0; i < 16; i++)
  {
    std::cout << horizontal;
  }
  std::cout << botright;
  std::cout << std::endl;
  for (int i = 0; i < 8; i++)
  {
    std::cout << " " << char(i + 65);
  }
}

float ChessState::evaluationFunction(ChessState state)
{
  float val = 0;
  // piece values
  val += __popcnt64(state.board[0]);
  val += __popcnt64(state.board[1]) * 3;
  val += __popcnt64(state.board[2]) * 3;
  val += __popcnt64(state.board[3]) * 5;
  val += __popcnt64(state.board[4]) * 9;
  val += __popcnt64(state.board[5]) * 128;
  val -= __popcnt64(state.board[6]) * 1;
  val -= __popcnt64(state.board[7]) * 3;
  val -= __popcnt64(state.board[8]) * 3;
  val -= __popcnt64(state.board[9]) * 5;
  val -= __popcnt64(state.board[10]) * 9;
  val -= __popcnt64(state.board[11]) * 128;
  // pawn position values
  val += __popcnt64(0x0000001818000000 & state.board[0]) * .04f;
  val -= __popcnt64(0x0000001818000000 & state.board[6]) * .04f;
  // knight position values
  val += __popcnt64(0x00003c3c3c3c0000 & state.board[1]) * .02f;
  val -= __popcnt64(0x00003c3c3c3c0000 & state.board[7]) * .02f;
  // bishop pair values
  val += (__popcnt64(state.board[2]) == 2) ? .1f : 0;
  val -= (__popcnt64(state.board[8]) == 2) ? .1f : 0;
  // queen position value
  if (state.move_number < 5)
  {
    val += ((state.board[4] & 0x1000000000000000ull) != 0) ? .1f : 0;
    val += ((state.board[10] & 0x0000000000000010ull) != 0) ? .1f : 0;
  }


  return val;
}

bool operator<(ChessState s1, ChessState s2)
{
  return s1.value < s2.value;
}

bool operator>(ChessState s1, ChessState s2)
{
  return s1.value > s2.value;
}
