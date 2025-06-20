#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

enum class ButtonState { Idle = 0, Hover, Clicked, Disabled };

class ButtonOverhead {
    // Set up a std::vector<std::unique_ptr<ButtonOverhead>>
    // buttons;
    //
    // Add buttons using
    // buttons.push_back(std::make_unique<MultiButton>(buttonptr));
    // Or buttons.push_back(std::make_unique<Button>(buttonptr));
public:
    virtual void Update(sf::RenderWindow& window) = 0;
    virtual void Draw(sf::RenderWindow& window)   = 0;
};

class MultiButton : public ButtonOverhead {
    // Create a button using the constructor then add functions to
    // it with auto buttonCallback =
    // std::make_shared<std::function<void()>>([this]() {
    // this->FunctionToCall(); }); button + buttonCallback;

private:
    sf::RectangleShape button;
    sf::Text           text;
    ButtonState        state = ButtonState::Idle;

    sf::Color idleColor, hoverColor, clickedColor, textIdleColor,
        textHoverColor, textClickedColor;
    sf::RenderWindow* window;
    bool              wasClicked = false;
    bool              isActive;

    std::vector<std::shared_ptr<std::function<void()>>> onClicks;

public:
    MultiButton(sf::Vector2f size, sf::Vector2f position,
                sf::Color idleCol, sf::Color hoverCol,
                sf::Color clickCol, sf::Color textIdle,
                sf::Color textHover, sf::Color textClick,
                const std::string& buttonText,
                const sf::Font& font, unsigned int textSize,
                bool isActiveAtStart = true)
        : state(ButtonState::Idle),
          idleColor(idleCol),
          hoverColor(hoverCol),
          clickedColor(clickCol),
          isActive(isActiveAtStart),
          textIdleColor(textIdle),
          textHoverColor(textHover),
          textClickedColor(textClick) {
        button.setSize(size);
        button.setPosition(position);
        button.setFillColor(idleCol);  // Button Idle color
        button.setOutlineColor(sf::Color::Black);
        button.setOutlineThickness(2);

        text.setFont(font);
        text.setCharacterSize(textSize);
        text.setString(buttonText);
        text.setFillColor(sf::Color::White);  // Text color
        centerTextInButton();
    }

    void Update(sf::RenderWindow& window) override {
        updateButtonState(window);  // Swaps between enum states
        updateButtonAppearance();  // Updates the button appearance
    }

    void Draw(sf::RenderWindow& window) override {
        window.draw(button);
        window.draw(text);
    }

    const void
    setCallback(std::shared_ptr<std::function<void()>> callback) {
        // Adds a functioncall to the button
        onClicks.emplace_back(callback);
    }

    const void removeCallback(
        std::shared_ptr<std::function<void()>> callback) {
        auto it = std::remove(onClicks.begin(), onClicks.end(),
                              callback);
        if (it != onClicks.end())
            onClicks.erase(it, onClicks.end());
        else
            std::cout << "removeCallback: " << callback
                      << " not found"
                      << std::endl;  // To inform user about an
                                     // unneseccary removecall
    }

    const MultiButton&
    operator+(std::shared_ptr<std::function<void()>> func) {
        setCallback(func);
        return *this;
    }

    const MultiButton&
    operator-(std::shared_ptr<std::function<void()>> func) {
        removeCallback(func);
        return *this;
    }

private:
    const void centerTextInButton() {
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);
        text.setPosition(button.getPosition() +
                         button.getSize() / 2.0f);
    }

    const void updateButtonAppearance() {
        switch (state) {
            case ButtonState::Disabled:
                button.setFillColor(sf::Color::Red);
                break;
            case ButtonState::Clicked:
                button.setFillColor(clickedColor);
                text.setFillColor(textClickedColor);
                break;
            case ButtonState::Hover:
                button.setFillColor(hoverColor);
                text.setFillColor(textHoverColor);
                break;
            case ButtonState::Idle:
            default:
                button.setFillColor(idleColor);
                text.setFillColor(textIdleColor);
                break;
        }
    }

    void updateButtonState(const sf::RenderWindow& window) {
        if (!isActive) {
            state = ButtonState::Disabled;
            return;
        }
        sf::Vector2i  mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect buttonBounds = button.getGlobalBounds();

        if (buttonBounds.contains(
                static_cast<sf::Vector2f>(mousePos))) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                state = ButtonState::Clicked;
                for (auto& onClick : onClicks) {
                    if (onClick && !wasClicked) {
                        (*onClick)();
                    }
                }
                wasClicked = true;
            } else {
                state      = ButtonState::Hover;
                wasClicked = false;
            }
        } else {
            state      = ButtonState::Idle;
            wasClicked = false;
        }
    }
};

