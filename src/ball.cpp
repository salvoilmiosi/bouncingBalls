#include "ball.h"
#include "globalvars.h"

#include "SDL_draw.h"

#include <cstdlib>

void ball::createTexture(SDL_Renderer *renderer) {
	SDL_Surface *image;
	Uint32 rmask,gmask,bmask,amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	double dotPos = 0.8;
	double shadeSize = 0.7;

	color.r = rand() % 0x100;
	color.g = rand() % 0x100;
	color.b = rand() % 0x100;

	image = SDL_CreateRGBSurface(0, radius * 2, radius * 2, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(image, &image->clip_rect, SDL_MapRGBA(image->format, 0x0, 0x0, 0x0, 0x0));
	Draw_FillCircle(image, radius, radius, radius, SDL_MapRGB(image->format, color.r, color.g, color.b));

	if (radius > 10) {
		int iters = 5;
		for(int i=1; i<=iters; ++i) {
			double circSize = shadeSize * (iters-i) / iters;
			int r = (0xff - color.r) * i / iters + color.r;
			int g = (0xff - color.g) * i / iters + color.g;
			int b = (0xff - color.b) * i / iters + color.b;

			Draw_FillCircle(image, radius * dotPos, radius * dotPos, radius * circSize, SDL_MapRGBA(image->format, r, g, b, 0xff));
		}
	}
	if (radius > 4)
		Draw_Circle(image, radius, radius, radius, SDL_MapRGB(image->format, 0x0, 0x0, 0x0));

	texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);
}

ball::ball() {
	dragging = false;

	texture = NULL;
}

ball::~ball() {
	SDL_DestroyTexture(texture);
}

static const double MASS_MULTIPLIER = 1;//(4.0/3.0) * M_PI;

void ball::setRadius(int radius) {
	this->radius = radius;
	mass = MASS_MULTIPLIER * radius * radius * radius;
}

void ball::draw(SDL_Renderer *renderer, int ox, int oy) {
	if (texture == NULL)
		createTexture(renderer);

	SDL_Rect srcrect;
	srcrect.x = 0;
	srcrect.y = 0;

	SDL_QueryTexture(texture, NULL, NULL, &(srcrect.w), &(srcrect.h));

	SDL_Rect dstrect = srcrect;
	dstrect.x = position.x - radius + ox;
	dstrect.y = position.y - radius + oy;

	SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void ball::screenBorder() {
	if (screenBounce) {
		if (position.x + radius > SCREEN_W) {
			if (velocity.x > 0) velocity.x = -velocity.x;
			position.x = SCREEN_W - radius;
		} else if (position.x - radius < 0) {
			if (velocity.x < 0) velocity.x = -velocity.x;
			position.x = radius;
		}

		if (position.y + radius > SCREEN_H) {
			if (velocity.y > 0) velocity.y = -velocity.y;
			position.y = SCREEN_H - radius;
		} else if (position.y - radius < 0) {
			if (velocity.y < 0) velocity.y = -velocity.y;
			position.y = radius;
		}
	} else if (loopBack) {
		if (position.x - radius > SCREEN_W && velocity.x > 0) position.x -= SCREEN_W + radius * 2;
		else if (position.x + radius < 0 && velocity.x < 0) position.x += SCREEN_W + radius * 2;

		if (position.y - radius > SCREEN_H && velocity.y > 0) position.y -= SCREEN_H + radius * 2;
		else if (position.x + radius < 0 && velocity.y < 0) position.y += SCREEN_W + radius * 2;
	}
}

void ball::tick() {
	if (dragging) {
		velocity *= dragForce;
		return;
	} else {
		position += velocity;
	}

	if (downGravity > 0) {
		velocity.y += downGravity;
	} else if (friction>0) {
		double frictionForce = mass * friction;
		double vel = velocity.length();
		double newVel = vel - frictionForce;
		if (newVel<0) {
			velocity.x = 0;
			velocity.y = 0;
		} else {
			velocity.x = vel / velocity.x * newVel;
			velocity.y = vel / velocity.y * newVel;
		}
	}

	screenBorder();
}

void ball::addGravity(ball *b) {
	vector2d delta = b->position - position;
	double d = delta.length();
	if (d<=0) return;
	vector2d accel = delta / d;
	double module = newtonGravity * b->mass / (d*d);
	accel *= module;
	velocity += accel;
}

bool ball::collidesWith(ball *b) {
	int radSum = radius + b->radius;
	if (abs(b->position.x - position.x) > radSum) return false;
	if (abs(b->position.y - position.y) > radSum) return false;
	return position.distance(b->position) < radSum;
}

void ball::startDrag(int x, int y) {
	dragging = true;
	dragOffset = point(x, y) - position;
	velocity = vector2d();
}

void ball::stopDrag() {
	dragging = false;
}

void ball::drag(int x, int y) {
	if (!dragging) return;

	point destin = point(x, y) - dragOffset;
	if (screenBounce) {
		if (destin.x + radius > SCREEN_W) destin.x = SCREEN_W - radius;
		else if (destin.x - radius < 0) destin.x = radius;

		if (destin.y + radius > SCREEN_H) destin.y = SCREEN_H - radius;
		else if (destin.y - radius < 0) destin.y = radius;
	}
	velocity += destin - position;
	position = destin;
}

void ball::resolveCollision(ball *b) {
	vector2d delta = position - b->position;
	double d = delta.length();

	vector2d mtd = delta * ((radius + b->radius - d) / d);

	double im1 = 1.0 / mass;
	double im2 = 1.0 / b->mass;

	if (d == 0) {
		// sono nello stesso posto, li separo in angoli casuali
		double angle = rand() / (double)RAND_MAX * M_PI;
		vector2d moveV(cos(angle), sin(angle));
		position += moveV * radius;
		b->position -= moveV * b->radius;
		return;
	}

	if (!dragging && !b->dragging) {
		position += mtd * (im1 / (im1 + im2));
		b->position -= mtd * (im2 / (im1 + im2));
	} else if (dragging) {
		b->position -= mtd;
	} else if (b->dragging) {
		position += mtd;
	}

	vector2d v = velocity - b->velocity;

	if (v.length()==0) {
		// divisione per 0
		return;
	}

	vector2d normMtd = mtd.normalize();
	double vn = v.normalize().dot(normMtd);
	if (vn > 0) return;

	double i =  -vn * (1.0 + restitution) / (im1+im2);
	vector2d impulse = mtd * i;

	velocity += impulse * im1;
	b->velocity -= impulse * im2;
}
