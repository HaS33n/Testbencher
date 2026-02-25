#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <functional>
#include <iostream>

class Display{

public:
    Display() = delete;
    Display(sf::Vector2u resolution);
    ~Display();

    template <typename... Args>
    void handleEvents(Args... args){window.handleEvents(args...);}

    void update(sf::Color pixel, bool hsync, bool vsync, bool n_blanking);
private:
    void renderFrame();

    //TODO: smartpointers or static
    sf::Image* pixels;
    sf::Texture* tx;
    sf::Sprite* frame;
    std::uint16_t hptr, vptr;
    bool prev_hsync, prev_vsync;

    sf::Clock fps_clk;
    uint16_t frame_ctr;

    sf::RenderWindow window;
};