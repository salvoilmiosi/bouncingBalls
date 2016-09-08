#ifndef __BALL_H
#define __BALL_H

#include "SDL2/SDL.h"

#include "vector.h"
#include "point.h"

class ball {
public:
    point position;
    vector2d velocity;

    ball();
    virtual ~ball();

    void draw(SDL_Renderer *renderer, int ox, int oy);

    void tick();

    bool collidesWith(ball *b);

    void addGravity(ball *b);

    void setRadius(int radius);
    int getRadius() { return radius; }

    double getMass() { return mass; }

    void startDrag(int x, int y);

    void stopDrag();

    void drag(int x, int y);

    void resolveCollision(ball* b);

    const SDL_Color &getColor() {
        return color;
    }

private:
    SDL_Texture *texture;
    SDL_Color color;
    void createTexture(SDL_Renderer *renderer);

    void screenBorder();

    int radius;
    double mass;

    bool dragging;
    vector2d dragOffset;
};

#endif // __BALL_H
