#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <string>
#include <iostream>

#include "Button.h"
#include "Pixel.h"

LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Program {
private:

    // Engine variables
    HWND hwnd;
    WNDPROC originalWndProc;
    sf::RenderWindow window;
    sf::Event event;
    // Setup variables
    sf::Image m_image;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    std::vector<std::unique_ptr<ButtonOverhead>> buttons;
    bool buttonHeld = false;
    sf::Text text;

    std::vector<Pixel> pixelVec;
    std::vector<Pixel> activePixelVec;

public:
    // Constructors/destructors
    Program();
    ~Program();
    // Engine functions
    void Run();
    void Initialize();
    void Update();
    void Render();
    void HandleEvent();
    void LoadImage(const std::string& filePathStr);
    LRESULT HandleCustomWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void initWindow();

    // Button functions
    void initButtons();
    void ButtonPress();
    void ButtonPress2();

    // Pixel stuff
    void updatePixels();
    void checkFixedPixels();
    void spawnSand();
};
