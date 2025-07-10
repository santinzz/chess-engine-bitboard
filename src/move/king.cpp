
#include <vector>
#include "board/board_utils.h"

void generateKingMoves(const GameState &state, std::vector<Move> &moves)
{
  Color currentColor = state.currentTurn;
  U64 ourKing = state.board.kings[currentColor];

  if (ourKing == 0ULL)
    return;

  Square fromSq = getLSB(ourKing);
  U64 ourPieces = state.board.byColor[currentColor];
  U64 opponentPieces = state.board.byColor[(currentColor == WHITE) ? BLACK : WHITE];

  U64 possibleTargets = KING_ATTACK_MASKS[fromSq];

  possibleTargets &= ~ourPieces;

  U64 tempTargets = possibleTargets;
  while (tempTargets)
  {
    Square toSq = popLSB(tempTargets);
    bool isCapture = (squareToBitboard(toSq) & opponentPieces) != 0ULL;
    moves.emplace_back(fromSq, toSq, NONE, isCapture);
  }

  if (currentColor == WHITE && (state.castlingRights & CastlingRights::WK))
  {
    if (isEmpty(state, F1) && isEmpty(state, G1) &&
        (state.board.rooks[WHITE] & squareToBitboard(H1)))
    {
      moves.emplace_back(E1, G1, NONE, false, false, true); // Kingside castling
    }
  }

  if (currentColor == WHITE && (state.castlingRights & CastlingRights::WQ))
  {
    if (isEmpty(state, B1) && isEmpty(state, C1) && isEmpty(state, D1) &&
        (state.board.rooks[WHITE] & squareToBitboard(A1)))
    { // Check if A1 rook exists
      // Further checks needed: no pieces on B1, C1, D1. No attacks on E1, D1, C1.
      moves.emplace_back(E1, C1, NONE, false, true);
    }
  }
  if (currentColor == BLACK && (state.castlingRights & CastlingRights::BK))
  {
    if (isEmpty(state, F8) && isEmpty(state, G8) &&
        (state.board.rooks[BLACK] & squareToBitboard(H8)))
    { // Check if H8 rook exists
      // Further checks needed: no pieces on F8, G8. No attacks on E8, F8, G8.
      moves.emplace_back(E8, G8, NONE, false, true);
    }
  }
  // Black Queenside Castling (E8 to C8)
  if (currentColor == BLACK && (state.castlingRights & CastlingRights::BQ))
  {
    if (isEmpty(state, B8) && isEmpty(state, C8) && isEmpty(state, D8) &&
        (state.board.rooks[BLACK] & squareToBitboard(A8)))
    { // Check if A8 rook exists
      // Further checks needed: no pieces on B8, C8, D8. No attacks on E8, D8, C8.
      moves.emplace_back(E8, C8, NONE, false, true);
    }
  }
}
