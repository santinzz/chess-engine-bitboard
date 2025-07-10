#include "board/board_utils.h"
#include <vector>

void generateRookMoves(const GameState& state, std::vector<Move>& moves) {
  Color currentColor = state.currentTurn;
  U64 ourRooks = state.board.rooks[currentColor];
  U64 ourPieces = state.board.byColor[currentColor];
  U64 opponentPieces = state.board.byColor[(currentColor == WHITE) ? BLACK : WHITE];
  U64 allPieces = state.board.allPieces;

  U64 tempRooks = ourRooks;
  while (tempRooks) {
    Square fromSq = popLSB(tempRooks);

    U64 possibleTargets = getSliderAttacks(fromSq, allPieces, ROOK);

    possibleTargets &= ~ourPieces; // Remove our own pieces

    U64 tempTargets = possibleTargets;
    while (tempTargets) {
      Square toSq = popLSB(tempTargets);
      bool isCapture = (squareToBitboard(toSq) & opponentPieces) != 0ULL;
      moves.emplace_back(fromSq, toSq, NONE, isCapture);
    }
  }
}