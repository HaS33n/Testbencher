#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <functional>
#include <iostream>

class Display{

public:
    Display(sf::Vector2u resolution, std::function<void()> callback);
    ~Display();

    void handleEvents();
    void update(sf::Color pixel, bool hsync, bool vsync, bool n_blanking);
private:
    void renderFrame();

    sf::Image pixels;
    sf::Texture tx;
    sf::Sprite frame;
    std::uint16_t hptr, vptr;

    sf::RenderWindow window;

    std::function<void()> on_window_close;
};