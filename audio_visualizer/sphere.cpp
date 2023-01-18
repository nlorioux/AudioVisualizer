#include "sphere.h"

sf::Clock clockSphere; // Timer for the sphere rotation
float meanDegree = 3.0f;

// Displays a sphere which size do not vary
GLvoid affichageSphere(float magnitude[15], int size) {

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    std::cout << "elapsed time : " << clockSphere.getElapsedTime().asSeconds() << std::endl;

    int nbSphere = 20;

    float indentCircle = M_PI / size;
    float indentSphere = M_PI / nbSphere;
    float angleCircle = M_PI / 2;
    float angleSphere = 0;
    float innerRadius = 0.6;
    float outerRadius = 1.0;
    float centerX = 0.0;
    float centerY = 0.0;
    float centerZ = 0.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 


    // Code for the rotation of the camera
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 50.0f, 1.f, 0.f, 0.f);
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 30.0f, 0.f, 1.f, 0.f);
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 90.0f, 0.f, 0.f, 1.f);

    float magnitudes[30][20];

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 10; j++) {
            magnitudes[i][j] = magnitude[i];
            magnitudes[29 - i][j] = magnitude[i];
            magnitudes[i][19-j] = magnitude[i];
            magnitudes[29 - i][19-j] = magnitude[i];
        }
    }    

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 20; j++) {
            displaySquare(magnitudes, 30, 20, i, j, innerRadius, outerRadius, centerX, centerY, centerZ);
        }
    }
    glFlush();
}

// Displays a sphere which size follows the current intensity
GLvoid affichageSphere(float magnitude[15], int size, float max_value) {

    cout << "max_value : " << max_value << endl;

    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << "elapsed time : " << clockSphere.getElapsedTime().asSeconds() << std::endl;

    int nbSphere = 20;

    float indentCircle = M_PI / size;
    float indentSphere = M_PI / nbSphere;
    float angleCircle = M_PI / 2;
    float angleSphere = 0;
    float innerRadius = 0.6 + max_value / 1e9;
    float outerRadius = 1.0 + 2 * max_value / 1e9;
    float centerX = 0.0;
    float centerY = 0.0;
    float centerZ = 0.0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 


    // Code for the rotation of the camera
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 50.0f, 1.f, 0.f, 0.f);
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 30.0f, 0.f, 1.f, 0.f);
    glRotatef(clockSphere.getElapsedTime().asSeconds() * 90.0f, 0.f, 0.f, 1.f);

    float magnitudes[30][20];

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 10; j++) {
            magnitudes[i][j] = magnitude[i];
            magnitudes[29 - i][j] = magnitude[i];
            magnitudes[i][19-j] = magnitude[i];
            magnitudes[29 - i][19-j] = magnitude[i];
        }
    }    

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 20; j++) {
            displaySquare(magnitudes, 30, 20, i, j, innerRadius, outerRadius, centerX, centerY, centerZ);
        }
    }
    glFlush();
}


GLvoid initSphere(sf::RenderWindow& window) {
    window.setActive(true);

    clockSphere.restart();


    float width = window.getSize().x;
    float height = window.getSize().y;
    GLfloat ratio = static_cast<float>(width) / static_cast<float>(height);

    // Setup a perspective projection
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, 500.0f);

    sf::View view;
    view.setSize(width, height);
    view.setCenter(width/2, height/2);
    window.setView(view);
    lookAt(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);

    window.setActive(false);
}

GLvoid lookAt(float eyex, float eyez, float eyey,
              float centerx, float centery, float centerz,
              float upx, float upy, float upz) {
    int i;
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    float forwardNorm = sqrt(pow(forward[0], 2) + pow(forward[1], 2) + pow(forward[2], 2));
    forward[0] /= forwardNorm;

    /* Side = forward x up */
    side[0] = forward[1]*up[2] - forward[2]*up[1];
    side[1] = forward[2]*up[0] - forward[0]*up[2];
    side[2] = forward[0]*up[1] - forward[1]*up[0];

    float sideNorm = sqrt(pow(side[0], 2) + pow(side[1], 2) + pow(side[2], 2));
    side[0] /= sideNorm;

    /* Recompute up as: up = side x forward */

    up[0] = side[1] * forward[2] - side[2] * forward[1];
    up[1] = side[2] * forward[0] - side[0] * forward[2];
    up[2] = side[0] * forward[1] - side[1] * forward[0];

    //__gluMakeIdentityf(&m[0][0]);

    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];
    m[3][0] = 0;

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];
    m[3][1] = 0;

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];
    m[3][2] = 0;

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;
    m[3][3] = 1;

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}

