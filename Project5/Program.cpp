#include "Program.h"

#include "Timer.h"
// Global pointer to the Program instance
Program* g_pProgram = nullptr;

LRESULT CALLBACK CustomWindowProc(HWND hwnd, UINT uMsg,
                                  WPARAM wParam, LPARAM lParam) {
    if (g_pProgram) {
        return g_pProgram->HandleCustomWindowProc(hwnd, uMsg,
                                                  wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Program::Program()
    : window(sf::VideoMode(1920, 1080), "SFML works!") {
    g_pProgram = this;

    // Load font
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        exit(1);
    }
}

Program::~Program() {}

void Program::Run() {
    Initialize();

    // Check if window is open
    if (!window.isOpen()) {
        std::cerr << "Window failed to open!" << std::endl;
        return;
    }

    while (window.isOpen()) {
        HandleEvent();
        Update();
        Render();
    }
}

void Program::Initialize() {
    initWindow();

    m_image.create(1900, 1000);
    pixelVec.reserve(300000);
    activePixelVec.reserve(10000);
    initButtons();
}

LRESULT Program::HandleCustomWindowProc(HWND hwnd, UINT uMsg,
                                        WPARAM wParam,
                                        LPARAM lParam) {
    switch (uMsg) {
        case WM_DROPFILES: {
            HDROP   hDrop = (HDROP)wParam;
            wchar_t filePath[MAX_PATH];
            DragQueryFile(hDrop, 0, filePath, MAX_PATH);
            DragFinish(hDrop);
            std::wstring wFilePath(filePath);
            std::string  filePathStr(wFilePath.begin(),
                                     wFilePath.end());

            if (g_pProgram) {
                g_pProgram->LoadImage(filePathStr);
            } else {
                std::cerr << "Program instance is null in "
                             "HandleCustomWindowProc!"
                          << std::endl;
                exit(1);
            }
            break;
        }
        default:
            return CallWindowProc(originalWndProc, hwnd, uMsg,
                                  wParam, lParam);
    }
    return 0;
}

void Program::initWindow() {
    // Get the window handle from the SFML window
    hwnd = window.getSystemHandle();

    // Enable drag and drop
    DragAcceptFiles(hwnd, TRUE);

    // Subclass the window to handle custom messages
    originalWndProc = (WNDPROC)SetWindowLongPtr(
        hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWindowProc);
}

void Program::initButtons() {
    // Create a button and add it to the vector
    // auto buttonptr = Button(
    //     sf::Vector2f(150.f, 100.f),
    //     sf::Vector2f(100.f, 100.f),
    //     sf::Color(100, 100, 100),
    //     sf::Color(150, 150, 150),
    //     sf::Color(180, 180, 180),
    //     sf::Color(255, 255, 255),
    //     sf::Color(255, 255, 255),
    //     sf::Color(255, 255, 255),
    //     "Restart",
    //     font, 25,
    //     [this]() { this->ButtonPress(); }
    // );
    // auto buttonCallback =
    // std::make_shared<std::function<void()>>([this]() {
    // this->ButtonPress(); }); auto buttonCallback2 =
    // std::make_shared<std::function<void()>>([this]() {
    // this->ButtonPress2(); });

    /*buttonptr + buttonCallback;
    buttonptr + buttonCallback2;*/

    // buttons.emplace_back(std::make_unique<Button>(buttonptr));
}

void Program::ButtonPress() {
    // image->create(1900, 1000, sf::Color(rand() % 256, rand() %
    // 256, rand() % 256)); texture->loadFromImage(*image);
    // sprite->setTexture(*texture);
}

void Program::ButtonPress2() {
    std::cout << "Clicked!!!" << std::endl;
}

void Program::updatePixels() {
    for (auto& pixel : activePixelVec) {
        if (pixel.m_flags & Fixed) continue;
        // cache values
        int x = pixel.m_pos.x;
        int y = pixel.m_pos.y;

        // Make sure position is within image bounds
        if (y < 0 ||
            y >= static_cast<int>(m_image.getSize().y - 1))
            continue;
        if (x < 0 ||
            x >= static_cast<int>(m_image.getSize().x - 1))
            continue;

        auto pix = m_image.getPixel(x, y + 1);
        if (pix == sf::Color::Black) {
            // set the new pixel location
            m_image.setPixel(x, y + 1, pixel.m_color);

            // clear old pixel location
            m_image.setPixel(x, y, sf::Color::Black);

            // Update pixel internal position
            pixel.m_pos.y = pixel.m_pos.y + 1;
        } else if (m_image.getPixel(x + 1, y + 1) ==
                       sf::Color::Black ||
                   m_image.getPixel(x - 1, y + 1) ==
                       sf::Color::Black) {
            bool canFallLeft =
                (x > 0) && (m_image.getPixel(x - 1, y + 1) ==
                            sf::Color::Black);
            bool canFallRight =
                (x < static_cast<int>(m_image.getSize().x - 1)) &&
                (m_image.getPixel(x + 1, y + 1) ==
                 sf::Color::Black);

            if (canFallLeft || canFallRight) {
                int dir = 0;

                if (canFallLeft && canFallRight)
                    dir = (rand() % 2 == 0) ? -1 : 1;
                else if (canFallLeft)
                    dir = -1;
                else
                    dir = 1;

                m_image.setPixel(x + dir, y + 1, pixel.m_color);
                m_image.setPixel(x, y, sf::Color::Black);
                pixel.m_pos.x += dir;
                pixel.m_pos.y += 1;
            }
        } else {
            pixel.setState(Fixed);
        }
    }
}

void Program::checkFixedPixels() {
    for (auto pixel = activePixelVec.begin();
         pixel != activePixelVec.end();) {
        if (pixel->m_flags & Fixed) {
            activePixelVec.erase(pixel);
        } else {
            pixel++;
        }
    }
}

void Program::Update() {
    updatePixels();
    spawnSand();
    for (auto& button : buttons) {
        button->Update(window);
    }
    checkFixedPixels();
}

void Program::Render() {
    window.clear();

    // for (auto& button : buttons) {
    //     button->Draw(window);
    // }
    if (!texture.loadFromImage(m_image)) {
        std::cerr << "Texture loading failed!" << std::endl;
    }
    sprite.setTexture(texture, true);

    window.draw(sprite);
    window.display();
}

void Program::HandleEvent() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        if (event.type == sf::Event::TextEntered) {
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            buttonHeld = true;
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            buttonHeld = false;
        }
    }
}

void Program::spawnSand() {
    if (!buttonHeld) return;

    auto pos     = sf::Mouse::getPosition(window);
    auto radius  = 30;
    auto xOffset = rand() % static_cast<int>(radius) - radius / 2;
    auto yOffset = rand() % static_cast<int>(radius) - radius / 2;

    auto posX = pos.x + xOffset;
    auto posY = pos.y + yOffset;

    if (m_image.getPixel(posX, posY) == sf::Color::Black &&
        (posX < m_image.getSize().x || posX > 0) &&
        (posY < m_image.getSize().y || posY > 0)) {
        auto pixel = Pixel(SAND, sf::Vector2i(posX, posY));
        pixelVec.push_back(pixel);
        activePixelVec.push_back(pixel);
        m_image.setPixel(posX, posY, pixelVec.back().m_color);
    }
}

void Program::LoadImage(const std::string& filePathStr) {
    // if (image.loadFromFile(filePathStr))
    // {
    //     texture.loadFromImage(image);
    //     sprite.setTexture(texture);
    // }
    // else
    // {
    //     MessageBox(hwnd, L"Failed to load image", L"Error",
    //     MB_OK | MB_ICONERROR);
    // }
}