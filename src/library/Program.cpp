#include "Program.h"
#include "Timer.h"



Program::Program()
    : m_window(sf::VideoMode(1920, 1080), "SFML works!")
{
}

Program::~Program()
{

}

void Program::Run()
{
    Initialize();
    FPS fps;

    // Check if window is open
    if (!m_window.isOpen())
    {
        std::cerr << "Window failed to open!" << std::endl;
        return;
    }

    while (m_window.isOpen())
    {
        HandleEvent();
        Update();
        Render();
        fps.update();
    }
}

void Program::Initialize()
{

    m_image.create(1900, 1000);
    m_pixelVec.reserve(300000);
    m_activePixelVec.reserve(10000);
}

void Program::updatePixels()
{
    for (auto& pixel : m_activePixelVec)
    {
        if (pixel.m_flags & Fixed) continue;

        // cache values
        int x = pixel.m_pos.x;
        int y = pixel.m_pos.y;

        // Make sure position is within image bounds
        if (y < 0 || y >= static_cast<int>(m_image.getSize().y - 1)) continue;
        if (x < 0 || x >= static_cast<int>(m_image.getSize().x - 1)) continue;

        switch (pixel.m_type) {
        case SAND:
            updateSandPixel(pixel, x, y);
            break;
        case WATER:
            updateWaterPixel(pixel, x, y);
            break;
        default:
            break;
        }


    }
}

void Program::updateSandPixel(Pixel& pixel, int x, int y)
{
    if (y + 1 >= static_cast<int>(m_image.getSize().y)) return;

    sf::Color below = m_image.getPixel(x, y + 1);

    // Fall straight down into empty space
    if (below == sf::Color::Black)
    {
        m_image.setPixel(x, y + 1, pixel.m_color);
        m_image.setPixel(x, y, sf::Color::Black);
        pixel.m_pos.y += 1;
        return;
    }

    // Swap with water directly below
    Pixel* belowPixel = getPixelAt(x, y + 1);
    if (belowPixel && belowPixel->m_type == WATER)
    {
        std::swap(pixel.m_color, belowPixel->m_color);
        std::swap(pixel.m_type, belowPixel->m_type);
        m_image.setPixel(x, y, pixel.m_color);
        m_image.setPixel(x, y + 1, belowPixel->m_color);
        pixel.setState(Active);
        belowPixel->setState(Active);
        return;
    }

    // --- Diagonal randomized direction ---
    int dirFirst = (rand() % 2 == 0) ? -1 : 1;
    int dirSecond = -dirFirst;

    auto tryDiagonal = [&](int dir) {
        int newX = x + dir;
        int newY = y + 1;

        if (newX < 0 || newX >= static_cast<int>(m_image.getSize().x)) return false;

        sf::Color diagColor = m_image.getPixel(newX, newY);
        if (diagColor == sf::Color::Black)
        {
            m_image.setPixel(newX, newY, pixel.m_color);
            m_image.setPixel(x, y, sf::Color::Black);
            pixel.m_pos.x += dir;
            pixel.m_pos.y += 1;
            return true;
        }

        Pixel* diagPixel = getPixelAt(newX, newY);
        if (diagPixel && diagPixel->m_type == WATER)
        {
            std::swap(pixel.m_color, diagPixel->m_color);
            std::swap(pixel.m_type, diagPixel->m_type);
            m_image.setPixel(x, y, pixel.m_color);
            m_image.setPixel(newX, newY, diagPixel->m_color);
            pixel.setState(Active);
            diagPixel->setState(Active);
            return true;
        }

        return false;
        };

    if (tryDiagonal(dirFirst) || tryDiagonal(dirSecond)) {
        return;
    }

    // Freeze only if nothing can be done
    pixel.setState(Fixed);
}

