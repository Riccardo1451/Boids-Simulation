

#ifndef FLOCK_H
#define FLOCK_H
#include "Boid.h"


class Flock {
public:
    explicit Flock(int n);

    std::vector<Boid> &getFlock();

private:
    int N; //Numero di boids
    std::vector<Boid> flock;

};



#endif //FLOCK_H
