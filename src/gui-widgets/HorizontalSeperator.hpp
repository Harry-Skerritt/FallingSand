//
// Created by Harry on 02/05/2025.
//

#ifndef HORIZONTALSEPERATOR_H
#define HORIZONTALSEPERATOR_H

#include <SFML/Graphics.hpp>

class HorizontalSeparator : public sf::Drawable {
public:
  HorizontalSeparator() = default;
  HorizontalSeparator(const sf::Vector2f& position, float width, float height, const sf::Color& color) {
    line.setPosition(position);
    line.setSize(sf::Vector2f(width, height));
    line.setFillColor(color);
  }

  void init(const sf::Vector2f& position, float width, float height, const sf::Color& color)
  {
    line.setPosition(position);
    line.setSize(sf::Vector2f(width, height));
    line.setFillColor(color);
  }

  void setPosition(const sf::Vector2f& position) {
    line.setPosition(position);
  }

  void setSize(float width, float height) {
    line.setSize(sf::Vector2f(width, height));
  }

  void setColor(const sf::Color& color) {
    line.setFillColor(color);
  }

private:
  sf::RectangleShape line;

protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{
    target.draw(line, states);
  }
};

#endif //HORIZONTALSEPERATOR_H
