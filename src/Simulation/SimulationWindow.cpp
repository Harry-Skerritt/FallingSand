//
// Created by Harry on 02/05/2025.
//

#include "SimulationWindow.h"
#include <iostream>

SimulationWindow::SimulationWindow(sf::RenderWindow &game_window, SharedData* sharedData)
    : window(game_window), shared_data(sharedData)
{
    srand(time(NULL));
}


SimulationWindow::~SimulationWindow() {}


bool SimulationWindow::initSimulation() {
  sandSimulation = new SandSimulation(window);
  if (!sandSimulation->init()) {
    return false;
  }

  // Set initial brush size
  sandSimulation->setBrushSize(4);

  std::cout << "Sim WIndow: \nImage: " << shared_data->use_image << "\n Solid Colour: " << shared_data->use_colour << "\n Rainbow: " << shared_data->use_rainbow << std::endl;


  // Setting up the sand simulation based on SharedData flags
  if (shared_data->use_image) {
    // Load image and set background color to average image color
    sandSimulation->setUseImage(true);
    sandSimulation->setUseColour(false);
    sandSimulation->setUseRainbow(false);
    sandSimulation->setImagePath(shared_data->path_to_image);
  }
  else if (shared_data->use_colour) {
    sandSimulation->setUseColour(true);
    sandSimulation->setUseImage(false);
    sandSimulation->setUseRainbow(false);
    sandSimulation->setSandColour(shared_data->selected_colour);
    sandSimulation->setBackgroundColour(shared_data->selected_background_color);
  }
  else if (shared_data->use_rainbow) {
    sandSimulation->setUseRainbow(true);
    sandSimulation->setUseImage(false);
    sandSimulation->setUseColour(false);
    sandSimulation->setSandColour(sf::Color::Red);
    sandSimulation->setBackgroundColour(sf::Color::Black); // Black background for rainbow mode
  }

  sandSimulation->init();

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








