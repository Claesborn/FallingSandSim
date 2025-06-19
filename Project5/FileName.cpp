#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

void drawBars(sf::RenderWindow& window, const std::vector<int>& data);

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Window configuration
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const unsigned int numValues = 800;

    // Create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
        "Selection Sort Visualization");
    window.setFramerateLimit(60);

    // Generate random data
    std::vector<int> data(numValues);
    for (unsigned int i = 0; i < numValues; ++i)
    {
        data[i] = std::rand() % windowHeight;
    }

    // SELECTION SORT
    // We'll sort from the right side (largest elements to the end).
    // i goes from the last index down to 1 (or up to numValues-1 if you prefer).
    for (unsigned int i = numValues - 1; i > 0; --i)
    {
        // Each pass: find index of the largest element in [0..i]
        unsigned int maxIndex = 0;
        for (unsigned int j = 1; j <= i; ++j)
        {
            // Check for window events in the inner loop 
            // (so we can close while sorting)
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return 0;
                }
            }

            // If we find a bigger value, update maxIndex
            if (data[j] > data[maxIndex])
            {
                maxIndex = j;
            }

            // (Optional) If you want to show every comparison, 
            // you could draw here. But it can be slow.
        }

        // Swap the largest found element with the element at index i
        if (maxIndex != i)
        {
            std::swap(data[maxIndex], data[i]);

            // Draw the bars to show the immediate placement of
            // the next highest bar
            window.clear(sf::Color::Black);
            drawBars(window, data);
            window.display();

            // Short delay to visualize the swap
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    // Keep the window open after sorting is finished
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        // Display final, sorted bars
        window.clear(sf::Color::Black);
        drawBars(window, data);
        window.display();
    }

    return 0;
}

// Draw the bars based on the vector
void drawBars(sf::RenderWindow& window, const std::vector<int>& data)
{
    float barWidth = static_cast<float>(window.getSize().x) / data.size();

    for (unsigned int i = 0; i < data.size(); ++i)
    {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth, static_cast<float>(data[i])));

        float xPos = i * barWidth;
        float yPos = static_cast<float>(window.getSize().y) - data[i];
        bar.setPosition(xPos, yPos);

        // White bars
        bar.setFillColor(sf::Color::White);

        window.draw(bar);
    }
}