#ifndef MOVE_H
#define MOVE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <string>
#include <algorithm>
#include <bitset>

class Move {
public:
    std::unordered_map<char, std::vector<int>> piece_tracker;
    std::vector<char> square_tracker;
    std::unordered_map<char, uint64_t> bitboard;
    char player;
    std::vector<char> castling_rights;
    int en_passant_square;
    int halfmove_clock;
    int fullmove_number;
    std::vector<std::string> pseudo_legal_moves;
    std::vector<std::string> legal_moves;
    std::vector<std::string> played_moves;
    std::vector<Move> prev_state;
    Move() {}
    Move(std::unordered_map<char, uint64_t> bitboard, char player, std::vector<char> castling_rights, int en_passant, int halfmove_clock, int fullmove_number, std::unordered_map<char, std::vector<int>> piece_tracker, std::vector<char> square_tracker) :
        bitboard(bitboard),
        player(player),
        castling_rights(castling_rights),
        en_passant_square(en_passant),
        halfmove_clock(halfmove_clock),
        fullmove_number(fullmove_number),
        piece_tracker(piece_tracker),
        square_tracker(square_tracker) {}

    Move(const Move& other) {
        this->bitboard = other.bitboard;
        this->player = other.player;
        this->castling_rights = other.castling_rights;
        this->en_passant_square = other.en_passant_square;
        this->halfmove_clock = other.halfmove_clock;
        this->fullmove_number = other.fullmove_number;
        this->piece_tracker = other.piece_tracker;
        this->square_tracker = other.square_tracker;
        this->prev_state = other.prev_state;
        this->played_moves = other.played_moves;
        this->pseudo_legal_moves = other.pseudo_legal_moves;
        this->legal_moves = other.legal_moves;
    }

    void set_members(std::unordered_map<char, uint64_t> bitboard, char player, std::vector<char> castling_rights, int en_passant, int halfmove_clock, int fullmove_number, std::unordered_map<char, std::vector<int>> piece_tracker, std::vector<char> square_tracker) {
        this->bitboard = bitboard;
        this->player = player;
        this->castling_rights = castling_rights;
        this->en_passant_square = en_passant;
        this->halfmove_clock = halfmove_clock;
        this->fullmove_number = fullmove_number;
        this->piece_tracker = piece_tracker;
        this->square_tracker = square_tracker;
    }

