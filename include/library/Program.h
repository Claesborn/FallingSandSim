#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include <Windows.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

#include "Button.h"
#include "Pixel.h"

LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg,
                                  WPARAM wParam, LPARAM lParam);

class Program {
private:
    // Engine variables
    sf::RenderWindow window;
    // Setup variables
    sf::Image                                    m_image;
    sf::Texture                                  m_texture;
    sf::Sprite                                   m_sprite;
    sf::Font                                     m_font;
    bool buttonHeld = false;

    std::vector<Pixel> m_pixelVec;
    std::vector<Pixel> m_activePixelVec;

public:
    // Constructors/destructors
    Program();
    ~Program();
    // Engine functions
    void    Run();
    void    Initialize();
    void    Update();
    void    Render();
    void    HandleEvent();
    void    initWindow();

    // Pixel stuff
    void updatePixels();
    void updateSandPixel(Pixel& pixel, int x, int y);
    void updateWaterPixel(Pixel& pixel, int x, int y);
    void checkFixedPixels();
    void spawnPixels();
};

#endif
