// audio_visualizer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fftw3.h>
#include <string>
#include "Audio.h"
using namespace std;

void fft(fftw_complex* in, fftw_complex* out, int N);
void displayFFT(fftw_complex* fft, int N);

int main(int argc, char** argv)
{
    const string audio_file = "Glass_Caves_Who_Are_You.wav";
    Audio audio = Audio(audio_file);

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML WORK !");
    sf::Music music;
    if (!music.openFromFile(audio_file))
        return -1; // error
    music.play();
    /*
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        window.clear();
        window.display();
    }*/

    const int batchSize = 10000;
    fftw_complex input[batchSize]; // double x[n][2];
    fftw_complex output[batchSize];
    float totalNumberBatch = audio.getSamplesCount() / batchSize;
    sf::Time batchDuration = audio.getDuration() / totalNumberBatch;
    cout << "totalNumberBatch :" << totalNumberBatch << endl;

    for (int batchNumber = 0; batchNumber < totalNumberBatch; batchNumber++) {
        //Fill batch of audio
        for (int i = 0; i < batchSize; i++) {
            input[i][0] = audio.getSamples()[batchNumber + i];
            input[i][1] = 0;
            //cout << "Input : " << input[i][0] << endl;
        }

        /*
        for (int i = 0; i < audio.getSamplesCount(); i++) {
            cout << audio.getSamples()[i];
        }*/

        fft(input, output, batchSize);
        cout << "fft_ok" << endl;
        sf::sleep(batchDuration);
    }
    displayFFT(output, batchSize);

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

void displayFFT(fftw_complex* fft, int N) {
    for (int i = 0; i < N; i++) {
        //cout << fft[i][0] << " | " << fft[i][1] << endl;
    }
}


// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
