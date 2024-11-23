

#include "Flock.h"

#include <random>
#define WIDTH  1290
#define HEIGH  720

Flock::Flock(int const n, bool parallel) {
    N  = n;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribuzioneX(240, WIDTH);
    std::uniform_real_distribution<> distribuzioneY(110, HEIGH);

    //TODO: potrei parallelizzare l'inizializzazione?

    for (int i = 0; i<N; i++) {
        currentflock.emplace_back(distribuzioneX(gen), distribuzioneY(gen), parallel);
        nextflock.emplace_back(); //Empty inizialization
    }
}

std::vector<Boid> & Flock::getCurrentFlock() {
    return currentflock;
}

std::vector<Boid> & Flock::getNextFlock() {
    return nextflock;
}

