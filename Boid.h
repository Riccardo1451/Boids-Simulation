#ifndef BOID_H
#define BOID_H
#include <SFML/Graphics/RenderWindow.hpp>


class Boid {
public:

    Boid(float posX, float posY);

    float get_x() const;

    void set_x(float x);

    float get_y() const;

    void set_y(float y);

    float get_vx() const;

    void set_vx(float vx);

    float get_vy() const;

    void set_vy(float vy);

    void show(sf::RenderWindow &window);

private:
    float x,y; //variabili di posizione
    float Vx,Vy; //variabili di velocità
};



#endif //BOID_H
