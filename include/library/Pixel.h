#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <SFML/Graphics.hpp>
#include <bitset>
#include <iostream>

enum PixelStatus {
    NOSTATUS = 0b0,
    Active   = 0b1 << 0,
    Fixed    = 0b1 << 1
};

enum PixelType { NOTYPE = 0, SAND = 1, WATER = 2, MURKYWATER = 3 };

struct SandColor {
    static sf::Color getRandom() {
        uint8_t r = 190 + rand() % 31;  // 190-220
        uint8_t g = 140 + rand() % 26;  // 140-165
        uint8_t b = 90 + rand() % 21;   // 90-110
        return sf::Color(r, g, b);
    }
};

struct WaterColor {
    static sf::Color getRandom() {
        uint8_t r = 10 + rand() % 21;  // 10-30
        uint8_t g = 60 + rand() % 31;  // 60-90
        uint8_t b = 180 + rand() % 51; // 180-230
        return sf::Color(r, g, b, 200);
    }
};

struct MurkyWaterColor {
    static sf::Color getRandom() {
        uint8_t r = 15 + rand() % 16;    // 15-30
        uint8_t g = 40 + rand() % 31;    // 40-70
        uint8_t b = 100 + rand() % 51;   // 100-150
        return sf::Color(r, g, b, 220);  // slightly opaque
    }
};

struct Pixel {
    sf::Color    m_color = sf::Color::Black;
    uint8_t      m_flags = 0b00000000;
    uint8_t      m_type  = PixelType::NOTYPE;
    sf::Vector2i m_pos   = {0, 0};

    Pixel(PixelType type, sf::Vector2i pos)
        : m_type(type), m_pos(pos) {
        m_flags |= Active;
        auto pixelType = type & 0b11111111;
        switch (type) {
            case SAND:
                m_color = SandColor::getRandom();
                break;
            case WATER:
                m_color = WaterColor::getRandom();
                break;
            case MURKYWATER:
                m_color = MurkyWaterColor::getRandom();
                break;
            default:
                break;
        }
    }

    void setState(PixelStatus flag) { m_flags |= flag; }
};

#endif
