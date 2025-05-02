//
// Created by Harry on 02/05/2025.
//

#include "SimulationWindow.h"

SimulationWindow::SimulationWindow(sf::RenderWindow &game_window)
    : window(game_window)
{
    srand(time(NULL));
}


SimulationWindow::~SimulationWindow() {}


bool SimulationWindow::initSimulation() {
    sandSimulation = new SandSimulation(window);
    if (!sandSimulation->init()) {
        return false;
    }
    sandSimulation->setSandSize(2);
    sandSimulation->setBrushSize(4);
    sandSimulation->setSandColour(sf::Color::Red);
    sandSimulation->setBackgroundColour(sf::Color::Green);
    sand_brush_size = sandSimulation->getBrushSize();


    return true;
}

void SimulationWindow::updateSimulation(float dt) {
    sandSimulation->update(dt);

    if (is_left_mouse_down) {
        sandSimulation->spawnNewSand(prev_mouse_pos);
    }
}

void SimulationWindow::renderSimulation() {
    sandSimulation->draw(window);
}


// Events
void SimulationWindow::keyPressed(sf::Event event) {
    if (event.key.code == sf::Keyboard::R) {
        sandSimulation->resetGrid();
    }
}

void SimulationWindow::keyReleased(sf::Event event) {

}

void SimulationWindow::mouseClicked(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        is_left_mouse_down = true;
        prev_mouse_pos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
    }
}

void SimulationWindow::mouseReleased(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        is_left_mouse_down = false;
    }
}

void SimulationWindow::mouseScroll(sf::Event event) {
    if (event.mouseWheelScroll.delta > 0) {
        sand_brush_size++;
    }
    else if (event.mouseWheelScroll.delta < 0) {
        if (sand_brush_size < 1) {
            sand_brush_size = 1;
        }
        else {
            sand_brush_size--;
        }
    }
    sandSimulation->setBrushSize(sand_brush_size);
 }








