#ifndef TEXTENTRYFIELD_H
#define TEXTENTRYFIELD_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class TextEntryField : public sf::Drawable
{
public:
  // Constructor
  TextEntryField(float width, float height, const sf::Color& backgroundColor, const sf::Color& textColor)
  : m_background(sf::Vector2f(width, height))
  {
    // Set background color
    m_background.setFillColor(backgroundColor);
    m_background.setOutlineThickness(2.f);
    m_background.setOutlineColor(sf::Color::Transparent);

    // Default text setup
    m_text.setFillColor(textColor);
    m_text.setCharacterSize(24);
    m_text.setPosition(10.f, height / 2.f - text_y_offset);  // Default position for text

    // Default font (you should set a valid font)
    m_text.setFont(sf::Font());

    m_enabled = true;
    m_focused = false;
  }

  // Destructor
  virtual ~TextEntryField() = default;

  // Customizable settings
  void setSize(float width, float height) {
    m_background.setSize(sf::Vector2f(width, height));
    m_text.setPosition(10.f, height / 2.f - text_y_offset);  // Adjust text position based on height
  }

  void setPosition(float x, float y) {
    m_background.setPosition(x, y);
    m_text.setPosition(x + 10.f, y + m_background.getSize().y / 2.f - text_y_offset);  // Adjust text position based on background
  }

  void setBackgroundColor(const sf::Color& color) {
    m_background.setFillColor(color);
  }

  void setTextColor(const sf::Color& color) {
    m_text.setFillColor(color);
  }

  void setFont(const sf::Font& font) {
    m_text.setFont(font);
  }

  void setTextSize(unsigned int size) {
    m_text.setCharacterSize(size);
  }

  // Enable/Disable text entry
  void setEnabled(bool enabled) {
    m_enabled = enabled;
  }

  bool isEnabled() const {
    return m_enabled;
  }

  // Getters and Setters for text
  void setString(const std::string& text) {
    m_currentText = text;
    m_text.setString(text);
  }

  std::string getString() const {
    return m_currentText;
  }

  // Event handling
  void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_enabled) return;

    if (event.type == sf::Event::MouseButtonPressed) {
      auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
      if (m_background.getGlobalBounds().contains(mousePos)) {
        m_focused = true;
        m_background.setOutlineColor(sf::Color::Black);
      } else {
        m_focused = false;
        m_background.setOutlineColor(sf::Color::Transparent);
      }
    }

    if (m_focused && event.type == sf::Event::TextEntered) {
      if (event.text.unicode == 8 && !m_currentText.empty()) { // Backspace
        m_currentText.pop_back();
      } else if (event.text.unicode >= 32 && event.text.unicode < 127) { // Printable chars
        m_currentText += static_cast<char>(event.text.unicode);
      }

      m_text.setString(m_currentText);

      if (m_onTextTypedCallback) {
        m_onTextTypedCallback(m_currentText);
      }
    }
  }

  // Triggered when a letter is typed
  void onTextTyped(std::function<void(const std::string&)> callback) {
    m_onTextTypedCallback = callback;
  }


protected:
    // Draw the text entry field
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(m_background, states);
        target.draw(m_text, states);
    }

private:
  sf::RectangleShape m_background;  // Background of the text field
  sf::Text m_text;  // The actual text being entered
  int text_y_offset = 6;
  bool m_enabled = true;  // Whether the text entry is enabled
  bool m_focused = false;
  std::string m_currentText;  // The current text in the text entry
  std::function<void(const std::string&)> m_onTextTypedCallback;  // Callback triggered on text input
};

#endif // TEXTENTRYFIELD_H