class Button : public ButtonOverhead {
    // Add functions using [this]() { this->ButtonPress(); }
private:
    sf::RectangleShape button;
    sf::Text           text;
    ButtonState        state = ButtonState::Idle;

    sf::Color idleColor, hoverColor, clickedColor, textIdleColor,
        textHoverColor, textClickedColor;
    sf::RenderWindow* window;
    bool              wasClicked = false;
    bool              isActive;

    std::function<void()> onClick;

public:
    Button(sf::Vector2f size, sf::Vector2f position,
           sf::Color idleCol, sf::Color hoverCol,
           sf::Color clickCol, sf::Color textIdle,
           sf::Color textHover, sf::Color textClick,
           const std::string& buttonText, const sf::Font& font,
           unsigned int          textSize,
           std::function<void()> clickAction,
           bool                  isActiveAtStart = true)
        : state(ButtonState::Idle),
          idleColor(idleCol),
          hoverColor(hoverCol),
          clickedColor(clickCol),
          onClick(std::move(clickAction)),
          isActive(isActiveAtStart),
          textIdleColor(textIdle),
          textHoverColor(textHover),
          textClickedColor(textClick) {
        button.setSize(size);
        button.setPosition(position);
        button.setFillColor(idleCol);  // Button Idle color
        button.setOutlineColor(sf::Color::Black);
        button.setOutlineThickness(2);

        text.setFont(font);
        text.setCharacterSize(textSize);
        text.setString(buttonText);
        text.setFillColor(sf::Color::White);  // Text color
        centerTextInButton();
    }

    void Update(sf::RenderWindow& window) override {
        updateButtonState(window);  // Swaps between enum states
        updateButtonAppearance();  // Updates the button appearance
    }

    void Draw(sf::RenderWindow& window) override {
        window.draw(button);
        window.draw(text);
    }

private:
    const void centerTextInButton() {
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);
        text.setPosition(button.getPosition() +
                         button.getSize() / 2.0f);
    }

    void updateButtonState(const sf::RenderWindow& window) {
        if (!isActive) {
            state = ButtonState::Disabled;
            return;
        }
        sf::Vector2i  mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect buttonBounds = button.getGlobalBounds();

        if (buttonBounds.contains(
                static_cast<sf::Vector2f>(mousePos))) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                state = ButtonState::Clicked;

                if (onClick && !wasClicked) {
                    onClick();  // Calls the stored function
                }

                wasClicked =
                    true;  // To avoid calling function every frame
            } else {
                state      = ButtonState::Hover;
                wasClicked = false;
            }
        } else {
            state      = ButtonState::Idle;
            wasClicked = false;
        }
    }

    const void updateButtonAppearance() {
        switch (state) {
            case ButtonState::Disabled:
                button.setFillColor(sf::Color::Red);
                break;
            case ButtonState::Clicked:
                button.setFillColor(clickedColor);
                text.setFillColor(textClickedColor);
                break;
            case ButtonState::Hover:
                button.setFillColor(hoverColor);
                text.setFillColor(textHoverColor);
                break;
            case ButtonState::Idle:
            default:
                button.setFillColor(idleColor);
                text.setFillColor(textIdleColor);
                break;
        }
    }
};

#endif
