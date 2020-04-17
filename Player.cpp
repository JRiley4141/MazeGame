/**
Jennifer Riley
Maze Game
Description: The creation of the player and enemies. Implements the player class.  
*/

#include <iostream>
#include <math.h>
#include "Player.h"

using namespace std;

Position::Position() {
    this->row = 0;
    this->col = 0;
}

Position::Position(int row, int col) {
    this->row = row;
    this->col = col;
}

Player::Player(string name, bool is_human) {
    name_ = name;
    is_human_ = is_human;
    pos_.row = 0;
    pos_.col = 0;
}

void Player::ChangePoints(const int x) {
    points_ = x;
}

void Player::SetPosition(Position pos) {
    pos_ = pos;
}

string Player::ToRelativePosition(Position other) {
    if (pos_.row < other.row && pos_.col == other.col) {
        return "DOWN";
    }
    else if (pos_.row > other.row && pos_.col == other.col) {
        return "UP";
    }
    else if (pos_.row == other.row && pos_.col < other.col) {
        return "RIGHT";
    }
    else if (pos_.row == other.row && pos_.col > other.col) {
        return "LEFT";
    }
    else {
        return "No Valid Moves";
    }
}

string Player::Stringify() {
    return name_ + ": " + std::to_string(points_);
}