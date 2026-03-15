#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdint>
#include <functional>
#include <iostream>

struct DisplayParameters{
    sf::Vector2u resolution;

    std::uint16_t h_frontporch;
    std::uint16_t h_sync_w;
    std::uint16_t h_backporch;
    bool h_sync_polarity;
    
    std::uint16_t v_frontporch;
    std::uint16_t v_sync_w;
    std::uint16_t v_backporch;
    bool v_sync_polarity;
};

constexpr DisplayParameters VGA_640_480_60 = {{640, 480}, 16, 96, 48, false, 10, 2, 33, false};

class Display{

public:
    Display() = delete;
    Display(DisplayParameters configuration, std::function<void(void)> callback);
    ~Display();

    template <typename... Args>
    void handleEvents(Args... args){window.handleEvents(args...);}

    void update();

    std::uint8_t clk;
    sf::Color pixel; 
    bool hsync; 
    bool vsync;
    bool n_blanking;
private:
    void renderFrame();

    //TODO: smartpointers or static
    DisplayParameters config;
    sf::Image* pixels;
    sf::Texture* tx;
    sf::Sprite* frame;
    std::uint16_t hptr, vptr;
    bool prev_hsync, prev_vsync;
    bool prev_clk;

    sf::Clock fps_clk;
    uint16_t frame_ctr;

    sf::RenderWindow window;
    std::function<void(void)> render_callback;
};