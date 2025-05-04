#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

enum class TruncateType
{
  CutOff,
  Wrap,
  Ellipsis
};

class TextElement : public sf::Drawable {
public:
  TextElement() = default;

  TextElement(const sf::Vector2f& position, const std::string& text, sf::Font* font, unsigned int size = 16, const sf::Color& color = sf::Color::White) {
    init(position, text, font, size, color);
  }

  void init(const sf::Vector2f& position, const std::string& text, sf::Font* font, unsigned int size = 16, const sf::Color& color = sf::Color::White) {
    m_text.setFont(*font);
    m_text.setString(text);
    m_text.setCharacterSize(size);
    m_text.setFillColor(color);
    m_text.setPosition(position);
  }

  void setText(const std::string& text) {
    m_text.setString(text);
    updateText();
  }

  void setFont(sf::Font* font) {
    m_text.setFont(*font);
  }

  void setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
  }

  void setFillColor(const sf::Color& color) {
    m_text.setFillColor(color);
  }

  void setPosition(const sf::Vector2f& position) {
    m_text.setPosition(position);
  }

   // Set truncation options
    void setTruncateType(TruncateType type) {
        m_truncate_type = type;
        updateText();  // Apply truncation logic immediately
    }

    void setMaxWidth(float width) {
        m_max_width = width;
        updateText();  // Re-apply truncation logic if width changes
    }

  sf::FloatRect getGlobalBounds() const {
    return m_text.getGlobalBounds();
  }

private:
  sf::Text m_text;
  TruncateType m_truncate_type = TruncateType::CutOff;
  float m_max_width = 0.0f; // Variable width for truncation

  void updateText() {
    std::string originalText = m_text.getString();

    switch (m_truncate_type) {
      case TruncateType::CutOff:
        cutOffText(originalText);
      break;
      case TruncateType::Wrap:
        wrapText(originalText);
      break;
      case TruncateType::Ellipsis:
        addEllipsis(originalText);
      break;
    }
  }

  // Cut-off method (just truncates the text without any additional symbols)
  void cutOffText(const std::string& originalText) {
    m_text.setString(originalText);
    while (m_text.getLocalBounds().width > m_max_width && !originalText.empty()) {
      std::string truncatedText = originalText;
      truncatedText.pop_back();
      m_text.setString(truncatedText);
    }
  }

  // Wrap method (simple line break based on maxWidth)
  void wrapText(const std::string& originalText) {
    std::string wrappedText;
    std::string currentLine;

    std::istringstream stream(originalText);
    std::string word;

    while (stream >> word) {
      if (currentLine.empty()) {
        currentLine = word;
      } else {
        std::string testLine = currentLine + " " + word;
        m_text.setString(testLine);
        if (m_text.getLocalBounds().width > m_max_width) {
          // New line needed
          wrappedText += currentLine + "\n";
          currentLine = word;
        } else {
          currentLine = testLine;
        }
      }
    }
    wrappedText += currentLine;
    m_text.setString(wrappedText);
  }

  // Ellipsis method (adds "..." at the end of the text if it's too long)
  void addEllipsis(const std::string& originalText)
  {
    m_text.setString(originalText);
    std::string truncatedText = originalText;

    while (m_text.getLocalBounds().width > m_max_width && !truncatedText.empty()) {
      truncatedText.pop_back();
      m_text.setString(truncatedText + "...");
    }
  }

protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_text, states);
  }
};

#endif // TEXTELEMENT_H