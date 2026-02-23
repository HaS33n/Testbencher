#include "../include/Display.hpp"

Display::Display(sf::Vector2u resolution, std::function<void()> callback) : on_window_close(callback), pixels(resolution, sf::Color::Black), frame(tx){
    window.create(sf::VideoMode(resolution), "Testbencher: VGA 0");
    window.setFramerateLimit(144);

    tx.resize(resolution);
    frame.setTexture(tx, true);

    hptr = vptr = 0;
}

Display::~Display(){
    window.close();
}

void Display::handleEvents(){
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            on_window_close();
    }
}

//TODO: rewrite this to better simulate real hardware inside displays + optimize
void Display::update(sf::Color pixel, bool hsync, bool vsync, bool n_blanking){
    if(n_blanking){
        if(hptr < window.getSize().x && vptr < window.getSize().y){
            pixels.setPixel({hptr, vptr}, pixel);
            //std::cout<<hptr<<"  "<<vptr<<std::endl;
        }
        
        hptr++;
    }

    hptr *= hsync;
    vptr += hsync;
    vptr *= vsync;

    if(!vsync)
        renderFrame();
}

void Display::renderFrame(){
    tx.update(pixels.getPixelsPtr());

    window.clear();
    window.draw(frame);
    window.display();
}