    // Implement other methods and functionalities here
    std::vector<std::string> generate_pseudo_legal_moves(bool threat_check = false) {
        std::vector<std::string> moves;
        char player = (threat_check == false) ? this->player : (this->player == 'b') ? 'w' : 'b';

        if (player == 'w') {
            for (auto& piece : piece_tracker) {
                if (piece.first == 'P') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> pawn_moves = _generate_pawn_moves(piece_pos, player);
                    moves.insert(moves.end(), pawn_moves.begin(), pawn_moves.end());
                } else if (piece.first == 'N') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> knight_moves = _generate_knight_moves(piece_pos, player);
                    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
                } else if (piece.first == 'B') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> bishop_moves = _generate_bishop_moves(piece_pos, player);
                    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
                } else if (piece.first == 'R') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> rook_moves = _generate_rook_moves(piece_pos, player);
                    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
                } else if (piece.first == 'Q') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> queen_moves = _generate_queen_moves(piece_pos, player);
                    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
                } else if (piece.first == 'K') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> king_moves = _generate_king_moves(piece_pos, player);
                    moves.insert(moves.end(), king_moves.begin(), king_moves.end());
                }
            }
        } else if (player == 'b') {
            for (auto& piece : piece_tracker) {
                if (piece.first == 'p') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> pawn_moves = _generate_pawn_moves(piece_pos, player);
                    moves.insert(moves.end(), pawn_moves.begin(), pawn_moves.end());
                } else if (piece.first == 'n') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> knight_moves = _generate_knight_moves(piece_pos, player);
                    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
                } else if (piece.first == 'b') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> bishop_moves = _generate_bishop_moves(piece_pos, player);
                    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
                } else if (piece.first == 'r') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> rook_moves = _generate_rook_moves(piece_pos, player);
                    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
                } else if (piece.first == 'q') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> queen_moves = _generate_queen_moves(piece_pos, player);
                    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
                } else if (piece.first == 'k') {
                    std::vector<int> piece_pos = piece.second;
                    std::vector<std::string> king_moves = _generate_king_moves(piece_pos, player);
                    moves.insert(moves.end(), king_moves.begin(), king_moves.end());
                }
            }
        }

        std::sort(moves.begin(), moves.end());
        moves.erase(std::unique(moves.begin(), moves.end()), moves.end());
        if (!threat_check) {
            this->pseudo_legal_moves = moves;
        }

        return moves;
    }

    bool is_check(bool threat_check = false) {
        std::vector<std::string> list_moves = generate_pseudo_legal_moves(!threat_check);
        char pp = (player == 'w') ? 'K' : 'k';

        if (threat_check) {
            pp = (player == 'w') ? 'k' : 'K';
        }

        std::vector<int> king_pos = this->piece_tracker[pp];

        for (std::string mv : list_moves) {
            // std::cout << convert_to_notation(king_pos[0]) << " " << mv.substr(2, 2) << std::endl;
            if (convert_to_notation(king_pos[0]) == mv.substr(2, 2)) {
                return true;
            }
        }
        
        return false;
    }

    bool is_checkmate() {
        std::vector<std::string> legal_moves = generate_legal_moves();
        if (legal_moves.size() == 0 && is_check()) {
            return true;
        }
        return false;
    }

    bool is_stalemate() {
        std::vector<std::string> legal_moves = generate_legal_moves();
        if (legal_moves.size() == 0 && !is_checkmate()) {
            return true;
        }
        return false;
    }

    bool is_fifty_moves() {
        if (halfmove_clock >= 100) {
            return true;
        }
        return false;
    }

    bool is_promotion(std::string move) {
        if (move[move.size()-1] >= 'a' && move[move.size()-1] <= 'z') return true;
        return false;
    }

    bool is_capture(std::string move) {
        if (move.size() > 4) return false;
        int initial, final; char promotion;
        std::tie(initial, final, promotion) = convert_to_position(move);
        // Old Code
        // for (const auto [piece_type, positions]: piece_tracker) {
        //     if (std::find(positions.begin(), positions.end(), final) != positions.end()) {
        //         return true;
        //     }
        // }

        // Optimized Code
        if (square_tracker[final] != '-') return true;
        return false;
    }

    char get_captured_piece(std::string move) {
        int initial, final; char promotion;
        std::tie(initial, final, promotion) = convert_to_position(move);
        // Old Code
        // for (const auto [piece_type, positions]: piece_tracker) {
        //     if (std::find(positions.begin(), positions.end(), final) != positions.end()) {
        //         return toupper(piece_type);
        //     }
        // }

        // Optimized Code
        if (square_tracker[final] != '-') return toupper(square_tracker[final]);
        return '-';
    }

    void sort_by_capture_value(std::vector<std::string> &capture_moves) {
        std::unordered_map<char, int> material_values{{'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100}};
        std::sort(capture_moves.begin(), capture_moves.end(), [&](const std::string move1, const std::string move2) {
            char captured_piece1 = get_captured_piece(move1);
            char captured_piece2 = get_captured_piece(move2);
            return material_values[captured_piece1] < material_values[captured_piece2];
        });
    }

    void sort_by_promotion_value(std::vector<std::string> &promotion_moves) {
        std::unordered_map<char, int> material_values{{'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100}};
        std::sort(promotion_moves.begin(), promotion_moves.end(), [&](const std::string move1, const std::string move2) {
            // Assign priorities to promotion moves based on their strategic potential
            int promotion_value1 = material_values[toupper(move1[move1.size()-1])];
            int promotion_value2 = material_values[toupper(move2[move2.size()-1])];
            return promotion_value1 > promotion_value2; // Higher value has higher priority
        });
    }

    void sort_by_attacker_count(std::vector<std::string> &remaining_moves) {
        std::unordered_map<std::string, int> frequency_map;
        for (const std::string& str : remaining_moves) {
            if (str.length() >= 2) {
                std::string last_two_chars = str.substr(str.length() - 2);
                frequency_map[last_two_chars]++;
            }
        }

        std::sort(remaining_moves.begin(), remaining_moves.end(), [&](const std::string move1, const std::string move2) {
            std::string last_two_chars1 = move1.substr(move1.length() - 2);
            std::string last_two_chars2 = move2.substr(move2.length() - 2);
            return frequency_map[last_two_chars1] > frequency_map[last_two_chars2]; // Higher count has higher priority
        });
    }

    bool is_insufficient_material() {
        std::vector<char> white_remaining_pcs;
        std::vector<char> black_remaining_pcs;
        
        for (const auto& piece : piece_tracker) {
            char piece_type = piece.first;
            const auto& squares = piece.second;
            
            if (piece_type >= 'A' && piece_type <= 'Z') {
                for (int i = 0; i < squares.size(); i++) {
                    white_remaining_pcs.push_back(piece_type);
                }
            } else if (piece_type >= 'a' && piece_type <= 'z') {
                for (int i = 0; i < squares.size(); i++) {
                    black_remaining_pcs.push_back(piece_type);
                }
            }
        }
        
        // Lone King
        if (white_remaining_pcs == std::vector<char>{'K'} && black_remaining_pcs == std::vector<char>{'k'}) {
            return true;
        }
        
        // King vs King and Knight
        if (white_remaining_pcs == std::vector<char>{'K'} && (black_remaining_pcs == std::vector<char>{'k', 'n'} || black_remaining_pcs == std::vector<char>{'n', 'k'})) {
            return true;
        }
        
        // King and Knight vs King
        if (black_remaining_pcs == std::vector<char>{'k'} && (white_remaining_pcs == std::vector<char>{'K', 'N'} || white_remaining_pcs == std::vector<char>{'N', 'K'})) {
            return true;
        }
        
        // King vs King and Bishop
        if (white_remaining_pcs == std::vector<char>{'K'} && (black_remaining_pcs == std::vector<char>{'k', 'b'} || black_remaining_pcs == std::vector<char>{'b', 'k'})) {
            return true;
        }
        
        // King and Bishop vs King
        if (black_remaining_pcs == std::vector<char>{'k'} && (white_remaining_pcs == std::vector<char>{'K', 'B'} || white_remaining_pcs == std::vector<char>{'B', 'K'})) {
            return true;
        }
        
        return false;
    }

    std::vector<std::string> generate_legal_moves() {
        std::vector<std::string> pseudo_legal_moves = generate_pseudo_legal_moves();
        std::vector<std::string> calc_legal_moves;

        for (const std::string& mv : pseudo_legal_moves) {
            Move move_copy(*this);

            if (get_captured_piece(mv) == 'K' || get_captured_piece(mv) == 'k') continue;

            // Check if it's a king move and the source and destination are apart by 2
            int src, dest;
            std::string promo;
            std::tie(src, dest, promo) = convert_to_position(mv);

            if (std::abs(dest - src) == 2) {
                if (std::count(piece_tracker['k'].begin(), piece_tracker['k'].end(), src) || std::count(piece_tracker['K'].begin(), piece_tracker['K'].end(), src)) {
                    if (move_copy.is_check()) continue;

                    std::string mmv = convert_to_notation(src, (dest > src) ? src + 1 : src - 1);
                    move_copy.push(mmv, true);

                    if (!move_copy.is_check(true)) {
                        calc_legal_moves.push_back(mv);
                    } else {
                        continue;
                    }
                }
            }

            move_copy.push(mv, true);

            if (!move_copy.is_check(true)) {
                calc_legal_moves.push_back(mv);
            }
        }

        std::sort(calc_legal_moves.begin(), calc_legal_moves.end());
        calc_legal_moves.erase(std::unique(calc_legal_moves.begin(), calc_legal_moves.end()), calc_legal_moves.end());

        this->legal_moves = ordered_moves(calc_legal_moves);
        return this->legal_moves;
    }

    std::vector<std::string> ordered_moves(std::vector<std::string> &moves) {
        std::vector<std::string> order_moves;

        std::vector<std::string> capture_moves;
        std::vector<std::string> promotion_moves;
        std::vector<std::string> remaining_moves;

        for (const auto move: moves) {
            if (is_capture(move)) {
                capture_moves.push_back(move);
            } else if (is_promotion(move)) {
                promotion_moves.push_back(move);
            } else {
                remaining_moves.push_back(move);
            }
        }

        sort_by_capture_value(capture_moves);
        sort_by_promotion_value(promotion_moves);
        sort_by_attacker_count(remaining_moves);

        order_moves.insert(order_moves.end(), capture_moves.begin(), capture_moves.end());
        order_moves.insert(order_moves.end(), promotion_moves.begin(), promotion_moves.end());
        order_moves.insert(order_moves.end(), remaining_moves.begin(), remaining_moves.end());

        return order_moves;
    }

    std::string get_fen() {
        std::string fen = "";

        for (int rank = 0; rank < 8; rank++) {
            int empty_count = 0;
            for (int file = 0; file < 8; file++) {
                int square = rank * 8 + file;
                std::string piece;

                for (const auto& entry : bitboard) {
                    const char piece_type = entry.first;
                    uint64_t bitboard = entry.second;
                    if (bitboard & ((uint64_t)1 << square)) {
                        piece = piece_type;
                        break;
                    }
                }

                if (!piece.empty()) {
                    if (empty_count > 0) {
                        fen += std::to_string(empty_count);
                        empty_count = 0;
                    }
                    fen += piece;
                } else {
                    empty_count++;
                }
            }

            if (empty_count > 0) {
                fen += std::to_string(empty_count);
            }

            if (rank < 7) {
                fen += '/';
            }
        }

        fen += " ";
        fen.push_back(player);

        fen += " " + (castling_rights.empty() ? "-" : std::string(castling_rights.begin(), castling_rights.end()));

        fen += " " + (en_passant_square == -1 ? "-" : convert_to_notation(en_passant_square));

        fen += " " + std::to_string(halfmove_clock);

        fen += " " + std::to_string(fullmove_number);

        return fen;
    }

    void push(std::string move, bool threat_check = false ) {
        if (threat_check || std::find(legal_moves.begin(), legal_moves.end(), move) != legal_moves.end()) {
            Move move_copy(*this);
            prev_state.push_back(move_copy);
            int source_square, dest_square;
            char promotion;
            std::tie(source_square, dest_square, promotion) = convert_to_position(move);
            played_moves.push_back(move);

            for (auto& [piece_type, board] : bitboard) {
                if (board & ((uint64_t)1 << source_square)) {
                    // Clearing the piece from the source
                    bitboard[piece_type] &= ~((uint64_t)1 << source_square);
                    piece_tracker[piece_type].erase(find(piece_tracker[piece_type].begin(), piece_tracker[piece_type].end(), source_square));
                    square_tracker[source_square] = '-';

                    if (std::toupper(piece_type) == 'P') {
                        halfmove_clock = -1;
                    }

                    for (auto& [other_piece_type, other_bitboard] : bitboard) {
                        if (other_bitboard & ((uint64_t)1 << dest_square)) {
                            // Removing Castling if rook captured
                            if (other_piece_type == 'R') {
                                if (dest_square == 63 && std::count(castling_rights.begin(), castling_rights.end(), 'K')) {
                                    castling_rights.erase(std::find(castling_rights.begin(), castling_rights.end(), 'K'));
                                } else if (dest_square == 56 && std::count(castling_rights.begin(), castling_rights.end(), 'Q')) {
                                    castling_rights.erase(std::find(castling_rights.begin(), castling_rights.end(), 'Q'));
                                }
                            } else if (other_piece_type == 'r') {
                                if (dest_square == 7 && std::count(castling_rights.begin(), castling_rights.end(), 'k')) {
                                    castling_rights.erase(std::find(castling_rights.begin(), castling_rights.end(), 'k'));
                                } else if (dest_square == 0 && std::count(castling_rights.begin(), castling_rights.end(), 'q')) {
                                    castling_rights.erase(std::find(castling_rights.begin(), castling_rights.end(), 'q'));
                                }
                            }

                            // Clearing the captured piece's square from the bitboard
                            bitboard[other_piece_type] &= ~((uint64_t)1 << dest_square);
                            piece_tracker[other_piece_type].erase(find(piece_tracker[other_piece_type].begin(), piece_tracker[other_piece_type].end(), dest_square));
                            square_tracker[dest_square] = '-';
                            halfmove_clock = -1;
                            break;
                        }
                    }

                    // Making an enPassant move or removing the enpassant pawn
                    if (std::toupper(piece_type) == 'P') {
                        if (dest_square == en_passant_square) {
                            bitboard[piece_type == 'P' ? 'p' : 'P'] &= ~((uint64_t)1 << (en_passant_square + (8 * (piece_type == 'P' ? 1 : -1))));
                            piece_tracker[piece_type == 'P' ? 'p' : 'P'].erase(find(piece_tracker[piece_type == 'P' ? 'p' : 'P'].begin(), piece_tracker[piece_type == 'P' ? 'p' : 'P'].end(), en_passant_square + (8 * (piece_type == 'P' ? 1 : -1))));
                            square_tracker[en_passant_square + (8 * (piece_type == 'P' ? 1 : -1))] = '-';
                        }
                    }

                    // Setting En_passant
                    if (std::toupper(piece_type) == 'P') {
                        if (std::abs(source_square - dest_square) == 16) {
                            en_passant_square = (source_square + dest_square) / 2;
                        } else {
                            en_passant_square = -1;
                        }
                    } else {
                        en_passant_square = -1;
                    }

                    // Castling moves
                    if (std::toupper(piece_type) == 'K') {
                        if (std::abs(source_square - dest_square) == 2) {
                            int rook_source_square, rook_dest_square;
                            if (dest_square > source_square) {
                                rook_source_square = dest_square + 1;
                                rook_dest_square = dest_square - 1;
                            } else {
                                rook_source_square = dest_square - 2;
                                rook_dest_square = dest_square + 1;
                            }

                            // Moving the rook
                            char pc_type = piece_type == 'K' ? 'R' : 'r';
                            uint64_t bitbrd = bitboard[pc_type];
                            if (bitbrd & ((uint64_t)1 << rook_source_square)) {
                                bitboard[pc_type] &= ~((uint64_t)1 << rook_source_square);
                                piece_tracker[pc_type].erase(std::find(piece_tracker[pc_type].begin(), piece_tracker[pc_type].end(), rook_source_square));
                                square_tracker[rook_source_square] = '-';
                                bitboard[pc_type] |= ((uint64_t)1 << rook_dest_square);
                                piece_tracker[pc_type].push_back(rook_dest_square);
                                square_tracker[rook_dest_square] = pc_type;
                            }
                        } else {
                            if (piece_type == 'K') {
                                if (count(castling_rights.begin(), castling_rights.end(), 'K')) {
                                    castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'K'));
                                }
                                if (count(castling_rights.begin(), castling_rights.end(), 'Q')) {
                                    castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'Q'));
                                }
                            } else if (piece_type == 'k') {
                                if (count(castling_rights.begin(), castling_rights.end(), 'k')) {
                                    castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'k'));
                                }
                                if (count(castling_rights.begin(), castling_rights.end(), 'q')) {
                                    castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'q'));
                                }
                            }
                        }
                    }

                    // Removing castling if rook moved
                    if (std::toupper(piece_type) == 'R') {
                        if (piece_type == 'R') {
                            if (source_square == 63 && std::count(castling_rights.begin(), castling_rights.end(), 'K')) {
                                castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'K'));
                            } else if (source_square == 56 && std::count(castling_rights.begin(), castling_rights.end(), 'Q')) {
                                castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'Q'));
                            }
                        } else if (piece_type == 'r') {
                            if (source_square == 7 && std::count(castling_rights.begin(), castling_rights.end(), 'k')) {
                                castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'k'));
                            } else if (source_square == 0 && std::count(castling_rights.begin(), castling_rights.end(), 'q')) {
                                castling_rights.erase(find(castling_rights.begin(), castling_rights.end(), 'q'));
                            }
                        }
                    }

                    if (std::toupper(piece_type) == 'P' && promotion) {
                        // Making a promotion
                        bitboard[piece_type == 'P' ? toupper(promotion) : promotion] |= ((uint64_t)1 << dest_square);
                        piece_tracker[piece_type == 'P' ? toupper(promotion) : promotion].push_back(dest_square);
                        square_tracker[dest_square] = piece_type == 'P' ? toupper(promotion) : promotion;
                    } else {
                        // Moving the piece to destination
                        bitboard[piece_type] |= ((uint64_t)1 << dest_square);
                        piece_tracker[piece_type].push_back(dest_square);
                        square_tracker[dest_square] = piece_type;
                    }

                    fullmove_number += (player == 'b') ? 1 : 0;
                    halfmove_clock += 1;
                    player = (player == 'w') ? 'b' : 'w';
                    break;
                }
            }
        } else {
            throw std::invalid_argument("Invalid move");
        }
    }

    void pop() {
        if (played_moves.size() == 0) {
            throw std::out_of_range("Invalid pop operation");
        }

        played_moves.pop_back();
        Move copied_self(prev_state.back());
        // prev_state.pop_back();

        // Copying member variables
        piece_tracker = copied_self.piece_tracker;
        square_tracker = copied_self.square_tracker;
        bitboard = copied_self.bitboard;
        player = copied_self.player;
        castling_rights = copied_self.castling_rights;
        en_passant_square = copied_self.en_passant_square;
        halfmove_clock = copied_self.halfmove_clock;
        fullmove_number = copied_self.fullmove_number;
        pseudo_legal_moves = copied_self.pseudo_legal_moves;
        legal_moves = copied_self.legal_moves;
        played_moves = copied_self.played_moves;
        prev_state = copied_self.prev_state;
    }

    bool has_piece(int pos) {
        // Old code
        // for (const auto& pair : this->piece_tracker) {
        //     if (std::find(pair.second.begin(), pair.second.end(), pos) != pair.second.end()) {
        //         return true;
        //     }
        // }

        // Optimized code
        if (square_tracker[pos] != '-') return true;
        return false;
    }

    bool has_enemy_piece(int pos, char color) {
        char startRange = (color == 'b') ? 'A' : 'a';
        char endRange = (color == 'b') ? 'Z' : 'z';

        // Old code
        // for (const auto& pair : piece_tracker) {
        //     char piece = pair.first;
        //     if (piece >= startRange && piece <= endRange) {
        //         if (std::find(pair.second.begin(), pair.second.end(), pos) != pair.second.end()) {
        //             return true;
        //         }
        //     }
        // }

        // Optimized Code
        if (square_tracker[pos] != '-' && square_tracker[pos] >= startRange && square_tracker[pos] <= endRange) return true;
        return false;
    }

    static std::tuple<int, int, char> convert_to_position(std::string move) {
        char promotion = '\0';
        if (move.length() == 5) {
            promotion = move[4];
        }

        int initial_col = move[0] - 'a';
        int initial_row = move[1] - '1';

        int final_col = move[2] - 'a';
        int final_row = move[3] - '1';

        int initial_pos = (7 - initial_row) * 8 + initial_col;
        int final_pos = (7 - final_row) * 8 + final_col;

        return std::make_tuple(initial_pos, final_pos, promotion);
    }

    static std::string convert_to_notation(int initial, int final = -1, char promotion = '\0') {
        int initial_row = 7 - initial / 8;
        int initial_col = initial % 8;
        char file_notation0 = 'a' + initial_col;
        char rank_notation0 = '1' + initial_row;

        if (final == -1) {
            std::string notation;
            notation.push_back(file_notation0);
            notation.push_back(rank_notation0);
            return notation;
        }

        int final_row = 7 - final / 8;
        int final_col = final % 8;
        char file_notation1 = 'a' + final_col;
        char rank_notation1 = '1' + final_row;

        std::string notation;
        notation.push_back(file_notation0);
        notation.push_back(rank_notation0);
        notation.push_back(file_notation1);
        notation.push_back(rank_notation1);

        if (promotion != '\0') {
            notation.push_back(promotion);
        }

        return notation;
    }

    static void print_chessboard(const std::unordered_map<char, uint64_t>& bitboards) {
        std::unordered_map<char, std::string> piece_symbols = {
            {'p', "♙"}, {'n', "♘"}, {'b', "♗"}, {'r', "♖"}, {'q', "♕"}, {'k', "♔"},
            {'P', "♟"}, {'N', "♞"}, {'B', "♝"}, {'R', "♜"}, {'Q', "♛"}, {'K', "♚"}
        };

        std::vector<std::vector<std::string>> board(8, std::vector<std::string>(8, "."));

        for (const auto& pair : bitboards) {
            char piece_type = pair.first;
            unsigned long long bitboard = pair.second;

            for (int square = 0; square < 64; ++square) {
                if (bitboard & ((uint64_t)1 << square)) {
                    int row = square / 8;
                    int col = square % 8;
                    board[row][col] = piece_symbols[piece_type];
                }
            }
        }

        for (const auto& row : board) {
            for (const std::string& square : row) {
                std::cout << square << ' ';
            }
            std::cout << std::endl;
        }
    }

    std::vector<std::string> _generate_pawn_moves(const std::vector<int>& piece_pos, char player) {
        std::vector<std::string> moves;
        std::vector<char> pr = { 'b', 'n', 'r', 'q' };

        if (player == 'w') {
            for (int pos : piece_pos) {
                // One step pawn move
                if (!has_piece(pos - 8)) {
                    // Promotion
                    if (pos >= 8 && pos < 16) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos - 8, pc));
                        }
                    }
                    // Normal One step move
                    else {
                        moves.push_back(convert_to_notation(pos, pos - 8));
                    }
                }

                // Two step pawn move
                if (pos >= 48 && pos < 56 && !has_piece(pos - 8) && !has_piece(pos - 16)) {
                    moves.push_back(convert_to_notation(pos, pos - 16));
                }

                // Capture moves
                if (pos % 8 != 7 && has_piece(pos - 8 + 1) && has_enemy_piece(pos - 8 + 1, player)) {
                    if (pos >= 8 && pos < 16) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos - 8 + 1, pc));
                        }
                    }
                    else {
                        moves.push_back(convert_to_notation(pos, pos - 8 + 1));
                    }
                }
                if (pos % 8 != 0 && has_piece(pos - 8 - 1) && has_enemy_piece(pos - 8 - 1, player)) {
                    if (pos >= 8 && pos < 16) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos - 8 - 1, pc));
                        }
                    }
                    else {
                        moves.push_back(convert_to_notation(pos, pos - 8 - 1));
                    }
                }

                // EnPasant moves
                if (pos >= 24 && pos < 32) {
                    if (en_passant_square == pos - 8 + 1) {
                        moves.push_back(convert_to_notation(pos, pos - 8 + 1));
                    }
                    else if (en_passant_square == pos - 8 - 1) {
                        moves.push_back(convert_to_notation(pos, pos - 8 - 1));
                    }
                }
            }
        }
        else if (player == 'b') {
            for (int pos : piece_pos) {
                // One step pawn move
                if (!has_piece(pos + 8)) {
                    // Promotion
                    if (pos >= 48 && pos < 56) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos + 8, pc));
                        }
                    }
                    // Normal One step move
                    else {
                        moves.push_back(convert_to_notation(pos, pos + 8));
                    }
                }

                // Two step pawn move
                if (pos >= 8 && pos < 16 && !has_piece(pos + 8) && !has_piece(pos + 16)) {
                    moves.push_back(convert_to_notation(pos, pos + 16));
                }

                // Capture moves
                if (pos % 8 != 7 && has_piece(pos + 8 + 1) && has_enemy_piece(pos + 8 + 1, player)) {
                    // And Promotion
                    if (pos >= 48 && pos < 56) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos + 8 + 1, pc));
                        }
                    }
                    else {
                        moves.push_back(convert_to_notation(pos, pos + 8 + 1));
                    }
                }
                if (pos % 8 != 0 && has_piece(pos + 8 - 1) && has_enemy_piece(pos + 8 - 1, player)) {
                    // And Promotion
                    if (pos >= 48 && pos < 56) {
                        for (char pc : pr) {
                            moves.push_back(convert_to_notation(pos, pos + 8 - 1, pc));
                        }
                    }
                    else {
                        moves.push_back(convert_to_notation(pos, pos + 8 - 1));
                    }
                }

                // EnPasant moves
                if (pos >= 32 && pos < 40) {
                    if (en_passant_square == pos + 8 + 1) {
                        moves.push_back(convert_to_notation(pos, pos + 8 + 1));
                    }
                    else if (en_passant_square == pos + 8 - 1) {
                        moves.push_back(convert_to_notation(pos, pos + 8 - 1));
                    }
                }
            }
        }

        return moves;
    }

    std::vector<std::string> _generate_knight_moves(const std::vector<int>& piece_pos, char player) {
        std::vector<std::string> moves;

        for (int pos : piece_pos) {
            // Top
            if (pos / 8 >= 2) {
                // Top Left
                if (pos % 8 >= 1) {
                    if (has_piece(pos - 16 - 1)) {
                        if (has_enemy_piece(pos - 16 - 1, player)) {
                            moves.push_back(convert_to_notation(pos, pos - 16 - 1));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos - 16 - 1));
                    }
                }
                // Top Right
                if (pos % 8 < 7) {
                    if (has_piece(pos - 16 + 1)) {
                        if (has_enemy_piece(pos - 16 + 1, player)) {
                            moves.push_back(convert_to_notation(pos, pos - 16 + 1));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos - 16 + 1));
                    }
                }
            }
            // Bottom
            if (pos / 8 < 6) {
                // Bottom Left
                if (pos % 8 >= 1) {
                    if (has_piece(pos + 16 - 1)) {
                        if (has_enemy_piece(pos + 16 - 1, player)) {
                            moves.push_back(convert_to_notation(pos, pos + 16 - 1));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos + 16 - 1));
                    }
                }
                // Bottom Right
                if (pos % 8 < 7) {
                    if (has_piece(pos + 16 + 1)) {
                        if (has_enemy_piece(pos + 16 + 1, player)) {
                            moves.push_back(convert_to_notation(pos, pos + 16 + 1));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos + 16 + 1));
                    }
                }
            }
            // Left
            if (pos % 8 >= 2) {
                // Left Top
                if (pos / 8 >= 1) {
                    if (has_piece(pos - 2 - 8)) {
                        if (has_enemy_piece(pos - 2 - 8, player)) {
                            moves.push_back(convert_to_notation(pos, pos - 2 - 8));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos - 2 - 8));
                    }
                }
                // Left Bottom
                if (pos / 8 < 7) {
                    if (has_piece(pos - 2 + 8)) {
                        if (has_enemy_piece(pos - 2 + 8, player)) {
                            moves.push_back(convert_to_notation(pos, pos - 2 + 8));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos - 2 + 8));
                    }
                }
            }
            // Right
            if (pos % 8 < 6) {
                // Right Top
                if (pos / 8 >= 1) {
                    if (has_piece(pos + 2 - 8)) {
                        if (has_enemy_piece(pos + 2 - 8, player)) {
                            moves.push_back(convert_to_notation(pos, pos + 2 - 8));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos + 2 - 8));
                    }
                }
                // Right Bottom
                if (pos / 8 < 7) {
                    if (has_piece(pos + 2 + 8)) {
                        if (has_enemy_piece(pos + 2 + 8, player)) {
                            moves.push_back(convert_to_notation(pos, pos + 2 + 8));
                        }
                    } else {
                        moves.push_back(convert_to_notation(pos, pos + 2 + 8));
                    }
                }
            }
        }

        return moves;
    }

    std::vector<std::string> _generate_bishop_moves(const std::vector<int>& piece_pos, char player) {
        std::vector<std::string> moves;

        for (int pos : piece_pos) {
            // Top Left
            int curr_pos = pos;
            while (curr_pos % 8 != 0 && curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8 - 1)) {
                    if (has_enemy_piece(curr_pos - 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8 - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8 - 1));
                }
                curr_pos = curr_pos - 8 - 1;
            }

            // Top Right
            curr_pos = pos;
            while (curr_pos % 8 != 7 && curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8 + 1)) {
                    if (has_enemy_piece(curr_pos - 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8 + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8 + 1));
                }
                curr_pos = curr_pos - 8 + 1;
            }

            // Bottom Left
            curr_pos = pos;
            while (curr_pos % 8 != 0 && curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8 - 1)) {
                    if (has_enemy_piece(curr_pos + 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8 - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8 - 1));
                }
                curr_pos = curr_pos + 8 - 1;
            }

            // Bottom Right
            curr_pos = pos;
            while (curr_pos % 8 != 7 && curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8 + 1)) {
                    if (has_enemy_piece(curr_pos + 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8 + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8 + 1));
                }
                curr_pos = curr_pos + 8 + 1;
            }
        }

        return moves;
    }

    std::vector<std::string> _generate_rook_moves(std::vector<int> piece_pos, char player) {
        std::vector<std::string> moves;

        for (int pos : piece_pos) {
            // Top
            int curr_pos = pos;
            while (curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8)) {
                    if (has_enemy_piece(curr_pos - 8, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8));
                }
                curr_pos -= 8;
            }

            // Bottom
            curr_pos = pos;
            while (curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8)) {
                    if (has_enemy_piece(curr_pos + 8, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8));
                }
                curr_pos += 8;
            }

            // Left
            curr_pos = pos;
            while (curr_pos % 8 != 0) {
                if (has_piece(curr_pos - 1)) {
                    if (has_enemy_piece(curr_pos - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 1));
                }
                curr_pos -= 1;
            }

            // Right
            curr_pos = pos;
            while (curr_pos % 8 != 7) {
                if (has_piece(curr_pos + 1)) {
                    if (has_enemy_piece(curr_pos + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 1));
                }
                curr_pos += 1;
            }
        }

        return moves;
    }

    std::vector<std::string> _generate_queen_moves(std::vector<int> piece_pos, char player) {
        std::vector<std::string> moves;

        for (int pos : piece_pos) {
            // Top
            int curr_pos = pos;
            while (curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8)) {
                    if (has_enemy_piece(curr_pos - 8, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8));
                }
                curr_pos -= 8;
            }

            // Bottom
            curr_pos = pos;
            while (curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8)) {
                    if (has_enemy_piece(curr_pos + 8, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8));
                }
                curr_pos += 8;
            }

            // Left
            curr_pos = pos;
            while (curr_pos % 8 != 0) {
                if (has_piece(curr_pos - 1)) {
                    if (has_enemy_piece(curr_pos - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 1));
                }
                curr_pos -= 1;
            }

            // Right
            curr_pos = pos;
            while (curr_pos % 8 != 7) {
                if (has_piece(curr_pos + 1)) {
                    if (has_enemy_piece(curr_pos + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 1));
                }
                curr_pos += 1;
            }

            // Top Left
            curr_pos = pos;
            while (curr_pos % 8 != 0 && curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8 - 1)) {
                    if (has_enemy_piece(curr_pos - 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8 - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8 - 1));
                }
                curr_pos = curr_pos - 8 - 1;
            }

            // Top Right
            curr_pos = pos;
            while (curr_pos % 8 != 7 && curr_pos / 8 != 0) {
                if (has_piece(curr_pos - 8 + 1)) {
                    if (has_enemy_piece(curr_pos - 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos - 8 + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos - 8 + 1));
                }
                curr_pos = curr_pos - 8 + 1;
            }

            // Bottom Left
            curr_pos = pos;
            while (curr_pos % 8 != 0 && curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8 - 1)) {
                    if (has_enemy_piece(curr_pos + 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8 - 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8 - 1));
                }
                curr_pos = curr_pos + 8 - 1;
            }

            // Bottom Right
            curr_pos = pos;
            while (curr_pos % 8 != 7 && curr_pos / 8 != 7) {
                if (has_piece(curr_pos + 8 + 1)) {
                    if (has_enemy_piece(curr_pos + 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, curr_pos + 8 + 1));
                    }
                    break;
                } else {
                    moves.push_back(convert_to_notation(pos, curr_pos + 8 + 1));
                }
                curr_pos = curr_pos + 8 + 1;
            }
        }

        return moves;
    }

    std::vector<std::string> _generate_king_moves(std::vector<int> piece_pos, char player) {
        std::vector<std::string> moves;

        for (int pos : piece_pos) {
            // Top
            if (pos / 8 != 0) {
                if (has_piece(pos - 8)) {
                    if (has_enemy_piece(pos - 8, player)) {
                        moves.push_back(convert_to_notation(pos, pos - 8));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos - 8));
                }
            }

            // Bottom
            if (pos / 8 != 7) {
                if (has_piece(pos + 8)) {
                    if (has_enemy_piece(pos + 8, player)) {
                        moves.push_back(convert_to_notation(pos, pos + 8));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos + 8));
                }
            }

            // Left
            if (pos % 8 != 0) {
                if (has_piece(pos - 1)) {
                    if (has_enemy_piece(pos - 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos - 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos - 1));
                }
            }

            // Right
            if (pos % 8 != 7) {
                if (has_piece(pos + 1)) {
                    if (has_enemy_piece(pos + 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos + 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos + 1));
                }
            }

            // Top Left
            if (pos % 8 != 0 && pos / 8 != 0) {
                if (has_piece(pos - 8 - 1)) {
                    if (has_enemy_piece(pos - 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos - 8 - 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos - 8 - 1));
                }
            }

            // Top Right
            if (pos % 8 != 7 && pos / 8 != 0) {
                if (has_piece(pos - 8 + 1)) {
                    if (has_enemy_piece(pos - 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos - 8 + 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos - 8 + 1));
                }
            }

            // Bottom Left
            if (pos % 8 != 0 && pos / 8 != 7) {
                if (has_piece(pos + 8 - 1)) {
                    if (has_enemy_piece(pos + 8 - 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos + 8 - 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos + 8 - 1));
                }
            }

            // Bottom Right
            if (pos % 8 != 7 && pos / 8 != 7) {
                if (has_piece(pos + 8 + 1)) {
                    if (has_enemy_piece(pos + 8 + 1, player)) {
                        moves.push_back(convert_to_notation(pos, pos + 8 + 1));
                    }
                } else {
                    moves.push_back(convert_to_notation(pos, pos + 8 + 1));
                }
            }

            // Castling
            if (player == 'w') {
                if (std::find(castling_rights.begin(), castling_rights.end(), 'K') != castling_rights.end()) {
                    if (!has_piece(62) && !has_piece(61)) {
                        moves.push_back(convert_to_notation(pos, pos + 2));
                    }
                }
                if (std::find(castling_rights.begin(), castling_rights.end(), 'Q') != castling_rights.end()) {
                    if (!has_piece(59) && !has_piece(58)) {
                        moves.push_back(convert_to_notation(pos, pos - 2));
                    }
                }
            } else if (player == 'b') {
                if (std::find(castling_rights.begin(), castling_rights.end(), 'k') != castling_rights.end()) {
                    if (!has_piece(5) && !has_piece(6)) {
                        moves.push_back(convert_to_notation(pos, pos + 2));
                    }
                }
                if (std::find(castling_rights.begin(), castling_rights.end(), 'q') != castling_rights.end()) {
                    if (!has_piece(3) && !has_piece(2)) {
                        moves.push_back(convert_to_notation(pos, pos - 2));
                    }
                }
            }
        }

        return moves;
    }

};

