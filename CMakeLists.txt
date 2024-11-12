cmake_minimum_required(VERSION 3.28)
project(Boids_Code)

set(CMAKE_CXX_STANDARD 17)

# Specifica la directory di SFML
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")

# Trova e collega SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Crea l'eseguibile chiamato Boids_Code
add_executable(Boids_Code main.cpp
        Boid.cpp
        Boid.h
        Flock.cpp
        Flock.h)

# Collega SFML al target Boids_Code
target_link_libraries(Boids_Code sfml-graphics sfml-window sfml-system)



