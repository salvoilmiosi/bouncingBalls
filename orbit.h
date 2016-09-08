#ifndef __ORBIT_H_
#define __ORBIT_H_

#include "ball.h"
#include <list>

class orbit {
public:
    ball *planet;
    ball *sun;

    unsigned int tickSkip;
    unsigned int historySize;

public:
    orbit(ball *p, ball *s);

public:
    void tick();

    void draw(SDL_Renderer *renderer, int ox, int oy);

private:
    std::list<point> history;

    unsigned int tickNum;
};

#endif // __ORBIT_H_
