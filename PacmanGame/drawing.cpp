#include "drawing.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <GL/freeglut_ext.h>

// Particle system for death animation
std::vector<Particle> particles;
int deathAnimationFrame = 0;
bool isDeathAnimating = false;

// Score popup data
struct ScorePopup {
    float x, y;
    int score;
    int frame;
    bool active;
};
std::vector<ScorePopup> scorePopups;

// Enhanced circle drawing with smooth edges
void drawCircle(float x, float y, float radius, float r, float g, float b) {
    int triangleAmount = 50;
    GLfloat twicePi = 2.0f * M_PI;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(x, y);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

// Draw rounded rectangle
void drawRoundedRect(float x, float y, float width, float height, float radius, float r, float g, float b) {
    glColor3f(r, g, b);

    // Main rectangle
    drawRect(x + radius, y, width - 2 * radius, height, r, g, b);
    drawRect(x, y + radius, width, height - 2 * radius, r, g, b);

    // Corners
    drawCircle(x + radius, y + radius, radius, r, g, b);
    drawCircle(x + width - radius, y + radius, radius, r, g, b);
    drawCircle(x + radius, y + height - radius, radius, r, g, b);
    drawCircle(x + width - radius, y + height - radius, radius, r, g, b);
}

// Draw gradient rectangle
void drawGradientRect(float x, float y, float width, float height,
    float r1, float g1, float b1, float r2, float g2, float b2) {
    glBegin(GL_QUADS);
    glColor3f(r1, g1, b1);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glColor3f(r2, g2, b2);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Draw cherry (táo)
void drawCherry(float x, float y) {
    x += 15;
    y += 15;

    // Cherry body
    drawCircle(x - 3, y - 2, 5, 0.8f, 0.1f, 0.1f);
    drawCircle(x + 3, y - 2, 5, 0.9f, 0.0f, 0.0f);

    // Stem
    glLineWidth(2);
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 2, y + 3);
    glVertex2f(x, y + 7);
    glVertex2f(x + 2, y + 3);
    glVertex2f(x, y + 7);
    glEnd();

    // Leaf
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 1, y + 6);
    glVertex2f(x + 5, y + 7);
    glVertex2f(x + 3, y + 4);
    glEnd();
}

// Draw strawberry (dâu)
void drawStrawberry(float x, float y) {
    x += 15;
    y += 15;

    // Body
    glBegin(GL_TRIANGLES);
    glColor3f(0.9f, 0.1f, 0.1f);
    glVertex2f(x, y + 5);
    glVertex2f(x - 6, y - 5);
    glVertex2f(x + 6, y - 5);
    glEnd();

    // Seeds
    glPointSize(2);
    glColor3f(1.0f, 1.0f, 0.6f);
    glBegin(GL_POINTS);
    glVertex2f(x - 3, y);
    glVertex2f(x + 3, y);
    glVertex2f(x, y + 2);
    glVertex2f(x - 2, y - 2);
    glVertex2f(x + 2, y - 2);
    glEnd();

    // Leaves
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 4, y + 5);
    glVertex2f(x, y + 8);
    glVertex2f(x + 4, y + 5);
    glEnd();
}

// Draw orange (cam)
void drawOrange(float x, float y) {
    x += 15;
    y += 15;

    // Orange body with gradient effect
    for (int i = 0; i < 5; i++) {
        float offset = i * 0.8f;
        drawCircle(x, y, 7 - offset, 1.0f - i * 0.05f, 0.5f - i * 0.05f, 0.0f);
    }

    // Highlight
    drawCircle(x - 2, y + 2, 2, 1.0f, 0.8f, 0.4f);
}

// Draw star (ngôi sao)
void drawStar(float x, float y) {
    x += 15;
    y += 15;

    float outerRadius = 8;
    float innerRadius = 3;
    int points = 5;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(x, y);

    for (int i = 0; i <= points * 2; i++) {
        float angle = M_PI * i / points - M_PI / 2;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        glVertex2f(
            x + radius * cos(angle),
            y + radius * sin(angle)
        );
    }
    glEnd();

    // Glow effect
    drawGlowEffect(x, y, 12, 1.0f, 1.0f, 0.0f, 0.3f);
}

