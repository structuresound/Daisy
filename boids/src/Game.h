#include <iostream>
// #include "Flock.h"
// #include "Boid.h"
// #include "Pvector.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>

#include "flox.hpp"

#ifndef GAME_H
#define GAME_H

// Game handles the instantiation of a flock of boids, game input, asks the
// model to compute the next step in the stimulation, and handles all of the
// program's interaction with SFML.

class Game
{
private:
    sf::RenderWindow sim;
    sf::RenderWindow controls;
    int window_width;
    int window_height;
    Pvector wrapSize;
    Flock flock;
    vector<sf::CircleShape> shapes;
    float boidsSize;

    void Render(sf::Clock &deltaClock);
    void HandleInput();

public:
    Game();
    void Run();
};

#endif
