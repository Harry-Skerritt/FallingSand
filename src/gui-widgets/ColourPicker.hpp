#ifndef COLOUR_PICKER_H
#define COLOUR_PICKER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../../thirdparty/TinyFileDialogs/tinyfiledialogs.h"
#include <sstream>
#include <iomanip>

class ColourPicker : public sf::Drawable {
public:
    ColourPicker(const sf::Vector2f& position = {0, 0}, const sf::Vector2f& size = {40, 40}, const sf::Color& defaultColour = sf::Color::White, sf::Font* font = nullptr)
    {
        m_box.setPosition(position);
        m_box.setSize(size);
        m_box.setFillColor(defaultColour);
        m_box.setOutlineThickness(2);
        m_box.setOutlineColor(sf::Color::Black);

        // Store the initial color as the selected color
        m_selectedColor = defaultColour;

        // Set up the label text element (to show the hex code)
        m_hexLabel.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f));
        m_hexLabel.setFillColor(m_selectedColor);
        m_hexLabel.setString(getColourHex());

        if (font)
        {
            setFont(font);
        }

        updateLayout();
    }

    void setFont(sf::Font* font)
    {
        m_font = font;
        m_hexLabel.setFont(*font);
        updateLayout();
    }

    void setPosition(const sf::Vector2f& pos)
    {
        m_box.setPosition(pos);
        updateLayout();
    }

    void setSize(const sf::Vector2f& size)
    {
        m_box.setSize(size);
        updateLayout();
    }

    void update(const sf::Vector2f& worldMousePos)
    {
        if (m_disabled) return; // If disabled, skip update logic

        bool isHovered = m_box.getGlobalBounds().contains(worldMousePos);

        if (isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_wasPressedLastFrame)
        {
            openColourPicker();
        }

        m_wasPressedLastFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

    sf::Color getColourRGB() const
    {
        return m_box.getFillColor();
    }

    std::string getColourHex() const
    {
        const sf::Color c = m_box.getFillColor();
        std::stringstream ss;
        ss << "#" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(c.r)
           << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(c.g)
           << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(c.b);
        return ss.str();
    }

    // Set the disabled state
    void setDisabled(bool disabled)
    {
        m_disabled = disabled;
        updateVisualState();
    }

private:
    sf::RectangleShape m_box;
    sf::Text m_hexLabel;
    sf::Font* m_font = nullptr;
    bool m_wasPressedLastFrame = false;
    bool m_disabled = false;
    sf::Color m_selectedColor;  // Store the currently selected color

    void openColourPicker()
    {
        unsigned char currentColour[3] = {
            m_box.getFillColor().r,
            m_box.getFillColor().g,
            m_box.getFillColor().b
        };

        const char* hexResult = tinyfd_colorChooser("Pick a Colour", nullptr, currentColour, currentColour);

        if (hexResult) {
            unsigned int r, g, b;
            if (sscanf(hexResult, "#%02x%02x%02x", &r, &g, &b) == 3) {
                m_selectedColor = sf::Color(r, g, b); // Save the selected color
                m_box.setFillColor(m_selectedColor); // Apply the selected color to the box
                m_hexLabel.setFillColor(m_selectedColor);
                m_hexLabel.setString(getColourHex()); // Update hex label with the new color
                updateLayout(); // Recalculate the layout to ensure proper text position
            }
        }
    }

    void updateLayout()
    {
        // Vertically center the text relative to the color square
        sf::FloatRect boxBounds = m_box.getGlobalBounds();
        sf::FloatRect textBounds = m_hexLabel.getLocalBounds();

        // Position the hex label outside the box, to the right of it
        m_hexLabel.setOrigin(0, textBounds.height / 2);
        m_hexLabel.setPosition(
            boxBounds.left + boxBounds.width + 8.0f, // Position on the right side of the square
            boxBounds.top + boxBounds.height / 2 - textBounds.top / 2 // Vertically center the text
        );
    }

    void updateVisualState()
    {
        if (m_disabled)
        {
            m_box.setFillColor(sf::Color(169, 169, 169)); // Gray color for disabled state
            m_box.setOutlineColor(sf::Color(128, 128, 128)); // Dimmed outline
            m_hexLabel.setFillColor(sf::Color(128, 128, 128)); // Dimmed hex label
        }
        else
        {
            // Apply the selected color when the picker is enabled
            m_box.setFillColor(m_selectedColor); // Use the previously selected color when enabled
            m_hexLabel.setFillColor(m_selectedColor); // Normal hex label text color
            m_box.setOutlineColor(sf::Color::Black); // Normal outline
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_box, states);
        target.draw(m_hexLabel, states);
    }
};

#endif // COLOUR_PICKER_H
