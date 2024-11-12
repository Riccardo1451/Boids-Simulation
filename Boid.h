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

    void edges(float width, float heigh);


private:
    float x,y; //variabili di posizione
    float Vx,Vy; //variabili di velocità
    bool scout; //True se appartiene al gruppo 1 False se appartiene al gruppo 2
    float angle;
    float visualrange = 17;
    float protectedrange = 20;
    float matchingFactor = 0.0005;
    float turnfactor = 0.00001;

};



#endif //BOID_H
