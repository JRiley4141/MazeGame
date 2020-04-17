/**
Jennifer Riley
Maze Game
Description: Creates the Board.  
*/
#include "Maze.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <cstdlib>

using namespace std;

//Global Function that return SquareType
string SquareTypeStringify(SquareType sq) {
    switch(sq){
        case SquareType::Wall:
            return " W ";
        case SquareType::Exit:
            return " X ";
        case SquareType::Empty:
            return " \u25a2 ";
        case SquareType::Human:
            return " H ";
        case SquareType::Enemy:
            return " E ";
        case SquareType::Treasure:
            return " $ ";
    }
    return "This type is not valid.";
}

Board::Board(){
    for (int r = 0; r < get_rows(); r++) {
        for (int c = 0; c < get_cols(); c++) {
            arr_[r][c] = SquareType::Empty;
        }
    }
    arr_[0][0] = SquareType::Human;
    arr_[get_rows() - 1][get_cols() - 1] = SquareType::Exit;
    
}

SquareType Board::get_square_value(Position pos) const {
    return arr_[pos.row][pos.col];
}

void Board::SetSquareValue(Position pos, SquareType value) {
    arr_[pos.row][pos.col] = value;
}

vector<Position> Board::GetMoves(Player *p) {
    vector<Position> moves;
    auto currPos = p->get_position();
    Position up(currPos.row - 1, currPos.col);
    Position down(currPos.row + 1, currPos.col);
    Position left(currPos.row, currPos.col - 1);
    Position right(currPos.row, currPos.col + 1);

    if(currPos.row > 0 && get_square_value(up) != SquareType::Wall) {
        moves.push_back(up);
    }
    if(currPos.row < get_rows() - 1 && get_square_value(down) != SquareType::Wall) {
        moves.push_back(down);
    }
    if(currPos.col > 0  && get_square_value(left) != SquareType::Wall) {
        moves.push_back(left);
    }
    if(currPos.col < get_cols() - 1  && get_square_value(right) != SquareType::Wall) {
        moves.push_back(right);
    }

    return moves;
}

bool Board::MovePlayer(Player *p, Position new_position) {
    vector<Position> possible_moves = GetMoves(p);
    bool allow_move = find(possible_moves.begin(), possible_moves.end(), new_position) != possible_moves.end();
    if (allow_move){
        // Update the board.
        SquareType square_type = get_square_value(new_position);
        if (square_type == SquareType::Treasure) {
            p->ChangePoints(p->get_points() + 100);
        }
        SetSquareValue(p->get_position(), SquareType::Empty);
        SetSquareValue(new_position, p->is_human() ? SquareType::Human : SquareType::Enemy);
        p->SetPosition(new_position);
    }
    return allow_move;
}
SquareType Board::GetExitOccupant() {
    return arr_[get_rows() - 1][get_cols() - 1];
}

std::ostream& operator<<(std::ostream& os, const Board &b) {
    for (int r = 0; r < b.get_rows(); r++) {
        for (int c = 0; c < b.get_cols(); c++) {
            Position p(r, c);
            SquareType t = b.get_square_value(p);
            os << SquareTypeStringify(t);
        }
        os << endl;
    }
    return os;
}

Maze::Maze() {
    board_ = new Board();
	turn_count_ = 0;
    current_player_ = 0;
}

