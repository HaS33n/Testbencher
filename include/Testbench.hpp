#pragma once
#include <cstdint>
#include <climits>
#include <vector>
#include <queue>
#include <memory>
#include "Display.hpp"
#include <tuple>

#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"

//CURRENTLY LIMITED TO ONLY PICOSECONDS!!

class TBClock{

public:
    class ClockProperties{
    public:
        ClockProperties() = delete;
        ClockProperties(std::uint64_t p, double dc = 0.5, std::uint64_t ps = 0);

        std::uint64_t period; //ps
        double duty_cycle; // [0:1]
        std::uint64_t phase_shift; //< period
    };

    TBClock() = delete;
    TBClock(const ClockProperties& properties, std::vector<std::uint8_t*>&& bindings);
    ~TBClock() = default;

    template <typename T>
    static std::uint64_t freqToPeriod(T freq){return (freq != 0? std::uint64_t(double(1/freq) * 10e12) : ULONG_MAX);} //Hz

    std::uint64_t timeToNextEdge() const;
    bool getClockSignal() const;
    void update(std::uint64_t time);

private:
    bool signal;
    ClockProperties clk_props;
    std::vector<std::uint8_t*> signal_targets;

    std::uint64_t counter;
    std::uint64_t duty_cmp;
};

using DisplayInterface = std::tuple<std::uint8_t*, sf::Color*, bool*, bool*, bool*>;

class Testbench{

public:
    Testbench(Vtop& model, std::vector<TBClock>&& clocks, sf::Vector2u display_res = {0,0});
    ~Testbench();

    std::uint64_t tick();
    
    template<typename... Args>
    void handleEvents(Args... args){display->handleEvents(args...);}
    
    DisplayInterface getDisplayInterface();
    void terminateDisplay();
    
private:
    Display* display;
    std::vector<TBClock> clocks;

    VerilatedVcdC* tfp;
    Vtop& dut;
};