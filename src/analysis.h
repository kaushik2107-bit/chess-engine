#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "const.h"
#include "move.h"
#include "transposition.h"
#include <future>
#include <vector>
#include <thread>
const int NUM_THREADS = 4;

int NODECOUNT = 0;
class Analysis {
public:
    std::pair<std::pair<std::string, int>, int> result;
    TranspositionTable transposition_table;
    Move board;
    int depth;

    std::pair<std::pair<std::string, int>, int> analyze(Move board, int depth) {
        NODECOUNT = 0;
        this->board = board; this->depth = depth;
        result = make_pair(iterativeDeepening(depth, board, board.player == 'w', 100), NODECOUNT);
        return result;
    }

    int evaluate(Move& board) {
        int white_score = 0;
        int black_score = 0;

        // 1. Material Imbalance
        for (const auto& entry : board.piece_tracker) {
            char piece_type = entry.first;
            const std::vector<int>& positions = entry.second;
            int number = positions.size();
            if (piece_type >= 'A' && piece_type <= 'Z') {
                white_score += piece_values[piece_type] * number;
            } else if (piece_type >= 'a' && piece_type <= 'z') {
                black_score += piece_values[piece_type] * number;
            }
        }

        // 2. Piece Square Tables
        for (const auto& entry : board.piece_tracker) {
            char piece_type = entry.first;
            const std::vector<int>& positions = entry.second;
            if (piece_type >= 'A' && piece_type <= 'Z') {
                for (int position : positions) {
                    white_score += piece_square_tables[piece_type][position];
                }
            } else if (piece_type >= 'a' && piece_type <= 'z') {
                for (int position : positions) {
                    black_score += piece_square_tables[piece_type][position];
                }
            }
        }

        // 3. Mobility of all pieces
        std::vector<std::string> lmw = board.generate_legal_moves();
        board.player = (board.player == 'b') ? 'w' : 'b';
        std::vector<std::string> lmb = board.generate_legal_moves();
        board.player = (board.player == 'b') ? 'w' : 'b';

        for (const auto& entry : board.piece_tracker) {
            char piece_type = entry.first;
            const std::vector<int>& positions = entry.second;
            if (piece_type >= 'A' && piece_type <= 'Z') {
                for (int position : positions) {
                    std::string curr_pos = Move::convert_to_notation(position);
                    std::vector<std::string> xx;
                    for (const std::string& move : lmw) {
                        if (move.substr(0, 2) == curr_pos) {
                            xx.push_back(move);
                        }
                    }
                    white_score += xx.size();
                }
            } else if (piece_type >= 'a' && piece_type <= 'z') {
                for (int position : positions) {
                    std::string curr_pos = Move::convert_to_notation(position);
                    std::vector<std::string> xx;
                    for (const std::string& move : lmb) {
                        if (move.substr(0, 2) == curr_pos) {
                            xx.push_back(move);
                        }
                    }
                    black_score += xx.size();
                }
            }
        }

        // 4. King Safety
        std::pair<int, int> safety_scores = evaluate_king_safety();
        int white_safety_score = safety_scores.first;
        int black_safety_score = safety_scores.second;
        white_score += white_safety_score;
        black_score += black_safety_score;

        return white_score - black_score;
    }

    std::pair<int, int> evaluate_king_safety() {
        int white_king_safety_score = 0;
        int black_king_safety_score = 0;

        // Evaluate Pawn shelter
        std::pair<int, int> pawn_shelter_scores = evaluate_pawn_shelter();
        int white_pawn_shelter_score = pawn_shelter_scores.first;
        int black_pawn_shelter_score = pawn_shelter_scores.second;
        white_king_safety_score += white_pawn_shelter_score;
        black_king_safety_score += black_pawn_shelter_score;

        // Open Files
        std::pair<int, int> open_file_scores = evaluate_open_files();
        int white_penalty = open_file_scores.first;
        int black_penalty = open_file_scores.second;
        white_king_safety_score -= white_penalty;
        black_king_safety_score -= black_penalty;

        // Piece Attacks

        return std::make_pair(white_king_safety_score, black_king_safety_score);
    }

    std::pair<int, int> evaluate_pawn_shelter() {
        int white_shelter_score = 0;
        int black_shelter_score = 0;

        int white_king_pos = board.piece_tracker['K'][0];
        int black_king_pos = board.piece_tracker['k'][0];
        std::vector<int> white_shelter_squares, black_shelter_squares;

        if (white_king_pos % 8 == 7) {
            white_shelter_squares = { white_king_pos - 8, white_king_pos - 16, white_king_pos - 8 - 1 };
        }
        else if (white_king_pos % 8 == 0) {
            white_shelter_squares = { white_king_pos - 8, white_king_pos - 16, white_king_pos - 8 + 1 };
        }
        else {
            white_shelter_squares = { white_king_pos - 8, white_king_pos - 16, white_king_pos - 8 - 1, white_king_pos - 8 + 1, (white_king_pos % 8 < 4) ? white_king_pos - 16 - 1 : white_king_pos - 16 + 1 };
        }

        if (black_king_pos % 8 == 7) {
            black_shelter_squares = { black_king_pos + 8, black_king_pos + 16, black_king_pos + 8 - 1 };
        }
        else if (black_king_pos % 8 == 0) {
            black_shelter_squares = { black_king_pos + 8, black_king_pos + 16, black_king_pos + 8 + 1 };
        }
        else {
            black_shelter_squares = { black_king_pos + 8, black_king_pos + 16, black_king_pos + 8 - 1, black_king_pos + 8 + 1, (black_king_pos % 8 < 4) ? black_king_pos + 16 - 1 : black_king_pos + 16 + 1 };
        }

        for (int square : white_shelter_squares) {
            if (board.has_piece(square) && !board.has_enemy_piece(square, 'w')) {
                white_shelter_score += 5;
            }
        }

        for (int square : black_shelter_squares) {
            if (board.has_piece(square) && !board.has_enemy_piece(square, 'b')) {
                black_shelter_score += 5;
            }
        }

        return std::make_pair(white_shelter_score, black_shelter_score);
    }

