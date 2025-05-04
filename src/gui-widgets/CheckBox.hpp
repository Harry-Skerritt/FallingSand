#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include <SFML/Graphics.hpp>
#include <string>

class CheckBox : public sf::Drawable {
public:
    CheckBox(const sf::Vector2f& position = {0, 0},
             const sf::Vector2f& size = {20, 20},
             const std::string& label_text = "",
             sf::Font* font = nullptr)
    {
        m_box.setPosition(position);
        m_box.setSize(size);
        m_box.setOutlineThickness(2);
        m_box.setOutlineColor(sf::Color::Black);
        m_box.setFillColor(sf::Color::White);

        m_check_mark.setCharacterSize(static_cast<unsigned int>(size.y * 0.8f));
        m_check_mark.setFillColor(sf::Color::Black);
        m_check_mark.setString("X");

        m_label.setCharacterSize(static_cast<unsigned int>(size.y * 0.8f));
        m_label.setFillColor(sf::Color::White);
        m_label.setString(label_text);

        if (font)
        {
            setFont(font);
        }

        updateLayout();
    }

    void setFont(sf::Font* font)
    {
        m_font = font;
        m_check_mark.setFont(*font);
        m_label.setFont(*font);
        updateLayout();
    }

    void setColour(sf::Color colour)
    {
      m_label.setFillColor(colour);
    }

    void setPosition(const sf::Vector2f& pos)
    {
        m_box.setPosition(pos);
        updateLayout();
    }

    void setChecked(bool checked)
    {
        m_is_checked = checked;
    }

    bool isChecked() const
    {
        return m_is_checked;
    }


    void update(const sf::Vector2f& world_mouse_pos)
    {
        bool is_hovered = m_box.getGlobalBounds().contains(world_mouse_pos);

        if (is_hovered && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_was_pressed_last_frame)
        {
            m_is_checked = !m_is_checked;
        }

        m_was_pressed_last_frame = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

private:
    sf::RectangleShape m_box;
    sf::Text m_check_mark;
    sf::Text m_label;
    sf::Font* m_font = nullptr;
    bool m_is_checked = false;
    bool m_was_pressed_last_frame = false;

    void updateLayout()
    {
        // Position checkmark in the center of the checkbox
        sf::FloatRect box_bounds = m_box.getGlobalBounds();
        sf::FloatRect mark_bounds = m_check_mark.getLocalBounds();
        m_check_mark.setOrigin(mark_bounds.width / 2, mark_bounds.height / 2);
        m_check_mark.setPosition(
            box_bounds.left + box_bounds.width / 2,
            box_bounds.top + box_bounds.height / 2 - mark_bounds.top / 2
        );

        // Position label to the right of checkbox
        m_label.setPosition(
            box_bounds.left + box_bounds.width + 8.0f,
            box_bounds.top
        );
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_box, states);
        if (m_is_checked)
            target.draw(m_check_mark, states);
        target.draw(m_label, states);
    }
};

#endif // CHECK_BOX_H
