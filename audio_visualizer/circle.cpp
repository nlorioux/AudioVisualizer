#include "circle.h"

// Displays the full circle
GLvoid affichageCircle(float magnitude[50], int size) {
    glClear(GL_COLOR_BUFFER_BIT);

    float indent = 2*M_PI/size;
    float angle = M_PI/2;
    float innerRadius = 0.2;
    float torRadius = 1.0 - innerRadius;
    float centerX = 0.0;
    float centerY = 0.0;

    glBegin(GL_LINE_STRIP);

    // First half of the circle
    for (int i = 0; i < size/2; i++) {
        glVertex2f(centerX + cos(angle) * (innerRadius + (torRadius * magnitude[i])), centerY + sin(angle) * (innerRadius + (torRadius * magnitude[i])));
        angle += indent;
    }

    // Second half of the circle : mirror of the first
    for (int i = size/2-1; i >=0 ; i--) {
        glVertex2f(centerX + cos(angle) * (innerRadius + (torRadius * magnitude[i])), centerY + sin(angle) * (innerRadius + (torRadius * magnitude[i])));
        angle += indent;
    }

    // Force the circle to loop on itself
    glVertex2f(centerX + cos(angle) * (innerRadius + (torRadius * magnitude[0])), centerY + sin(angle) * (innerRadius + (torRadius * magnitude[0])));

    glEnd();

    // Force display of openGL
    glFlush();
}