//
// Created by Harry on 02/05/2025.
//

#ifndef BUTTON_H
#define BUTTON_H

#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <iostream>

class Button
{

public:

  Button() : m_font(nullptr) {};
  Button(sf::Vector2f pos, sf::Vector2f size, sf::Color color, std::string text, sf::Font* font = nullptr)
  {
    init(pos, size, color, text, font);
  };


  ~Button(){};


  void init(sf::Vector2f pos, sf::Vector2f size, sf::Color color, std::string text, sf::Font* font = nullptr)
  {
    setFont(font);

    m_background.setSize(size);
    setButtonColours(color, m_hover_color);
    m_background.setPosition(pos);

    m_background.setOutlineColor(m_outline_color);

    m_label.setFont(*m_font);
    m_label.setString(text);
    m_label.setFillColor(m_text_color);
    m_label.setCharacterSize(16);
    centreInButton(&m_label);


  }

  void update(sf::Vector2f mouse_world, float dt)
  {
    bool was_hovered = m_is_hovered;
    m_is_hovered = m_background.getGlobalBounds().contains(mouse_world);

    if (m_fade)
    {
      sf::Color target_colour = m_is_hovered ? m_hover_color : m_fill_color;
      m_current_colour = lerpColour(m_current_colour, target_colour, m_fade_speed * dt);
      m_background.setFillColor(m_current_colour);
    }
    else
    {
      m_background.setFillColor( m_is_hovered ? m_hover_color : m_fill_color);
    }


  }

  void draw(sf::RenderWindow& window)
  {
    window.draw(m_background);
    window.draw(m_label);
  }

  // Funcs
  void clicked()
  {
    m_on_click();
  }


  // Setters
  void useOutline(bool state)
  {
    m_show_outline = state;
    if (m_show_outline)
    {
      m_outline_weight = 2;
    }
    else
    {
      m_outline_weight = 0;
    }

    m_background.setOutlineThickness(m_outline_weight);
  }

  void setFade(bool state)
  {
    m_fade = state;
  }

  void setOutlineThickness(int value)
  {
    m_outline_weight = value;
    useOutline(m_show_outline);
  }

  void setFont(sf::Font* font)
  {
    m_font = font;
  }

  void setCallback(std::function<void()> callback)
  {
    m_on_click = callback;
  };

  void setButtonColours(sf::Color fill_colour, sf::Color hover_colour)
  {
    m_fill_color = fill_colour;
    m_hover_color = hover_colour;

    m_background.setFillColor(m_fill_color);
  }

  void setTextColor(sf::Color fill_color)
  {
    m_text_color = fill_color;
    m_label.setFillColor(m_text_color);
  }

  void setPosition(sf::Vector2f pos)
  {
    m_background.setPosition(pos);
    centreInButton(&m_label);
  }

  void setPosition(float x, float y)
  {
    setPosition({x, y});
  }

  void setSize(sf::Vector2f size)
  {
    m_background.setSize(size);
    init(size, m_background.getPosition(), m_fill_color, m_label.getString());
  }

  void setSize(float x, float y)
  {
    setSize({x, y});
  }

  void setString(std::string str)
  {
    m_label.setString(str);
    centreInButton(&m_label);
  }

  // Getters
  sf::FloatRect getGlobalBounds()
  {
    return m_background.getGlobalBounds();
  }

  sf::FloatRect getLocalBounds()
  {
    return m_background.getLocalBounds();
  }

  sf::Vector2f getPosition()
  {
    return m_background.getPosition();
  }



private:
  sf::RectangleShape m_background;
  sf::Text m_label;

  sf::Font* m_font;

  std::function<void()> m_on_click;

  sf::Color m_fill_color = sf::Color(255, 255, 0, 255);
  sf::Color m_outline_color = sf::Color(0, 0, 0, 255);
  sf::Color m_hover_color = sf::Color(128, 128, 128, 255);
  sf::Color m_text_color = sf::Color(255, 255, 255, 255);
  sf::Color m_current_colour = m_fill_color;

  bool m_show_outline = false;
  bool m_is_hovered = false;
  bool m_fade = true;
  float m_fade_speed = 5.0f;
  int m_outline_weight = 0;


  void centreInButton(sf::Text* label)
  {
    label->setOrigin(label->getLocalBounds().width / 2, label->getLocalBounds().height / 2);
    label->setPosition(m_background.getPosition().x + m_background.getSize().x / 2,
      m_background.getPosition().y + m_background.getSize().y / 2);
  }

  sf::Color lerpColour(const sf::Color& a, const sf::Color& b, float t) {
    return sf::Color(
        static_cast<sf::Uint8>(a.r + t * (b.r - a.r)),
        static_cast<sf::Uint8>(a.g + t * (b.g - a.g)),
        static_cast<sf::Uint8>(a.b + t * (b.b - a.b)),
        static_cast<sf::Uint8>(a.a + t * (b.a - a.a))
    );
  }
};

#endif //BUTTON_H
