#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape box;
    sf::RectangleShape slider;
    sf::Vector2f       pos;

public:
    sf::Vector2f valueRange;

    Slider(sf::Vector2f pos, sf::Vector2f size,
           sf::Vector2f values);
    void update(const sf::RenderWindow& window);
    void render(const sf::RenderWindow& window);
};

#endif
