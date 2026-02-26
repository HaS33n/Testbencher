#include "../include/Display.hpp"
#include <string>

Display::Display(sf::Vector2u resolution){
    window.create(sf::VideoMode(resolution), "Testbencher: VGA 0");
    //window.setFramerateLimit(144);

    pixels = new sf::Image(resolution, sf::Color::Black);
    tx = new sf::Texture(*pixels);
    frame = new sf::Sprite(*tx);

    hptr = vptr = 0;
    prev_hsync = prev_vsync = 0;

    frame_ctr = 0;

    window.clear(sf::Color::Black);
    window.display();
}

Display::~Display(){
    delete tx;
    delete pixels;
    delete frame;

    window.close();
}

//TODO: rewrite this to better simulate real hardware inside displays + dynamic resolution & HZ calculation + optimize
void Display::update(sf::Color pixel, bool hsync, bool vsync, bool n_blanking){
    if(n_blanking){
        if(hptr < window.getSize().x && vptr < window.getSize().y)
            pixels->setPixel({hptr, vptr}, pixel);
        
        hptr++;
    }

    hptr *= hsync;

    vptr += (prev_hsync & !hsync);
    vptr *= vsync;

    if(prev_vsync & !vsync)
        renderFrame();
    
    prev_hsync = hsync;
    prev_vsync = vsync;
}

//TODO: better fps calculation
void Display::renderFrame(){
    frame_ctr++;
    auto time = fps_clk.getElapsedTime();
    if(time >= sf::seconds(1)){
        fps_clk.restart();
        uint16_t fps = frame_ctr / time.asSeconds();

        std::string str = std::to_string(fps);
        window.setTitle("Testbencher: VGA 0 | FPS: " + str);
        frame_ctr = 0;
    }

    tx->update(*pixels);

    window.clear();
    window.draw(*frame);
    window.display();
}