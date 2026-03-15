#include "../include/Display.hpp"
#include "../include/Testbench.hpp"
//#include "Vtop.h"

Testbench* tb_ptr;

void callback(const sf::Event::Closed& ev){
	tb_ptr->terminateDisplay();
	exit(0);
}

void cb2(const sf::Event::KeyPressed& keyPress){
	if (keyPress.scancode == sf::Keyboard::Scancode::Escape){
		tb_ptr->terminateDisplay();
		exit(0);
	}
}

void onRender(){
	tb_ptr->handleEvents(callback, cb2);
}

int main(int argc, char** argv){
	Verilated::commandArgs(argc, argv);

	Vtop dut;
	TBClock clk({40000}, std::vector<uint8_t*>({&dut.clock}));

	Testbench tb(dut, std::vector<TBClock>({clk}), onRender, VGA_640_480_60);
	tb_ptr = &tb;


	dut.reset = 0;
	dut.color_in = 0xFF0000;

	auto interface = tb.getDisplayInterface();

	for(;;){
		*(std::get<0>(interface)) = dut.clk;
		*(std::get<1>(interface)) = sf::Color(dut.red, dut.green, dut.blue);
		*(std::get<2>(interface)) = dut.hsync;
		*(std::get<3>(interface)) = dut.vsync;
		*(std::get<4>(interface)) = dut.blank;

		tb.tick();
	}

	return 0;
}
