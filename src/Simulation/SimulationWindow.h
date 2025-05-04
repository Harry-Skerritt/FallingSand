//
// Created by Harry on 02/05/2025.
//

#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <SFML/Graphics.hpp>

#include "../SharedData.h"
#include "SandSimulation.h"

class SimulationWindow {
public:
    SimulationWindow(sf::RenderWindow& game_window, SharedData* sharedData);
    ~SimulationWindow();

    bool initSimulation();

    void updateSimulation(float dt);

    void renderSimulation();

    bool isOpen() { return window.isOpen(); }

    void mouseClicked(sf::Event event);
    void mouseReleased(sf::Event event);
    void keyPressed(sf::Event event);
    void keyReleased(sf::Event event);
    void mouseScroll(sf::Event event);

     SandSimulation* getSandSimulation() { return sandSimulation; };


private:
    sf::RenderWindow& window;
    SharedData* shared_data;

    SandSimulation* sandSimulation;

    bool is_left_mouse_down = false;
    sf::Vector2f prev_mouse_pos;
    int sand_brush_size = 0;


};



#endif //SIMULATIONWINDOW_H
