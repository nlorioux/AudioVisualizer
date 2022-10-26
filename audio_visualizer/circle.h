#pragma once

#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <fftw3.h>
#include <string>
#include "Audio.h"
#include <GL/gl.h>
using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>

GLvoid affichageCircle(float magnitude[30], int size);
