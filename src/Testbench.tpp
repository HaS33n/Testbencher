template <typename VerilatedModel>
Testbench<VerilatedModel>::Testbench(VerilatedModel& model, std::vector<TBClock>& clocks, std::vector<Display>& displays, std::function<void(void)> event_callback) : clocks(clocks), displays(displays), dut(model){
    dut.eval();
}

template <typename VerilatedModel>
Testbench<VerilatedModel>::~Testbench(){
    dut.final();
}

template <typename VerilatedModel>
std::uint64_t Testbench<VerilatedModel>::tick(){
    // TODO sort clocks in constructor, then take the first one as tmin, update and put it back
    std::uint64_t tmin = ULONG_MAX;
    for(auto& it : clocks){
        auto tte = it.timeToNextEdge();

        bool cnd = tte < tmin;
        tmin = tmin * !cnd + tte * cnd;
    }
    
    dut.eval();

    for(auto& display : displays)
        display.update();

    for(auto& it : clocks)
        it.update(tmin);
        
    Verilated::timeInc(tmin);

    dut.eval();

    for(auto& display : displays)
        display.update();

    return tmin;
}