// Glow effect
void drawGlowEffect(float x, float y, float size, float r, float g, float b, float alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 3; i++) {
        glColor4f(r, g, b, alpha / (i + 1));
        drawCircle(x, y, size + i * 4, r, g, b);
    }

    glDisable(GL_BLEND);
}

// Create death particles
void createDeathParticles(float x, float y) {
    particles.clear();
    x += 15;
    y += 15;

    for (int i = 0; i < 20; i++) {
        Particle p;
        float angle = (2.0f * M_PI * i) / 20.0f;
        float speed = 2.0f + (rand() % 3);

        p.x = x;
        p.y = y;
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;
        p.life = 1.0f;
        p.size = 3.0f + (rand() % 3);
        p.r = 1.0f;
        p.g = 1.0f;
        p.b = 0.0f;

        particles.push_back(p);
    }
}

// Update particles
void updateParticles() {
    for (auto& p : particles) {
        p.x += p.vx;
        p.y += p.vy;
        p.vy -= 0.2f; // Gravity
        p.life -= 0.02f;
        p.size *= 0.95f;
    }

    // Remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0; }),
        particles.end()
    );
}

// Draw particles
void drawParticles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (const auto& p : particles) {
        glColor4f(p.r, p.g, p.b, p.life);
        glPointSize(p.size);
        glBegin(GL_POINTS);
        glVertex2f(p.x, p.y);
        glEnd();
    }

    glDisable(GL_BLEND);
}

// Draw Pacman death animation
void drawPacmanDeath(GLfloat x, GLfloat y, int frame) {
    if (frame == 0) {
        createDeathParticles(x, y);
    }

    float progress = frame / 60.0f; // 60 frames = 1 second
    float scale = 1.0f - progress;
    float rotation = progress * 720.0f; // 2 full rotations
    float alpha = 1.0f - progress;

    if (scale <= 0) return;

    x += 15;
    y += 15;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw rotating, shrinking Pacman
    int triangleAmount = 360;
    GLfloat twicePi = 2.0f * M_PI;
    GLfloat radius = 12 * scale;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(-x, -y, 0);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 1.0f, 0.0f, alpha);
    glVertex2f(x, y);
    for (int i = 40; i <= 320; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_BLEND);

    // Draw particles
    drawParticles();
    updateParticles();
}

// Score popup
void drawScorePopup(float x, float y, int score, int frame) {
    float alpha = 1.0f - (frame / 60.0f);
    float offsetY = frame * 0.5f;

    if (alpha <= 0) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    std::string text = "+" + std::to_string(score);
    glRasterPos2f(x, y + offsetY);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());

    glDisable(GL_BLEND);
}

