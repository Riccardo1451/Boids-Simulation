#include <iostream>
#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Flock.h"
#include "omp.h"
#include "chrono"
#include <fstream>

#define WIDTH 2560
#define HEIGH 1440

#define vgaWIDTH 1280
#define vgaHEIGH 720

int main() {
    //std::ofstream outfile("/Users/riccardofantechi/Desktop/Universita/Quarto anno/Parrallel Programming/Boids/Boids_Code/ExecutionTime/parallel_update_8_threads.csv");
    //outfile << "Time\n";
    int N = 1200; //Number of boids
    bool parallel = true; //Set it to false for the sequential version

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGH), "Flock");
    window.setFramerateLimit(60);
    window.setView(window.getDefaultView());

    sf::RectangleShape vgaBounds(sf::Vector2f(vgaWIDTH, vgaHEIGH));  // 800x500 è la dimensione dell'area VGA
    vgaBounds.setFillColor(sf::Color::Transparent);
    vgaBounds.setOutlineColor(sf::Color::White);
    vgaBounds.setOutlineThickness(2.f);
    float vx = (WIDTH - vgaWIDTH)/2;
    float vy = (HEIGH - vgaHEIGH)/2;

    std::cout << vx << vy << std::endl;

    //vgaBounds.setPosition((WIDTH - vgaWIDTH)/2, (HEIGH - vgaHEIGH)/2 ); // Centrato nella finestra
    vgaBounds.setPosition(240, 100);

    Flock f = Flock(N, parallel);
    std::vector<Boid>& currentFlock = f.getCurrentFlock();
    std::vector<Boid>& nextFlock = f.getNextFlock();


    int frameCount = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Cancella la finestra con uno sfondo bianco
        window.clear(sf::Color::Black);

        //auto start = std::chrono::high_resolution_clock::now();
        if(parallel){
            #pragma omp parallel for
            for (size_t i = 0; i < currentFlock.size() ; i++){
                currentFlock[i].update(currentFlock,nextFlock[i]);
                //La funzione di update non deve accedere o modificare altri dati di altri boid il che evita race condition
            }
            currentFlock.swap(nextFlock);
        } else {
           for(size_t i = 0; i < currentFlock.size(); i++) {
               currentFlock[i].update(currentFlock, nextFlock[i]);
           }
            currentFlock.swap(nextFlock);
        }

        //auto end = std::chrono::high_resolution_clock::now();
        //double elapsed = std::chrono::duration<double>(end-start).count();
        //std::cout << (parallel ? "Parallel":"Sequential") << " update time " << elapsed << " seconds" << std::endl;
        //outfile << elapsed << "\n";
        //implicit barrier
        window.clear();

        for (auto Boid : currentFlock) {
            Boid.show(window);
        }

        // Mostra il contenuto della finestra
        window.draw(vgaBounds);
        window.display();
        frameCount ++;
    }
    //outfile.close();
    return 0;

}

//######## CODICE PER CALCOLO SCALABILITA DEI BOIDS
/*#include <iostream>
#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Flock.h"
#include "omp.h"
#include <chrono>
#include <fstream>
#include <vector>

#define WIDTH 2560
#define HEIGH 1440

#define vgaWIDTH 1280
#define vgaHEIGH 720

int main() {
    std::ofstream outfile("/Users/riccardofantechi/Desktop/Universita/Quarto anno/Parrallel Programming/Boids/Boids_Code/ExecutionTime/boids_scalability.csv");
    outfile << "Num_Boids,Num_Threads,Execution_Time\n";

    std::vector<int> boids_count = {100, 200, 400, 800, 1600, 3200}; // Numero di boids da testare
    std::vector<int> max_threads = {1, 2, 4, 8, 16, 32, 64}; // Numero massimo di thread disponibili

    for (int num_boids : boids_count) {
        for (int num_threads : max_threads) {
            omp_set_num_threads(num_threads);

            // Configurazione della finestra SFML (puoi disabilitarla per test più rapidi)
            sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGH), "Flock");
            window.setFramerateLimit(60);
            window.setView(window.getDefaultView());

            sf::RectangleShape vgaBounds(sf::Vector2f(vgaWIDTH, vgaHEIGH));
            vgaBounds.setFillColor(sf::Color::Transparent);
            vgaBounds.setOutlineColor(sf::Color::White);
            vgaBounds.setOutlineThickness(2.f);
            vgaBounds.setPosition(240, 100);

            Flock f(num_boids, true); // Inizializza il flock con il numero di boid
            std::vector<Boid>& currentFlock = f.getCurrentFlock();
            std::vector<Boid>& nextFlock = f.getNextFlock();

            int frameCount = 0;
            double total_time = 0.0; // Tempo totale per il test
            const int num_frames = 1000; // Numero di frame da simulare

            while (frameCount < num_frames) {
                // Simula una sola iterazione (nessuna interazione con l'utente per questo test)
                auto start = std::chrono::high_resolution_clock::now();
                #pragma omp parallel for
                for (size_t i = 0; i < currentFlock.size(); i++) {
                    currentFlock[i].update(currentFlock, nextFlock[i]);
                }
                currentFlock.swap(nextFlock);
                auto end = std::chrono::high_resolution_clock::now();

                double elapsed = std::chrono::duration<double>(end - start).count();
                total_time += elapsed;
                frameCount++;
            }

            // Salva il tempo medio per boid e numero di thread
            double avg_time = total_time / num_frames;
            outfile << num_boids << "," << num_threads << "," << avg_time << "\n";
            std::cout << "Num_Boids: " << num_boids << ", Threads: " << num_threads
                      << ", Avg Execution Time: " << avg_time << "s\n";

            window.close(); // Chiudi la finestra dopo ogni configurazione
        }
    }

    outfile.close();
    return 0;
}*/
