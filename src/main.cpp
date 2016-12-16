#include "SDL2/SDL.h"

#include <vector>

#include <cstdio>
#include <cstdlib>

#include "globalvars.h"
#include "ball.h"
#include "orbit.h"
#include "timer.h"

static SDL_Window *window;
static SDL_Renderer *renderer;

typedef std::vector<ball *> ballList;
typedef std::vector<orbit *> orbitList;

static ballList balls;
static orbitList orbits;

static ball *centerBall = NULL;
static int ox = 0, oy = 0;

static const int moveSpeed = 5;

static ball *dragging = NULL;

static const char *const LOG_FILE_NAME = "bouncingBalls.log";

int random_between(int min_num, int max_num) {
	if (max_num > min_num) {
		return rand() % (max_num - min_num) + min_num;
	} else {
		return min_num;
	}
}

point offset() {
	if (centerBall == NULL) {
		return point(ox, oy);
	} else {
		return point(SCREEN_W / 2 - centerBall->position.x, SCREEN_H / 2 - centerBall->position.y) ;
	}
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	window = SDL_CreateWindow("Palle rimbalzanti",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		return false;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}

ball *addPlanet(ball *sun, double distance, int radius, double angle) {
	ball *planet = new ball;
	planet->setRadius(radius);

	planet->position.x = sun->position.x + cos(angle)*distance;
	planet->position.y = sun->position.y + sin(angle)*distance;

	double velocity = -sqrt(newtonGravity * sun->getMass() / distance);

	angle += M_PI / 2.0;

	planet->velocity = sun->velocity;
	planet->velocity.x += cos(angle) * velocity;
	planet->velocity.y += sin(angle) * velocity;

	balls.push_back(planet);

	return planet;
}

ball *addPlanet(ball *sun, double distance, int radius) {
	double angle = (rand() % 2000) / 1000.0 * M_PI;
	return addPlanet(sun, distance, radius, angle);
}

orbit *addOrbit(ball *planet, ball *sun) {
	orbit *o = new orbit(planet, sun);

	orbits.push_back(o);

	return o;
}

void initBalls() {
	srand(SDL_GetTicks());

	ball *b;
	int radius;

	if (SOLAR_SYSTEM) {
		ball *sun = new ball;
		sun->position.x = SCREEN_W / 2;
		sun->position.y = SCREEN_H / 2;
		sun->setRadius(sunRadius);
		sun->velocity.x = 0;
		sun->velocity.y = 0;
		balls.push_back(sun);
		orbit *sunOrbit = addOrbit(sun, NULL);
		sunOrbit->historySize = 10000;
		sunOrbit->tickSkip = 50;

		centerBall = sun;

		int distance;

		orbit *o;

		for (int i=0; i<ballNum; ++i) {
			distance = random_between(minDistance, maxDistance);
			radius = random_between(minRadius, maxRadius);
			b = addPlanet(sun, distance, radius);
			o = addOrbit(b, sun);
			o->historySize = distance * 2 * M_PI / b->velocity.length();
		}
	} else {
		for (int i=0; i<ballNum; ++i) {
			b = new ball;
			radius = random_between(minRadius, maxRadius);
			b->setRadius(radius);

			b->position.x = random_between(radius, SCREEN_W - radius);
			b->position.y = random_between(radius, SCREEN_H - radius);

			if (maxSpeed > 0) {
				b->velocity.x = random_between(-maxSpeed, maxSpeed);
				b->velocity.y = random_between(-maxSpeed, maxSpeed);
			}
			balls.push_back(b);
			addOrbit(b, NULL);
		}
	}
}

bool render() {
	point o = offset();

	SDL_SetRenderDrawColor(renderer, 0xff,0xff,0xff,0xff);
	SDL_RenderClear(renderer);

	if (screenBounce && (o.x != 0 || o.y != 0)) {
		SDL_SetRenderDrawColor(renderer, 0x0,0x0,0x0,0xff);
		SDL_Rect boundRect;
		boundRect.x = o.x;
		boundRect.y = o.y;
		boundRect.w = SCREEN_W;
		boundRect.h = SCREEN_H;
		SDL_RenderDrawRect(renderer, &boundRect);
	}

	for(ballList::const_iterator i = balls.begin(); i != balls.end(); ++i) {
		(*i)->draw(renderer, o.x, o.y);
	}

	if (drawOrbits) {
		for(orbitList::const_iterator i = orbits.begin(); i != orbits.end(); ++i) {
			(*i)->draw(renderer, o.x, o.y);
		}
	}

	SDL_RenderPresent(renderer);

	return true;
}

bool tick() {
	if (centerBall == NULL) {
		const Uint8 *keysHeld = SDL_GetKeyboardState(NULL);
		if (keysHeld[SDL_SCANCODE_LEFT]) ox -= moveSpeed;
		if (keysHeld[SDL_SCANCODE_RIGHT]) ox += moveSpeed;
		if (keysHeld[SDL_SCANCODE_UP]) oy -= moveSpeed;
		if (keysHeld[SDL_SCANCODE_DOWN]) oy += moveSpeed;
	}

	ballList::const_iterator i, j;
	for(i = balls.begin(); i != balls.end(); ++i) {
		(*i)->tick();
	}

	for(orbitList::const_iterator i = orbits.begin(); i != orbits.end(); ++i) {
		(*i)->tick();
	}

	ball *ba, *bb;
	for (i = balls.begin(); i != balls.end(); ++i) {
		ba = *i;
		for (j = balls.begin(); j != balls.end(); ++j) {
			bb = *j;
			if (ba==bb) continue;
			if (newtonGravity > 0) {
				ba->addGravity(bb);
			}
			if (ba->collidesWith(bb)) {
				ba->resolveCollision(bb);
				//continue;
			}
		}
	}

	return true;
}

void cleanUp() {
	for (ballList::const_iterator i = balls.begin(); i != balls.end(); ++i) {
		delete *i;
	}

	for (orbitList::const_iterator i = orbits.begin(); i != orbits.end(); ++i) {
		delete *i;
	}

	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

ball *findBall(int x, int y) {
	point o = offset();
	point pt(x-o.x, y-o.y);
	ballList::const_iterator i;
	ball *b;
	for(i = balls.begin(); i != balls.end(); ++i) {
		b = *i;
		if ((pt - b->position).length() < b->getRadius()) {
			return b;
		}
	}
	return NULL;
}

void logBallInfo() {
	static FILE *logfile = NULL;
	static int logCount = 1;

	if (logfile==NULL) {
		logfile = fopen(LOG_FILE_NAME, "w");
	} else {
		logfile = fopen(LOG_FILE_NAME, "a");
	}

	ballList::const_iterator i;
	ball *b;
	int n = 0;

	fprintf(logfile, "====== bouncingBalls log no. %d ======\n\n", logCount);

	for (i = balls.begin(); i != balls.end(); ++i) {
		b = *i;
		fprintf(logfile, "ball[%d]:\n  position = (%f, %f)\n  velocity = (%f, %f)\n  radius = %d\n",
			   n, b->position.x, b->position.y, b->velocity.x, b->velocity.y, b->getRadius());
		++n;
	}

	fprintf(logfile, "\n");

	fclose(logfile);
	++logCount;
}

void handleEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			dragging = findBall(event.button.x, event.button.y);
			if (dragging != NULL && enableDrag)
				dragging->startDrag(event.button.x, event.button.y);
			break;
		case SDL_BUTTON_RIGHT:
			centerBall = findBall(event.button.x, event.button.y);
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (dragging != NULL && enableDrag)
			dragging->stopDrag();
		dragging = NULL;
		break;
	case SDL_MOUSEMOTION:
		if (dragging != NULL)
			dragging->drag(event.motion.x, event.motion.y);
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_o:
			drawOrbits = !drawOrbits;
			break;
		case SDLK_l:
			logBallInfo();
			break;
		case SDLK_b:
			screenBounce = !screenBounce;
			break;
		case SDLK_f:
			orbitFade = !orbitFade;
			break;
		case SDLK_x:
			optionsDialog();
			break;
		}
		break;
	case SDL_WINDOWEVENT:
		switch (event.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SCREEN_W = event.window.data1;
			SCREEN_H = event.window.data2;
		}
		break;
	}
}

int main (int argc, char** argv) {
	const char *dir = argc > 0 ? argv[0] : nullptr;
	readOptions(dir);

	if (!init()) return 1;

	initBalls();

	timer fps;
	int frame = 0;

	SDL_Event event;

	bool quit = false;
	while(!quit) {
		fps.start();

		if (!tick()) return 2;
		if (!render()) return 3;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			default:
				handleEvent(event);
			}
		}

		++frame;

		if(fps.get_ticks() < 1000 / FPS) {
			SDL_Delay(1000 / FPS - fps.get_ticks());
		}
	}

	cleanUp();
	saveOptions(dir);
	return 0;
}