// Displays one square of the mesh, with its center and triangulation
GLvoid displaySquare(const float magnitudes[30][20], const int& N, const int& M, const int i, const int j, const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ) {

    if ((i < 0) || (i >= N) || (j < 0) || (j >= M)) {
        std::cerr << "Valeur d'index hors des limites imposées" << ": i = " << i << " et j = " << j << std::endl;
    }

    float centerMagnitude = 0.0f;

    if (i == N - 1) {
        if (j == M - 1) {
            centerMagnitude = getMean(magnitudes[i][j], magnitudes[0][j], magnitudes[i][0], magnitudes[0][0], meanDegree);

            glBegin(GL_LINE_STRIP);

            addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i+1, j, N, M, magnitudes[0][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i, j+1, N, M, magnitudes[i][0], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i+1, j+1, N, M, magnitudes[0][0], innerRadius, outerRadius, centerX, centerY, centerZ);

            glEnd();
        }
        else {
            centerMagnitude = getMean(magnitudes[i][j], magnitudes[0][j], magnitudes[i][j + 1], magnitudes[0][j + 1], meanDegree);

            glBegin(GL_LINE_STRIP);

            addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i+1, j, N, M, magnitudes[0][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i, j + 1, N, M, magnitudes[i][j + 1], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
            addPoint(i+1, j + 1, N, M, magnitudes[0][j + 1], innerRadius, outerRadius, centerX, centerY, centerZ);

            glEnd();
        }
    }
    else if (j == M - 1) {
        centerMagnitude = getMean(magnitudes[i][j], magnitudes[i + 1][j], magnitudes[i][0], magnitudes[i + 1][0], meanDegree);

        glBegin(GL_LINE_STRIP);

        addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i + 1, j, N, M, magnitudes[i + 1][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i, j+1, N, M, magnitudes[i][0], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i + 1, j+1, N, M, magnitudes[i + 1][0], innerRadius, outerRadius, centerX, centerY, centerZ);

        glEnd();
    }
    else {
        centerMagnitude = getMean(magnitudes[i][j], magnitudes[i + 1][j], magnitudes[i][j + 1], magnitudes[i + 1][j + 1], meanDegree);

        glBegin(GL_LINE_STRIP);

        addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i + 1, j, N, M, magnitudes[i + 1][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i, j + 1, N, M, magnitudes[i][j + 1], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i, j, N, M, magnitudes[i][j], innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint((float)i + 0.5, (float)j + 0.5, N, M, centerMagnitude, innerRadius, outerRadius, centerX, centerY, centerZ);
        addPoint(i + 1, j + 1, N, M, magnitudes[i + 1][j + 1], innerRadius, outerRadius, centerX, centerY, centerZ);

        glEnd();
    }
}

float getMean(const float& a, const float& b, const float& c, const float& d, const float& degree) {
    return pow(pow(a, degree) + pow(b, degree) + pow(c, degree) + pow(d, degree), 1.0f / degree);
}

// Adds a point to the current OpenGL shape
GLvoid addPoint(const int& i, const int& j, const int& N, const int& M, const float magnitudes[30][20], const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ) {
    float theta = 0 + M_PI * (i) / (float)N;
    float phi = 0 + M_PI * 2 * (j) / (float)M;
    glColor3f(i / N, j / M, 0.5);
    glVertex3f(centerX + (innerRadius + (outerRadius - innerRadius) * magnitudes[i][j]) * cos(theta) * sin(phi),
               centerZ + (innerRadius + (outerRadius - innerRadius) * magnitudes[i][j]) * cos(phi),
               centerY + (innerRadius + (outerRadius - innerRadius) * magnitudes[i][j]) * sin(theta) * sin(phi));
}

// Adds a point to the current OpenGL shape
GLvoid addPoint(const float& i, const float& j, const int& N, const int& M, const float& magnitude, const float& innerRadius, const float& outerRadius, const float& centerX, const float& centerY, const float& centerZ){
    float theta = 0 + M_PI * (i) / (float)N;
    float phi = 0 + M_PI * 2 * (j) / (float)M;
    glColor3f(abs(i - 15) * 2 / N, abs(j - 10) * 2 / M, 0.5);
    glVertex3f(centerX + (innerRadius + (outerRadius - innerRadius) * magnitude) * cos(theta)* sin(phi),
               centerZ + (innerRadius + (outerRadius - innerRadius) * magnitude)* cos(phi),
               centerY + (innerRadius + (outerRadius - innerRadius) * magnitude) * sin(theta)* sin(phi));
}