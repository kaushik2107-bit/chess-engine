#include "chess.h"

int main() {
    // Chess chess;
    Chess chess("8/8/8/8/8/6k1/5p1K/8 w - - 0 1");
    Move& board = chess.board;

    // board.generate_legal_moves();
    // for (auto move : board.legal_moves) {
    //     std::cout << move << " ";
    // } std:: cout << std::endl;

    // board.push("a7a6");

    // board.generate_legal_moves();
    // for (auto move : board.legal_moves) {
    //     std::cout << move << " ";
    // } std:: cout << std::endl;

    // board.push("f2f1q");

    // std::pair<std::pair<std::string, int>, int> result = chess.analyze(board, 3);
    // std::cout << "Best Move: " << result.first.first << std::endl;
    // std::cout << "Evaluation: " << result.first.second << std::endl;
    // std::cout << "Nodes: " << result.second << std::endl;

    // board.push("d7d8q");

    // std::pair<std::pair<std::string, int>, int> resultt = chess.analyze(board, 3);
    // std::cout << "Best Move: " << resultt.first.first << std::endl;
    // std::cout << "Evaluation: " << resultt.first.second << std::endl;
    // std::cout << "Nodes: " << resultt.second << std::endl;
    
    while (!board.is_checkmate() && !board.is_fifty_moves() && !board.is_insufficient_material() && !board.is_stalemate()) {
        board.print_chessboard(board.bitboard);
        board.generate_pseudo_legal_moves();
        board.generate_legal_moves();
        std::clock_t start = std::clock();
        std::pair<std::pair<std::string, int>, int> result = chess.analyze(board, 40);
        double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

        std::cout << "Best Move: " << result.first.first << std::endl;
        std::cout << "Evaluation: " << result.first.second << std::endl;
        std::cout << "Nodes: " << result.second << std::endl;
        std::cout << "Time: " << duration << " seconds" << std::endl;

        // std::cout << result.first.first << std::endl;

        board.push(result.first.first);
    }

    board.print_chessboard(board.bitboard);

    // chess.analysis.transposition_table.print();

    if (board.is_checkmate()) {
        std::cout << (board.player == 'w' ? "Black Wins" : "White Wins") << std::endl;
    }

    if (board.is_insufficient_material() || board.is_fifty_moves() || board.is_stalemate()) {
        std::cout << "Draw" << std::endl;
    }



    return 0;
}