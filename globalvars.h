#ifndef __MAIN_H
#define __MAIN_H

#define GLOBAL_VARS				\
VAR(int, SCREEN_W, 640)			\
VAR(int, SCREEN_H, 480)			\
VAR(int, FPS, 60)				\
VAR(bool, SOLAR_SYSTEM, false)     \
VAR(bool, drawOrbits, false)     \
VAR(unsigned int, orbitTickSkip, 5)       \
VAR(unsigned int, orbitHistorySize, 100)     \
VAR(bool, orbitFade, true)      \
VAR(int, ballNum, 100)			\
VAR(int, minRadius, 10)			\
VAR(int, maxRadius, 40)			\
VAR(int, sunRadius, 50)         \
VAR(int, maxSpeed, 10)			\
VAR(int, minDistance, 200)       \
VAR(int, maxDistance, 500)      \
VAR(double, downGravity, 0.2)	\
VAR(double, newtonGravity, 0)	\
VAR(double, restitution, 0.8)   \
VAR(bool, screenBounce, true)	\
VAR(bool, loopBack, false)  	\
VAR(bool, enableDrag, true)		\
VAR(double, dragForce, 0.5)		\
VAR(double, friction, 0)

#define VAR(type, name, default) extern type name;

GLOBAL_VARS

static const char *const OPTIONS_FILENAME = "options.txt";
static const char *const OPTIONS_FIRST_LINE = "Palle Rimbalzanti: File di opzioni";

void readOptions();
void saveOptions();
void optionsDialog();

#endif // __MAIN_H
