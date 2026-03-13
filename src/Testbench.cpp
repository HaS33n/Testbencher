#include "../include/Testbench.hpp"
#include "../build/compilation_config.h"

TBClock::ClockProperties::ClockProperties(std::uint64_t p, double dc, std::uint64_t ps){
    if(dc > 1.0)
        dc = 1.0;
    else if(dc < 0.0)
        dc = 0.0;
    
    duty_cycle = dc;

    period = p;
    if(ps >= period)
        ps = period - 1;
    
    phase_shift = ps;
}

TBClock::TBClock(const ClockProperties& properties, std::vector<std::uint8_t*>&& bindings) : clk_props(properties), signal_targets(bindings){
    counter = properties.phase_shift;
    duty_cmp = double(properties.period) * properties.duty_cycle;

    update(0);
}

std::uint64_t TBClock::timeToNextEdge() const{
    bool cond = counter >= duty_cmp;
    return (clk_props.period - counter) * cond + (duty_cmp - counter) * !cond;
}

bool TBClock::getClockSignal() const{
    return signal;
}

void TBClock::update(std::uint64_t time){
    counter += time;
    counter -= (counter >= clk_props.period) * clk_props.period;

    signal = (counter < duty_cmp);
    for(auto& it : signal_targets)
        *it = signal;
}

Testbench::Testbench(Vtop& model, std::vector<TBClock>&& clocks, sf::Vector2u display_res) : clocks(clocks), dut(model){
    #ifdef USE_DISPLAY_SIM
        display = new Display(display_res);
    #else
        display = nullptr;
    #endif
    
    #ifdef TRACE_ENABLE
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        dut.trace(tfp, 99);
        tfp->open("waveform.vcd");
    #else
        tfp = nullptr;
    #endif

    dut.eval();
    #ifdef TRACE_ENABLE
        tfp->dump(Verilated::time());
        tfp->flush();
    #endif
}

Testbench::~Testbench(){
    delete display;

    tfp->close();
    delete tfp;
}

std::uint64_t Testbench::tick(){
    std::uint64_t tmin = ULONG_MAX;
    for(auto& it : clocks){
        auto tte = it.timeToNextEdge();

        bool cnd = tte < tmin;
        tmin = tmin * !cnd + tte * cnd;
    }
    
    dut.eval();

    #ifdef USE_DISPLAY_SIM
        display->update();
    #endif

    for(auto& it : clocks)
        it.update(tmin);
        
    Verilated::timeInc(tmin);
    dut.eval();

    #ifdef USE_DISPLAY_SIM
        display->update();
    #endif

    #ifdef TRACE_ENABLE
        tfp->dump(Verilated::time());
        tfp->flush();
    #endif

    return tmin;
}

#ifdef USE_DISPLAY_SIM
DisplayInterface Testbench::getDisplayInterface(){
    return {&(display->clk), &(display->pixel), &(display->hsync), &(display->vsync), &(display->n_blanking)};
}

void Testbench::terminateDisplay(){
    delete display;
    display = nullptr;
}
#endif