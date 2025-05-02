#include <iostream>
#include <set>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Simulation/SimulationWindow.h"


int main()
{
  sf::RenderWindow settings_window(sf::VideoMode(500, 630), "Falling Sand Simulator - Settings");
  sf::RenderWindow* sim_window = nullptr;
  SimulationWindow* simulation_window = nullptr;

  sf::Image icon = sf::Image();
  if (!icon.loadFromFile("../Data/icon.png")) {
    std::cerr << "Error loading icon" << std::endl;
  }
  settings_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  Game game(settings_window);

  settings_window.setFramerateLimit(game.TARGET_FPS);
  sf::Clock clock;

  if (!game.init())
    return 0;

  while (settings_window.isOpen())
  {
    sf::Event event;

    float dt = clock.restart().asSeconds();

    while (settings_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        settings_window.close();
      }

      if (event.type == sf::Event::KeyPressed)
      {
        game.keyPressed(event);
      }

      if (event.type == sf::Event::KeyReleased)
      {
        game.keyReleased(event);
      }

      if (event.type == sf::Event::MouseButtonPressed)
      {
        game.mouseClicked(event);
      }

      if (event.type == sf::Event::MouseButtonReleased)
      {
        game.mouseReleased(event);
      }

      if (event.type == sf::Event::MouseWheelScrolled)
      {
        game.mouseScroll(event);
      }

      if (event.type == sf::Event::MouseMoved)
      {
        game.mouseDragged(event);
      }
    }

    // Create sim window
    if (game.showSimulation() & !sim_window) {
      sim_window = new sf::RenderWindow(sf::VideoMode(game.sim_window_size.x, game.sim_window_size.y), "Falling Sand Simulator - Simulation");
      sim_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
      sim_window->setFramerateLimit(game.TARGET_FPS);

      sim_window->setPosition(sf::Vector2i(
        settings_window.getPosition().x + static_cast<int>(settings_window.getSize().x) + 10,
        settings_window.getPosition().y));

      simulation_window = new SimulationWindow(*sim_window);
      if (!simulation_window->initSimulation()) return 0;
    }

    // Close sim window
    if (!game.showSimulation() && sim_window && sim_window->isOpen()) {
      sim_window->close();
      delete sim_window;
      sim_window = nullptr;

      delete simulation_window;
      simulation_window = nullptr;
    }

    // Sim window event polling
    if (sim_window && sim_window->isOpen()) {
      while (sim_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          sim_window->close();
        }

        if (event.type == sf::Event::KeyPressed) {
          simulation_window->keyPressed(event);
        }

        if (event.type == sf::Event::KeyReleased) {
          simulation_window->keyReleased(event);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
          simulation_window->mouseClicked(event);
        }

        if (event.type == sf::Event::MouseButtonReleased) {
          simulation_window->mouseReleased(event);
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
          simulation_window->mouseScroll(event);
        }
      }

    }



    game.update(dt);
    settings_window.clear(sf::Color(84, 114, 78, 255));
    game.render();
    settings_window.display();

    if (sim_window && sim_window->isOpen()) {
      simulation_window->updateSimulation(dt);
      sim_window->clear(sf::Color::Black);
      simulation_window->renderSimulation();
      sim_window->display();
    }


    //if (sim_window) delete sim_window;
   // if (simulation_window) delete simulation_window;
  }

  return 0;
}
