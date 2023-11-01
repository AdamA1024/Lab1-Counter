#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

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

    // initialize simulation inputs
    // top is name of top level entity.
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cylces
    for (i = 0; i < 300; i++)
    {
        for (clk = 0; clk < 2; clk++)
        {
            // this for loop toggles the clock.
            // outputs trace for each half of clock cycle
            tfp->dump(2 * i + clk);
            top->clk = !top->clk;
            top->eval();
        }
        top->rst = (i < 2) | (i == 15);
        top->en = (i > 4);
        if (Verilated::gotFinish())
            exit(0);
    }

    tfp->close();
    exit(0);
}