void Maze::NewGame(Player *human, const int enemies) {
    players_.push_back(human);
    for (int i = 0; i < enemies; i++) {
        Player *enemy = new Player(("Enemy" + to_string(i)), false);
        players_.push_back(enemy);
        
        // Randomly find an open position for this enemy.
        int num_squares = board_->get_rows() * board_->get_cols();
        bool found_open_space = false;
        while (!found_open_space) {
            int board_index = 1 + rand() % (num_squares - 2);
            Position enemy_position(board_index / board_->get_cols(), board_index % board_->get_cols());
            if (board_->get_square_value(enemy_position) != SquareType::Enemy) {
                enemy->SetPosition(enemy_position);
                board_->SetSquareValue(enemy_position, SquareType::Enemy);
                found_open_space = true;
            }
        }
    }
    for (int r = 0; r < board_->get_rows(); r++) {
        for (int c = 0; c < board_->get_cols(); c++) {
            Position square_position(r,c);
            SquareType current_square_type = board_->get_square_value(square_position);
            if (current_square_type == SquareType::Empty) {
                int randNum = rand() % 100 + 1;
                if (randNum >= 1 && randNum <= 10) {
                    board_->SetSquareValue(square_position, SquareType::Treasure);
                }
                else if (randNum > 10 && randNum <= 30) {
                    board_->SetSquareValue(square_position, SquareType::Wall);
                }
            }
        }
    }

}

bool Maze::IsGameOver() {
    for (size_t p = 1; p < players_.size(); p++) {
        if (players_[0]->get_position() == players_[p]->get_position()) {
            return true;
        }
    }
    if (board_->GetExitOccupant() == SquareType::Human) {
        return true;
    }

    Position right_of_entrance(0, 1);
    Position below_entrance(1, 0);

    if (board_->get_square_value(right_of_entrance) == SquareType::Wall && board_->get_square_value(below_entrance) == SquareType::Wall) {
        return true;
    }

    Position above_exit(board_->get_rows()-2, board_->get_cols()-1);
    Position left_of_exit(board_->get_rows()-1, board_->get_cols()-2);
    
    if (board_->get_square_value(above_exit) == SquareType::Wall && board_->get_square_value(left_of_exit) == SquareType::Wall) {
        return true;
    }

    return false;
}

void Maze::TakeTurn(Player *p) {
    vector<Position> moves = board_->GetMoves(p);
    if (moves.size() == 0){
        turn_count_++;
        cout << "No Move Possible. Skipping Turn" << endl;
        return;
    }
    stringstream directions;
    for (size_t m = 0; m < moves.size(); m++) {
        directions << p->ToRelativePosition(moves[m]) << " ";
    }
    cout << p->get_name() << " can go: " << directions.str() << endl;
    

    // TODO: verify input before using it
    bool allowed = false;
    do {
        cout << "Please enter your choice: ";
        string move;
        cin >> move;

        Position new_position;
        if (move == "UP") {
            Position old_position = p->get_position();
            new_position = Position(old_position.row - 1, old_position.col);
        }
        else if (move == "DOWN") {
            Position old_position = p->get_position();
            new_position = Position(old_position.row + 1, old_position.col);
        }
        else if (move == "LEFT") {
            Position old_position = p->get_position();
            new_position = Position(old_position.row, old_position.col - 1);
        }
        else if (move == "RIGHT") {
            Position old_position = p->get_position();
            new_position = Position(old_position.row, old_position.col + 1);
        } 
        else {
            cout << "Invalid input. Try again." << endl;
            continue;
        }
        
        if (!p->is_human() && board_->get_square_value(new_position) == SquareType::Enemy) {
            break;
        } else {
            allowed = board_->MovePlayer(p, new_position);
            if (!allowed) {
                cout << "Invalid move. Try again." << endl;
            }
        }
    } while (!allowed);
    turn_count_++;
}

Player* Maze::GetNextPlayer() {
    Player *p = players_[current_player_];
    current_player_++;
    if (current_player_ == players_.size()) {
        current_player_ = 0;
    }
    return p;
}

std::string Maze::GenerateReport() {
    bool human_won = board_->GetExitOccupant() == SquareType::Human;
    stringstream report;
    report << "--- Results ---" << endl;
    for (size_t p = 0; p < players_.size(); p ++) {
        report << players_[p]->Stringify() << endl;
    }
    report << players_[0]->get_name() << (human_won ? " won" : " lost") << endl;
    return report.str();
}
std::ostream& operator<<(std::ostream& os, const Maze &m) {
    os << *m.board_;
    return os;
}