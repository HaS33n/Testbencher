#include "../include/Testbench.hpp"

TBClock::ClockProperties::ClockProperties(std::uint64_t p, double dc, std::uint64_t ps){
    if(dc > 1.0)
        dc = 1.0;
    else if(dc < 0.0)
        dc = 0.0;
    
    duty_cycle = dc;

    if(ps >= period)
        ps = period - 1;
    
    phase_shift = ps;
    period = p;
}

TBClock::TBClock(const ClockProperties &properties) : clk_props(properties){
    signal = false;

    counter = properties.phase_shift;
    duty_cmp = double(properties.period) * properties.duty_cycle;
}

std::uint64_t TBClock::timeToNextEdge() const{
    bool cond = counter > duty_cmp;
    return (clk_props.period - counter) * cond + (duty_cmp - counter) * !cond;
}

bool TBClock::getClockSignal() const{
    return signal;
}

void TBClock::update(std::uint64_t time){
    counter += time;
    counter -= (counter >= clk_props.period) * clk_props.period;

    signal = (counter <= duty_cmp);
}