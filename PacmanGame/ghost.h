#ifndef GHOST_H
#define GHOST_H

class Ghost {
private:
    float x, y;
    int i, j;
    int direction;
    bool move;
    bool dead;
    int death_timer;
    float colors[3];

public:
    Ghost(int rows, int columns, int d, float r, float g, float b);

    float getX();
    float getY();
    int getI();
    int getJ();
    int getDirection();
    bool getMove();
    float getR();
    float getG();
    float getB();
    bool isDead();
    int getTimer();

    void setDirection(int d);
    void setMove(bool m);
    void decreaseTimer();

    void moveUp(int distance);
    void moveDown(int distance);
    void moveRight(int distance);
    void moveLeft(int distance);
    void die();

    bool checkWall(int i, int j);
    void checkPossibleMovement();
};

#endif