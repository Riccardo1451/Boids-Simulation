#include <iostream>
#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Flock.h"

#define WIDHT 1290
#define HEIGH 720

#define vgaWIDTH 800
#define vgaHEIGH 500

int main() {
    int N = 40;
    sf::RenderWindow window(sf::VideoMode(WIDHT,HEIGH), "SFML, esempio cerchio");

    sf::RectangleShape vgaBounds(sf::Vector2f(vgaWIDTH, vgaHEIGH));  // 800x500 è la dimensione dell'area VGA
    vgaBounds.setFillColor(sf::Color::Transparent);
    vgaBounds.setOutlineColor(sf::Color::White);
    vgaBounds.setOutlineThickness(2.f);
    vgaBounds.setPosition(WIDHT/2 - vgaWIDTH/2, HEIGH/2 - vgaHEIGH/2); // Centrato nella finestra

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
            Boid.edges(vgaWIDTH,vgaHEIGH);
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