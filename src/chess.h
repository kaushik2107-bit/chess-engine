#ifndef CHESS_H
#define CHESS_H
#include <iostream>
#include <string>
#include <ctime>
#include "bitboard.h"
#include "move.h"
#include "analysis.h"

class Chess {
public:
    std::unordered_map<char, uint64_t> bitboard;
    char player;
    std::string castling_rights;
    std::string en_passant;
    std::string halfmove_clock;
    std::string fullmove_number;
    std::unordered_map<char, std::vector<int>> piece_tracker;
    std::vector<char> square_tracker;
    Move board;
    Analysis analysis;

    Chess(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
        BitboardGenerator bitboardGenerator(fen);
        std::tie(bitboard, player, castling_rights, en_passant, halfmove_clock, fullmove_number, piece_tracker, square_tracker) = bitboardGenerator.get_info();
        std::vector<char> castling_rights_v(castling_rights.begin(), castling_rights.end());
        if (castling_rights == "-") castling_rights_v.clear();    
        int en_passant_v = en_passant == "-" ? -1 : std::stoi(en_passant);
        int halfmove_clock_v = std::stoi(halfmove_clock);
        int fullmove_number_v = std::stoi(fullmove_number);
        board.set_members(bitboard, player, castling_rights_v, en_passant_v, halfmove_clock_v, fullmove_number_v, piece_tracker, square_tracker);
        board.generate_pseudo_legal_moves();
        board.generate_legal_moves();
    }

    std::pair<std::pair<std::string, int>, int> analyze(Move& board, int depth = 3) {
        return analysis.analyze(board, depth);
    }


};


#endif