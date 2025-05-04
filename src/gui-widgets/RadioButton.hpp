//
// Created by Harry on 04/05/2025.
//

#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

class RadioButtonGroup;

class RadioButton : public sf::Drawable {
public:
    RadioButton(RadioButtonGroup* group,
                const sf::Vector2f& position,
                float radius,
                const sf::Font& font,
                const std::string& label_text,
                unsigned int text_size = 16);

    void update(const sf::Vector2f mouse_pos);
    void setSelected(bool selected);
    bool isSelected() const;

    void setTextColor(const sf::Color& color);
    void setCircleColor(const sf::Color& outline, const sf::Color& fill);
    void setPosition(const sf::Vector2f& position);

    std::function<void()> onSelect;

  bool selected = false;

private:
    sf::CircleShape outer_circle;
    sf::CircleShape inner_circle;
    sf::Text label;

    bool was_mouse_pressed = false;

    RadioButtonGroup* group;

    bool containsPoint(const sf::Vector2f& point) const;

protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class RadioButtonGroup {
public:
    void addButton(RadioButton* button);
    void clearSelection(RadioButton* except);

private:
    std::vector<RadioButton*> buttons;
};

// ==== Implementation ===


inline RadioButton::RadioButton(RadioButtonGroup* group,
                         const sf::Vector2f& position,
                         float radius,
                         const sf::Font& font,
                         const std::string& label_text,
                         unsigned int text_size)
    : group(group)
{
    outer_circle.setRadius(radius);
    outer_circle.setOutlineThickness(2);
    outer_circle.setFillColor(sf::Color::Transparent);
    outer_circle.setOutlineColor(sf::Color::White);
    outer_circle.setPosition(position);

    inner_circle.setRadius(radius - 4);
    inner_circle.setFillColor(sf::Color::White);
    inner_circle.setPosition(position.x + 4, position.y + 4); // inset

    label.setFont(font);
    label.setString(label_text);
    label.setCharacterSize(text_size);
    label.setFillColor(sf::Color::White);
    label.setPosition(position.x + radius * 2 + 10, position.y - 4);

    if (group) {
        group->addButton(this);
    }
}

inline void RadioButton::update(const sf::Vector2f mouse_pos) {
    bool hovered = containsPoint(mouse_pos);
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    // Only trigger on click-release within bounds
    if (!pressed && was_mouse_pressed && hovered) {
        setSelected(true);
        if (onSelect) onSelect();
    }

    was_mouse_pressed = pressed;
}

inline bool RadioButton::containsPoint(const sf::Vector2f& point) const {
    return outer_circle.getGlobalBounds().contains(point);
}

inline void RadioButton::setSelected(bool value) {
    if (selected == value) return;
    if (value && group) group->clearSelection(this);
    selected = value;
}

inline bool RadioButton::isSelected() const {
    return selected;
}

inline void RadioButton::setTextColor(const sf::Color& color) {
    label.setFillColor(color);
}

inline void RadioButton::setCircleColor(const sf::Color& outline, const sf::Color& fill) {
    outer_circle.setOutlineColor(outline);
    inner_circle.setFillColor(fill);
}

inline void RadioButton::setPosition(const sf::Vector2f& position) {
    outer_circle.setPosition(position);
    inner_circle.setPosition(position.x + 4, position.y + 4);
    label.setPosition(position.x + outer_circle.getRadius() * 2 + 10, position.y - 4);
}

inline void RadioButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(outer_circle, states);
    if (selected) {
        target.draw(inner_circle, states);
    }
    target.draw(label, states);
}

inline void RadioButtonGroup::addButton(RadioButton* button) {
    buttons.push_back(button);
}

inline void RadioButtonGroup::clearSelection(RadioButton* except) {
    for (RadioButton* btn : buttons) {
        if (btn != except) {
            btn->selected = false;
        }
    }
}


#endif //RADIOBUTTON_H
