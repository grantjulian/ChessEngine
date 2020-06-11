#pragma once
class ChessState
{
private:
  char pieces[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};
public:
  float value = 0;
  short move_number = 0;
  unsigned long long board[12] = { 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull };
  unsigned long long attacked_squares = 0ull;
  bool white_king_moved = false;
  bool black_king_moved = false;
  void initializeBoard();
  void clearPiece(unsigned long long pos);
  void placePiece(short id, unsigned long long pos);
  unsigned long long validWhiteMoves();
  unsigned long long validBlackMoves();
  ChessState* generateStates(int turn, int * length);
  void printBoard();
  static float evaluationFunction(ChessState state);
  friend bool operator<(ChessState s1, ChessState s2);
  friend bool operator>(ChessState s1, ChessState s2);
};

