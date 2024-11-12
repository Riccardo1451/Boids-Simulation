

#include "Flock.h"
#define WIDTH  1290
#define HIGHT  720

Flock::Flock(int const n) {
    N  = n;
    for (int i = 0; i<N; i++) {
        flock.emplace_back(WIDTH/2, HIGHT/2);//TODO:Inserire posizione iniziale
    }
    biasingBoids();
}

void Flock::biasingBoids() {
    //From Couzin's paper, add a bias to every Boid in the flock
    for(auto &Boid : flock) {
        if(Boid.isScout()) {
            Boid.set_vx((1-biasval)*Boid.get_vx()+(biasval*1)); //distorsione verso il lato destro
        }else {
            Boid.set_vx((1-biasval)*Boid.get_vx()+(biasval*(-1))); //distorsione verso il lato sinistro
        }
    }
}

std::vector<Boid> Flock::getFlock() const {
    return flock;
}
