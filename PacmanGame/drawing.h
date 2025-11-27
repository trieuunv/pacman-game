/*
VARIOUS FUNCTIONS FOR DRAWING ELEMENTS ON THE SCREEN
*/

#pragma once
#ifndef DRAWING_H
#define DRAWING_H

#include "globals.h"

struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float size;
    float r, g, b;
};

enum ItemType {
    ITEM_NONE = 0,
    ITEM_CHERRY = 3,
    ITEM_STRAWBERRY = 4,
    ITEM_ORANGE = 5,
    ITEM_STAR = 6
};

void drawCircleFan(GLfloat x, GLfloat y, int o);

void drawText(float x, float y, std::string text);

void drawTextBig(float x, float y, std::string text);

void drawEmptyRect(float x, float y, float width, float height, float r, float g, float b);

void drawRect(float x, float y, float width, float height, float r, float g, float b);

void drawOpaqueRect(float x, float y, float width, float height, float r, float g, float b);

void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b);

void drawPoint(float x, float y, float r, float g, float b, float size);

void drawPacman(GLfloat x, GLfloat y, int o);

void drawGhost(GLfloat x, GLfloat y, float r, float g, float b);

void drawBoard();

void drawRoundedRect(float x, float y, float width, float height, float radius, float r, float g, float b);
void drawCircle(float x, float y, float radius, float r, float g, float b);
void drawGradientRect(float x, float y, float width, float height,
    float r1, float g1, float b1, float r2, float g2, float b2);

// Item drawing functions
void drawCherry(float x, float y);
void drawStrawberry(float x, float y);
void drawOrange(float x, float y);
void drawStar(float x, float y);

// Effect functions
void drawPacmanDeath(GLfloat x, GLfloat y, int frame);
void drawParticles();
void updateParticles();
void createDeathParticles(float x, float y);
void drawGlowEffect(float x, float y, float size, float r, float g, float b, float alpha);
void drawScorePopup(float x, float y, int score, int frame);

void drawEnhancedBoard();

#endif
