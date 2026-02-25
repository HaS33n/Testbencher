#include "../include/Display.hpp"
#include "../include/Testbench.hpp"

Display* dspl;
void callback(const sf::Event::Closed& ev){
	delete dspl;
	exit(0);
}

void cb2(const sf::Event::KeyPressed& keyPress){
	if (keyPress.scancode == sf::Keyboard::Scancode::Escape){
		delete dspl;
		exit(0);
	}
}

int main() {
	dspl = new Display({640, 480});

	while(1){
		for(int i = 0; i < 480; i++){
			for(int j = 0; j < 640; j++){
				sf::Color clr = (j % 10 == 0? sf::Color::Red : sf::Color::Blue);
				dspl->update(clr, true, true, true);
			}
			dspl->update(sf::Color::Black, false, true, false);
		}
		dspl->update(sf::Color::Black, false, false, false);
		dspl->handleEvents(callback, cb2);
	}


    return 0;
}