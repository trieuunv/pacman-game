#include "game_logic.h"
#include "drawing.h"
#include <cstdlib>
#include <ctime>

// External objects
extern Pacman pacman(1, 1);
extern Ghost ghost1(1, 26, 2, 1.0f, 0.0f, 0.0f);
extern Ghost ghost2(21, 12, 3, 0.0f, 1.0f, 0.0f);
extern Ghost ghost3(8, 12, 2, 1.0f, 1.0f, 0.0f);
extern Ghost ghost4(29, 15, 1, 0.0f, 1.0f, 1.0f);

// Death animation control
bool isDying = false;
int deathFrame = 0;

// Item spawn control
int itemSpawnTimer = 0;
const int ITEM_SPAWN_INTERVAL = 300;

// Speed boost from orange
int speedBoostTimer = 0;
const int SPEED_BOOST_DURATION = 180;

// Invincibility from star
bool isInvincible = false;
int invincibilityTimer = 0;
const int INVINCIBILITY_DURATION = 300;

void drawLives() {
    int x = 735;
    int y = 915;
    int triangleAmount = 360;
    GLfloat twicePi = 2.0f * M_PI;
    GLfloat radius = 9;

    for (int i = 1; i <= pacman.getLives(); i++) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 0);
        glVertex2f(x, y);
        for (int j = 40; j <= 320; j++) {
            glVertex2f(
                x + (radius * cos(j * twicePi / triangleAmount)),
                y + (radius * sin(j * twicePi / triangleAmount))
            );
        }
        glEnd();
        x += 30;
    }
}

void restart() {
    pacman.moveToStart();
    pacman.resetLives();
    pacman.resetScore();
    isDying = false;
    deathFrame = 0;
    speedBoostTimer = 0;
    isInvincible = false;
    invincibilityTimer = 0;
    itemSpawnTimer = 0;
}

void keyboard() {
    if (isDying) return;

    if (GetAsyncKeyState(VK_UP)) {
        if (!pacman.checkWall(pacman.getI() - 1, pacman.getJ())) {
            direction = 1;
            move = true;
        }
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        if (!pacman.checkWall(pacman.getI() + 1, pacman.getJ())) {
            direction = 2;
            move = true;
        }
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        if (!pacman.checkWall(pacman.getI(), pacman.getJ() - 1)) {
            direction = 3;
            move = true;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        if (!pacman.checkWall(pacman.getI(), pacman.getJ() + 1)) {
            direction = 4;
            move = true;
        }
    }
}

void movePacman() {
    if (isDying) return;

    float baseSpeed = 3.0f;
    float currentSpeed = baseSpeed;

    if (speedBoostTimer > 0) {
        currentSpeed = 6.0f;
    }

    if (direction == 1) { // LÊN
        if (pacman.getY() % 30 == 0) {
            move = !pacman.checkWall(pacman.getI() - 1, pacman.getJ());
        }
        if (move) {
            
            float targetY = pacman.getI() * 30;
            float maxMove = pacman.getY() - targetY;


            float actualSpeed = (maxMove > 0 && currentSpeed > maxMove) ? maxMove : currentSpeed;

            pacman.moveUp(actualSpeed);
            pacman.setOrientation(50);
        }
    }
    else if (direction == 2) {
        if (pacman.getY() % 30 == 0) {
            move = !pacman.checkWall(pacman.getI() + 1, pacman.getJ());
        }
        if (move) {
            float targetY = (pacman.getI() + 1) * 30;
            float maxMove = targetY - pacman.getY();

            float actualSpeed = (maxMove > 0 && currentSpeed > maxMove) ? maxMove : currentSpeed;

            pacman.moveDown(actualSpeed);
            pacman.setOrientation(230);
        }
    }
    else if (direction == 3) {
        if (pacman.getX() % 30 == 0) {
            move = !pacman.checkWall(pacman.getI(), pacman.getJ() - 1);
        }
        if (move) {
            float targetX = pacman.getJ() * 30;
            float maxMove = pacman.getX() - targetX;

            float actualSpeed = (maxMove > 0 && currentSpeed > maxMove) ? maxMove : currentSpeed;

            pacman.moveLeft(actualSpeed);
            pacman.setOrientation(140);
        }
        
        if (pacman.getI() == 14 && pacman.getJ() == 0) {
            pacman.increaseScore(10);
            board[pacman.getI()][pacman.getJ()] = -1;
            pebble_counter += 1;
            pacman.moveTo(14, 27);
        }
    }
    else if (direction == 4) {
        if (pacman.getX() % 30 == 0) {
            move = !pacman.checkWall(pacman.getI(), pacman.getJ() + 1);
        }
        if (move) {
            float targetX = (pacman.getJ() + 1) * 30;
            float maxMove = targetX - pacman.getX();

            float actualSpeed = (maxMove > 0 && currentSpeed > maxMove) ? maxMove : currentSpeed;

            pacman.moveRight(actualSpeed);
            pacman.setOrientation(320);
        }
        
        if (pacman.getI() == 14 && pacman.getJ() == 27) {
            pacman.increaseScore(10);
            board[pacman.getI()][pacman.getJ()] = -1;
            pebble_counter += 1;
            pacman.moveTo(14, 0);
        }
    }
}

void spawnRandomItem() {
    std::vector<std::pair<int, int>> emptySpaces;

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (board[i][j] == 1) {
                emptySpaces.push_back({ i, j });
            }
        }
    }

    if (emptySpaces.empty()) return;

    int idx = rand() % emptySpaces.size();
    std::pair<int, int> pos = emptySpaces[idx];
    int i = pos.first;
    int j = pos.second;

    int rnd = rand() % 100;
    if (rnd < 40) {
        board[i][j] = ITEM_CHERRY;
    }
    else if (rnd < 70) {
        board[i][j] = ITEM_STRAWBERRY;
    }
    else if (rnd < 90) {
        board[i][j] = ITEM_ORANGE;
    }
    else {
        board[i][j] = ITEM_STAR;
    }
}

