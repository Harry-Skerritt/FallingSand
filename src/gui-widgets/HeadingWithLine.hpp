//
// Created by Harry on 04/05/2025.
//

#ifndef HEADING_H
#define HEADING_H

#include "HorizontalSeperator.hpp"
#include <SFML/Graphics.hpp>

enum class AlignmentMode
{
  Left,
  Centre,
  Right
};

class HeadingWithLine : public sf::Drawable, public sf::Transformable
{
public:
  HeadingWithLine();
  ~HeadingWithLine();
  HeadingWithLine(const sf::Font* font, const std::string text, const sf::Vector2f pos, const float width, AlignmentMode alignment = AlignmentMode::Left);

  void init(const sf::Font* font, const std::string text, const sf::Vector2f pos, const float width, AlignmentMode alignment = AlignmentMode::Left);

  void setText(const std::string& text);
  void setAlignment(AlignmentMode alignment);

  void setPosition(const sf::Vector2f& position);
  void setPosition(const float x, const float y);

  void setHeaderWidth(const float width);
  void setHeaderHeight(const float height);

  void setCharacterSize(const float size);

  void setTextColour(sf::Color colour, bool line_match = true);
  void setLineColour(sf::Color colour);


private:
  HorizontalSeparator* m_separator;
  sf::Text* m_title;

  AlignmentMode m_alignment_mode;

  sf::Color m_text_colour = sf::Color::White;
  sf::Color m_line_colour = m_text_colour;

  sf::Vector2f m_pos;

  int m_line_weight = 2;
  int m_line_width;
  float m_character_size = 16;

  const sf::Font* m_font = nullptr;
  void updateLayout();

protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

// ==== Implementation ====
inline HeadingWithLine::HeadingWithLine() {};
inline HeadingWithLine::~HeadingWithLine() = default;

inline HeadingWithLine::HeadingWithLine(
  const sf::Font* font, const std::string text, const sf::Vector2f pos,
  const float width, AlignmentMode alignment)
{
  init(font, text, pos, width, alignment);
}

inline void HeadingWithLine::init(
  const sf::Font* font, const std::string text, const sf::Vector2f pos,
  const float width, AlignmentMode alignment)
{
  m_font = font;
  m_title = new sf::Text(text, *m_font, m_character_size);
  m_separator = new HorizontalSeparator(pos, width, m_line_weight, m_line_colour);
  m_alignment_mode = alignment;
  m_line_width = width;

  m_title->setFillColor(m_text_colour);
  m_line_width = width;
  m_pos = pos;
  updateLayout();
}

inline void HeadingWithLine::setText(const std::string& text)
{
  m_title->setString(text);
  updateLayout();
}

inline void HeadingWithLine::setAlignment(AlignmentMode alignment)
{
  m_alignment_mode = alignment;
  updateLayout();
}

inline void HeadingWithLine::setPosition(const sf::Vector2f& position)
{
  m_pos = position;
  updateLayout();
}

inline void HeadingWithLine::setPosition(const float x, const float y)
{
  setPosition(sf::Vector2f(x, y));
}

inline void HeadingWithLine::setHeaderWidth(const float width)
{
  m_line_width = width;
  updateLayout();
}

inline void HeadingWithLine::setHeaderHeight(const float height)
{
  m_line_weight = height;
  updateLayout();
}


inline void HeadingWithLine::setCharacterSize(const float size)
{
  m_title->setCharacterSize(size);
  updateLayout();
}


inline void HeadingWithLine::setTextColour(sf::Color colour, bool line_match)
{
  m_text_colour = colour;
  if (line_match)
  {
    m_line_colour = m_text_colour;
    m_separator->setColor(m_line_colour);
  }

  m_title->setFillColor(m_text_colour);
}


inline void HeadingWithLine::setLineColour(sf::Color colour)
{
  m_line_colour = colour;
  m_separator->setColor(m_line_colour);
}


inline void HeadingWithLine::updateLayout()
{
  sf::FloatRect text_bounds = m_title->getLocalBounds();

  // Align the text correctly (top-left corner as actual top-left, not baseline-left)
  sf::Vector2f text_pos;

  switch (m_alignment_mode) {
    case AlignmentMode::Left:
      text_pos.x = m_pos.x;
    break;
    case AlignmentMode::Centre:
      text_pos.x = m_pos.x + (m_line_width - text_bounds.width) / 2.f - text_bounds.left;
    break;
    case AlignmentMode::Right:
      text_pos.x = m_pos.x + m_line_width - text_bounds.width - text_bounds.left;
    break;
  }

  // Adjust for the text baseline offset
  text_pos.y = m_pos.y;
  m_title->setPosition(text_pos);

  // Place the line just below the text with some vertical padding (e.g., 4 pixels)
  float text_height = text_bounds.height;
  float separator_y = m_title->getPosition().y + (text_height*2) + 4;
  m_separator->setSize(m_line_width, m_line_weight);
  m_separator->setPosition({m_pos.x, separator_y});
}


inline void
HeadingWithLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(*m_title, states);
  target.draw(*m_separator, states);
}











#endif //HEADING_H
