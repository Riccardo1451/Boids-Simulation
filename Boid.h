#ifndef BOID_H
#define BOID_H
#include <SFML/Graphics/RenderWindow.hpp>


class Boid {
public:

    Boid(float posX, float posY, bool parallel);
    Boid() : Boid(0, 0, false) {} //Costruttore di default

    //Getter
    float get_x() const;
    float get_y() const;
    float get_vx() const;
    float get_vy() const;
    //Setter
    void set_x(float x);
    void set_y(float y);
    void set_vx(float vx);
    void set_vy(float vy);

    bool isScout() const;

    //Method for alignment, cohesion, separation

    void show(sf::RenderWindow &window);

    void update(std::vector<Boid> &flock);

    void align(std::vector<Boid> flock);

    void cohesion(std::vector<Boid> flock);

    void separation(std::vector<Boid> flock);

    void edges(float width, float heigh);


private:
    float x,y; //variabili di posizione
    float Vx,Vy; //variabili di velocità
    bool scout; //True se appartiene al gruppo 1 False se appartiene al gruppo 2
    float angle;
    float visualrange;
    float protectedrange;
    float matchingfactor;
    float centeringfactor;
    float avoidfactor;
    float turnfactor;
    float minspeed;
    float maxspeed;

    float biasval = 0.001;
    float maxbias = 0.01;
    float bias_increment = 0.0004;

    bool parallel;
};



#endif //BOID_H
