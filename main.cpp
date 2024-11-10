#include <iostream>
#include <SFML/Graphics.hpp>
#include "Boid.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800,600), "SFML, esempio cerchio");

    Boid b1 = Boid(0,0);
    Boid b2 = Boid(200,10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Cancella la finestra con uno sfondo bianco
        window.clear(sf::Color::White);

        // Disegna il cerchio nella finestra
        b1.show(window);
        b2.show(window);

        // Mostra il contenuto della finestra
        window.display();
    }
    return 0;
}