void checkScore() {
    int currentCell = board[pacman.getI()][pacman.getJ()];

    if (currentCell == 1) {
        if ((pacman.getI() == 1 && pacman.getJ() == 26) ||
            (pacman.getI() == 29 && pacman.getJ() == 1) ||
            (pacman.getI() == 29 && pacman.getJ() == 26)) {
            pacman.increaseScore(100);
            if (is_blue_mode == true) {
                duration += 300;
            }
            else {
                is_blue_mode = true;
            }
        }
        else {
            pacman.increaseScore(10);
        }
        board[pacman.getI()][pacman.getJ()] = -1;
        pebble_counter += 1;
    }
    else if (currentCell == ITEM_CHERRY) {
        pacman.increaseScore(50);
        board[pacman.getI()][pacman.getJ()] = -1;
        pebble_counter += 1;
    }
    else if (currentCell == ITEM_STRAWBERRY) {
        pacman.increaseScore(75);
        board[pacman.getI()][pacman.getJ()] = -1;
        pebble_counter += 1;
    }
    else if (currentCell == ITEM_ORANGE) {
        pacman.increaseScore(100);
        speedBoostTimer = SPEED_BOOST_DURATION;
        board[pacman.getI()][pacman.getJ()] = -1;
        pebble_counter += 1;
    }
    else if (currentCell == ITEM_STAR) {
        pacman.increaseScore(200);
        isInvincible = true;
        invincibilityTimer = INVINCIBILITY_DURATION;
        board[pacman.getI()][pacman.getJ()] = -1;
        pebble_counter += 1;
    }

    if (pebble_counter >= 300) {
        pebble_counter = 0;
        resetBoard();
    }
}

void moveGhost(Ghost& ghost) {
    if (ghost.isDead() == true && ghost.getTimer() > 0) {
        ghost.decreaseTimer();
    }
    else {
        if (ghost.isDead() && ghost.getTimer() == 0) {
            ghost.setMove(false);
        }

        if (ghost.getMove() == false)
            ghost.checkPossibleMovement();

        if (ghost.getDirection() == 1) {
            ghost.setMove(!ghost.checkWall(ghost.getI() - 1, ghost.getJ()));
            if (ghost.getMove()) {
                ghost.moveUp(ghost_speed);
            }
        }
        if (ghost.getDirection() == 2) {
            ghost.setMove(!ghost.checkWall(ghost.getI() + 1, ghost.getJ()));
            if (ghost.getMove()) {
                ghost.moveDown(ghost_speed);
            }
        }
        if (ghost.getDirection() == 3) {
            ghost.setMove(!ghost.checkWall(ghost.getI(), ghost.getJ() - 1));
            if (ghost.getMove()) {
                ghost.moveLeft(ghost_speed);
            }
        }
        if (ghost.getDirection() == 4) {
            ghost.setMove(!ghost.checkWall(ghost.getI(), ghost.getJ() + 1));
            if (ghost.getMove()) {
                ghost.moveRight(ghost_speed);
            }
        }
    }
}

