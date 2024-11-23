#include "Boid.h"
#include <SFML/Graphics.hpp>
#include <random>
#define vgaWIDTH 1280
#define vgaHEIGH 720

Boid::Boid(float const posX, float const posY, bool parallel){

    static std::random_device rd;  // Seme per il generatore
    static std::mt19937 gen(rd()); // Generatore Mersenne Twister
    std::uniform_real_distribution<> distribuzione(-0.5, 0.5);

    x = posX;
    y = posY;
    Vx = distribuzione(gen);
    Vy = distribuzione(gen);
    targetAngle = 0.0f;

    std::random_device rdb;        // Generatore di entropia
    std::mt19937 genb(rdb());       // Motore di numeri casuali
    std::bernoulli_distribution d(0.5); // Distribuzione bernoulliana con p=0.5
    scout = d(genb);

    //Set the parallel parameters
    this->parallel = parallel;
    if(parallel){
        visualrange = 55.0f;
        protectedrange = 8.0f;
        matchingfactor = 0.01f;
        centeringfactor = 0.0006f;
        avoidfactor = 0.05f;
        turnfactor = 0.05f;
        minspeed = 3.0f;
        maxspeed = 6.0f;
    }
    else {
        visualrange = 60.0f;
        protectedrange = 15.0f;
        matchingfactor = 0.1f;
        centeringfactor = 0.0008f;
        avoidfactor = 0.05f;
        turnfactor = 0.5f;
        minspeed = 3.0f;
        maxspeed = 6.0f;
    }
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

void Boid::show(sf::RenderWindow &window){
    sf::CircleShape triangle(5,3);
    triangle.setFillColor(sf::Color::White);

    /*sf::CircleShape Circle(3);
    Circle.setFillColor(sf::Color::White);
    Circle.setPosition(x,y);
    window.draw(Circle);*/

    targetAngle = (180 * std::atan2(Vy, Vx) / M_PI)+90;
    float smoothingFactor = 0.5f;  // Il fattore di "smoothness" per l'interpolazione
    targetAngle += smoothingFactor * (targetAngle - targetAngle);


    triangle.setPosition(x,y);
    triangle.setRotation(targetAngle);
    window.draw(triangle);
}

void Boid::update(std::vector<Boid> &Currentflock, Boid &updateBoid){ //TODO: aggiungere un riferimento a boid in modo da poter salvare le velocità su un valore temporaneo
    //In questa classe ora andremo a riunire tutte le regole per il flocking
    float xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0, close_dx = 0, close_dy = 0;
    float new_Vx = Vx, new_Vy = Vy;
    int neighboring_boids = 0;

    for (auto & otherboid : Currentflock){
        if(&otherboid == this) continue;
        //Calcolo delle distanze
        float dx = x - otherboid.get_x();
        float dy = y - otherboid.get_y();

        if( abs(dx) < visualrange && abs(dy) < visualrange){
            //calcoliamo la distanza quadratica
            float squareddistance = sqrt(dx*dx + dy*dy);
            //se la squared è minore della protected_sqaured
            if(squareddistance < protectedrange){
                close_dx += x - otherboid.get_x();
                close_dy += y - otherboid.get_y();
            }
            //se la squared è minore della visual_squared
            else if(squareddistance < visualrange) {

                xpos_avg += otherboid.get_x();
                ypos_avg += otherboid.get_y();

                xvel_avg += otherboid.get_vx();
                yvel_avg += otherboid.get_vy();

                neighboring_boids ++;
            }
        }
    }

    if (neighboring_boids > 0){
        xpos_avg = xpos_avg/neighboring_boids;
        ypos_avg = ypos_avg/neighboring_boids;

        xvel_avg = xvel_avg/neighboring_boids;
        yvel_avg = yvel_avg/neighboring_boids;

        new_Vx += (xpos_avg - x) * centeringfactor + (xvel_avg - Vx) * matchingfactor;
        new_Vy += (ypos_avg - y) * centeringfactor + (yvel_avg - Vy) * matchingfactor;
    }

    //Contributo della avoidance
    new_Vx += close_dx * avoidfactor;
    new_Vy += close_dy * avoidfactor;



    //Add a dynamic bias -> From couzin's paper
    if(scout){// biased to right of the screen
        if(new_Vx > 0){
            biasval = std::min(maxbias, biasval+bias_increment);
        }else{
            biasval = std::max(bias_increment, biasval-bias_increment);
        }
    }else{
        if(new_Vx < 0){
            biasval = std::min(maxbias, biasval + bias_increment);
        }else{
            biasval = std::max(bias_increment, biasval - bias_increment);
        }
    }
    //Update the velocity with the bias
    if(scout){ //Biased to the right
        new_Vx = (1 - biasval) * new_Vx + (biasval * 1);
    }else{ //Biased to the left
        new_Vx = (1 - biasval) * new_Vx +(biasval * (-1));
    }

    float speed = sqrt(new_Vx*new_Vx + new_Vy*new_Vy);

    if (speed < minspeed){
        new_Vx = (new_Vx/speed) * minspeed;
        new_Vy = (new_Vy/speed) * minspeed;
    }else if (speed > maxspeed){
        new_Vx = (new_Vx/speed) * maxspeed;
        new_Vy = (new_Vy/speed) * maxspeed;
    }
    edges(240+vgaWIDTH,100+vgaHEIGH, new_Vx,new_Vy);

    updateBoid.set_vx(new_Vx);
    updateBoid.set_vy(new_Vy);
    updateBoid.set_x(x + new_Vx);
    updateBoid.set_y(y + new_Vy);
}

void Boid::edges(float width, float heigh, float &new_Vx, float &new_Vy){
    if (x > width){
        new_Vx -= turnfactor;
    }else if (x < 240){
        new_Vx += turnfactor;
    }
    if (y > heigh){
        new_Vy -= turnfactor;
    }else if (y < 110){
        new_Vy += turnfactor;
    }
}

void Boid::align(std::vector<Boid> flock){
    float xvel_avg = 0;
    float yvel_avg = 0;
    int neighboring_boids = 0;

    for (auto otherboid:flock){
        float dx = x - otherboid.get_x();
        float dy = y - otherboid.get_y();
        if(&otherboid != this && abs(dx) < visualrange && abs(dy) < visualrange){
            float squareddistance = dx*dx + dy*dy;
            if(squareddistance < visualrange * visualrange){
                xvel_avg += otherboid.get_vx();
                yvel_avg += otherboid.get_vy();
                neighboring_boids += 1;
            }
        }
    }
    if(neighboring_boids > 0){
        xvel_avg = xvel_avg/neighboring_boids ;
        yvel_avg = yvel_avg/neighboring_boids;

        Vx += (xvel_avg-Vx) * matchingfactor;
        Vy += (yvel_avg-Vy) * matchingfactor;
    }

}

void Boid::cohesion(std::vector<Boid> flock){
    float xpos_avg = 0;
    float ypos_avg = 0;
    int neighboring_boids = 0;

    for(auto otherboid:flock){
        float dx = x-otherboid.get_x();
        float dy = y-otherboid.get_y();
        if (&otherboid != this && abs(dx) < visualrange && abs(dy)<  visualrange){
            float squareddistance = dx*dx +dy*dy;
            if (squareddistance < visualrange * visualrange) {
                xpos_avg += otherboid.get_x();
                ypos_avg += otherboid.get_y();
                neighboring_boids += 1;
            }
        }
    }
    if(neighboring_boids > 0){
        xpos_avg = xpos_avg/neighboring_boids;
        ypos_avg = ypos_avg/neighboring_boids;

        Vx += (xpos_avg - x) * centeringfactor;
        Vy += (ypos_avg - y) * centeringfactor;
    }
}

void Boid::separation(std::vector<Boid> flock){
    float close_dx = 0;
    float close_dy = 0;

    for (auto otherboid:flock) {
        float dx = x - otherboid.get_x();
        float dy = y - otherboid.get_y();
        if (&otherboid != this && abs(dx) < visualrange && abs(dy) < visualrange){
            float squareddistance = dx*dx + dy*dy;
            if (squareddistance < protectedrange * protectedrange){
                close_dx += x - otherboid.get_x();
                close_dy += y - otherboid.get_y();
            }
        }
    }
    Vx += close_dx * avoidfactor;
    Vy += close_dy * avoidfactor;
}