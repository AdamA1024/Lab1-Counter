#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env)
{
    int i;   // keeps track of number of clock cycle simulations
    int clk; // clock signal

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    // instantiate the counter module as Vcounter
    // which is name of all generated files

    Vcounter *top = new Vcounter;
    // init trace dump
    // turns on csignal tracing and tells verilator
    // to dump waveform data to counter.vcd
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 1: Counter, task2");

    // initialize simulation inputs
    // top is name of top level entity.
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cylces
    for (i = 0; i < 900; i++)
    {
        for (clk = 0; clk < 2; clk++)
        {
            // this for loop toggles the clock.
            // outputs trace for each half of clock cycle
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        //++++ send count value to Vbuddy
        // vbdHex(4, (int(top->count) >> 16) & 0xF);
        // vbdHex(3, (int(top->count) >> 8) & 0xF);
        // vbdHex(2, (int(top->count) >> 4) & 0xF);
        // vbdHex(1, int(top->count) & 0xF);

        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i + 1);
        // ----- end of Vbuddy output system

        // change input stimuli

        top->rst = (i < 2) | (i == 15);
        top->en = vbdFlag();
        if (Verilated::gotFinish())
            exit(0);
    }

    vbdClose(); //++++

    tfp->close();
    exit(0);
}
