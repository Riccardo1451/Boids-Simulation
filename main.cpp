#include <iostream>
#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Flock.h"

#define WIDTH 2560
#define HEIGH 1440

#define vgaWIDTH 1280
#define vgaHEIGH 720

int main() {
    int N = 300;
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

    Flock f = Flock(N);
    std::vector<Boid> flock = f.getFlock();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Cancella la finestra con uno sfondo bianco
        window.clear(sf::Color::Black);

        for (auto &Boid : flock) {
            Boid.align(flock);
            Boid.cohesion(flock);
            Boid.separation(flock);
            Boid.edges(vgaBounds.getPosition().x + vgaBounds.getSize().x,vgaBounds.getPosition().y+vgaBounds.getSize().y);
            Boid.update();
        }
        for (auto &Boid : flock) {
            Boid.show(window);
        }

        // Mostra il contenuto della finestra
        window.draw(vgaBounds);
        window.display();
    }
    return 0;
}