// int main() {
//     std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//     // std::string fen = "rnbqkbnr/ppp1ppp1/7p/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1"
//     BitboardGenerator bitboardGenerator(fen);
//     std::unordered_map<char, uint64_t> bitboard;
//     char player;
//     std::string castling_rights;
//     std::string en_passant;
//     std::string halfmove_clock;
//     std::string fullmove_number;
//     std::unordered_map<char, std::vector<int>> piece_tracker;

//     std::tie(bitboard, player, castling_rights, en_passant, halfmove_clock, fullmove_number, piece_tracker) = bitboardGenerator.get_info();
//     std::vector<char> castling_rights_v(castling_rights.begin(), castling_rights.end());
//     if (castling_rights == "-") castling_rights_v.clear();    
//     int en_passant_v = en_passant == "-" ? -1 : std::stoi(en_passant);
//     int halfmove_clock_v = std::stoi(halfmove_clock);
//     int fullmove_number_v = std::stoi(fullmove_number);
//     Move move(bitboard, player, castling_rights_v, en_passant_v, halfmove_clock_v, fullmove_number_v, piece_tracker);

//     std::cout << move.get_fen() << std::endl;
//     move.print_chessboard(move.bitboard);
//     move.generate_legal_moves();
//     move.push("d2d4");
//     std::cout << move.get_fen() << std::endl;
//     for (const auto& played_move : move.played_moves) {
//         std::cout << played_move << " ";
//     }
//     std::cout << std::endl;
//     move.print_chessboard(move.bitboard);
//     move.generate_legal_moves();
//     move.push("e7e5");
//     std::cout << move.get_fen() << std::endl;
//     for (const auto& played_move : move.played_moves) {
//         std::cout << played_move << " ";
//     }
//     std::cout << std::endl;
//     move.print_chessboard(move.bitboard);

//     move.pop();
//     std::cout << move.get_fen() << std::endl;
//     for (const auto& played_move : move.played_moves) {
//         std::cout << played_move << " ";
//     }
//     std::cout << std::endl;
//     move.print_chessboard(move.bitboard);

//     return 0;
// }


#endif