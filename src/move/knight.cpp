#include <vector>
#include "board/board_utils.h"

void generateKnightMoves(const GameState &state, std::vector<Move> &moves) {
  Color currentColor = state.currentTurn;
  U64 ourKnights = state.board.knights[currentColor];
  U64 ourPieces = state.board.byColor[currentColor];
  U64 opponentPieces = state.board.byColor[(currentColor == WHITE) ? BLACK : WHITE];

  U64 tempKnights = ourKnights;
  while (tempKnights) {
    Square fromSq = popLSB(tempKnights);

    U64 possibleTargets = KNIGHT_ATTACK_MASKS[fromSq];

    possibleTargets &= ~ourPieces;

    U64 tempTargets = possibleTargets;
    while (tempTargets) {
      Square toSq = popLSB(tempTargets);
      bool isCapture = (squareToBitboard(toSq) & opponentPieces) != 0ULL;
      moves.emplace_back(fromSq, toSq, NONE, isCapture);
    }
  }
}
