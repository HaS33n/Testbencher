#include "../include/Testbench.hpp"
#include <algorithm>

TBClock::TBClock(const ClockProperties &properties) : clk_props(properties){
    signal = false;

    counter = properties.phase_shift;
    duty_cmp = double(properties.period) * properties.duty_cycle;
}

std::uint64_t TBClock::timeToNextEdge() const{
    return (counter > duty_cmp? clk_props.period - counter : duty_cmp - counter);
}

bool TBClock::getClockSignal() const{
    return signal;
}

void TBClock::update(std::uint64_t time){
    counter += time;
    counter %= clk_props.period;

    signal = (counter <= duty_cmp);
}