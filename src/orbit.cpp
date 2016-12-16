#include "orbit.h"

#include "globalvars.h"

orbit::orbit(ball *p, ball *s) {
	planet = p;
	sun = s;

	tickSkip = orbitTickSkip;
	historySize = orbitHistorySize;
	tickNum = 0;
}

void orbit::tick() {
	if (drawOrbits) {
		if (tickNum == 0) {
			if (sun == NULL) {
				history.push_front(planet->position);
			} else {
				history.push_front(point(planet->position.x - sun->position.x, planet->position.y - sun->position.y));
			}
			if (history.size() > (historySize / tickSkip)) {
				history.pop_back();
			}
		}
		if (++tickNum >= tickSkip) {
			tickNum = 0;
		}
	} else if (history.size() > 0) {
		history.clear();
	}
}

void orbit::draw(SDL_Renderer *renderer, int ox, int oy) {
	std::list<point>::const_iterator i;
	point pt1 = planet->position;
	point pt2;

	int pr = planet->getColor().r;
	int pg = planet->getColor().g;
	int pb = planet->getColor().b;
	int alpha = 0xff;

	double n = 0;
	for (i = history.begin(); i != history.end(); ++i) {
		pt2 = *i;
		if (sun != NULL) {
			pt2.x += sun->position.x;
			pt2.y += sun->position.y;
		}
		alpha = orbitFade ? 0xff * (1 - n / historySize) : 0xff;
		SDL_SetRenderDrawColor(renderer, pr, pg, pb, alpha);
		SDL_RenderDrawLine(renderer,
		   (int)pt1.x + ox, (int)pt1.y + oy,
		   (int)pt2.x + ox, (int)pt2.y + oy);
		pt1 = pt2;
		n += tickSkip;
	}
}