    std::pair<int, int> evaluate_open_files() {
        int white_file_score = 0;
        int black_file_score = 0;

        int white_king_pos = board.piece_tracker['K'][0];
        int black_king_pos = board.piece_tracker['k'][0];

        std::vector<int> white_files = { (white_king_pos % 8) + 1 };
        if (white_king_pos % 8 != 0) white_files.push_back(white_king_pos % 8);
        if (white_king_pos % 8 != 7) white_files.push_back((white_king_pos % 8) + 2);

        std::vector<int> black_files = { (black_king_pos % 8) + 1 };
        if (black_king_pos % 8 != 0) black_files.push_back(black_king_pos % 8);
        if (black_king_pos % 8 != 7) black_files.push_back((black_king_pos % 8) + 2);

        for (int file : white_files) {
            uint64_t file_bitboard = 0x0101010101010101ULL << file;
            if ((board.bitboard['p'] & file_bitboard) == 0) {
                white_file_score += 5;
            }
        }

        for (int file : black_files) {
            uint64_t file_bitboard = 0x0101010101010101ULL << file;
            if ((board.bitboard['P'] & file_bitboard) == 0) {
                black_file_score += 5;
            }
        }

        return std::make_pair(white_file_score, black_file_score);
    }

    int minimax(Move& board, int depth, int alpha, int beta, bool maximize) {
        if (board.is_checkmate()) {
            if (board.player == 'w') return -10000000;
            else if (board.player == 'b') return 10000000;
        }
        if (board.is_stalemate() || board.is_insufficient_material() || board.is_fifty_moves()) return 0;
        if (depth == 0) return evaluate(board);


        std::pair<int, int> lookup_result = transposition_table.lookup(board.get_fen());
        if (lookup_result.first != -1 && depth <= lookup_result.first) return lookup_result.second;

        if (maximize) {
            int best_value = -9999999;
            std::vector<std::string> legal_moves = board.generate_legal_moves();
            std::vector<std::future<int>> futures;
            
            for (const std::string& move : legal_moves) {
                board.push(move);
                NODECOUNT++;
                best_value = std::max(best_value, minimax(board, depth - 1, alpha, beta, !maximize));
                board.pop();
                transposition_table.store(board.get_fen(), depth, best_value);
                alpha = std::max(alpha, best_value);
                if (alpha >= beta) {
                    return best_value;
                }
            }
            return best_value;
        } else {
            int best_value = 9999999;
            std::vector<std::string> legal_moves = board.generate_legal_moves();
            for (const std::string& move : legal_moves) {
                board.push(move);
                NODECOUNT++;
                best_value = std::min(best_value, minimax(board, depth - 1, alpha, beta, !maximize));
                board.pop();
                transposition_table.store(board.get_fen(), depth, best_value);
                beta = std::min(beta, best_value);
                if (beta <= alpha) {
                    return best_value;
                }
            }
            return best_value;
        }
    }

    std::pair<std::string, int> getNextMove(int depth, Move board, bool maximize, int time_limit) {
        std::vector<std::string> legal_moves = board.generate_legal_moves();
        // for (const auto move : legal_moves) std::cout << move << " ";
        // std::cout << std::endl;
        std::string bestMove;
        int bestValue = maximize ? -99999999 : 99999999;
        auto start_time = std::chrono::steady_clock::now();
        for (const std::string& move : legal_moves) {
            board.push(move);
            if (depth == 1) {
                NODECOUNT++;
            }
            int value = minimax(board, depth - 1, -9999999, 9999999, maximize);
            board.pop();
            if (maximize) {
                if (value > bestValue) {
                    bestValue = value;
                    bestMove = move;
                }
            } else {
                if (value < bestValue) {
                    bestValue = value;
                    bestMove = move;
                }
            }
        }
        return std::make_pair(bestMove, bestValue);
    }


    std::pair<std::string, int> iterativeDeepening(int max_depth, Move& board, bool maximize, int time_limit) {
        std::string best_move;
        int best_value = maximize ? -99999999 : 99999999;
        
        std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
        
        for (int depth = 1; depth <= max_depth; ++depth) {
            std::pair<std::string, int> result = getNextMove(depth, board, maximize, time_limit);
            
            if (result.second == 10000000 || result.second == -10000000) {
                return result; // Found a checkmate, return immediately
            }
            
            best_move = result.first;
            best_value = result.second;

            std::cout << "Depth: " << depth << " " << "Best Move: " << best_move << " " << "Eval: " << best_value << std::endl;
            
            std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_time = current_time - start_time;
            
            if (elapsed_time.count() >= time_limit) {
                break; // Time limit reached, exit the loop
            }
        }
        
        return std::make_pair(best_move, best_value);
    }

};

#endif