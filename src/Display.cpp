#include "../include/Display.hpp"
#include <string>

Display::Display(DisplayParameters configuration, std::function<void(void)> callback) : render_callback(callback), config(configuration){
    window.create(sf::VideoMode(configuration.resolution), "Testbencher: VGA 0");
    //window.setFramerateLimit(144);

    pixels = new sf::Image(configuration.resolution, sf::Color::Black);
    tx = new sf::Texture(*pixels);
    frame = new sf::Sprite(*tx);

    hptr = vptr = 0;
    prev_hsync = prev_vsync = 0;

    frame_ctr = 0;
    prev_clk = 0; //may cause problems

    window.clear(sf::Color::Black);
    window.display();
}

Display::~Display(){
    delete tx;
    delete pixels;
    delete frame;

    window.close();
}

void Display::update(){
    //update only on clk posedge
    if(!(prev_clk == 0 && clk == 1)){
        prev_clk = clk;
        return;
    }

    //if positive signal polarity, then negate sync signals, TODO: optimize to avoid branching
    bool hsync_v = config.h_sync_polarity? !hsync : hsync;
    bool vsync_v = config.v_sync_polarity? !vsync : vsync;

    if(n_blanking){
        if(hptr >= config.h_backporch && hptr < config.resolution.x + config.h_backporch && vptr >= config.v_backporch && vptr < config.resolution.y + config.v_backporch)
            pixels->setPixel({hptr - config.h_backporch, vptr - config.v_backporch}, pixel);
        
        hptr++;
    }

    hptr *= hsync_v;

    vptr += (prev_hsync & !hsync_v);
    vptr *= vsync_v;

    if(prev_vsync & !vsync_v)
        renderFrame();
    
    prev_hsync = hsync_v;
    prev_vsync = vsync_v;

    prev_clk = clk;
}

void Display::renderFrame(){
    frame_ctr++;
    auto time = fps_clk.restart();

    if(frame_ctr > 30){
        std::string ftime = std::to_string(time.asMilliseconds());
        window.setTitle("Testbencher: VGA 0 | FrameTime(ms): " + ftime);

        frame_ctr = 0;
    }

    tx->update(*pixels);

    window.clear();
    window.draw(*frame);
    window.display();

    render_callback();
}