#ifndef BITBOARD_H
#define BITBOARD_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>

class BitboardGenerator {
private:
public:
    std::string fen;
    std::unordered_map<char, std::vector<int>> piece_tracker;
    std::unordered_map<char, uint64_t> bitboards;

    BitboardGenerator(const std::string& fen) : fen(fen) {
        std::vector<char> pcs = {'Q', 'K', 'R', 'B', 'N', 'P', 'q', 'k', 'r', 'b', 'n', 'p'};
        for (char pc : pcs) {
            piece_tracker[pc] = {};
            bitboards[pc] = 0;
        }
        generate_bitboards();
    }

    void generate_bitboards() {
        std::vector<std::string> parts;
        std::stringstream ss(fen);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            parts.push_back(token);
        }

        std::string position = parts[0];
        int rank = 7, file = 0;

        for (char c : position) {
            if (c > '0' && c <= '8') {
                file += (c - '0');
            } else if (c == '/') {
                rank--;
                file = 0;
            } else {
                uint64_t bitboard = 1ULL << ((7 - rank) * 8 + file);
                char piece = c;

                piece_tracker[piece].push_back((7 - rank) * 8 + file);
                bitboards[piece] |= bitboard;

                file++;
            }
        }
    }

    std::unordered_map<char, uint64_t> get_bitboard() {
        return bitboards;
    }

    std::tuple<std::unordered_map<char, uint64_t>, char, std::string, std::string, std::string, std::string, std::unordered_map<char, std::vector<int>>> get_info() {
        std::vector<std::string> parts;
        std::stringstream ss(fen);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            parts.push_back(token);
        }

        std::unordered_map<char, uint64_t> bitboard = get_bitboard();
        char player = parts[1][0];
        std::string castling_rights = parts[2];
        std::string en_passant = parts[3];
        std::string halfmove_clock = parts[4];
        std::string fullmove_number = parts[5];
        std::unordered_map<char, std::vector<int>> piece_tracker_copy = piece_tracker;

        return std::make_tuple(bitboard, player, castling_rights, en_passant, halfmove_clock, fullmove_number, piece_tracker_copy);
    }
};

// int main() {
//     std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//     BitboardGenerator board(fen);
//     std::unordered_map<char, std::vector<int>> piece_tracker = board.piece_tracker;

//     for (const auto& pair : piece_tracker) {
//         char piece = pair.first;
//         std::cout << "Piece: " << piece << ", Bitboard: " << std::bitset<64>(board.bitboards[piece]) << std::endl;
//     }

//     return 0;
// }


#endif