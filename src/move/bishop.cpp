#include "board/board_utils.h"
#include <vector>

void generateBishopMoves(const GameState& state, std::vector<Move>& moves) {
  Color currentColor = state.currentTurn;
  U64 ourBishops = state.board.bishops[currentColor];
  U64 ourPieces = state.board.byColor[currentColor];
  U64 opponentPieces = state.board.byColor[(currentColor == WHITE) ? BLACK : WHITE];
  U64 allPieces = state.board.allPieces;

  U64 tempBishops = ourBishops;
  while (tempBishops) {
    Square fromSq = popLSB(tempBishops);

    U64 possibleTargets = getSliderAttacks(fromSq, allPieces, BISHOP);

    possibleTargets &= ~ourPieces; // Remove our own pieces

    U64 tempTargets = possibleTargets;
    while (tempTargets) {
      Square toSq = popLSB(tempTargets);
      bool isCapture = (squareToBitboard(toSq) & opponentPieces) != 0ULL;
      moves.emplace_back(fromSq, toSq, NONE, isCapture);
    }
  }
}