#include "Boid.h"
#include <SFML/Graphics.hpp>

Boid::Boid(float const posX, float const posY) {
    x = posX;
    y = posY;
}

float Boid::get_x() const {
    return x;
}

void Boid::set_x(float x) {
    this->x = x;
}

float Boid::get_y() const {
    return y;
}

void Boid::set_y(float y) {
    this->y = y;
}

float Boid::get_vx() const {
    return Vx;
}

void Boid::set_vx(float vx) {
    Vx = vx;
}

float Boid::get_vy() const {
    return Vy;
}

void Boid::set_vy(float vy) {
    Vy = vy;
}

void Boid::show(sf::RenderWindow &window) {

    sf::CircleShape triangle(15,3);
    triangle.setFillColor(sf::Color::Blue);
    sf::Vector2u windowSize = window.getSize();

    //calcolo del centro della finestra
    float centerX = windowSize.x / 2.f;
    float centerY = windowSize.y / 2.f;

    triangle.setPosition(x,y);
    window.draw(triangle);
}
