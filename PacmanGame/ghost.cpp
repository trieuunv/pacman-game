#include "ghost.h"
#include "globals.h"

Ghost::Ghost(int rows, int columns, int d, float r, float g, float b) {
    colors[0] = r;
    colors[1] = g;
    colors[2] = b;
    i = rows;
    j = columns;
    direction = d;
    move = true;
    dead = false;
    death_timer = 180;
    x = j * 30;
    y = 900 - i * 30;
}

float Ghost::getX() {
    return x;
}

float Ghost::getY() {
    return y;
}

int Ghost::getI() {
    return i;
}

int Ghost::getJ() {
    return j;
}

int Ghost::getDirection() {
    return direction;
}

bool Ghost::getMove() {
    return move;
}

float Ghost::getR() {
    return colors[0];
}

float Ghost::getG() {
    return colors[1];
}

float Ghost::getB() {
    return colors[2];
}

void Ghost::setDirection(int d) {
    direction = d;
}

void Ghost::setMove(bool m) {
    move = m;
}

bool Ghost::isDead() {
    return dead;
}

int Ghost::getTimer() {
    return death_timer;
}

void Ghost::decreaseTimer() {
    death_timer -= 1;
}

void Ghost::moveUp(int distance) {
    y += distance;
    if (int(y) % 30 == 0) {
        i = 30 - y / 30;
    }
}

void Ghost::moveDown(int distance) {
    y -= distance;
    if (int(y) % 30 == 0) {
        i = 30 - y / 30;
    }
}

void Ghost::moveRight(int distance) {
    x += distance;
    if (int(x) % 30 == 0) {
        j = x / 30;
    }
}

void Ghost::moveLeft(int distance) {
    x -= distance;
    if (int(x) % 30 == 0) {
        j = x / 30;
    }
}

void Ghost::die() {
    i = 13;
    j = 13;
    x = j * 30;
    y = 900.0f - i * 30.0f;
    direction = 1;
    dead = true;
    death_timer = 180;
    checkPossibleMovement();
}

// FIXED: Check wall properly - only walls (0) are blocked for ghosts
// Ghosts can go through ghost house (2) when respawning
bool Ghost::checkWall(int i, int j) {
    // Out of bounds check
    if (i < 0 || i >= 31 || j < 0 || j >= 28) {
        return true;
    }

    // Only wall (0) is blocked for ghosts
    // Ghost house (2), pellets (1), eaten (-1), and items (3-6) are walkable
    if (board[i][j] == 0)
        return true;
    return false;
}
// Ghost.cpp - Hàm Ghost::checkPossibleMovement()

void Ghost::checkPossibleMovement() {
    std::vector<int> valid_directions;

    if (i >= 12 && i <= 14 && j >= 11 && j <= 16) {
      
        if (checkWall(i - 1, j) == false) {
            
            direction = 1;
            return;
        }
    }
    
    if (!checkWall(i - 1, j) && direction != 2)
        valid_directions.push_back(1);
    if (!checkWall(i + 1, j) && direction != 1)
        valid_directions.push_back(2);
    if (!checkWall(i, j - 1) && direction != 4)
        valid_directions.push_back(3);
    if (!checkWall(i, j + 1) && direction != 3)
        valid_directions.push_back(4);

    if (valid_directions.empty()) {
        if (direction == 1) valid_directions.push_back(2);
        else if (direction == 2) valid_directions.push_back(1);
        else if (direction == 3) valid_directions.push_back(4);
        else if (direction == 4) valid_directions.push_back(3);
    }

    if (!valid_directions.empty()) {
        int random_index = rand() % valid_directions.size();
        direction = valid_directions[random_index];
    }
}