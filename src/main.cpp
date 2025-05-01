#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"


int main()
{
  // create window and set up
  sf::RenderWindow window(sf::VideoMode(800, 800), "Falling Sand Simulator");

  sf::Image icon = sf::Image();
  if (!icon.loadFromFile("../Data/icon.png")) {
    std::cerr << "Error loading icon" << std::endl;
  }
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  Game game(window);

  window.setFramerateLimit(game.TARGET_FPS);


  // A Clock starts counting as soon as it's created
  sf::Clock clock;

  //initialise an instance of the game class


  //run the init function of the game class and check it all initialises ok
  if (!game.init())
  {
    return 0;
  }


  // Game loop: run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;

    //calculate delta time
    sf::Time time = clock.restart();
    float dt = time.asSeconds();

    //'process inputs' element of the game loop
    while (window.pollEvent(event))
    {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
      {
        window.close();
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


    //'update' element of the game loop
    game.update(dt);

    window.clear(sf::Color(255, 241, 204, 255));

    //'render' element of the game loop
    game.render();
    window.display();
  }

  return 0;
}
