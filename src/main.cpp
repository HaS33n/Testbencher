#include "../include/Display.hpp"

Display* dspl;
void callback(){
	delete dspl;
	exit(0);
}

int main() {
	dspl = new Display({600, 600}, callback);

	while(1){
		for(int i = 0; i < 600; i++){
			for(int j = 0; j < 600; j++){
				sf::Color clr = (j%2? sf::Color::Red : sf::Color:: Blue);
				dspl->update(clr, true, true, true);

				dspl->handleEvents();
			}
			dspl->update(sf::Color::Black, false, true, false);
		}
		dspl->update(sf::Color::Black, false, false, false);
	}


    return 0;
}