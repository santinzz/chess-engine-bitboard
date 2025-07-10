#include "board/board_utils.h"
#include <vector>

void generateQueenMoves(const GameState& state, std::vector<Move>& moves) {
  Color currentColor = state.currentTurn;
  U64 ourQueens = state.board.queens[currentColor];
  U64 ourPieces = state.board.byColor[currentColor];
  U64 opponentPieces = state.board.byColor[(currentColor == WHITE) ? BLACK : WHITE];
  U64 allPieces = state.board.allPieces;

  U64 tempQueens = ourQueens;
  while (tempQueens) {
    Square fromSq = popLSB(tempQueens);

    U64 possibleTargets = getSliderAttacks(fromSq, allPieces, QUEEN);

    possibleTargets &= ~ourPieces; // Remove our own pieces

    U64 tempTargets = possibleTargets;
    while (tempTargets) {
      Square toSq = popLSB(tempTargets);
      bool isCapture = (squareToBitboard(toSq) & opponentPieces) != 0ULL;
      moves.emplace_back(fromSq, toSq, NONE, isCapture);
    }
  }
}