// Original functions (keeping compatibility)
void drawCircleFan(GLfloat x, GLfloat y, int o) {
    GLfloat twicePi = 2.0f * M_PI;
    GLfloat radius = 12;
    int triangleAmount = 360;
    int i;

    if (o == 320) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 0);
        glVertex2f(x, y);
        for (i = 40 - open; i <= 320 + open; i++) {
            glVertex2f(
                x + (radius * cos(i * twicePi / triangleAmount)),
                y + (radius * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
    }
    else {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 0);
        glVertex2f(x, y);
        for (i = 0; i <= o + open; i++) {
            glVertex2f(
                x + (radius * cos(i * twicePi / triangleAmount)),
                y + (radius * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 0);
        glVertex2f(x, y);
        for (i = o + 80 - open; i <= 360; i++) {
            glVertex2f(
                x + (radius * cos(i * twicePi / triangleAmount)),
                y + (radius * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
    }

    open += 3;
}

void drawText(float x, float y, std::string text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

void drawTextBig(float x, float y, std::string text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)text.c_str());
}

void drawEmptyRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawOpaqueRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor4f(r, g, b, 0.3f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawPoint(float x, float y, float r, float g, float b, float size) {
    glColor3f(r, g, b);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawPacman(GLfloat x, GLfloat y, int o) {
    x += 15;
    y += 15;

    if (open == 60)
        open = 0;

    drawCircleFan(x, y, o);
}

void drawGhost(GLfloat x, GLfloat y, float r, float g, float b) {
    int i;
    int triangleAmount = 360;
    GLfloat twicePi = 2.0f * M_PI;
    GLfloat radius = 10;

    GLfloat circlex = x + 15;
    GLfloat circley = y + 20;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(circlex, circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            circlex + (radius * cos(i * twicePi / triangleAmount)),
            circley + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    drawRect(x + 5, y + 10, 20, 10, r, g, b);

    radius = 4;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 1);
    glVertex2f(circlex, circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            circlex + (radius * cos(i * twicePi / triangleAmount)),
            circley + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    radius = 2;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0, 0, 0);
    glVertex2f(circlex, circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            circlex + (radius * cos(i * twicePi / triangleAmount)),
            circley + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    GLfloat small_circlex = x + 8.80;
    GLfloat small_circley = y + 10;
    GLfloat small_radius = 3.35;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(small_circlex, small_circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            small_circlex + (small_radius * cos(i * twicePi / triangleAmount)),
            small_circley + (small_radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    small_circlex += 6.7;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(small_circlex, small_circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            small_circlex + (small_radius * cos(i * twicePi / triangleAmount)),
            small_circley + (small_radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();

    small_circlex += 6.65;
    small_radius = 3.3;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b);
    glVertex2f(small_circlex, small_circley);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            small_circlex + (small_radius * cos(i * twicePi / triangleAmount)),
            small_circley + (small_radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawEnhancedBoard() {
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            float x = 30 * j;
            float y = 900 - 30 * i;

            if (board[i][j] == 0) {
                // Draw wall with gradient
                drawGradientRect(x, y, 30, 30, 0.0f, 0.1f, 0.3f, 0.1f, 0.0f, 0.2f);
            }
            else if (board[i][j] == 1) {
                // Regular pellets
                drawPoint(x + 15, y + 15, 255, 255, 255, 3.0f);

                // Power pellets with glow
                if ((i == 1 && j == 26) || (i == 29 && j == 1) || (i == 29 && j == 26)) {
                    drawGlowEffect(x + 15, y + 15, 8, 1.0f, 1.0f, 1.0f, 0.5f);
                    drawPoint(x + 15, y + 15, 255, 255, 255, 10.0f);
                }
            }
            else if (board[i][j] == ITEM_CHERRY) {
                drawCherry(x, y);
            }
            else if (board[i][j] == ITEM_STRAWBERRY) {
                drawStrawberry(x, y);
            }
            else if (board[i][j] == ITEM_ORANGE) {
                drawOrange(x, y);
            }
            else if (board[i][j] == ITEM_STAR) {
                drawStar(x, y);
            }
        }
    }

    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 28; j++) {
            if (board[i][j] == 0) {
                float x = 30 * j;
                float y = 900 - 30 * i;

                // Left border
                if (j > 0 && board[i][j - 1] != 0) {
                    drawLine(x, y, x, y + 30, 0.2f, 0.4f, 1.0f);
                }

                // Right border
                if (j < 27 && board[i][j + 1] != 0) {
                    drawLine(x + 30, y, x + 30, y + 30, 0.3f, 0.2f, 0.9f);
                }

                // Bottom border
                if (i < 30 && board[i + 1][j] != 0) {
                    drawLine(x, y, x + 30, y, 0.2f, 0.3f, 1.0f);
                }

                // Top border
                if (i > 0 && board[i - 1][j] != 0) {
                    drawLine(x, y + 30, x + 30, y + 30, 0.3f, 0.4f, 0.9f);
                }
            }
        }
    }
}

void drawBoard() {
    drawEnhancedBoard();
}