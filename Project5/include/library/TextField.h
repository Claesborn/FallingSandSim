#ifndef __TEXT_FIELD_H__
#define __TEXT_FIELD_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <iostream>
#include <string>

// std::vector<std::unique_ptr<TextField> textFields;
// textFields.push_back(std::make_unique<TextField>(position, font,
// textsize, function<void>(const std::string&)) Functions require
// string input by const string reference

/*Update handle events with

while(window.pollEvent(event))
{
    for (auto& textField : textFields)
    {
        textField->Update(window);
    }

    if (event.type == sf::Event::Closed)
    {
        window.close();
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
        if (event.key.code == sf::Keyboard::Enter)
        {
            selectedTextField->execute();
        }
    }

    if (event.type == sf::Event::TextEntered && selectedTextField
!= nullptr)
    {
        if (event.text.unicode == '\b' && !userInput.empty())
        {
            userInput.pop_back();
        }
        else if (event.text.unicode < 128 && event.text.unicode !=
'\b')
        {
            userInput += static_cast<char>(event.text.unicode);
        }

        selectedTextField->setString(userInput);

    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        selectedTextField = TextField::textFieldSelected;

        if (selectedTextField != nullptr)
        {
            userInput.clear();
            userInput = selectedTextField->getString();
        }
    }
}
*/

class TextField {
private:
    sf::RectangleShape                     field;
    sf::Text                               text;
    bool                                   isSelected = false;
    std::function<void(const sf::String&)> onExecute;

public:
    static TextField*  textFieldSelected;
    static std::string userInput;

    TextField(sf::Vector2f pos, sf::Font& font, int textSize,
              std::function<void(const sf::String&)> function)
        : text(font, "", textSize), onExecute(function) {
        field.setPosition(pos);
        field.setFillColor(sf::Color::White);
        field.setSize(sf::Vector2f(50.f, 30.f));
        field.setOutlineColor(sf::Color::Black);
        field.setOutlineThickness(2);

        text.setFillColor(sf::Color::Black);
        text.setPosition(field.getPosition());
        userInput.reserve(30);
    }

    void Update(sf::RenderWindow& window) {
        sf::FloatRect fieldBounds = field.getGlobalBounds();
        sf::Vector2i  mousePos    = sf::Mouse::getPosition(window);

        if (fieldBounds.contains(
                static_cast<sf::Vector2f>(mousePos)) &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (textFieldSelected != nullptr &&
                textFieldSelected != this) {
                textFieldSelected->isSelected = false;
                textFieldSelected->field.setFillColor(
                    sf::Color::White);
            }

            textFieldSelected = this;
            isSelected        = true;
            field.setFillColor(sf::Color(150, 150, 150));

        } else if (sf::Mouse::isButtonPressed(
                       sf::Mouse::Button::Left)) {
            if (textFieldSelected == this) {
                isSelected = false;
                field.setFillColor(sf::Color::White);
                textFieldSelected = nullptr;
            }
        }
    }

    void Render(sf::RenderWindow& window) {
        window.draw(field);
        window.draw(text);
    }

    sf::String getString() {
        if (text.getString().isEmpty()) return std::string();
        return text.getString();
    }

    void execute() {
        onExecute(text.getString());
        text.setString("");
    }

    void setString(std::string str) {
        if (textFieldSelected != nullptr) {
            textFieldSelected->text.setString(str);
            field.setSize(sf::Vector2f(
                std::max(45, static_cast<int>(
                                 text.getGlobalBounds().size.x)) +
                    5,
                std::max(25, static_cast<int>(
                                 text.getGlobalBounds().size.y) +
                                 5)));
        } else {
            std::cout << "No textfield set" << std::endl;
        }
    }
};

#endif
