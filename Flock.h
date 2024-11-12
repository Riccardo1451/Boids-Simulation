

#ifndef FLOCK_H
#define FLOCK_H
#include "Boid.h"


class Flock {
public:
    explicit Flock(int n);
    void biasingBoids();


    std::vector<Boid> getFlock() const;

private:
    int N; //Numero di boids
    std::vector<Boid> flock;
    float biasval = 0.01;
};



#endif //FLOCK_H
