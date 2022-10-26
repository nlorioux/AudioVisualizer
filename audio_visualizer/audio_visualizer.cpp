//#include <Windows.h>
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

void fft(fftw_complex* in, fftw_complex* out, int N);
void displayFFT(fftw_complex* fft, int N);
void initGL();

/* Display fft curve with openGL
*/
GLvoid affichage(float magnitude[30], int size) {
    glClear(GL_COLOR_BUFFER_BIT);

    float gap = 1;
    float coordX = 0;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < size; i++) {
        glVertex2f(((float)coordX)/ size - 0.5, ((float)magnitude[i]) - 0.5);
        coordX += gap;
    }
    glEnd();

    // Force display of openGL
    glFlush();
}


int main(int argc, char** argv)
{
    //const string audio_file = "Glass_Caves_Who_Are_You.wav";
    //const string audio_file = "BEEP.wav";
    const string audio_file = "Nyan-Cat.wav";
    //const string audio_file = "bip_fft.wav";
    Audio audio = Audio(audio_file);
    sf::Clock clock;

    sf::Music music;
    if (!music.openFromFile(audio_file))
        return -1; // error
    cout << "SampleRate : " << music.getSampleRate() << endl;
    music.play();

    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");
    window.setVerticalSyncEnabled(true);

    // activate window
    window.setActive(true);

    initGL();
    // Configure the viewport (the same size as the window)
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);

    // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // Disable normal and color vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // desactivate window
    window.setActive(false);

    // Init variables
    const int batchSize = 3200;
    fftw_complex input[batchSize];
    fftw_complex output[batchSize];
    float totalNumberBatch = audio.getSamplesCount() / batchSize;
    sf::Time batchDuration = audio.getDuration() / totalNumberBatch;
    vector<int> power;
    const int barTotalNumber = 100;
    float summedPower[100];

    //Main calcul Loop
    bool first = true;
    float maxChunkFreq = 1;

    for (int batchNumber = 0; batchNumber < totalNumberBatch; batchNumber++) {
        //Fill batch of audio
        for (int i = 0; i < batchSize && i + batchNumber * batchSize < audio.getSamplesCount(); i++) {
            input[i][0] = audio.getSamples()[batchNumber * batchSize + i];
            input[i][1] = 0;
        }

        // Calulate fft of batch
        fft(input, output, batchSize);

        power.clear();

        // Calculate magnitude of batch
        for (int i = 0; i < batchSize / 4 && (i + batchNumber * batchSize < audio.getSamplesCount()); i++) {
            power.push_back(sqrt(pow(output[i + 1][0], 2.0) + pow(output[i + 1][1], 2.0)));
        }

        // Sum through bars to have barTotalNumber bars
        for (int i = 0; i < barTotalNumber; i++) {
            summedPower[i] = 0;
            for (int j = 0; j < batchSize / 4 / barTotalNumber && j + i * batchSize / 4 / barTotalNumber < power.size(); j++) {
                summedPower[i] += power[j + i * batchSize / 4 / barTotalNumber];
            }
            
            // To avoid too high values
            if (summedPower[i] > maxChunkFreq) {
                maxChunkFreq = summedPower[i];
            }
        }
        for (int i = 0; i < barTotalNumber; i++) {
            summedPower[i] = (float)summedPower[i] / maxChunkFreq;
        }

        // Make the window the active window for OpenGL calls
        if (!window.setActive(true))
        {
            std::cerr << "Failed to set window to active" << std::endl;
            return EXIT_FAILURE;
        }


        // Erase framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_TEXTURE_2D);

        affichage(summedPower, barTotalNumber);
        
        // Play music
        if (first) {
            music.play();
            first = false;
        }

        // Draw the background
        window.pushGLStates();
        window.popGLStates();

        // end current frame
        window.display();

        // Make the window no longer the active window for OpenGL calls
        if (!window.setActive(false))
        {
            std::cerr << "Failed to set window to inactive" << std::endl;
            return EXIT_FAILURE;
        }

        // Manage window events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // Stop program
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::Resized)
            {
                // Ajust viewport when window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // Delay start of new batch to sync with music
        sf::sleep(batchDuration - clock.getElapsedTime());
        clock.restart();
    }
    return 0;
}

void fft(fftw_complex* in, fftw_complex* out, int N) {
    //create a DFT plan and execute it
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    //clean
    fftw_destroy_plan(plan);
    fftw_cleanup();
}


void initGL() {
    // Define erasing color of framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Deactivate Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Correct perspective
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
