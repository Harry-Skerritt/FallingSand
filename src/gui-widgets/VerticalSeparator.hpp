#ifndef VERTICALSEPARATOR_H
#define VERTICALSEPARATOR_H

#include <SFML/Graphics.hpp>

class VerticalSeparator : public sf::Drawable {
public:
  VerticalSeparator() = default;

  VerticalSeparator(const sf::Vector2f& position, float height, float width, const sf::Color& color) {
    line.setPosition(position);
    line.setSize(sf::Vector2f(width, height));
    line.setFillColor(color);
  }

  void init(const sf::Vector2f& position, float height, float width, const sf::Color& color) {
    line.setPosition(position);
    line.setSize(sf::Vector2f(width, height));
    line.setFillColor(color);
  }

  void setPosition(const sf::Vector2f& position) {
    line.setPosition(position);
  }

  void setSize(float height, float width) {
    line.setSize(sf::Vector2f(width, height));
  }

  void setColor(const sf::Color& color) {
    line.setFillColor(color);
  }

private:
  sf::RectangleShape line;

protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(line, states);
  }
};

#endif // VERTICALSEPARATOR_H