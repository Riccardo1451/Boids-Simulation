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
    std::ofstream outfile("/Users/riccardofantechi/Desktop/Universita/Quarto anno/Parrallel Programming/Boids/Boids_Code/sequential_update.csv");
    outfile << "Time\n";
    int N = 1200; //Number of boids
    bool parallel = false; //Set it to false for the sequential version

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGH), "Flock");

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
    sf::FloatRect bounds = vgaBounds.getGlobalBounds();

    Flock f = Flock(N, parallel);
    std::vector<Boid> flock = f.getFlock();
    int count = 0;

    while (window.isOpen() && count < 1000 ) {
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

        auto start = std::chrono::high_resolution_clock::now();
        if(parallel){
            #pragma omp parallel for
            for (size_t i = 0; i < flock.size(); i++){
                flock[i].update(flock);
                //La funzione di update non deve accedere o modificare altri dati di altri boid il che evita race condition
            }
        } else {
            for (auto &Boid:flock) {
                Boid.update(flock);
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end-start).count();
        std::cout << (parallel ? "Parallel":"Sequential") << " update time " << elapsed << " seconds" << std::endl;
        outfile << elapsed << "\n";
        //implicit barrier
        window.clear();
        for (auto &Boid : flock) {
            Boid.show(window);
        }

        // Mostra il contenuto della finestra
        window.draw(vgaBounds);
        window.display();
        count ++;
    }
    outfile.close();
    return 0;

}