void Program::updateWaterPixel(Pixel& pixel, int x, int y)
{
    // Try falling directly down
    if (m_image.getPixel(x, y + 1) == sf::Color::Black) {
        m_image.setPixel(x, y + 1, pixel.m_color);
        m_image.setPixel(x, y, sf::Color::Black);
        pixel.m_pos.y += 1;
        return;
    }

    // Try flowing diagonally
    bool moved = false;
    if (x > 0 && m_image.getPixel(x - 1, y + 1) == sf::Color::Black) {
        m_image.setPixel(x - 1, y + 1, pixel.m_color);
        m_image.setPixel(x, y, sf::Color::Black);
        pixel.m_pos.x -= 1;
        pixel.m_pos.y += 1;
        moved = true;
    }
    else if (x < static_cast<int>(m_image.getSize().x - 1) && m_image.getPixel(x + 1, y + 1) == sf::Color::Black) {
        m_image.setPixel(x + 1, y + 1, pixel.m_color);
        m_image.setPixel(x, y, sf::Color::Black);
        pixel.m_pos.x += 1;
        pixel.m_pos.y += 1;
        moved = true;
    }

    if (moved) return;

    // Try flowing left or right horizontally if blocked vertically
    int maxFlow = 3;  // how far water can try to move sideways
    for (int offset = 1; offset <= maxFlow; ++offset) {
        // Try left
        if (x - offset >= 0 && m_image.getPixel(x - offset, y) == sf::Color::Black) {
            m_image.setPixel(x - offset, y, pixel.m_color);
            m_image.setPixel(x, y, sf::Color::Black);
            pixel.m_pos.x -= offset;
            return;
        }
        // Try right
        if (x + offset < static_cast<int>(m_image.getSize().x) && m_image.getPixel(x + offset, y) == sf::Color::Black) {
            m_image.setPixel(x + offset, y, pixel.m_color);
            m_image.setPixel(x, y, sf::Color::Black);
            pixel.m_pos.x += offset;
            return;
        }
    }
}

Pixel* Program::getPixelAt(int x, int y)
{
    for (auto& p : m_activePixelVec)
        if (p.m_pos.x == x && p.m_pos.y == y)
            return &p;
    return nullptr;
}

void Program::checkFixedPixels()
{
    for (auto pixel = m_activePixelVec.begin(); pixel != m_activePixelVec.end();) {
        if (pixel->m_flags & Fixed)
        {
            m_activePixelVec.erase(pixel);
        }
        else {
            pixel++;
        }
    }
}

void Program::Update()
{
    updatePixels();
    spawnPixels();
    checkFixedPixels();
}

void Program::Render()
{
    m_window.clear();

    if (!m_texture.loadFromImage(m_image)) {
        std::cerr << "Texture loading failed!" << std::endl;
    }
    m_sprite.setTexture(m_texture, true);

    m_window.draw(m_sprite);
    m_window.display();
}

void Program::HandleEvent()
{
    while (m_window.pollEvent(m_event))
    {
        if (m_event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (m_event.type == sf::Event::KeyPressed)
        {
            if (m_event.key.code == sf::Keyboard::Escape)
            {
                m_window.close();
            }
        }

        if (m_event.type == sf::Event::MouseButtonPressed)
        {
            if (m_event.mouseButton.button == sf::Mouse::Right) {
                m_pixelType = m_pixelType == SAND ? WATER : SAND;
            }
            else {
                m_buttonHeld = true;
            }
        }

        if (m_event.type == sf::Event::MouseButtonReleased)
        {
            m_buttonHeld = false;
        }
    }
}

void Program::spawnPixels()
{
    if (!m_buttonHeld) return;

    auto pos = sf::Mouse::getPosition(m_window);
    auto radius = 30;
    auto xOffset = rand() % static_cast<int>(radius) - radius / 2;
    auto yOffset = rand() % static_cast<int>(radius) - radius / 2;

    auto posX = pos.x + xOffset;
    auto posY = pos.y + yOffset;

    if (m_image.getPixel(posX, posY) == sf::Color::Black &&
        (posX < m_image.getSize().x || posX > 0) &&
        (posY < m_image.getSize().y || posY > 0))
    {
        auto pixel = Pixel(m_pixelType, sf::Vector2i(posX, posY));
        m_pixelVec.push_back(pixel);
        m_activePixelVec.push_back(pixel);
        m_image.setPixel(posX, posY, m_pixelVec.back().m_color);
    }

}