void checkCollision(Ghost& ghost) {
    if (pacman.getI() == ghost.getI() && pacman.getJ() == ghost.getJ()) {
        if (is_blue_mode || isInvincible) {
            pacman.increaseScore(200);
            ghost.die();
        }
        else {
            isDying = true;
            deathFrame = 0;
            pacman.die();
        }
    }
}

void checkColisions() {
    if (isDying) return;

    checkCollision(ghost1);
    checkCollision(ghost2);
    checkCollision(ghost3);
    checkCollision(ghost4);
}

std::string int2str(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

void resetBoard() {
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (board[i][j] == -1 || board[i][j] >= 3)
                board[i][j] = 1;
        }
    }
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawBoard();

    if (isDying) {
        drawPacmanDeath(pacman.getX(), pacman.getY(), deathFrame);
    }
    else {
        if (move) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1, 1, 0, 0.3f);
            drawCircle(pacman.getX() + 15, pacman.getY() + 15, 10, 1, 1, 0);
            glDisable(GL_BLEND);
        }

        drawPacman(pacman.getX(), pacman.getY(), pacman.getOrientation());

        if (isInvincible) {
            drawGlowEffect(pacman.getX() + 15, pacman.getY() + 15,
                15, 1.0f, 1.0f, 0.0f, 0.5f);
        }
    }

    if (is_blue_mode == false) {
        drawGhost(ghost1.getX(), ghost1.getY(), ghost1.getR(), ghost1.getG(), ghost1.getB());
        drawGhost(ghost2.getX(), ghost2.getY(), ghost2.getR(), ghost2.getG(), ghost2.getB());
        drawGhost(ghost3.getX(), ghost3.getY(), ghost3.getR(), ghost3.getG(), ghost3.getB());
        drawGhost(ghost4.getX(), ghost4.getY(), ghost4.getR(), ghost4.getG(), ghost4.getB());
    }
    else {
        drawGhost(ghost1.getX(), ghost1.getY(), 0, 0, 1);
        drawGhost(ghost2.getX(), ghost2.getY(), 0, 0, 1);
        drawGhost(ghost3.getX(), ghost3.getY(), 0, 0, 1);
        drawGhost(ghost4.getX(), ghost4.getY(), 0, 0, 1);
    }

    drawLives();
    drawText(30, height - 15, "Score: " + int2str(pacman.getScore()));

    if (speedBoostTimer > 0) {
        drawText(30, height - 35, "SPEED BOOST!");
    }
    if (isInvincible) {
        drawText(30, height - 55, "INVINCIBLE!");
    }

    if (still_alive == false) {
        drawOpaqueRect(0, 0, 840, 930, 0.0f, 0.0f, 0.0f);
        drawTextBig(11.5 * 30, 900 - 13 * 30, "GAME OVER");
        drawTextBig(11.5 * 30, 900 - 14 * 30, "Press W to start again");
        // keyboard();
    }

    glutSwapBuffers();
}

void update(int value) {
    if (still_alive == false) {
        if (GetAsyncKeyState('W')) {
            resetBoard();
            restart();
            still_alive = true;
        }

        glutTimerFunc(interval, update, 0);
        glutPostRedisplay();
        return;
    }

    if (isDying) {
        deathFrame++;
        if (deathFrame >= 60) {
            isDying = false;
            deathFrame = 0;
            if (still_alive) {
                pacman.moveToStart();
            }
        }
        glutTimerFunc(interval, update, 0);
        glutPostRedisplay();
        return;
    }

    keyboard();
    movePacman();
    checkScore();

    if (is_blue_mode && duration > 0)
        duration -= 1;
    else {
        is_blue_mode = false;
        duration = 300;
    }

    if (speedBoostTimer > 0)
        speedBoostTimer--;

    if (isInvincible && invincibilityTimer > 0)
        invincibilityTimer--;
    else
        isInvincible = false;

    itemSpawnTimer++;
    if (itemSpawnTimer >= ITEM_SPAWN_INTERVAL) {
        spawnRandomItem();
        itemSpawnTimer = 0;
    }

    moveGhost(ghost1);
    moveGhost(ghost2);
    moveGhost(ghost3);
    moveGhost(ghost4);

    checkColisions();

    glutTimerFunc(interval, update, 0);
    glutPostRedisplay();
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(NULL));
}