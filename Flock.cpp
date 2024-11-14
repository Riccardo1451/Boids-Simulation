

#include "Flock.h"

#include <random>
#define WIDTH  1290
#define HEIGH  720

Flock::Flock(int const n) {
    N  = n;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribuzioneX(240, WIDTH);
    std::uniform_real_distribution<> distribuzioneY(110, HEIGH);

    for (int i = 0; i<N; i++) {
        flock.emplace_back(distribuzioneX(gen), distribuzioneY(gen));
    }
}

std::vector<Boid> &Flock::getFlock() {
    return flock;
}
