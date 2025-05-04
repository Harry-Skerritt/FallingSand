//
// Created by Harry on 04/05/2025.
//

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <SFML/Graphics.hpp>

class SharedData {
public:
  bool use_image = false;
  bool use_colour = false;
  bool use_rainbow = false;

  sf::Color selected_colour = sf::Color::White;
  sf::Color selected_background_color = sf::Color::Black;

  std::string path_to_image = "";
};



#endif //SHAREDDATA_H
