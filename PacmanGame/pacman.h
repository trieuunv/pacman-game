/*
DEFINITON OF PACMAN CLASS AND LOGIC
*/

#pragma once
#ifndef PACMAN_H
#define PACMAN_H

class Pacman
{
    int position[2];
    int score;
    int lives;
    int coordinates[2];
    int orientation;

public:

    Pacman(int i, int j);

    void increaseScore(int points);

    void die();

    int getX();

    void setX(int x);

    int getY();

    void setY(int y);

    int getI();

    int getJ();

    int getScore();

    int getLives();

    void resetLives();

    void resetScore();

    void moveUp(int distance);

    void moveDown(int distance);

    void moveRight(int distance);

    void moveLeft(int distance);

    void setOrientation(int o);

    int getOrientation();

    bool checkWall(int i, int j);

    void moveTo(int i, int j);

    void moveToStart();
};

#endif