#ifndef BOID_H
#define BOID_H
#include <SFML/Graphics/RenderWindow.hpp>


class Boid {
public:

    Boid(float posX, float posY);
    Boid() : Boid(0, 0) {} //Costruttore di default

    float get_x() const;

    void set_x(float x);

    float get_y() const;

    void set_y(float y);

    float get_vx() const;

    void set_vx(float vx);

    float get_vy() const;

    void set_vy(float vy);

    bool isScout() const;

    void show(sf::RenderWindow &window);

    void update();

    void align(std::vector<Boid> flock);

    void cohesion(std::vector<Boid> flock);

    void separation(std::vector<Boid> flock);

    void edges(float width, float heigh);


private:
    float x,y; //variabili di posizione
    float Vx,Vy; //variabili di velocità
    bool scout; //True se appartiene al gruppo 1 False se appartiene al gruppo 2
    float angle;
    float visualrange = 50.0f;
    float protectedrange = 20.0f;
    float matchingfactor = 0.03f;
    float centeringfactor = 0.005f;
    float avoidfactor = 0.05f;
    float turnfactor = 0.8f;
    float minspeed = 1.5f;
    float maxspeed = 3.0f;



};



#endif //BOID_H
