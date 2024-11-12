#include "Boid.h"
#include <SFML/Graphics.hpp>
#include <random>

Boid::Boid(float const posX, float const posY) {

    static std::random_device rd;  // Seme per il generatore
    static std::mt19937 gen(rd()); // Generatore Mersenne Twister
    std::uniform_real_distribution<> distribuzione(-0.05, 0.05);

    x = posX;
    y = posY;
    Vx = distribuzione(gen);
    Vy = distribuzione(gen);
    /*Vx = 0.1;
    Vy = 0.1;*/
    angle = 0.0f;

    std::random_device rdb;        // Generatore di entropia
    std::mt19937 genb(rdb());       // Motore di numeri casuali
    std::bernoulli_distribution d(0.5); // Distribuzione bernoulliana con p=0.5
    scout = d(genb);
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

bool Boid::isScout() const {
    return scout;
}

void Boid::show(sf::RenderWindow &window) {

    sf::CircleShape triangle(10,3);
    triangle.setFillColor(sf::Color::White);


    angle = (180 * std::atan2(Vy, Vx) / M_PI)+90;


    triangle.setPosition(x,y);
    triangle.setRotation(angle);
    window.draw(triangle);
}

void Boid::update() {
    x += Vx;
    y += Vy;
}

void Boid::align(std::vector<Boid> flock) {
    float xvel_avg = 0;
    float yvel_avg = 0;
    int neighboring_boids = 0;

    for (auto &otherboid:flock) {
        float dx = x-otherboid.get_x();
        float dy = y-otherboid.get_y();
        if(&otherboid != this && abs(dx)<visualrange && abs(dy)<visualrange) {
            float squareddistance = dx*dx + dy*dy;
            if(squareddistance < visualrange*visualrange) {
                xvel_avg += otherboid.get_vx();
                yvel_avg += otherboid.get_vy();
                neighboring_boids += 1;
            }
        }
    }
    if(neighboring_boids > 0) {
        xvel_avg = xvel_avg/neighboring_boids ;
        yvel_avg = yvel_avg/neighboring_boids;

        Vx += (xvel_avg-Vx) * matchingFactor;
        Vy += (yvel_avg-Vy) * matchingFactor;
    }

}

void Boid::edges(float width, float heigh) {
    if (x > width+245) {
        Vx -= turnfactor;
    }else if (x < 245) {
        Vx += turnfactor;
    }
    if (y > heigh+110) {
        Vy -= turnfactor;
    }else if (y < 110) {
        Vy += turnfactor;
    }
}


