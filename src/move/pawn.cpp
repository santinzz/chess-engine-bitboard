#include <vector>
#include "board/board_utils.h"

inline bool isOccupiedByOpponent(const GameState &state, Square sq, Color friendlyColor)
{
  Color opponentColor = (friendlyColor == WHITE) ? BLACK : WHITE;
  return (state.board.byColor[opponentColor] & squareToBitboard(sq));
}

void generatePawnMoves(const GameState &state, std::vector<Move> &moves)
{
  Color currentColor = state.currentTurn;
  Color opponentColor = (currentColor == WHITE) ? BLACK : WHITE;

  U64 ourPawns = state.board.pawns[currentColor];
  U64 opponentPieces = state.board.byColor[opponentColor];

  int forward = (currentColor == WHITE) ? 8 : -8;  // 8 for white pawns, -8 for black pawns
  int pawnRank2 = (currentColor == WHITE) ? 1 : 6; // Rank 2 for white, Rank 7 for black

  U64 tempPawns = ourPawns;

  while (tempPawns)
  {
    Square sq = popLSB(tempPawns);

    Square targetSq1 = sq + forward;
    if (targetSq1 >= 0 && targetSq1 < 64 && isEmpty(state, targetSq1))
    {
      if ((currentColor == WHITE && getRank(targetSq1) == 7) ||
          (currentColor == BLACK && getRank(targetSq1) == 0))
      {
        moves.emplace_back(sq, targetSq1, QUEEN);
        moves.emplace_back(sq, targetSq1, ROOK);
        moves.emplace_back(sq, targetSq1, BISHOP);
        moves.emplace_back(sq, targetSq1, KNIGHT);
      }
      else
      {
        moves.emplace_back(sq, targetSq1);
      }

      if (getRank(sq) == pawnRank2)
      {
        Square targetSq2 = sq + (forward * 2);
        if (isEmpty(state, targetSq2))
        {
          moves.emplace_back(sq, targetSq2);
        }
      }
    }

    U64 possibleAttacks = PAWN_ATTACK_MASKS[currentColor][sq];
    U64 validCaptures = possibleAttacks & opponentPieces;

    U64 tempCaptures = validCaptures;

    while (tempCaptures)
    {
      Square toSq = popLSB(tempCaptures);

      if ((currentColor == WHITE && getRank(toSq) == 7) ||
          (currentColor == BLACK && getRank(toSq) == 0))
      {
        moves.emplace_back(sq, toSq, QUEEN, true);
        moves.emplace_back(sq, toSq, ROOK, true);
        moves.emplace_back(sq, toSq, BISHOP, true);
        moves.emplace_back(sq, toSq, KNIGHT, true);
      }
      else
      {
        moves.emplace_back(sq, toSq, NONE, true);
      }
    }

    if (state.enPassantSquare != NO_SQUARE) {
      if ( (PAWN_ATTACK_MASKS[currentColor][sq] & squareToBitboard(state.enPassantSquare)) ) {
        moves.emplace_back(sq, state.enPassantSquare, NONE, true, false, true);
      }
    }
  }
}