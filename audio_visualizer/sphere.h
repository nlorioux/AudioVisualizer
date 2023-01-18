#pragma once

#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <fftw3.h>
#include <string>
#include <ctime>

#include "Audio.h"

#include <Windows.h>
#include <GL/gl.h>
#include <gl/GLU.h>

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>

const int N_ = 30;
const int M_ = 20;

GLvoid affichageSphere(float magnitude[], int size);
GLvoid affichageSphere(float magnitude[], int size, float max_value);
GLvoid initSphere(sf::RenderWindow& window);
GLvoid lookAt(float eyex, float eyey, float eyez,
              float centerx, float centery, float centerz,
              float upx, float upy, float upz);
GLvoid displaySquare(const float magnitudes[N_][M_], const int& N, const int& M, const int i, const int j, const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ);
float getMean(const float& a, const float& b, const float& c, const float& d, const float& degree);
GLvoid addPoint(const int& i, const int& j, const int& N, const int& M, const float magnitudes[N_][M_], const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ);
GLvoid addPoint(const float& i, const float& j, const int& N, const int& M, const float& magnitude, const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ);
