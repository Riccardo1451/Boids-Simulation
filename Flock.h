

#ifndef FLOCK_H
#define FLOCK_H
#include "Boid.h"


class Flock {
public:
    explicit Flock(int n, bool parallel);

    std::vector<Boid>& getCurrentFlock();
    //TODO: facciamo creare il vettore temporaneo in questa classe
    std::vector<Boid>& getNextFlock();
private:
    int N; //Numero di boids
    std::vector<Boid> currentflock;
    std::vector<Boid> nextflock;

};



#endif //FLOCK_H
