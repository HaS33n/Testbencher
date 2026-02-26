#pragma once
#include <cstdint>
#include <climits>
#include <vector>
#include "Display.hpp"

#include <verilated.h>
#include "Vtop.h"

//CURRENTLY LIMITED TO ONLY PICOSECONDS!!

class TBClock{

public:
    class ClockProperties{
    public:
        ClockProperties() = delete;
        ClockProperties(std::uint64_t p, double dc, std::uint64_t ps);

        std::uint64_t period; //ps
        double duty_cycle; // [0:1]
        std::uint64_t phase_shift; //< period
    };

    TBClock() = delete;
    TBClock(const ClockProperties& properties);
    ~TBClock() = default;

    template <typename T>
    static std::uint64_t freqToPeriod(T freq){return (freq != 0? std::uint64_t(double(1/freq) * 10e12) : ULONG_MAX);} //Hz

    std::uint64_t timeToNextEdge() const;
    bool getClockSignal() const;
    void update(std::uint64_t time);

private:
    bool signal;
    ClockProperties clk_props;

    std::uint64_t counter;
    std::uint64_t duty_cmp;
};

class Testbench{

public:
    Testbench();
    ~Testbench();

private:
    bool trace_enable;
    Display* display;
    std::vector<TBClock> clocks;
};