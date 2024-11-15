#include "Boid.h"
#include <SFML/Graphics.hpp>
#include <random>
#define vgaWIDTH 1280
#define vgaHEIGH 720

Boid::Boid(float const posX, float const posY) {

    static std::random_device rd;  // Seme per il generatore
    static std::mt19937 gen(rd()); // Generatore Mersenne Twister
    std::uniform_real_distribution<> distribuzione(-0.5, 0.5);

    x = posX;
    y = posY;
    Vx = distribuzione(gen);
    Vy = distribuzione(gen);
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

    /*sf::CircleShape triangle(10,3);
    triangle.setFillColor(sf::Color::White);*/

    sf::CircleShape Circle(3);
    Circle.setFillColor(sf::Color::White);
    Circle.setPosition(x,y);
    window.draw(Circle);

    angle = (180 * std::atan2(Vy, Vx) / M_PI)+90;


    /*
    triangle.setPosition(x,y);
    triangle.setRotation(angle);
    window.draw(triangle);*/
}

void Boid::update(std::vector<Boid> &flock) {
    //In questa classe ora andremo a riunire tutte le regole per il flocking
    float xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0, close_dx = 0, close_dy = 0;
    int neighboring_boids = 0;

    for (auto & otherboid : flock) {
        //Calcolo delle distanze
        float dx = x - otherboid.get_x();
        float dy = y - otherboid.get_y();

        if( abs(dx) < visualrange && abs(dy) < visualrange && dx != 0.f && dy != 0.f) {
            //calcoliamo la distanza quadratica
            float squareddistance = sqrt(dx*dx + dy*dy);
            //se la squared è minore della protected_sqaured
            if(squareddistance < (protectedrange)) {
                close_dx += x - otherboid.get_x();
                close_dy += y - otherboid.get_y();
            }
            //se la squared è minore della visual_squared
            else if(squareddistance < (visualrange)) {

                xpos_avg += otherboid.get_x();
                ypos_avg += otherboid.get_y();

                xvel_avg += otherboid.get_vx();
                yvel_avg += otherboid.get_vy();

                neighboring_boids += 1;
            }
        }
    }

    if (neighboring_boids > 0) {
        xpos_avg = xpos_avg/neighboring_boids;
        ypos_avg = ypos_avg/neighboring_boids;

        xvel_avg = xvel_avg/neighboring_boids;
        yvel_avg = yvel_avg/neighboring_boids;

        Vx += (xpos_avg - x) * centeringfactor + (xvel_avg - Vx) *matchingfactor;
        Vy += (ypos_avg - y) * centeringfactor + (yvel_avg - Vy) *matchingfactor;
    }

    //Contributo della avoidance
    Vx += close_dx * avoidfactor;
    Vy += close_dy * avoidfactor;

    edges(240+vgaWIDTH,100+vgaHEIGH);


    //Add a dynamic bias -> From couzin's paper
    if(scout) {// biased to right of the screen
        if(Vx > 0) {
            biasval = std::min(maxbias, biasval+bias_increment);
        }else {
            biasval = std::max(bias_increment, biasval-bias_increment);
        }
    }else {
        if(Vx < 0) {
            biasval = std::min(maxbias, biasval + bias_increment);
        }else {
            biasval = std::max(bias_increment, biasval - bias_increment);
        }
    }
    //Update the velocity with the bias
    if(scout){ //Biased to the right
        Vx = (1-biasval) * Vx + (biasval * 1);
    }else { //Biased to the left
        Vx = (1-biasval) * Vx +(biasval * (-1));
    }

    float speed = sqrt(Vx*Vx+Vy*Vy);

    if (speed < minspeed) {
        Vx = (Vx/speed)*minspeed;
        Vy = (Vy/speed)*minspeed;
    }else if (speed > maxspeed) {
        Vx = (Vx/speed)*maxspeed;
        Vy = (Vy/speed)*maxspeed;
    }

    x += Vx;
    y += Vy;
}

void Boid::align(std::vector<Boid> flock) {
    float xvel_avg = 0;
    float yvel_avg = 0;
    int neighboring_boids = 0;

    for (auto otherboid:flock) {
        float dx = x-otherboid.get_x();
        float dy = y-otherboid.get_y();
        if(&otherboid != this && abs(dx)<visualrange && abs(dy)<visualrange){
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

        Vx += (xvel_avg-Vx) * matchingfactor;
        Vy += (yvel_avg-Vy) * matchingfactor;
    }

}

void Boid::cohesion(std::vector<Boid> flock) {
    float xpos_avg = 0;
    float ypos_avg = 0;
    int neighboring_boids = 0;

    for(auto otherboid:flock) {
        float dx = x-otherboid.get_x();
        float dy = y-otherboid.get_y();
        if (&otherboid != this && abs(dx)< visualrange && abs(dy)< visualrange) {
            float squareddistance = dx*dx +dy*dy;
            if (squareddistance < visualrange*visualrange) {
                xpos_avg += otherboid.get_x();
                ypos_avg += otherboid.get_y();
                neighboring_boids += 1;
            }
        }
    }
    if(neighboring_boids > 0) {
        xpos_avg = xpos_avg/neighboring_boids;
        ypos_avg = ypos_avg/neighboring_boids;

        Vx += (xpos_avg - x) * centeringfactor;
        Vy += (ypos_avg - y) *centeringfactor;
    }
}

void Boid::separation(std::vector<Boid> flock) {
    float close_dx = 0;
    float close_dy = 0;

    for (auto otherboid:flock) {
        float dx = x-otherboid.get_x();
        float dy = y-otherboid.get_y();
        if (&otherboid != this && abs(dx)< visualrange && abs(dy)< visualrange) {
            float squareddistance = dx*dx + dy*dy;
            if (squareddistance < protectedrange * protectedrange) {
                close_dx += x - otherboid.get_x();
                close_dy += y - otherboid.get_y();
            }
        }
    }
    Vx += close_dx*avoidfactor;
    Vy += close_dy*avoidfactor;
}

void Boid::edges(float width, float heigh) {
    if (x > width) {
        Vx -= turnfactor;
    }else if (x < 240) {
        Vx += turnfactor;
    }
    if (y > heigh) {
        Vy -= turnfactor;
    }else if (y < 110) {
        Vy += turnfactor;
    }
}


