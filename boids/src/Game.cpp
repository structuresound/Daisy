// #include "Boid.h"
// #include "Pvector.h"
#include "Game.h"

#include "imgui.h"
#include "imgui-SFML.h"

int bRate = 10;
int dRate = 20;
int maxBoids = 40;
int nPredators = 0;
int maxPredators = 2;
float universe = .33;
float simRate = 0.66f;
int simSteps = 1;
int f = 0;
int t = 0;

// Construct window using SFML
Game::Game()
{
    this->boidsSize = 5.0f;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    this->window_width = desktop.width * universe;
    this->window_height = desktop.height * universe;
    this->wrapSize.x = this->window_width;
    this->wrapSize.y = this->window_height;
    sim.create(sf::VideoMode(window_width, window_height, desktop.bitsPerPixel), "Boids");
    ImGui::SFML::Init(sim);
    // Try to achieve 60 FPS
    sim.setFramerateLimit(60);

    controls.create(sf::VideoMode(640, 480), "Controls");
    ImGui::SFML::Init(controls);
    controls.setFramerateLimit(60);
}

// Run the simulation. Run creates the boids that we'll display, checks for user
// input, and updates the view
void Game::Run()
{
    for (int i = 0; i < maxBoids; i++)
    {
        Boid b(wrapSize, wrapSize.x * .5, wrapSize.y * .5); // Starts all boids in the center of the screen
        b.simRate = &simRate;
        flock.addBoid(b);

        // Drawing
        sf::CircleShape shape(8, 3);

        // Changing the Visual Properties of the shape
        shape.setPosition(b.location.x, b.location.y); // Sets position of shape to random location that boid was set to.
        // shape.setPosition(window_width, window_height); // Testing purposes, starts all shapes in the center of screen.
        shape.setOutlineColor(sf::Color(0, 255, 0));
        shape.setFillColor(sf::Color::Green);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setRadius(boidsSize);

        // Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>

        shapes.push_back(shape);
    }

    sf::Clock deltaClock;
    while (sim.isOpen())
    {
        if (f == 2)
        {
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            this->window_width = desktop.width * universe;
            this->window_height = desktop.height * universe;
            this->wrapSize.x = this->window_width;
            this->wrapSize.y = this->window_height;
            HandleInput();
            f = 0;
        }
        f++;
        Render(deltaClock);
    }
}

void Game::HandleInput()
{
    t++;
    sf::Event event;
    while (sim.pollEvent(event))
    {
        // "close requested" event: we close the window
        // Implemented alternate ways to close the window. (Pressing the escape, X, and BackSpace key also close the program.)
        if ((event.type == sf::Event::Closed) ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::Escape) ||
            (event.type == sf::Event::KeyPressed &&
             event.key.code == sf::Keyboard::BackSpace))
        {
            sim.close();
            controls.close();
        }
    }

    while (controls.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(controls, event);
    }

    // Check for mouse click, draws and adds boid to flock if so.

    if (nPredators < maxPredators)
    {
        Boid b(wrapSize, wrapSize.x * .5, wrapSize.y * .5, true);
        b.simRate = &simRate;
        flock.addPredator(b);
        nPredators++;
        sf::CircleShape shape(10, 3);

        // Changing visual properties of newly created boid
        shape.setOutlineColor(sf::Color(255, 0, 0));
        shape.setFillColor(sf::Color(255, 0, 0));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1);
        shape.setRadius(boidsSize);

        // Adds newly created boid and shape to their respective data structure
        shapes.push_back(shape);
    }

    // Check for mouse click, draws and adds boid to flock if so.
    // if (t % dRate == 0)
    // {
    //     int i = flock.removePredator();
    //     if (i != -1)
    //     {
    //         shapes.erase(shapes.begin() + i);
    //         nPredators--;
    //     }
    // }
}

void Game::Render(sf::Clock &deltaClock)
{

    sim.clear();

    // Draws all of the Boids out, and applies functions that are needed to update.
    for (int i = 0; i < shapes.size(); i++)
    {
        // cout << "Boid "<< i <<" Coordinates: (" << shapes[i].getPosition().x << ", " << shapes[i].getPosition().y << ")" << endl;
        // cout << "Boid Code " << i << " Location: (" << flock.getBoid(i).location.x << ", " << flock.getBoid(i).location.y << ")" << endl;
        Boid &boid = flock.getBoid(i);

        // Matches up the location of the shape to the boid
        shapes[i].setPosition(boid.location.x, boid.location.y);

        // Calculates the angle where the velocity is pointing so that the triangle turns towards it.
        float theta = boid.angle(boid.velocity);
        shapes[i].setRotation(theta);
    }

    for (int i = 0; i < shapes.size(); i++)
    {
        sim.draw(shapes[i]);
    }

    // Applies the three rules to each boid in the flock and changes them accordingly.
    for (int i = 0; i < simSteps; i++)
    {
        flock.flocking();
    }

    sim.display();

    ImGui::SFML::Update(controls, deltaClock.restart());

    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    controls.clear();
    ImGui::SFML::Render(controls);
    controls.display();
}
